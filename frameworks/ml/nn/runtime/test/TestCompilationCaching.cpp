/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "Manager.h"
#include "SampleDriver.h"
#include "TestNeuralNetworksWrapper.h"

#include <gtest/gtest.h>
#include <cstdlib>
#include <filesystem>
#include <numeric>

using namespace android::nn;
using Result = test_wrapper::Result;
using Type = test_wrapper::Type;
using HidlToken = hidl_array<uint8_t, ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN>;
const Timing kBadTiming = {.timeOnDevice = UINT64_MAX, .timeInDriver = UINT64_MAX};
template <typename T>
using MQDescriptorSync = ::android::hardware::MQDescriptorSync<T>;

namespace android::hardware::neuralnetworks::V1_0 {

::std::ostream& operator<<(::std::ostream& os, ErrorStatus errorStatus) {
    return os << toString(errorStatus);
}

}  // namespace android::hardware::neuralnetworks::V1_0

namespace {

enum class HasCalledPrepareModel { NO, WITHOUT_CACHING, WITH_CACHING };

// Whether the driver supports caching based on the returns from getNumberOfCacheFilesNeeded.
bool isCachingSupportedAndNoError(ErrorStatus error, uint32_t numModelCache,
                                  uint32_t numDataCache) {
    return error == ErrorStatus::NONE &&
           numModelCache <= static_cast<uint32_t>(Constant::MAX_NUMBER_OF_CACHE_FILES) &&
           numDataCache <= static_cast<uint32_t>(Constant::MAX_NUMBER_OF_CACHE_FILES) &&
           (numModelCache != 0 || numDataCache != 0);
}

// This is an IDevice for testing purposes which overrides several methods from sample driver:
// - supports all the operations and is faster than cpu fallback.
// - overrides getNumberOfCacheFilesNeeded to report according to given parameters.
// - overrides prepareModelFromCache to return error status according to
//   mErrorStatusPrepareFromCache.
// - produces CachingPreparedModel on prepareModel and prepareModelFromCache.
//
// The cache entry is written by prepareModel_1_2 and is checked later by
// CachingDriver::prepareModelFromCache.
//
// The CachingDriver has 2 flags mHasCalledPrepareModelFromCache and mHasCalledPrepareModel
// to check if the correct methods are invoked by the runtime.
class CachingDriver : public sample_driver::SampleDriver {
   private:
    static constexpr size_t kCacheSize = 256;

    class CachingPreparedModel : public IPreparedModel {
       public:
        CachingPreparedModel() = default;

        Return<ErrorStatus> execute(const Request&, const sp<V1_0::IExecutionCallback>&) override {
            return ErrorStatus::DEVICE_UNAVAILABLE;
        }
        Return<ErrorStatus> execute_1_2(const Request&, MeasureTiming,
                                        const sp<V1_2::IExecutionCallback>&) override {
            return ErrorStatus::DEVICE_UNAVAILABLE;
        }
        Return<void> executeSynchronously(const Request&, MeasureTiming,
                                          executeSynchronously_cb cb) override {
            cb(ErrorStatus::DEVICE_UNAVAILABLE, {}, kBadTiming);
            return Void();
        }
        Return<void> configureExecutionBurst(const sp<V1_2::IBurstCallback>&,
                                             const MQDescriptorSync<V1_2::FmqRequestDatum>&,
                                             const MQDescriptorSync<V1_2::FmqResultDatum>&,
                                             configureExecutionBurst_cb cb) override {
            cb(ErrorStatus::DEVICE_UNAVAILABLE, nullptr);
            return Void();
        }
    };

   public:
    CachingDriver(const char* name, ErrorStatus errorStatusGetNumCacheFiles, uint32_t numModelCache,
                  uint32_t numDataCache, ErrorStatus errorStatusPrepareFromCache)
        : SampleDriver(name),
          mErrorStatusGetNumCacheFiles(errorStatusGetNumCacheFiles),
          mNumModelCache(numModelCache),
          mNumDataCache(numDataCache),
          mErrorStatusPrepareFromCache(errorStatusPrepareFromCache) {
        mModelCacheData.resize(kCacheSize);
        std::iota(mModelCacheData.begin(), mModelCacheData.end(), 0);
        mDataCacheData.resize(kCacheSize);
        std::iota(mDataCacheData.begin(), mDataCacheData.end(), 1);
    }
    ~CachingDriver() override {}

    // Reports faster than cpu.
    Return<void> getCapabilities_1_2(getCapabilities_1_2_cb cb) override {
        android::nn::initVLogMask();
        const PerformanceInfo kPerf = {.execTime = 0.1, .powerUsage = 0.1};
        Capabilities capabilities = {
                .relaxedFloat32toFloat16PerformanceScalar = kPerf,
                .relaxedFloat32toFloat16PerformanceTensor = kPerf,
                .operandPerformance = android::nn::nonExtensionOperandPerformance(kPerf)};
        cb(ErrorStatus::NONE, capabilities);
        return Void();
    }

    // Reports supporting all operations.
    Return<void> getSupportedOperations_1_2(const Model& model,
                                            getSupportedOperations_cb cb) override {
        std::vector<bool> supported(model.operations.size(), true);
        cb(ErrorStatus::NONE, supported);
        return Void();
    }

    // Reports according to mGetNumCacheFiles.
    Return<void> getNumberOfCacheFilesNeeded(getNumberOfCacheFilesNeeded_cb cb) override {
        cb(mErrorStatusGetNumCacheFiles, mNumModelCache, mNumDataCache);
        return Void();
    }

    // Generates CachingPreparedModel.
    // Writes the cache entry per mCacheXData and sets mHasCalledPrepareModel.
    Return<ErrorStatus> prepareModel_1_2(const Model&, ExecutionPreference,
                                         const hidl_vec<hidl_handle>& modelCacheHandle,
                                         const hidl_vec<hidl_handle>& dataCacheHandle,
                                         const HidlToken&,
                                         const sp<IPreparedModelCallback>& cb) override {
        checkNumberOfCacheHandles(modelCacheHandle.size(), dataCacheHandle.size());
        if (modelCacheHandle.size() != 0 || dataCacheHandle.size() != 0) {
            writeToCache(modelCacheHandle, mModelCacheData);
            writeToCache(dataCacheHandle, mDataCacheData);
            mHasCalledPrepareModel = HasCalledPrepareModel::WITH_CACHING;
        } else {
            mHasCalledPrepareModel = HasCalledPrepareModel::WITHOUT_CACHING;
        }
        cb->notify_1_2(ErrorStatus::NONE, new CachingPreparedModel());
        return ErrorStatus::NONE;
    }

    // Checks if the cache entry is correct, notifies error status according to
    // mErrorStatusPrepareFromCache, sets mHasCalledPrepareModelFromCache.
    Return<ErrorStatus> prepareModelFromCache(
            const hidl_vec<hidl_handle>& modelCacheHandle,
            const hidl_vec<hidl_handle>& dataCacheHandle, const HidlToken&,
            const sp<V1_2::IPreparedModelCallback>& callback) override {
        readFromCache(modelCacheHandle, mModelCacheData);
        readFromCache(dataCacheHandle, mDataCacheData);
        mHasCalledPrepareModelFromCache = true;
        if (mErrorStatusPrepareFromCache == ErrorStatus::NONE) {
            callback->notify_1_2(mErrorStatusPrepareFromCache, new CachingPreparedModel());
        } else {
            callback->notify_1_2(mErrorStatusPrepareFromCache, nullptr);
        }
        return ErrorStatus::NONE;
    };

    bool hasCalledPrepareModelFromCache() const { return mHasCalledPrepareModelFromCache; }
    HasCalledPrepareModel hasCalledPrepareModel() const { return mHasCalledPrepareModel; }

   private:
    // Checks the number of cache files passed to the driver from runtime.
    void checkNumberOfCacheHandles(size_t modelCache, size_t dataCache) {
        if (isCachingSupportedAndNoError(mErrorStatusGetNumCacheFiles, mNumModelCache,
                                         mNumDataCache)) {
            if (modelCache != 0 || dataCache != 0) {
                ASSERT_EQ(modelCache, mNumModelCache);
                ASSERT_EQ(dataCache, mNumDataCache);
            }
        } else {
            ASSERT_EQ(modelCache, 0ul);
            ASSERT_EQ(dataCache, 0ul);
        }
    }

    void writeToCache(const hidl_vec<hidl_handle>& handles, const std::vector<uint8_t>& cache) {
        for (uint32_t i = 0; i < handles.size(); ++i) {
            ASSERT_EQ(handles[i]->numFds, 1);
            EXPECT_EQ(write(handles[i]->data[0], cache.data(), kCacheSize),
                      static_cast<ssize_t>(kCacheSize));
        }
    }

    void readFromCache(const hidl_vec<hidl_handle>& handles, const std::vector<uint8_t>& expected) {
        for (uint32_t i = 0; i < handles.size(); ++i) {
            ASSERT_EQ(handles[i]->numFds, 1);
            std::vector<uint8_t> actual(kCacheSize);
            EXPECT_EQ(read(handles[i]->data[0], actual.data(), kCacheSize),
                      static_cast<ssize_t>(kCacheSize));
            EXPECT_EQ(actual, expected);
        }
    }

    std::vector<uint8_t> mModelCacheData;
    std::vector<uint8_t> mDataCacheData;

    const ErrorStatus mErrorStatusGetNumCacheFiles;
    const uint32_t mNumModelCache;
    const uint32_t mNumDataCache;
    const ErrorStatus mErrorStatusPrepareFromCache;

    bool mHasCalledPrepareModelFromCache = false;
    HasCalledPrepareModel mHasCalledPrepareModel = HasCalledPrepareModel::NO;
};

void CreateBroadcastAddModel(test_wrapper::Model* model) {
    test_wrapper::OperandType matrixType(Type::TENSOR_FLOAT32, {2, 2});
    test_wrapper::OperandType vectorType(Type::TENSOR_FLOAT32, {2});
    test_wrapper::OperandType scalarType(Type::INT32, {});
    int32_t activation(ANEURALNETWORKS_FUSED_NONE);
    auto a = model->addOperand(&matrixType);
    auto b = model->addOperand(&vectorType);
    auto c = model->addOperand(&matrixType);
    auto d = model->addOperand(&scalarType);
    model->setOperandValue(d, &activation, sizeof(activation));
    model->addOperation(ANEURALNETWORKS_ADD, {a, b, d}, {c});
    model->identifyInputsAndOutputs({a, b}, {c});
    ASSERT_TRUE(model->isValid());
    ASSERT_EQ(model->finish(), Result::NO_ERROR);
}

// Test model compilation with a driver parameterized with
// - ErrorStatus returning from getNumberOfCacheFilesNeeded
// - Number of model cache files returning from getNumberOfCacheFilesNeeded
// - Number of data cache files returning from getNumberOfCacheFilesNeeded
// - ErrorStatus returning from prepareModelFromCache
using CompilationCachingTestParam = std::tuple<ErrorStatus, uint32_t, uint32_t, ErrorStatus>;

class CompilationCachingTest : public ::testing::TestWithParam<CompilationCachingTestParam> {
   protected:
    virtual void SetUp() override {
        char cacheDirTemp[] = "/data/local/tmp/TestCompilationCachingXXXXXX";
        char* cacheDir = mkdtemp(cacheDirTemp);
        ASSERT_NE(cacheDir, nullptr);
        mCacheDir = cacheDir;
        CreateBroadcastAddModel(&mModel);
        mToken = std::vector<uint8_t>(ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN, 0);
        mIsCachingSupportedAndNoError = isCachingSupportedAndNoError(kErrorStatusGetNumCacheFiles,
                                                                     kNumModelCache, kNumDataCache);
    }

    virtual void TearDown() override {
        if (!::testing::Test::HasFailure()) {
            std::filesystem::remove_all(mCacheDir);
        }
    }

    void compileModel(const sp<CachingDriver>& driver, bool withToken) {
        DeviceManager::get()->forTest_registerDevice(kDeviceName, driver);

        // Make device list including only a single driver device.
        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
        EXPECT_GE(numDevices, (uint32_t)1);
        std::vector<ANeuralNetworksDevice*> devices;
        for (uint32_t i = 0; i < numDevices; i++) {
            ANeuralNetworksDevice* device = nullptr;
            EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
            const char* buffer = nullptr;
            int result = ANeuralNetworksDevice_getName(device, &buffer);
            if (result == ANEURALNETWORKS_NO_ERROR && strcmp(buffer, kDeviceName) == 0) {
                devices.push_back(device);
                break;
            }
        }
        ASSERT_EQ(devices.size(), 1u);

        // Compile the model with the device.
        ANeuralNetworksCompilation* compilation = nullptr;
        ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), devices.data(),
                                                              devices.size(), &compilation),
                  ANEURALNETWORKS_NO_ERROR);
        if (withToken) {
            ASSERT_EQ(ANeuralNetworksCompilation_setCaching(compilation, mCacheDir.c_str(),
                                                            mToken.data()),
                      ANEURALNETWORKS_NO_ERROR);
        }
        ASSERT_EQ(ANeuralNetworksCompilation_finish(compilation), ANEURALNETWORKS_NO_ERROR);

        DeviceManager::get()->forTest_reInitializeDeviceList();
    }

    void createCache() {
        sp<CachingDriver> driver = new CachingDriver(kDeviceName, ErrorStatus::NONE, kNumModelCache,
                                                     kNumDataCache, ErrorStatus::NONE);
        compileModel(driver, /*withToken=*/true);
    }

    static constexpr char kDeviceName[] = "deviceTestCompilationCaching";
    const ErrorStatus kErrorStatusGetNumCacheFiles = std::get<0>(GetParam());
    const uint32_t kNumModelCache = std::get<1>(GetParam());
    const uint32_t kNumDataCache = std::get<2>(GetParam());
    const ErrorStatus kErrorStatusPrepareFromCache = std::get<3>(GetParam());
    bool mIsCachingSupportedAndNoError;
    test_wrapper::Model mModel;
    std::string mCacheDir;
    std::vector<uint8_t> mToken;
};

TEST_P(CompilationCachingTest, TokenProvidedAndCacheNotExist) {
    if (DeviceManager::get()->getUseCpuOnly()) {
        return;
    }
    sp<CachingDriver> driver =
            new CachingDriver(kDeviceName, kErrorStatusGetNumCacheFiles, kNumModelCache,
                              kNumDataCache, kErrorStatusPrepareFromCache);
    compileModel(driver, /*withToken=*/true);

    // When cache file does not exist, the runtime should never call prepareModelFromCache.
    EXPECT_EQ(driver->hasCalledPrepareModelFromCache(), false);

    // The runtime should call prepareModel_1_2. It should request caching iff caching supported.
    EXPECT_EQ(driver->hasCalledPrepareModel(), mIsCachingSupportedAndNoError
                                                       ? HasCalledPrepareModel::WITH_CACHING
                                                       : HasCalledPrepareModel::WITHOUT_CACHING);
}

TEST_P(CompilationCachingTest, TokenProvidedAndCacheExist) {
    if (DeviceManager::get()->getUseCpuOnly()) {
        return;
    }
    createCache();
    sp<CachingDriver> driver =
            new CachingDriver(kDeviceName, kErrorStatusGetNumCacheFiles, kNumModelCache,
                              kNumDataCache, kErrorStatusPrepareFromCache);
    compileModel(driver, /*withToken=*/true);

    // When cache files exist, the runtime should call prepareModelFromCache iff caching supported.
    EXPECT_EQ(driver->hasCalledPrepareModelFromCache(), mIsCachingSupportedAndNoError);

    HasCalledPrepareModel expectHasCalledPrepareModel;
    if (mIsCachingSupportedAndNoError) {
        if (kErrorStatusPrepareFromCache == ErrorStatus::NONE) {
            // The runtime should not call prepareModel_1_2 iff caching supported and
            // prepareModelFromCache succeeds.
            expectHasCalledPrepareModel = HasCalledPrepareModel::NO;
        } else {
            // The runtime should call prepareModel_1_2 and request caching iff caching supported
            // but prepareModelFromCache fails.
            expectHasCalledPrepareModel = HasCalledPrepareModel::WITH_CACHING;
        }
    } else {
        // The runtime should call prepareModel_1_2 without caching iff caching not supported.
        expectHasCalledPrepareModel = HasCalledPrepareModel::WITHOUT_CACHING;
    }
    EXPECT_EQ(driver->hasCalledPrepareModel(), expectHasCalledPrepareModel);
}

TEST_P(CompilationCachingTest, TokenNotProvided) {
    if (DeviceManager::get()->getUseCpuOnly()) {
        return;
    }
    sp<CachingDriver> driver =
            new CachingDriver(kDeviceName, kErrorStatusGetNumCacheFiles, kNumModelCache,
                              kNumDataCache, kErrorStatusPrepareFromCache);
    compileModel(driver, /*withToken=*/false);

    // When no NDK token is provided by the client, the runtime should never call
    // prepareModelFromCache or request caching with prepareModel_1_2.
    EXPECT_EQ(driver->hasCalledPrepareModelFromCache(), false);
    EXPECT_EQ(driver->hasCalledPrepareModel(), HasCalledPrepareModel::WITHOUT_CACHING);
}

static const auto kErrorStatusGetNumCacheFilesChoices =
        testing::Values(ErrorStatus::NONE, ErrorStatus::DEVICE_UNAVAILABLE);
static const auto kNumCacheChoices =
        testing::Values(0ul, 1ul, static_cast<uint32_t>(Constant::MAX_NUMBER_OF_CACHE_FILES),
                        static_cast<uint32_t>(Constant::MAX_NUMBER_OF_CACHE_FILES) + 1);
static const auto kErrorStatusPrepareFromCacheChoices =
        testing::Values(ErrorStatus::NONE, ErrorStatus::GENERAL_FAILURE,
                        ErrorStatus::DEVICE_UNAVAILABLE, ErrorStatus::INVALID_ARGUMENT);

INSTANTIATE_TEST_CASE_P(TestCompilationCaching, CompilationCachingTest,
                        testing::Combine(kErrorStatusGetNumCacheFilesChoices, kNumCacheChoices,
                                         kNumCacheChoices, kErrorStatusPrepareFromCacheChoices));

}  // end namespace
