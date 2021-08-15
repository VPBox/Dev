/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#undef NDEBUG

#include "Callbacks.h"
#include "CompilationBuilder.h"
#include "Manager.h"
#include "ModelBuilder.h"
#include "NeuralNetworks.h"
#include "SampleDriver.h"
#include "TestNeuralNetworksWrapper.h"
#include "ValidateHal.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include <gtest/gtest.h>

namespace android {

using CompilationBuilder = nn::CompilationBuilder;
using Device = nn::Device;
using DeviceManager = nn::DeviceManager;
using HidlModel = hardware::neuralnetworks::V1_2::Model;
using HidlToken = hardware::hidl_array<uint8_t, ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN>;
using PreparedModelCallback = hardware::neuralnetworks::V1_2::implementation::PreparedModelCallback;
using Result = nn::test_wrapper::Result;
using SampleDriver = nn::sample_driver::SampleDriver;
using WrapperCompilation = nn::test_wrapper::Compilation;
using WrapperEvent = nn::test_wrapper::Event;
using WrapperExecution = nn::test_wrapper::Execution;
using WrapperModel = nn::test_wrapper::Model;
using WrapperOperandType = nn::test_wrapper::OperandType;
using WrapperType = nn::test_wrapper::Type;

template <typename T>
using MQDescriptorSync = ::android::hardware::MQDescriptorSync<T>;

namespace {

const Timing kBadTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};

// Wraps an V1_2::IPreparedModel to allow dummying up the execution status.
class TestPreparedModel12 : public V1_2::IPreparedModel {
   public:
    // If errorStatus is NONE, then execute behaves normally (and sends back
    // the actual execution status).  Otherwise, don't bother to execute, and
    // just send back errorStatus (as the execution status, not the launch
    // status).
    TestPreparedModel12(sp<V1_0::IPreparedModel> preparedModel, ErrorStatus errorStatus)
        : mPreparedModelV1_0(preparedModel),
          mPreparedModelV1_2(V1_2::IPreparedModel::castFrom(preparedModel).withDefault(nullptr)),
          mErrorStatus(errorStatus) {}

    Return<ErrorStatus> execute(const Request& request,
                                const sp<V1_0::IExecutionCallback>& callback) override {
        CHECK(mPreparedModelV1_0 != nullptr) << "V1_0 prepared model is nullptr.";
        if (mErrorStatus == ErrorStatus::NONE) {
            return mPreparedModelV1_0->execute(request, callback);
        } else {
            callback->notify(mErrorStatus);
            return ErrorStatus::NONE;
        }
    }

    Return<ErrorStatus> execute_1_2(const Request& request, MeasureTiming measure,
                                    const sp<V1_2::IExecutionCallback>& callback) override {
        CHECK(mPreparedModelV1_2 != nullptr) << "V1_2 prepared model is nullptr.";
        if (mErrorStatus == ErrorStatus::NONE) {
            return mPreparedModelV1_2->execute_1_2(request, measure, callback);
        } else if (mErrorStatus == ErrorStatus::OUTPUT_INSUFFICIENT_SIZE) {
            OutputShape shape = {.dimensions = {1}, .isSufficient = false};
            callback->notify_1_2(mErrorStatus, {shape}, kBadTiming);
            return ErrorStatus::NONE;
        } else {
            callback->notify_1_2(mErrorStatus, {}, kBadTiming);
            return ErrorStatus::NONE;
        }
    }

    Return<void> executeSynchronously(const Request& request, MeasureTiming measure,
                                      executeSynchronously_cb cb) override {
        CHECK(mPreparedModelV1_2 != nullptr) << "V1_2 prepared model is nullptr.";
        if (mErrorStatus == ErrorStatus::NONE) {
            return mPreparedModelV1_2->executeSynchronously(
                    request, measure,
                    [&cb](ErrorStatus error, const hidl_vec<OutputShape>& outputShapes,
                          const Timing& timing) { cb(error, outputShapes, timing); });
        } else if (mErrorStatus == ErrorStatus::OUTPUT_INSUFFICIENT_SIZE) {
            OutputShape shape = {.dimensions = {1}, .isSufficient = false};
            cb(mErrorStatus, {shape}, kBadTiming);
            return Void();
        } else {
            cb(mErrorStatus, {}, kBadTiming);
            return Void();
        }
    }

    Return<void> configureExecutionBurst(
            const sp<V1_2::IBurstCallback>& callback,
            const MQDescriptorSync<V1_2::FmqRequestDatum>& requestChannel,
            const MQDescriptorSync<V1_2::FmqResultDatum>& resultChannel,
            configureExecutionBurst_cb cb) override {
        if (mErrorStatus == ErrorStatus::NONE) {
            return mPreparedModelV1_2->configureExecutionBurst(callback, requestChannel,
                                                               resultChannel, cb);
        } else {
            cb(mErrorStatus, nullptr);
            return Void();
        }
    }

   private:
    const sp<V1_0::IPreparedModel> mPreparedModelV1_0;
    const sp<V1_2::IPreparedModel> mPreparedModelV1_2;
    ErrorStatus mErrorStatus;
};

// Like TestPreparedModel12, but implementing 1.0
class TestPreparedModel10 : public V1_0::IPreparedModel {
   public:
    TestPreparedModel10(sp<V1_0::IPreparedModel> preparedModel, ErrorStatus errorStatus)
        : m12PreparedModel(new TestPreparedModel12(preparedModel, errorStatus)) {}

    Return<ErrorStatus> execute(const Request& request,
                                const sp<V1_0::IExecutionCallback>& callback) override {
        return m12PreparedModel->execute(request, callback);
    }

   private:
    const sp<V1_2::IPreparedModel> m12PreparedModel;
};

// Behaves like SampleDriver, except that it produces wrapped IPreparedModel.
class TestDriver12 : public SampleDriver {
   public:
    // Allow dummying up the error status for execution of all models
    // prepared from this driver.  If errorStatus is NONE, then
    // execute behaves normally (and sends back the actual execution
    // status).  Otherwise, don't bother to execute, and just send
    // back errorStatus (as the execution status, not the launch
    // status).
    TestDriver12(const std::string& name, ErrorStatus errorStatus)
        : SampleDriver(name.c_str()), mErrorStatus(errorStatus) {}

    Return<void> getCapabilities_1_2(getCapabilities_1_2_cb _hidl_cb) override {
        android::nn::initVLogMask();
        const PerformanceInfo kPerf = {.execTime = 0.75f, .powerUsage = 0.75f};
        Capabilities capabilities = {
                .relaxedFloat32toFloat16PerformanceScalar = kPerf,
                .relaxedFloat32toFloat16PerformanceTensor = kPerf,
                .operandPerformance = nn::nonExtensionOperandPerformance(kPerf)};
        _hidl_cb(ErrorStatus::NONE, capabilities);
        return Void();
    }

    Return<void> getSupportedOperations_1_2(const HidlModel& model,
                                            getSupportedOperations_1_2_cb cb) override {
        if (nn::validateModel(model)) {
            std::vector<bool> supported(model.operations.size(), true);
            cb(ErrorStatus::NONE, supported);
        } else {
            std::vector<bool> supported;
            cb(ErrorStatus::INVALID_ARGUMENT, supported);
        }
        return Void();
    }

    Return<ErrorStatus> prepareModel_1_2(
            const HidlModel& model, ExecutionPreference preference,
            const hidl_vec<hidl_handle>& modelCache, const hidl_vec<hidl_handle>& dataCache,
            const HidlToken& token, const sp<IPreparedModelCallback>& actualCallback) override {
        sp<PreparedModelCallback> localCallback = new PreparedModelCallback;
        Return<ErrorStatus> prepareModelReturn = SampleDriver::prepareModel_1_2(
                model, preference, modelCache, dataCache, token, localCallback);
        if (!prepareModelReturn.isOkUnchecked()) {
            return prepareModelReturn;
        }
        if (prepareModelReturn != ErrorStatus::NONE) {
            actualCallback->notify_1_2(
                    localCallback->getStatus(),
                    V1_2::IPreparedModel::castFrom(localCallback->getPreparedModel()));
            return prepareModelReturn;
        }
        localCallback->wait();
        if (localCallback->getStatus() != ErrorStatus::NONE) {
            actualCallback->notify_1_2(
                    localCallback->getStatus(),
                    V1_2::IPreparedModel::castFrom(localCallback->getPreparedModel()));
        } else {
            actualCallback->notify_1_2(
                    ErrorStatus::NONE,
                    new TestPreparedModel12(localCallback->getPreparedModel(), mErrorStatus));
        }
        return prepareModelReturn;
    }

    Return<ErrorStatus> prepareModel_1_1(
            const V1_1::Model& model, ExecutionPreference preference,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        sp<PreparedModelCallback> localCallback = new PreparedModelCallback;
        Return<ErrorStatus> prepareModelReturn =
                SampleDriver::prepareModel_1_1(model, preference, localCallback);
        if (!prepareModelReturn.isOkUnchecked()) {
            return prepareModelReturn;
        }
        if (prepareModelReturn != ErrorStatus::NONE) {
            actualCallback->notify(localCallback->getStatus(), localCallback->getPreparedModel());
            return prepareModelReturn;
        }
        localCallback->wait();
        if (localCallback->getStatus() != ErrorStatus::NONE) {
            actualCallback->notify(localCallback->getStatus(), localCallback->getPreparedModel());
        } else {
            actualCallback->notify(
                    ErrorStatus::NONE,
                    new TestPreparedModel10(localCallback->getPreparedModel(), mErrorStatus));
        }
        return prepareModelReturn;
    }

    Return<ErrorStatus> prepareModel(
            const V1_0::Model& model,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return prepareModel_1_1(nn::convertToV1_1(model), ExecutionPreference::FAST_SINGLE_ANSWER,
                                actualCallback);
    }

private:
    ErrorStatus mErrorStatus;
};

// Like TestDriver, but implementing 1.1
class TestDriver11 : public V1_1::IDevice {
   public:
    TestDriver11(const std::string& name, ErrorStatus errorStatus)
        : m12Driver(new TestDriver12(name, errorStatus)) {}
    Return<void> getCapabilities_1_1(getCapabilities_1_1_cb _hidl_cb) override {
        return m12Driver->getCapabilities_1_1(_hidl_cb);
    }
    Return<void> getSupportedOperations_1_1(const V1_1::Model& model,
                                            getSupportedOperations_1_1_cb _hidl_cb) override {
        return m12Driver->getSupportedOperations_1_1(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel_1_1(
            const V1_1::Model& model, ExecutionPreference preference,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return m12Driver->prepareModel_1_1(model, preference, actualCallback);
    }
    Return<DeviceStatus> getStatus() override { return m12Driver->getStatus(); }
    Return<void> getCapabilities(getCapabilities_cb _hidl_cb) override {
        return m12Driver->getCapabilities(_hidl_cb);
    }
    Return<void> getSupportedOperations(const V1_0::Model& model,
                                        getSupportedOperations_cb _hidl_cb) override {
        return m12Driver->getSupportedOperations(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel(
            const V1_0::Model& model,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return m12Driver->prepareModel(model, actualCallback);
    }

   private:
    const sp<V1_2::IDevice> m12Driver;
};

// Like TestDriver, but implementing 1.0
class TestDriver10 : public V1_0::IDevice {
   public:
    TestDriver10(const std::string& name, ErrorStatus errorStatus)
        : m12Driver(new TestDriver12(name, errorStatus)) {}
    Return<void> getCapabilities(getCapabilities_cb _hidl_cb) override {
        return m12Driver->getCapabilities(_hidl_cb);
    }
    Return<void> getSupportedOperations(const V1_0::Model& model,
                                        getSupportedOperations_cb _hidl_cb) override {
        return m12Driver->getSupportedOperations(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel(
            const V1_0::Model& model,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return m12Driver->prepareModel(model, actualCallback);
    }
    Return<DeviceStatus> getStatus() override { return m12Driver->getStatus(); }

   private:
    const sp<V1_2::IDevice> m12Driver;
};

// This class adds some simple utilities on top of WrapperCompilation in order
// to provide access to certain features from CompilationBuilder that are not
// exposed by the base class.
template<typename DriverClass>
class TestCompilation : public WrapperCompilation {
public:
    // Allow dummying up the error status for all executions from this
    // compilation.  If errorStatus is NONE, then execute behaves
    // normally (and sends back the actual execution status).
    // Otherwise, don't bother to execute, and just send back
    // errorStatus (as the execution status, not the launch status).
    TestCompilation(const WrapperModel* model, const std::string& deviceName,
                    ErrorStatus errorStatus) {
        std::vector<std::shared_ptr<Device>> devices;
        auto device = DeviceManager::forTest_makeDriverDevice(
                deviceName, new DriverClass(deviceName, errorStatus));
        devices.push_back(device);

        nn::ModelBuilder* m = reinterpret_cast<nn::ModelBuilder*>(model->getHandle());
        CompilationBuilder* c = nullptr;
        int result = m->createCompilation(&c, devices);
        EXPECT_EQ(result, 0);
        // We need to ensure that we use our TestDriver and do not
        // fall back to CPU.  (If we allow CPU fallback, then when our
        // TestDriver reports an execution failure, we'll re-execute
        // on CPU, and will not see the failure.)
        c->setPartitioning(DeviceManager::kPartitioningWithoutFallback);
        mCompilation = reinterpret_cast<ANeuralNetworksCompilation*>(c);
    }
};

// This class has roughly the same functionality as TestCompilation class.
// The major difference is that Introspection API is used to select the device.
class TestIntrospectionCompilation : public WrapperCompilation {
   public:
    TestIntrospectionCompilation(const WrapperModel* model, const std::string& deviceName) {
        std::vector<ANeuralNetworksDevice*> mDevices;
        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
        EXPECT_GE(numDevices, (uint32_t)1);

        for (uint32_t i = 0; i < numDevices; i++) {
            ANeuralNetworksDevice* device = nullptr;
            EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
            const char* buffer = nullptr;
            int result = ANeuralNetworksDevice_getName(device, &buffer);
            if (result == ANEURALNETWORKS_NO_ERROR && deviceName.compare(buffer) == 0) {
                mDevices.push_back(device);
            }
        }
        // In CPU only mode, DeviceManager::getDrivers() will not be able to
        // provide the actual device list. We will not be able to find the test
        // driver with specified deviceName.
        if (!DeviceManager::get()->getUseCpuOnly()) {
            EXPECT_EQ(mDevices.size(), (uint32_t)1);

            int result = ANeuralNetworksCompilation_createForDevices(
                    model->getHandle(), mDevices.data(), mDevices.size(), &mCompilation);
            EXPECT_EQ(result, ANEURALNETWORKS_NO_ERROR);
        }
    }
};

template <class DriverClass>
class ExecutionTestTemplate
    : public ::testing::TestWithParam<std::tuple<ErrorStatus, Result, bool>> {
   public:
    ExecutionTestTemplate()
        : kName(toString(std::get<0>(GetParam()))),
          kForceErrorStatus(std::get<0>(GetParam())),
          kExpectResult(std::get<1>(GetParam())),
          kUseIntrospectionAPI(std::get<2>(GetParam())),
          mModel(makeModel()) {
        if (kUseIntrospectionAPI) {
            DeviceManager::get()->forTest_registerDevice(kName.c_str(),
                                                         new DriverClass(kName, kForceErrorStatus));
            mCompilation = TestIntrospectionCompilation(&mModel, kName);
        } else {
            mCompilation = TestCompilation<DriverClass>(&mModel, kName, kForceErrorStatus);
        }
    }

   protected:
    // Unit test method
    void TestWait();

    virtual void TearDown() {
        // Reinitialize the device list since Introspection API path altered it.
        if (kUseIntrospectionAPI) {
            DeviceManager::get()->forTest_reInitializeDeviceList();
        }
    }

    const std::string kName;

    // Allow dummying up the error status for execution.  If
    // kForceErrorStatus is NONE, then execution behaves normally (and
    // sends back the actual execution status).  Otherwise, don't
    // bother to execute, and just send back kForceErrorStatus (as the
    // execution status, not the launch status).
    const ErrorStatus kForceErrorStatus;

    // What result do we expect from the execution?  (The Result
    // equivalent of kForceErrorStatus.)
    const Result kExpectResult;

    // Whether mCompilation is created via Introspection API or not.
    const bool kUseIntrospectionAPI;

    WrapperModel mModel;
    WrapperCompilation mCompilation;

    void setInputOutput(WrapperExecution* execution) {
        mInputBuffer = kInputBuffer;
        mOutputBuffer = kOutputBufferInitial;
        ASSERT_EQ(execution->setInput(0, &mInputBuffer, sizeof(mInputBuffer)), Result::NO_ERROR);
        ASSERT_EQ(execution->setOutput(0, &mOutputBuffer, sizeof(mOutputBuffer)), Result::NO_ERROR);
    }

    const float kInputBuffer = 3.14;
    const float kOutputBufferInitial = 0;
    float mInputBuffer;
    float mOutputBuffer;
    const float kOutputBufferExpected = 3;
    const std::vector<uint32_t> kOutputDimensionsExpected = {1};

   private:
    static WrapperModel makeModel() {
        static const WrapperOperandType tensorType(WrapperType::TENSOR_FLOAT32, { 1 });

        WrapperModel model;
        uint32_t input = model.addOperand(&tensorType);
        uint32_t output = model.addOperand(&tensorType);
        model.addOperation(ANEURALNETWORKS_FLOOR, { input }, { output });
        model.identifyInputsAndOutputs({ input }, { output } );
        assert(model.finish() == Result::NO_ERROR);

        return model;
    }
};

template<class DriverClass> void ExecutionTestTemplate<DriverClass>::TestWait() {
    SCOPED_TRACE(kName);
    // Skip Introspection API tests when CPU only flag is forced on.
    if (kUseIntrospectionAPI && DeviceManager::get()->getUseCpuOnly()) {
        GTEST_SKIP();
    }

    ASSERT_EQ(mCompilation.finish(), Result::NO_ERROR);

    {
        SCOPED_TRACE("startCompute");
        WrapperExecution execution(&mCompilation);
        ASSERT_NO_FATAL_FAILURE(setInputOutput(&execution));
        WrapperEvent event;
        ASSERT_EQ(execution.startCompute(&event), Result::NO_ERROR);
        ASSERT_EQ(event.wait(), kExpectResult);
        if (kExpectResult == Result::NO_ERROR) {
            ASSERT_EQ(mOutputBuffer, kOutputBufferExpected);
        }
        std::vector<uint32_t> dimensions;
        if (kExpectResult == Result::OUTPUT_INSUFFICIENT_SIZE) {
            // Only one output operand, hardcoded as index 0.
            ASSERT_EQ(execution.getOutputOperandDimensions(0, &dimensions),
                      Result::OUTPUT_INSUFFICIENT_SIZE);
        } else {
            ASSERT_EQ(execution.getOutputOperandDimensions(0, &dimensions), Result::NO_ERROR);
        }
        if (kExpectResult == Result::NO_ERROR ||
            kExpectResult == Result::OUTPUT_INSUFFICIENT_SIZE) {
            ASSERT_EQ(dimensions, kOutputDimensionsExpected);
        }
    }
    {
        SCOPED_TRACE("compute");
        WrapperExecution execution(&mCompilation);
        ASSERT_NO_FATAL_FAILURE(setInputOutput(&execution));
        ASSERT_EQ(execution.compute(), kExpectResult);
        if (kExpectResult == Result::NO_ERROR) {
            ASSERT_EQ(mOutputBuffer, kOutputBufferExpected);
        }
        std::vector<uint32_t> dimensions;
        if (kExpectResult == Result::OUTPUT_INSUFFICIENT_SIZE) {
            // Only one output operand, hardcoded as index 0.
            ASSERT_EQ(execution.getOutputOperandDimensions(0, &dimensions),
                      Result::OUTPUT_INSUFFICIENT_SIZE);
        } else {
            ASSERT_EQ(execution.getOutputOperandDimensions(0, &dimensions), Result::NO_ERROR);
        }
        if (kExpectResult == Result::NO_ERROR ||
            kExpectResult == Result::OUTPUT_INSUFFICIENT_SIZE) {
            ASSERT_EQ(dimensions, kOutputDimensionsExpected);
        }
    }
}

auto kTestValues = ::testing::Values(
        std::make_tuple(ErrorStatus::NONE, Result::NO_ERROR, /* kUseIntrospectionAPI */ false),
        std::make_tuple(ErrorStatus::DEVICE_UNAVAILABLE, Result::UNAVAILABLE_DEVICE,
                        /* kUseIntrospectionAPI */ false),
        std::make_tuple(ErrorStatus::GENERAL_FAILURE, Result::OP_FAILED,
                        /* kUseIntrospectionAPI */ false),
        std::make_tuple(ErrorStatus::OUTPUT_INSUFFICIENT_SIZE, Result::OUTPUT_INSUFFICIENT_SIZE,
                        /* kUseIntrospectionAPI */ false),
        std::make_tuple(ErrorStatus::INVALID_ARGUMENT, Result::BAD_DATA,
                        /* kUseIntrospectionAPI */ false));

class ExecutionTest12 : public ExecutionTestTemplate<TestDriver12> {};
TEST_P(ExecutionTest12, Wait) {
    TestWait();
}
INSTANTIATE_TEST_CASE_P(Flavor, ExecutionTest12, kTestValues);

class ExecutionTest11 : public ExecutionTestTemplate<TestDriver11> {};
TEST_P(ExecutionTest11, Wait) {
    if (kForceErrorStatus == ErrorStatus::OUTPUT_INSUFFICIENT_SIZE) return;
    TestWait();
}
INSTANTIATE_TEST_CASE_P(Flavor, ExecutionTest11, kTestValues);

class ExecutionTest10 : public ExecutionTestTemplate<TestDriver10> {};
TEST_P(ExecutionTest10, Wait) {
    if (kForceErrorStatus == ErrorStatus::OUTPUT_INSUFFICIENT_SIZE) return;
    TestWait();
}
INSTANTIATE_TEST_CASE_P(Flavor, ExecutionTest10, kTestValues);

auto kIntrospectionTestValues = ::testing::Values(
        std::make_tuple(ErrorStatus::NONE, Result::NO_ERROR, /* kUseIntrospectionAPI */ true),
        std::make_tuple(ErrorStatus::DEVICE_UNAVAILABLE, Result::UNAVAILABLE_DEVICE,
                        /* kUseIntrospectionAPI */ true),
        std::make_tuple(ErrorStatus::GENERAL_FAILURE, Result::OP_FAILED,
                        /* kUseIntrospectionAPI */ true),
        std::make_tuple(ErrorStatus::OUTPUT_INSUFFICIENT_SIZE, Result::OUTPUT_INSUFFICIENT_SIZE,
                        /* kUseIntrospectionAPI */ true),
        std::make_tuple(ErrorStatus::INVALID_ARGUMENT, Result::BAD_DATA,
                        /* kUseIntrospectionAPI */ true));

INSTANTIATE_TEST_CASE_P(IntrospectionFlavor, ExecutionTest12, kIntrospectionTestValues);

}  // namespace
}  // namespace android
