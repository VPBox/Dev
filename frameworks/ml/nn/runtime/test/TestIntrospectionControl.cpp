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

#include "CompilationBuilder.h"
#include "ExecutionBurstServer.h"
#include "HalInterfaces.h"
#include "Manager.h"
#include "NeuralNetworks.h"
#include "NeuralNetworksOEM.h"
#include "SampleDriver.h"
#include "TestNeuralNetworksWrapper.h"
#include "Utils.h"
#include "ValidateHal.h"

#include <gtest/gtest.h>

#include <iterator>
#include <map>
#include <queue>
#include <set>

namespace {

using namespace ::android;

using CompilationBuilder = nn::CompilationBuilder;
using Device = nn::Device;
using DeviceManager = nn::DeviceManager;
using ExecutePreference = nn::test_wrapper::ExecutePreference;
using ExecutionBurstServer = nn::ExecutionBurstServer;
using HidlModel = hardware::neuralnetworks::V1_2::Model;
using HidlToken = hardware::hidl_array<uint8_t, ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN>;
using PreparedModelCallback = hardware::neuralnetworks::V1_2::implementation::PreparedModelCallback;
using Result = nn::test_wrapper::Result;
using SampleDriver = nn::sample_driver::SampleDriver;
using SamplePreparedModel = nn::sample_driver::SamplePreparedModel;
using WrapperModel = nn::test_wrapper::Model;
using WrapperOperandType = nn::test_wrapper::OperandType;
using WrapperType = nn::test_wrapper::Type;

template <typename T>
using MQDescriptorSync = hardware::MQDescriptorSync<T>;

const Timing kBadTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};

// This is an IDevice for testing purposes. The test driver has customized
// getCapabilities_1_1 and getSupportedOperations_1_2.
class TestDriver : public SampleDriver {
   public:
    TestDriver(const char* name, Capabilities capabilities, const std::vector<bool>& supportedOps)
        : SampleDriver(name), mCapabilities(capabilities), mSupportedOps(supportedOps) {}
    ~TestDriver() override {}

    Return<void> getCapabilities_1_2(getCapabilities_1_2_cb cb) override {
        cb(ErrorStatus::NONE, mCapabilities);
        return Void();
    }

    Return<void> getSupportedOperations_1_2(const Model& model,
                                            getSupportedOperations_cb cb) override {
        if (!android::nn::validateModel(model)) {
            cb(ErrorStatus::INVALID_ARGUMENT, std::vector<bool>());
            return Void();
        }
        const size_t count = model.operations.size();
        std::vector<bool> supported(count);
        std::transform(
                model.operations.begin(), model.operations.end(), supported.begin(),
                [this](Operation op) { return mSupportedOps[static_cast<int32_t>(op.type)]; });
        cb(ErrorStatus::NONE, supported);
        return Void();
    }

   private:
    Capabilities mCapabilities;
    std::vector<bool> mSupportedOps;
};

class IntrospectionControlTest : public ::testing::Test {
   protected:
    virtual void SetUp() {}
    virtual void TearDown() {
        if (mEvent) {
            ANeuralNetworksEvent_free(mEvent);
        }
        if (mExecution) {
            ANeuralNetworksExecution_free(mExecution);
        }
        if (mCompilation) {
            ANeuralNetworksCompilation_free(mCompilation);
        }
        DeviceManager::get()->forTest_reInitializeDeviceList();
    }

    struct DeviceSpecification {
        DeviceSpecification(const std::string& name, float perf, std::vector<bool>& supportedOps)
            : mName(name), mSupportedOps(supportedOps) {
            PerformanceInfo perfInfo = {.execTime = perf, .powerUsage = perf};
            mCapabilities = {.relaxedFloat32toFloat16PerformanceScalar = perfInfo,
                             .relaxedFloat32toFloat16PerformanceTensor = perfInfo,
                             .operandPerformance = nn::nonExtensionOperandPerformance(perfInfo)};
        }
        std::string mName;
        Capabilities mCapabilities;
        std::vector<bool> mSupportedOps;
    };

    // From a vector of DeviceSpecification, register new Devices.
    void registerDevices(std::vector<DeviceSpecification> specifications) {
        for (const auto& specification : specifications) {
            DeviceManager::get()->forTest_registerDevice(
                    specification.mName.c_str(),
                    new TestDriver(specification.mName.c_str(), specification.mCapabilities,
                                   specification.mSupportedOps));
        }
    }

    bool selectDeviceByName(const std::string& name) {
        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
        EXPECT_GE(numDevices, (uint32_t)1);

        for (uint32_t i = 0; i < numDevices; i++) {
            ANeuralNetworksDevice* device = nullptr;
            EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
            const char* buffer = nullptr;
            int result = ANeuralNetworksDevice_getName(device, &buffer);
            if (result == ANEURALNETWORKS_NO_ERROR && name.compare(buffer) == 0) {
                mDevices.push_back(device);
                return true;
            }
        }
        return false;
    }

    bool isSupportedOpListExpected(const std::vector<bool>& expected) {
        const uint32_t kMaxNumberOperations = 256;
        EXPECT_LE(expected.size(), kMaxNumberOperations);
        ANeuralNetworksModel* modelHandle = mModel.getHandle();
        bool supported[kMaxNumberOperations] = {false};
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(
                          modelHandle, mDevices.data(), mDevices.size(), supported),
                  ANEURALNETWORKS_NO_ERROR);
        return std::equal(expected.begin(), expected.end(), supported);
    }

    int prepareForExecution(bool measureTiming = false) {
        ANeuralNetworksModel* modelHandle = mModel.getHandle();
        int result = ANeuralNetworksCompilation_createForDevices(modelHandle, mDevices.data(),
                                                                 mDevices.size(), &mCompilation);
        if (result != ANEURALNETWORKS_NO_ERROR) {
            return result;
        }
        EXPECT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &mExecution),
                  ANEURALNETWORKS_NO_ERROR);
        if (measureTiming) {
            // Don't call setMeasureTiming unless we need to -- cannot call this
            // API unless there is exactly one device.
            EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(mExecution, true),
                      ANEURALNETWORKS_NO_ERROR);
        }
        return ANEURALNETWORKS_NO_ERROR;
    }

    std::vector<ANeuralNetworksDevice*> mDevices;
    ANeuralNetworksEvent* mEvent = nullptr;
    ANeuralNetworksExecution* mExecution = nullptr;
    ANeuralNetworksCompilation* mCompilation = nullptr;
    WrapperModel mModel;
};

void createSimpleAddModel(WrapperModel* model) {
    WrapperOperandType type0(WrapperType::TENSOR_FLOAT32, {2});
    WrapperOperandType type1(WrapperType::INT32, {});
    // Phase 1, operands
    auto op1 = model->addOperand(&type0);
    auto op2 = model->addOperand(&type0);
    auto act = model->addOperand(&type1);
    auto op3 = model->addOperand(&type0);
    // Phase 2, operations
    static int32_t act_init[] = {0};
    model->setOperandValue(act, act_init, sizeof(act_init));
    model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
    // Phase 3, inputs and outputs
    model->identifyInputsAndOutputs({op1, op2}, {op3});
    model->finish();
    ASSERT_TRUE(model->isValid());
}

// This test verifies that a simple ADD model is able to run on a single device that claims being
// able to handle all operations.
TEST_F(IntrospectionControlTest, SimpleAddModel) {
    // This is needed before we have the CPU fallback path being treated as a Device.
    // TODO(miaowang): remove once b/72506261 is fixed.
    if (DeviceManager::get()->getUseCpuOnly()) {
        GTEST_SKIP();
    }

    createSimpleAddModel(&mModel);

    std::string driverName = "test-all";
    std::vector<bool> ops(android::nn::kNumberOfOperationTypes, true);
    registerDevices({{driverName, 0.9, ops}});

    EXPECT_TRUE(selectDeviceByName(driverName));
    EXPECT_TRUE(isSupportedOpListExpected({true}));
    EXPECT_EQ(prepareForExecution(), ANEURALNETWORKS_NO_ERROR);

    // Verify that the mCompilation is actually using the "test-all" device.
    CompilationBuilder* c = reinterpret_cast<CompilationBuilder*>(mCompilation);
    const char* deviceNameBuffer =
            c->forTest_getExecutionPlan().forTest_simpleGetDevice()->getName();
    EXPECT_TRUE(driverName.compare(deviceNameBuffer) == 0);

    float input1[2] = {1.0f, 2.0f};
    float input2[2] = {3.0f, 4.0f};
    float output[2];
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 1, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(mExecution, true),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksExecution_startCompute(mExecution, &mEvent), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksEvent_wait(mEvent), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(output[0], input1[0] + input2[0]);
    EXPECT_EQ(output[1], input1[1] + input2[1]);

    uint64_t timeOnHardware, timeInDriver;
    EXPECT_EQ(ANeuralNetworksExecution_getDuration(mExecution, ANEURALNETWORKS_DURATION_ON_HARDWARE,
                                                   &timeOnHardware),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_getDuration(mExecution, ANEURALNETWORKS_DURATION_IN_DRIVER,
                                                   &timeInDriver),
              ANEURALNETWORKS_NO_ERROR);
    if (timeOnHardware != UINT64_MAX && timeInDriver != UINT64_MAX) {
        EXPECT_LE(timeOnHardware, timeInDriver);
    }
}

/*-- Begin timing tests -------------------------------------------------------------------------*/

namespace timing_tests {

constexpr Timing kGoodTiming = {.timeOnDevice = 123, .timeInDriver = 456};

enum class DriverKind {
    CPU,
    OLD,  // too old to support timing (1.1 or earlier)
    NEW   // new enough to support timing (1.2 or later)
};

std::ostream& operator<<(std::ostream& os, DriverKind kind) {
    const char* names[] = {"CPU", "OLD", "NEW"};
    const uint32_t index = static_cast<uint32_t>(kind);
    CHECK(index < std::size(names));
    return os << names[index];
}

enum class Success {
    // ASYNC: Return ErrorStatus::NONE; notify ErrorStatus::NONE and timing
    // SYNC, BURST: Return ErrorStatus::NONE and timing
    PASS_NEITHER,  // timing = kBadTiming
    PASS_DEVICE,   // timing = kGoodTiming.timeOnDevice, kBadTiming.timeInDriver
    PASS_DRIVER,   // timing = kBadTiming.timeOnDevice, kGoodTiming.timeInDriver
    PASS_BOTH,     // timing = kGoodTiming
    PASS_CPU,      // timing = { kBadTiming.timeOnDevice or 0, kBadTiming.timeInDriver or 0 }

    // ASYNC: Return ErrorStatus::GENERAL_FAILURE; notify ErrorStatus::GENERAL_FAILURE and
    // kBadTiming
    // SYNC, BURST: Return ErrorStatus::GENERAL_FAILURE and kBadTiming
    FAIL_LAUNCH,

    // ASYNC: Return ErrorStatus::NONE; notify ErrorStatus::GENERAL_FAILURE and kBadTiming
    FAIL_WAIT
};

std::ostream& operator<<(std::ostream& os, Success success) {
    const char* names[] = {"PASS_NEITHER", "PASS_DEVICE", "PASS_DRIVER", "PASS_BOTH",
                           "PASS_CPU",     "FAIL_LAUNCH", "FAIL_WAIT"};
    const uint32_t index = static_cast<uint32_t>(success);
    CHECK(index < std::size(names));
    return os << names[index];
}

std::map<Success, Timing> expectedTimingMap = {
        {Success::PASS_NEITHER, kBadTiming},
        {Success::PASS_DEVICE,
         {.timeOnDevice = kGoodTiming.timeOnDevice, .timeInDriver = kBadTiming.timeInDriver}},
        {Success::PASS_DRIVER,
         {.timeOnDevice = kBadTiming.timeOnDevice, .timeInDriver = kGoodTiming.timeInDriver}},
        {Success::PASS_BOTH, kGoodTiming},
        {Success::FAIL_LAUNCH, kBadTiming},
        {Success::FAIL_WAIT, kBadTiming}};

std::set<Success> expectedPassSet = {Success::PASS_NEITHER, Success::PASS_DEVICE,
                                     Success::PASS_DRIVER, Success::PASS_BOTH, Success::PASS_CPU};

enum class Compute { ASYNC, SYNC, BURST };

std::ostream& operator<<(std::ostream& os, Compute compute) {
    const char* names[] = {"ASYNC", "SYNC", "BURST"};
    const uint32_t index = static_cast<uint32_t>(compute);
    CHECK(index < std::size(names));
    return os << names[index];
}

// For these tests we don't care about actually running an inference -- we
// just want to dummy up execution status and timing results.
class TestPreparedModel12 : public SamplePreparedModel {
   public:
    TestPreparedModel12(const HidlModel& model, const SampleDriver* driver, Success success)
        : SamplePreparedModel(model, driver), mSuccess(success) {}

    Return<ErrorStatus> execute(const Request&,
                                const sp<V1_0::IExecutionCallback>& callback) override {
        switch (mSuccess) {
            case Success::PASS_NEITHER:
                callback->notify(ErrorStatus::NONE);
                return ErrorStatus::NONE;
            case Success::FAIL_LAUNCH:
                callback->notify(ErrorStatus::GENERAL_FAILURE);
                return ErrorStatus::GENERAL_FAILURE;
            case Success::FAIL_WAIT:
                callback->notify(ErrorStatus::GENERAL_FAILURE);
                return ErrorStatus::NONE;
            default:
                ADD_FAILURE() << "Unexpected Success kind";
                return ErrorStatus::GENERAL_FAILURE;
        }
    }

    Return<ErrorStatus> execute_1_2(const Request&, MeasureTiming measure,
                                    const sp<V1_2::IExecutionCallback>& callback) override {
        EXPECT_EQ(measure, MeasureTiming::YES);
        switch (mSuccess) {
            case Success::PASS_NEITHER:
            case Success::PASS_DEVICE:
            case Success::PASS_DRIVER:
            case Success::PASS_BOTH:
                callback->notify_1_2(ErrorStatus::NONE, {}, expectedTimingMap.at(mSuccess));
                return ErrorStatus::NONE;
            case Success::FAIL_LAUNCH:
                callback->notify(ErrorStatus::GENERAL_FAILURE);
                return ErrorStatus::GENERAL_FAILURE;
            case Success::FAIL_WAIT:
                callback->notify(ErrorStatus::GENERAL_FAILURE);
                return ErrorStatus::NONE;
            default:
                ADD_FAILURE() << "Unexpected Success kind";
                return ErrorStatus::GENERAL_FAILURE;
        }
    }

    Return<void> executeSynchronously(const Request&, MeasureTiming measure,
                                      executeSynchronously_cb cb) override {
        EXPECT_EQ(measure, MeasureTiming::YES);
        switch (mSuccess) {
            case Success::PASS_NEITHER:
            case Success::PASS_DEVICE:
            case Success::PASS_DRIVER:
            case Success::PASS_BOTH:
                cb(ErrorStatus::NONE, {}, expectedTimingMap.at(mSuccess));
                return Void();
            case Success::FAIL_LAUNCH:
            case Success::FAIL_WAIT:
                // While this is a synchronous execution method, the NNAPI
                // runtime may call it even for asynchronous execution, so we
                // need to tolerate Success::FAIL_WAIT here, not just
                // Success::FAIL_LAUNCH.
                cb(ErrorStatus::GENERAL_FAILURE, {}, kBadTiming);
                return Void();
            default:
                ADD_FAILURE() << "Unexpected Success kind";
                cb(ErrorStatus::GENERAL_FAILURE, {}, kBadTiming);
                return Void();
        }
    }

    // ExecutionBurstServer::create has an overload that will use
    // IPreparedModel::executeSynchronously(), so we can rely on that, rather
    // than having to implement ExecutionBurstServer::IExecutorWithCache.
    Return<void> configureExecutionBurst(
            const sp<V1_2::IBurstCallback>& callback,
            const MQDescriptorSync<V1_2::FmqRequestDatum>& requestChannel,
            const MQDescriptorSync<V1_2::FmqResultDatum>& resultChannel,
            configureExecutionBurst_cb cb) override {
        const sp<V1_2::IBurstContext> burst =
                ExecutionBurstServer::create(callback, requestChannel, resultChannel, this);

        cb(burst == nullptr ? ErrorStatus::GENERAL_FAILURE : ErrorStatus::NONE, burst);
        return Void();
    }

   private:
    Success mSuccess;
};

// Like TestPreparedModel12, but implementing 1.0
class TestPreparedModel10 : public V1_0::IPreparedModel {
   public:
    TestPreparedModel10(const HidlModel& model, const SampleDriver* driver, Success success)
        : m12PreparedModel(new TestPreparedModel12(model, driver, success)) {}

    Return<ErrorStatus> execute(const Request& request,
                                const sp<V1_0::IExecutionCallback>& callback) override {
        return m12PreparedModel->execute(request, callback);
    }

   private:
    const sp<V1_2::IPreparedModel> m12PreparedModel;
};

// Behaves like SampleDriver, except that it produces customized IPrepareModel.
class TestDriver12 : public SampleDriver {
   public:
    TestDriver12(const std::string& name, Success success)
        : SampleDriver(name.c_str()), mSuccess(success) {}

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

    Return<ErrorStatus> prepareModel_1_2(const HidlModel& model, ExecutionPreference,
                                         const hidl_vec<hidl_handle>&, const hidl_vec<hidl_handle>&,
                                         const HidlToken&,
                                         const sp<IPreparedModelCallback>& callback) override {
        callback->notify_1_2(ErrorStatus::NONE, new TestPreparedModel12(model, this, mSuccess));
        return ErrorStatus::NONE;
    }

    Return<ErrorStatus> prepareModel_1_1(
            const V1_1::Model& model, ExecutionPreference,
            const sp<V1_0::IPreparedModelCallback>& callback) override {
        callback->notify(ErrorStatus::NONE,
                         new TestPreparedModel10(nn::convertToV1_2(model), this, mSuccess));
        return ErrorStatus::NONE;
    }

    Return<ErrorStatus> prepareModel(const V1_0::Model& model,
                                     const sp<V1_0::IPreparedModelCallback>& callback) override {
        return prepareModel_1_1(nn::convertToV1_1(model), ExecutionPreference::FAST_SINGLE_ANSWER,
                                callback);
    }

   private:
    Success mSuccess;
};

// Like TestDriver, but implementing 1.1
class TestDriver11 : public V1_1::IDevice {
   public:
    TestDriver11(const std::string& name, Success success)
        : m12Driver(new TestDriver12(name, success)) {}
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

class TimingTest : public IntrospectionControlTest,
                   public ::testing::WithParamInterface<std::tuple<DriverKind, Success, Compute>> {
   public:
    TimingTest()
        : kDriverKind(std::get<0>(GetParam())),
          kSuccess(std::get<1>(GetParam())),
          kCompute(std::get<2>(GetParam())) {}

   protected:
    const DriverKind kDriverKind;
    const Success kSuccess;
    const Compute kCompute;
};

TEST_P(TimingTest, Test) {
    // There's no straightforward way to force CPU execution to fail.
    ASSERT_EQ(kDriverKind == DriverKind::CPU, kSuccess == Success::PASS_CPU);

    // FAIL_WAIT only makes sense for ASYNC.
    ASSERT_TRUE(kCompute == Compute::ASYNC || kSuccess != Success::FAIL_WAIT);

    if (DeviceManager::get()->getUseCpuOnly() != (kDriverKind == DriverKind::CPU)) {
        // We don't have an elegant way to request the CPU driver.  Therefore,
        // we rely on our test framework to make the choice between CPU and
        // non-CPU.
        GTEST_SKIP();
    }

    createSimpleAddModel(&mModel);

    switch (kDriverKind) {
        case DriverKind::CPU: {
            // There should be only one driver -- the CPU
            const char* name = DeviceManager::get()->getDrivers()[0]->getName();
            ASSERT_TRUE(selectDeviceByName(name));
            break;
        }
        case DriverKind::OLD: {
            static const char name[] = "old";
            DeviceManager::get()->forTest_registerDevice(name, new TestDriver11(name, kSuccess));
            ASSERT_TRUE(selectDeviceByName(name));
            break;
        }
        case DriverKind::NEW: {
            static const char name[] = "new";
            DeviceManager::get()->forTest_registerDevice(name, new TestDriver12(name, kSuccess));
            ASSERT_TRUE(selectDeviceByName(name));
            break;
        }
        default:
            FAIL() << "Unexpected DriverKind";
    }

    EXPECT_EQ(prepareForExecution(true /*measureTiming*/), ANEURALNETWORKS_NO_ERROR);

    float input1[2] = {1.0f, 2.0f};
    float input2[2] = {3.0f, 4.0f};
    float output[2];
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 1, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(mExecution, true),
              ANEURALNETWORKS_NO_ERROR);

    auto Check = [](bool expectPass, int result) {
        if (expectPass) {
            ASSERT_EQ(result, ANEURALNETWORKS_NO_ERROR);
        } else {
            ASSERT_NE(result, ANEURALNETWORKS_NO_ERROR);
        }
    };

    const bool isPass = expectedPassSet.count(kSuccess) != 0;

    switch (kCompute) {
        case Compute::ASYNC: {
            // Ideally what we'd like to do here is
            //
            //     Check(kSuccess != Success::FAIL_LAUNCH,
            //         ANeuralNetworksExecution_startCompute(mExecution, &mEvent));
            //     Check(isPass, ANeuralNetworksEvent_wait(mEvent));
            //
            // However, in the current implementation of the runtime, a launch
            // failure at the HAL level does not show up as a launch failure at
            // the NDK level ("startCompute"): The NNAPI runtime does not call a
            // driver until it (the runtime) begins execution, so a launch
            // failure at the HAL level looks like an execution failure at the
            // NDK level ("wait").
            SCOPED_TRACE("ASYNC startCompute");
            Check(true,  // rather than kSuccess != Success::FAIL_LAUNCH
                  ANeuralNetworksExecution_startCompute(mExecution, &mEvent));
            SCOPED_TRACE("ASYNC wait");
            Check(isPass, ANeuralNetworksEvent_wait(mEvent));
            break;
        }
        case Compute::SYNC: {
            SCOPED_TRACE("SYNC");
            Check(isPass, ANeuralNetworksExecution_compute(mExecution));
            break;
        }
        case Compute::BURST: {
            SCOPED_TRACE("BURST");
            ANeuralNetworksBurst* burst;
            ASSERT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst), ANEURALNETWORKS_NO_ERROR);
            Check(isPass, ANeuralNetworksExecution_burstCompute(mExecution, burst));
            ANeuralNetworksBurst_free(burst);
            break;
        }
        default:
            FAIL() << "unreachable";
    }

    uint64_t timeOnHardware, timeInDriver;
    EXPECT_EQ(ANeuralNetworksExecution_getDuration(mExecution, ANEURALNETWORKS_DURATION_ON_HARDWARE,
                                                   &timeOnHardware),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_getDuration(mExecution, ANEURALNETWORKS_DURATION_IN_DRIVER,
                                                   &timeInDriver),
              ANEURALNETWORKS_NO_ERROR);
    switch (kDriverKind) {
        case DriverKind::CPU: {
            // TODO: Should we require timing to be reported as 0?
            EXPECT_TRUE(timeOnHardware == 0 || timeOnHardware == UINT64_MAX)
                    << "timeOnHardware = " << timeOnHardware;
            EXPECT_TRUE(timeInDriver == 0 || timeInDriver == UINT64_MAX)
                    << "timeInDriver = " << timeOnHardware;
            break;
        }
        case DriverKind::OLD: {
            EXPECT_EQ(timeOnHardware, UINT64_MAX);
            EXPECT_EQ(timeInDriver, UINT64_MAX);
            break;
        }
        case DriverKind::NEW: {
            auto microsToNanos = [](uint64_t micros) {
                constexpr uint64_t kNanosPerMicro = 1000;
                return micros == UINT64_MAX ? UINT64_MAX : kNanosPerMicro * micros;
            };
            const Timing expectedTiming = expectedTimingMap.at(kSuccess);
            EXPECT_EQ(timeOnHardware, microsToNanos(expectedTiming.timeOnDevice));
            EXPECT_EQ(timeInDriver, microsToNanos(expectedTiming.timeInDriver));
            break;
        }
        default:
            FAIL() << "unreachable";
    }
    if (timeOnHardware != UINT64_MAX && timeInDriver != UINT64_MAX) {
        EXPECT_LE(timeOnHardware, timeInDriver);
    }
}

auto kTimingTestValues = ::testing::Values(
        // NOTE: We cannot force CPU execution to fail
        std::make_tuple(DriverKind::CPU, Success::PASS_CPU, Compute::ASYNC),
        std::make_tuple(DriverKind::CPU, Success::PASS_CPU, Compute::SYNC),
        std::make_tuple(DriverKind::CPU, Success::PASS_CPU, Compute::BURST),

        // NOTE: OLD driver does not provide timing
        std::make_tuple(DriverKind::OLD, Success::PASS_NEITHER, Compute::ASYNC),
        std::make_tuple(DriverKind::OLD, Success::PASS_NEITHER, Compute::SYNC),
        std::make_tuple(DriverKind::OLD, Success::PASS_NEITHER, Compute::BURST),

        std::make_tuple(DriverKind::OLD, Success::FAIL_LAUNCH, Compute::ASYNC),
        std::make_tuple(DriverKind::OLD, Success::FAIL_LAUNCH, Compute::SYNC),
        std::make_tuple(DriverKind::OLD, Success::FAIL_LAUNCH, Compute::BURST),

        // NOTE: Only ASYNC is paired with a wait
        std::make_tuple(DriverKind::OLD, Success::FAIL_WAIT, Compute::ASYNC),

        std::make_tuple(DriverKind::NEW, Success::PASS_NEITHER, Compute::ASYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_NEITHER, Compute::SYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_NEITHER, Compute::BURST),

        std::make_tuple(DriverKind::NEW, Success::PASS_DEVICE, Compute::ASYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_DEVICE, Compute::SYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_DEVICE, Compute::BURST),

        std::make_tuple(DriverKind::NEW, Success::PASS_DRIVER, Compute::ASYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_DRIVER, Compute::SYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_DRIVER, Compute::BURST),

        std::make_tuple(DriverKind::NEW, Success::PASS_BOTH, Compute::ASYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_BOTH, Compute::SYNC),
        std::make_tuple(DriverKind::NEW, Success::PASS_BOTH, Compute::BURST),

        std::make_tuple(DriverKind::NEW, Success::FAIL_LAUNCH, Compute::ASYNC),
        std::make_tuple(DriverKind::NEW, Success::FAIL_LAUNCH, Compute::SYNC),
        std::make_tuple(DriverKind::NEW, Success::FAIL_LAUNCH, Compute::BURST),

        // NOTE: Only ASYNC is paired with a wait
        std::make_tuple(DriverKind::NEW, Success::FAIL_WAIT, Compute::ASYNC));

INSTANTIATE_TEST_CASE_P(Flavor, TimingTest, kTimingTestValues);

}  // namespace timing_tests

/*-- End   timing tests -------------------------------------------------------------------------*/

const float kSimpleMultiplier = 2.0f;

void createAddMulModel(WrapperModel* model, bool reverseOrder) {
    WrapperOperandType type0(WrapperType::TENSOR_FLOAT32, {2});
    WrapperOperandType type1(WrapperType::INT32, {});
    // Phase 1, operands
    auto op1 = model->addOperand(&type0);
    auto op2 = model->addOperand(&type0);
    auto act = model->addOperand(&type1);
    auto op3 = model->addOperand(&type0);
    auto op4 = model->addOperand(&type0);
    auto op5 = model->addOperand(&type0);
    // Phase 2, operations
    static int32_t act_init[] = {0};
    model->setOperandValue(act, act_init, sizeof(act_init));
    static float multiplier[] = {kSimpleMultiplier, kSimpleMultiplier};
    model->setOperandValue(op4, multiplier, sizeof(multiplier));
    if (reverseOrder) {
        // In this case, add MUL first, but the execution order is still ADD -> MUL.
        model->addOperation(ANEURALNETWORKS_MUL, {op3, op4, act}, {op5});
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
    } else {
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
        model->addOperation(ANEURALNETWORKS_MUL, {op3, op4, act}, {op5});
    }
    // Phase 3, inputs and outputs
    model->identifyInputsAndOutputs({op1, op2}, {op5});
    model->finish();
    ASSERT_TRUE(model->isValid());
}

// TODO(miaowang): add a test to make sure ANNCompilation_create() has CPU
// fallback.
// This test verifies that a device that could only handle ADD would correctly report that an
// ADD->MUL model could not be fully supported.
TEST_F(IntrospectionControlTest, PartialModelNotSupported) {
    // This is needed before we have the CPU fallback path being treated as a Device.
    // TODO(miaowang): remove once b/72506261 is fixed.
    if (DeviceManager::get()->getUseCpuOnly()) {
        GTEST_SKIP();
    }

    createAddMulModel(&mModel, false);

    std::string addOnlyDriver = "test-onlyAdd";
    std::vector<bool> addOnlyOp(android::nn::kNumberOfOperationTypes, false);
    addOnlyOp[ANEURALNETWORKS_ADD] = true;

    registerDevices({{addOnlyDriver, 0.9, addOnlyOp}});

    EXPECT_TRUE(selectDeviceByName(addOnlyDriver));
    EXPECT_TRUE(isSupportedOpListExpected({true, false}));

    ANeuralNetworksModel* modelHandle = mModel.getHandle();
    EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(modelHandle, mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    // The compilation must fail as there is no fallback when using
    // Introspection API.
    EXPECT_NE(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
}

// This test verifies that a device that could only handle ADD would correctly report that an
// ADD->MUL model could not be fully supported. Also verifies that the indices of returned
// supported op list correctly map to the order of operations being added by the user.
TEST_F(IntrospectionControlTest, PartialModelNotSupportedOrder) {
    // This is needed before we have the CPU fallback path being treated as a Device.
    // TODO(miaowang): remove once b/72506261 is fixed.
    if (DeviceManager::get()->getUseCpuOnly()) {
        GTEST_SKIP();
    }

    createAddMulModel(&mModel, true);

    std::string addOnlyDriver = "test-onlyAdd";
    std::vector<bool> addOnlyOp(android::nn::kNumberOfOperationTypes, false);
    addOnlyOp[ANEURALNETWORKS_ADD] = true;

    registerDevices({{addOnlyDriver, 0.9, addOnlyOp}});

    EXPECT_TRUE(selectDeviceByName(addOnlyDriver));
    EXPECT_TRUE(isSupportedOpListExpected({false, true}));
}

// TODO(miaowang): update the test to make sure the model is actually running on the test devices.
// This test verifies that an ADD->MUL model is able to run on two selected devices that together
// can handle all operations.
TEST_F(IntrospectionControlTest, ModelNeedTwoDevices) {
    // This is needed before we have the CPU fallback path being treated as a Device.
    // TODO(miaowang): remove once b/72506261 is fixed.
    if (DeviceManager::get()->getUseCpuOnly()) {
        GTEST_SKIP();
    }

    createAddMulModel(&mModel, false);

    std::string addOnlyDriver = "test-onlyAdd";
    std::vector<bool> addOnlyOp(android::nn::kNumberOfOperationTypes, false);
    addOnlyOp[ANEURALNETWORKS_ADD] = true;

    std::string mulOnlyDriver = "test-onlyMul";
    std::vector<bool> mulOnlyOp(android::nn::kNumberOfOperationTypes, false);
    mulOnlyOp[ANEURALNETWORKS_MUL] = true;

    registerDevices({
            {addOnlyDriver, 0.9, addOnlyOp},
            {mulOnlyDriver, 0.9, mulOnlyOp},
    });

    EXPECT_TRUE(selectDeviceByName(addOnlyDriver));
    EXPECT_TRUE(selectDeviceByName(mulOnlyDriver));
    EXPECT_TRUE(isSupportedOpListExpected({true, true}));
    EXPECT_EQ(prepareForExecution(), ANEURALNETWORKS_NO_ERROR);

    float input1[2] = {1.0f, 2.0f};
    float input2[2] = {3.0f, 4.0f};
    float output[2];
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 1, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksExecution_startCompute(mExecution, &mEvent), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksEvent_wait(mEvent), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(output[0], kSimpleMultiplier * (input1[0] + input2[0]));
    EXPECT_EQ(output[1], kSimpleMultiplier * (input1[1] + input2[1]));
}
}  // namespace
