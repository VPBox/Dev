/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "CompilationBuilder.h"
#include "Manager.h"
#include "ModelBuilder.h"
#include "NeuralNetworks.h"
#include "SampleDriver.h"
#include "TestNeuralNetworksWrapper.h"
#include "Utils.h"
#include "ValidateHal.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iterator>
#include <random>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include <unistd.h>

#include <android-base/logging.h>
#include <android/sharedmem.h>
#include <gtest/gtest.h>

// Uncomment the following line to generate some debugging output that
// may be useful when analyzing failures:
//
// #define VERBOSE VERBOSE

// We randomly generate tests (model + input data) at runtime, and verify
// that we get the same results whether we do partitioned compilation/execution
// or non partitioned compilation/execution.  We perform a test as follows:
//
// (1) Randomly generate a model (graph and weights), randomly generate input
//     data, randomly assign inputs and outputs to CPU memory or to shared
//     memory.
//
//     Randomly leaves dimensions unset for intermediate operands.
//
// (2) Randomly generate drivers based on the sample driver, each of which
//     executes models on the CPU.  They differ according to which operations
//     they support.
//
// (3) Compile and execute without partitioning, saving off the results.
//
// (4) Compile and execute with partitioning.
//
// (5) Verify that the saved results from (3) match the results from (4).
//
// For simplicity, all data (model inputs, model outputs, weights,
// temps) are of the same type: a 2-D TENSOR_FLOAT32 where the two
// dimensions are fixed throughout a particular test case (and
// randomly determined).  This prevents us from having to find a
// mechanism to "resize" data (e.g., if ADD#a operates on data of size
// 2x2, ADD#b operates on data of size 3x3, and the outputs of ADD#a
// and ADD#b become inputs of ADD#c, do we need to insert one or more
// operations between (say) ADD#a and ADD#c to convert ADD#2's data
// from size 2x2 to size 3x3 in order to match ADD#b).  In the few
// cases where an operand cannot be of this type, it is a constant
// (e.g., activation functions and RNN bias).
//
// Each operation we generate has a signature (described in more
// detail later).  The randomly generated drivers decide which
// operations they can execute by checking operation signatures.  Once
// we have built the model and know the set of signatures, we randomly
// assign each signature to a driver.  No signature is supported by
// multiple drivers -- we're not testing the logic that the
// partitioning algorithm uses to select the best driver for an
// operation.

namespace android {

using CompilationBuilder = nn::CompilationBuilder;
using Device = nn::Device;
using DeviceManager = nn::DeviceManager;
using ExecutionPlan = nn::ExecutionPlan;
using HalVersion = nn::HalVersion;
using HidlModel = hardware::neuralnetworks::V1_2::Model;
using HidlToken =
        ::android::hardware::hidl_array<uint8_t, ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN>;
using MemoryBuilder = nn::Memory;
using ModelBuilder = nn::ModelBuilder;
using Result = nn::test_wrapper::Result;
using SampleDriver = nn::sample_driver::SampleDriver;
using WrapperCompilation = nn::test_wrapper::Compilation;
using WrapperExecution = nn::test_wrapper::Execution;
using WrapperMemory = nn::test_wrapper::Memory;
using WrapperModel = nn::test_wrapper::Model;
using WrapperOperandType = nn::test_wrapper::OperandType;
using WrapperType = nn::test_wrapper::Type;

namespace {

/// Configure test size //////////////////////////////////////////////////////////

// We may exceed this in order to connect otherwise disjoint subgraphs.
static const unsigned kMaxNumOperations = 100;

// We build models to process 2-D square tensors up to this size in each dimension;
// note that the API promotes by-value weights larger than 128 to by-reference,
// so we want to ensure that we can pick both types that exceed and types that do
// not exceed this size.
static const unsigned kMaxProblemSize = 8;

// First seed for pseudorandom test generation.
static const unsigned kFirstSeed = 0;

// Number of test cases.
static const unsigned kNumTestCases = 225;

// Force all graph weights into a single pool (as we recommend to users)
// or allow them to be distributed across multiple pools (more stress
// on the partitioning algorithm and the rest of the runtime)?
// Forcing all graph weights into a single pool may be necessary to
// prevent large graphs from running up against http://b/70302693
// "NNAPI overuses (?) fds".
static const bool kAllWeightsInOnePool = false;

//////////////////////////////////////////////////////////////////////////////////

// The signature of an operation consists of the operation type (e.g.,
// ADD) and the activation function (use -1 in the case of an
// operation type for which the activation function is inapplicable).
typedef std::pair<ANeuralNetworksOperationType, int> Signature;

// This class adds some simple utilities on top of WrapperModel.  For example,
// it provides access to certain features from ModelBuilder that are not exposed
// by the base class (such as inputCount() and operation index).
class TestModel : public WrapperModel {
public:

    uint32_t addOperation(ANeuralNetworksOperationType type, const std::vector<uint32_t>& inputs,
                          const std::vector<uint32_t>& outputs) {
        const uint32_t operationIndex = operationCount();
        mOperations.push_back(outputs);
        WrapperModel::addOperation(type, inputs, outputs);
        return operationIndex;
    }

    uint32_t operationCount() const {
        return mOperations.size();
    }

    uint32_t inputCount() const {
        return builder()->inputCount();
    }
    uint32_t outputCount() const {
        return builder()->outputCount();
    }

    const std::vector<uint32_t>& getOperationOutputs(uint32_t index) const {
        CHECK(index < mOperations.size());
        return mOperations[index];
    }

    // All values are immediately copied into the model (we need to do
    // this ourselves in cases where the underlying NNAPI does not).
    void setOperandValue(uint32_t index, const std::vector<float>& value) {
        const size_t length = value.size() * sizeof(float);

        if (length <= ANEURALNETWORKS_MAX_SIZE_OF_IMMEDIATELY_COPIED_VALUES) {
            WrapperModel::setOperandValue(index, value.data(), length);
        } else {
            mOperandValues.push_back(value);
            WrapperModel::setOperandValue(index, mOperandValues.back().data(), length);
        }
    }

    void setOperandValue(uint32_t index, const std::vector<int32_t>& value) {
        const size_t length = value.size() * sizeof(int32_t);

        CHECK(length <= ANEURALNETWORKS_MAX_SIZE_OF_IMMEDIATELY_COPIED_VALUES);
        WrapperModel::setOperandValue(index, value.data(), length);
    }

    void setOperandValue(uint32_t index, int32_t value) {
        CHECK(sizeof(value) <= ANEURALNETWORKS_MAX_SIZE_OF_IMMEDIATELY_COPIED_VALUES);
        WrapperModel::setOperandValue(index, &value, sizeof(value));
    }

private:

    const ModelBuilder* builder() const {
        return reinterpret_cast<const ModelBuilder*>(getHandle());
    }

    // Representation of operations: vector index is operation number,
    // vector value is operation's output operands.
    std::vector<std::vector<uint32_t>> mOperations;

    // Large operand values -- not immediately copied into the
    // WrapperModel, so remembered here instead.
    std::vector<std::vector<float>> mOperandValues;
};

// This class adds some simple utilities on top of WrapperCompilation in order
// to provide access to certain features from CompilationBuilder that are not
// exposed by the base class.
class TestCompilation : public WrapperCompilation {
public:
    TestCompilation(const WrapperModel* model) : WrapperCompilation(model) {}

    TestCompilation(const WrapperModel* model, std::vector<std::shared_ptr<Device>> devices) {
        ModelBuilder* m = reinterpret_cast<ModelBuilder*>(model->getHandle());
        CompilationBuilder* c = nullptr;
        int result = m->createCompilation(&c, devices);
        EXPECT_EQ(result, 0);
        mCompilation = reinterpret_cast<ANeuralNetworksCompilation*>(c);
    }

    using WrapperCompilation::finish;

    Result setPartitioning(uint32_t partitioning) {
        return static_cast<Result>(builder()->setPartitioning(partitioning));
    }

    const ExecutionPlan& getExecutionPlan() const {
        return builder()->forTest_getExecutionPlan();
    }

private:
    const CompilationBuilder* builder() const {
        return reinterpret_cast<const CompilationBuilder*>(getHandle());
    }
    CompilationBuilder* builder() {
        return reinterpret_cast<CompilationBuilder*>(getHandle());
    }
};

// This class is used to manage a collection of memory regions,
// disjoint windows onto a set of Memory instances, each of which is
// associated with a single shared memory region.  Each region and
// Memory instance is assigned a number.  The usage pattern is as
// follows:
// - Call addMemory() and addRegion() as many times as needed to
//   declare (but not define) Memory instances and declare region
//   instances.
// - Call layout() to define the Memory instances.
// - Call getRegion() as many times as needed to get the details
//   of memory regions (such as address, or Memory/offset/length).
// The Memory instances created by layout() are owned by the
// TestMemories instance, and are destroyed when the TestMemories
// instance is destroyed.
class TestMemories {
public:
    TestMemories() = default;
    ~TestMemories();

    TestMemories(const TestMemories&) = delete;
    TestMemories& operator=(const TestMemories&) = delete;

    unsigned addMemory() {
        CHECK(!mLayoutDone);
        mMemorySizes.push_back(0);
        return memoryCount() - 1;
    }
    unsigned memoryCount() const {
        return mMemorySizes.size();
    }

    unsigned addRegion(unsigned memoryIndex, uint32_t length) {
        CHECK(!mLayoutDone);
        CHECK(memoryIndex < memoryCount());
        uint32_t& memorySize = mMemorySizes[memoryIndex];
        auto desc = std::make_tuple(memoryIndex, (uint32_t)memorySize, length);
        mRegions.push_back(desc);
        memorySize += length;
        return regionCount() - 1;
    }
    unsigned regionCount() const {
        return mRegions.size();
    }

    void layout();

    void* getRegion(unsigned regionIndex,
                    const WrapperMemory** pMemory, uint32_t* pOffset, uint32_t* pLength) {
        CHECK(mLayoutDone);
        CHECK(regionIndex < regionCount());
        const auto& regionDescriptor = mRegions[regionIndex];
        const WrapperMemory* memory = &mMemorys[std::get<0>(regionDescriptor)];
        uint32_t offset = std::get<1>(regionDescriptor);
        uint32_t length = std::get<2>(regionDescriptor);

        uint8_t* buffer;
        if (reinterpret_cast<MemoryBuilder*>(memory->get())->getPointer(&buffer) !=
            ANEURALNETWORKS_NO_ERROR) {
            CHECK(0);
        }

        if (pMemory) *pMemory = memory;
        if (pOffset) *pOffset = offset;
        if (pLength) *pLength = length;

        return buffer + offset;
    }

    void* getRegion(unsigned regionIndex) {
        return getRegion(regionIndex, nullptr, nullptr, nullptr);
    }

private:
    // Index is the memory index; value is the size of the memory
    // (aggregate size of all regions in the memory).
    std::vector<uint32_t> mMemorySizes;

    // Index is the memory index.
    std::vector<WrapperMemory> mMemorys;
    std::vector<int> mFDs;

    // Index is the region index; tuple represents memory index,
    // region offset within memory, region length.
    std::vector<std::tuple<unsigned, uint32_t, uint32_t>> mRegions;

    // For sanity checking.
    bool mLayoutDone = false;
};

void TestMemories::layout() {
    CHECK(!mLayoutDone);
    for (uint32_t memorySize : mMemorySizes) {
        const int fd = ASharedMemory_create(nullptr, memorySize);
        CHECK(fd >= 0);
        mMemorys.emplace_back(memorySize, PROT_READ | PROT_WRITE, fd, 0);
        mFDs.push_back(fd);
    }
    mLayoutDone = true;
}

TestMemories::~TestMemories() {
    for (int fd : mFDs) {
        close(fd);
    }
}

class RandomPartitioningTest : public ::testing::TestWithParam<unsigned> {
public:
    RandomPartitioningTest() : mRandNumEng(GetParam() /* seed */), mRandNumUnitDist(0.0, 1.0) {}

    static Signature getSignature(const HidlModel& model, const Operation& operation);

protected:
 static V1_0::IDevice* makeTestDriver(HalVersion version, const char* name,
                                      std::set<Signature> signatures);

 static HalVersion getMinHalVersion(ANeuralNetworksOperationType type);

 static std::string to_string(HalVersion version);

 bool randBool() { return randUInt(2) == 1; }

 double randFrac() {  // [0.0, 1.0)
     return mRandNumUnitDist(mRandNumEng);
    }

    unsigned randUInt(unsigned limit) {  // [0, limit)
        return unsigned(randFrac() * limit);
    }

    // Represents an operation in which every input and output operand
    // is a TENSOR_FLOAT32 of dimensions [problemSize, problemSize] except:
    // - One input operand may be an activation function.
    // - Any number of input operands may be "special" in some other way
    //   (and in this implementation, not produced by any other operation).
    // We require that:
    // - There be at least one input operand that is neither an
    //    activation function nor "special".
    struct OperationPattern {
        HalVersion mMinHalVersion;
        int mOperationType;
        unsigned mNumInputs;
        unsigned mNumOutputs;
        int mActivationFunctionInputIndex;  // <0 if none

        // Returns operand index, or <0 if input is normal (must not
        // be called for an activation function operand).  Function
        // should have the following prototype:
        //
        //     int makeSpecialInput(unsigned problemSize, TestModel* model, unsigned inputIndex);
        //
        int (RandomPartitioningTest::*mMakeSpecialInput)(unsigned, TestModel*, unsigned);
    };

    static const OperationPattern kOperationPatterns[];

    // See OperationPattern::mMakeSpecialInput.  This function is used to
    // manufacture an RNN input operand that doesn't fit the general operand
    // pattern known to the graph generator infrastructure.
    int makeRnnSpecialInput(unsigned problemSize, TestModel* model, unsigned inputIndex) {
        if (inputIndex != 3) {
            return -1;
        }

        // input operand 3 is bias, a 1-D tensor
        const WrapperOperandType biasType(WrapperType::TENSOR_FLOAT32, { problemSize });
        const uint32_t operandIndex = model->addOperand(&biasType);
        std::vector<float> biasValue(problemSize);
        std::generate(biasValue.begin(), biasValue.end(),
                      [this]{ return randFrac(); });
        model->setOperandValue(operandIndex, biasValue);
        return int(operandIndex);
    }

    // See OperationPattern::mMakeSpecialInput.  This function is used to
    // manufacture a TRANSPOSE input operand that doesn't fit the general operand
    // pattern known to the graph generator infrastructure.
    int makeTransposeSpecialInput(unsigned /* problemSize */, TestModel* model,
                                  unsigned inputIndex) {
        if (inputIndex != 1) {
            return -1;
        }

        // input operand 1 is perm, a 1-D tensor
        const WrapperOperandType permType(WrapperType::TENSOR_INT32, {2});
        const uint32_t operandIndex = model->addOperand(&permType);
        std::vector<int32_t> permValue = {1, 0};
        model->setOperandValue(operandIndex, permValue);
        return int(operandIndex);
    }

#ifdef VERBOSE
    class ModelStats {
    public:
        ModelStats(const ModelBuilder* model) :
                mBuilder(model) { }
        ModelStats(const WrapperModel* model) :
                mBuilder(reinterpret_cast<const ModelBuilder*>(model->getHandle())) { }
        friend std::ostream& operator<<(std::ostream& out, const ModelStats& stats) {
            const uint32_t operandCount = stats.mBuilder->operandCount();
            const uint32_t inputCount = stats.mBuilder->inputCount();
            const uint32_t outputCount = stats.mBuilder->outputCount();
            out << "operationCount = " << stats.mBuilder->operationCount()
                << ", operandCount = " << operandCount
                << ", inputCount = " << inputCount
                << " (" << (double(inputCount) / operandCount) << ")"
                << ", outputCount = " << outputCount
                << " (" << (double(outputCount) / operandCount) << ")";
            return out;
        }
    private:
        const ModelBuilder* mBuilder;
    };

    template <typename T_iterator>
    static void dump(T_iterator I, T_iterator E) {
        std::cout << "{";
        for (; I != E; I++) {
            std::cout << " " << *I;
        }
        std::cout << " }" << std::endl;
    }
#endif

    std::mt19937 mRandNumEng;

   private:
    std::uniform_real_distribution<double> mRandNumUnitDist;
};

const RandomPartitioningTest::OperationPattern RandomPartitioningTest::kOperationPatterns[] = {
        {HalVersion::V1_0, ANEURALNETWORKS_ADD, 3, 1, 2, nullptr},
        {HalVersion::V1_0, ANEURALNETWORKS_LOGISTIC, 1, 1, -1, nullptr},
        {HalVersion::V1_0, ANEURALNETWORKS_MUL, 3, 1, 2, nullptr},
        {HalVersion::V1_0, ANEURALNETWORKS_RNN, 6, 2, 5,
         &RandomPartitioningTest::makeRnnSpecialInput},
        {HalVersion::V1_0, ANEURALNETWORKS_TANH, 1, 1, -1, nullptr},

        {HalVersion::V1_1, ANEURALNETWORKS_SUB, 3, 1, 2, nullptr},
        {HalVersion::V1_1, ANEURALNETWORKS_TRANSPOSE, 2, 1, -1,
         &RandomPartitioningTest::makeTransposeSpecialInput},

        {HalVersion::V1_2, ANEURALNETWORKS_MAXIMUM, 2, 1, -1, nullptr},
        {HalVersion::V1_2, ANEURALNETWORKS_NEG, 1, 1, -1, nullptr},
        {HalVersion::V1_2, ANEURALNETWORKS_SIN, 1, 1, -1, nullptr},
};

HalVersion RandomPartitioningTest::getMinHalVersion(ANeuralNetworksOperationType type) {
    static const auto kOperationToVersion = [] {
        std::map<ANeuralNetworksOperationType, HalVersion> result;
        for (const auto& pattern : kOperationPatterns) {
            result[pattern.mOperationType] = pattern.mMinHalVersion;
        }
        return result;
    }();

    return kOperationToVersion.at(type);
}

Signature RandomPartitioningTest::getSignature(const HidlModel& model, const Operation& operation) {
    static const auto kOperationToActivation = [] {
        std::map<ANeuralNetworksOperationType, int> result;
        for (const auto& pattern : kOperationPatterns) {
            result[pattern.mOperationType] = pattern.mActivationFunctionInputIndex;
        }
        return result;
    }();

    const ANeuralNetworksOperationType operationType =
            static_cast<ANeuralNetworksOperationType>(operation.type);
    const int activationFunctionInputIndex = kOperationToActivation.at(operationType);
    if (activationFunctionInputIndex < 0) {
        return Signature(operationType, -1);
    }

    const Operand& operand = model.operands[operation.inputs[activationFunctionInputIndex]];
    CHECK(operand.lifetime == OperandLifeTime::CONSTANT_COPY);
    CHECK(operand.type == OperandType::INT32);
    int32_t value;
    memcpy(&value,
           &model.operandValues[operand.location.offset],
           operand.location.length);
    return Signature(operationType, value);
}

std::string RandomPartitioningTest::to_string(HalVersion version) {
    switch (version) {
        case HalVersion::V1_0:
            return "V1_0";
        case HalVersion::V1_1:
            return "V1_1";
        case HalVersion::V1_2:
            return "V1_2";
        default:
            return "V_UNKNOWN";
    }
};

class TestDriver : public SampleDriver {
public:
    // Behaves like SampleDriver, except that it only supports
    // operations with the specified signatures.
    TestDriver(const char* name, std::set<Signature> signatures) :
            SampleDriver(name), mSignatures(std::move(signatures)) { }

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
                                            getSupportedOperations_cb cb) override {
        if (nn::validateModel(model)) {
            const size_t count = model.operations.size();
            std::vector<bool> supported(count);
            for (size_t i = 0; i < count; i++) {
                supported[i] =
                    (mSignatures.count(
                        RandomPartitioningTest::getSignature(
                            model,
                            model.operations[i])) != 0);
            }
            cb(ErrorStatus::NONE, supported);
        } else {
            std::vector<bool> supported;
            cb(ErrorStatus::INVALID_ARGUMENT, supported);
        }
        return Void();
    }

    Return<ErrorStatus> prepareModel_1_2(const HidlModel& model, ExecutionPreference preference,
                                         const hidl_vec<hidl_handle>& modelCache,
                                         const hidl_vec<hidl_handle>& dataCache,
                                         const HidlToken& token,
                                         const sp<IPreparedModelCallback>& callback) override {
        // NOTE: We verify that all operations in the model are supported.
        ErrorStatus outStatus = ErrorStatus::INVALID_ARGUMENT;
        auto ret = getSupportedOperations_1_2(
            model,
            [&outStatus](ErrorStatus inStatus, const hidl_vec<bool>& supportedOperations) {
                if (inStatus == ErrorStatus::NONE) {
                    if (std::all_of(supportedOperations.begin(), supportedOperations.end(),
                                    [](bool v){ return v; })) {
                        outStatus = ErrorStatus::NONE;
                    }
                }
            });
        if (ret.isOk() && (outStatus == ErrorStatus::NONE)) {
            return SampleDriver::prepareModel_1_2(model, preference, modelCache, dataCache, token,
                                                  callback);
        } else {
            callback->notify_1_2(ErrorStatus::INVALID_ARGUMENT, nullptr);
            return ErrorStatus::INVALID_ARGUMENT;
        }
    }

private:
    const std::set<Signature> mSignatures;
};

// Like TestDriver, but implementing 1.1
class TestDriverV1_1 : public V1_1::IDevice {
   public:
    TestDriverV1_1(const char* name, std::set<Signature> signatures)
        : mDriverV1_2(new TestDriver(name, std::move(signatures))) {}
    Return<void> getCapabilities_1_1(getCapabilities_1_1_cb _hidl_cb) override {
        return mDriverV1_2->getCapabilities_1_1(_hidl_cb);
    }
    Return<void> getSupportedOperations_1_1(const V1_1::Model& model,
                                            getSupportedOperations_1_1_cb _hidl_cb) override {
        return mDriverV1_2->getSupportedOperations_1_1(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel_1_1(
            const V1_1::Model& model, ExecutionPreference preference,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return mDriverV1_2->prepareModel_1_1(model, preference, actualCallback);
    }
    Return<DeviceStatus> getStatus() override { return mDriverV1_2->getStatus(); }
    Return<void> getCapabilities(getCapabilities_cb _hidl_cb) override {
        return mDriverV1_2->getCapabilities(_hidl_cb);
    }
    Return<void> getSupportedOperations(const V1_0::Model& model,
                                        getSupportedOperations_cb _hidl_cb) override {
        return mDriverV1_2->getSupportedOperations(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel(
            const V1_0::Model& model,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return mDriverV1_2->prepareModel(model, actualCallback);
    }

   private:
    const sp<V1_2::IDevice> mDriverV1_2;
};

// Like TestDriver, but implementing 1.0
class TestDriverV1_0 : public V1_0::IDevice {
   public:
    TestDriverV1_0(const char* name, std::set<Signature> signatures)
        : mDriverV1_2(new TestDriver(name, std::move(signatures))) {}
    Return<void> getCapabilities(getCapabilities_cb _hidl_cb) override {
        return mDriverV1_2->getCapabilities(_hidl_cb);
    }
    Return<void> getSupportedOperations(const V1_0::Model& model,
                                        getSupportedOperations_cb _hidl_cb) override {
        return mDriverV1_2->getSupportedOperations(model, _hidl_cb);
    }
    Return<ErrorStatus> prepareModel(
            const V1_0::Model& model,
            const sp<V1_0::IPreparedModelCallback>& actualCallback) override {
        return mDriverV1_2->prepareModel(model, actualCallback);
    }
    Return<DeviceStatus> getStatus() override { return mDriverV1_2->getStatus(); }

   private:
    const sp<V1_2::IDevice> mDriverV1_2;
};

V1_0::IDevice* RandomPartitioningTest::makeTestDriver(HalVersion version, const char* name,
                                                      std::set<Signature> signatures) {
    switch (version) {
        case HalVersion::V1_0:
            return new TestDriverV1_0(name, std::move(signatures));
        case HalVersion::V1_1:
            return new TestDriverV1_1(name, std::move(signatures));
        case HalVersion::V1_2:
            return new TestDriver(name, std::move(signatures));
        default:
            ADD_FAILURE() << "Unexpected HalVersion " << static_cast<int32_t>(version);
            return nullptr;
    }
}

INSTANTIATE_TEST_CASE_P(Seed, RandomPartitioningTest,
                        ::testing::Range(kFirstSeed, kFirstSeed + kNumTestCases));

TEST_P(RandomPartitioningTest, Test) {
    LOG(INFO) << "RandomPartitioningTest: GetParam() = " << GetParam();

#ifdef VERBOSE
    std::cout << std::setprecision(2) << std::fixed << std::setw(4);
#endif

    const unsigned problemSize = 1+randUInt(kMaxProblemSize);
    const WrapperOperandType problemType(WrapperType::TENSOR_FLOAT32, { problemSize, problemSize });
    const WrapperOperandType unknownDimensionsType(WrapperType::TENSOR_FLOAT32, { 0, 0 });

    static const WrapperOperandType activationFunctionType(WrapperType::INT32, { });

    const unsigned numOperations = 2+randUInt(kMaxNumOperations-1);
    const bool allowDeadOperations = (randFrac() < 0.2);
    const bool allowUnknownDimensions = (randFrac() < 0.25);

    // TODO: The current algorithm builds the graph in a forward
    // direction (i.e., later-generated operations consume outputs
    // from earlier-generated operations).  In order to get more
    // variation in graph topology, perhaps we should also create an
    // algorithm to build the graph in a backward direction (i.e.,
    // later-generated operations produce outputs to be consumed by
    // earlier-generated operations).
    [[maybe_unused]] const bool buildForward = randBool();

    // TODO: Add a form of forced connectivity that operates by
    // joining disjoint subgraphs rather than by forcing a root.
    const bool forceCommonRoot = (randFrac() < 0.75);

    TestModel model;
    std::vector<uint32_t> modelInputs;
    std::vector<uint32_t> modelOutputs;

    // Each region in weights is a problem-sized 2-D TENSOR_FLOAT32.
    TestMemories weights;

    // Keep track of all normal (i.e., not activation function and not
    // "special") operands that are values (from setOperandValue*()).
    // .first: operand index
    // .second: if the operand is already defined (via setOperandValue*()) then ~0U;
    //          otherwise, the operand has yet to be defined, and this is the corresponding
    //          region index in "weights"
    std::vector<std::pair<uint32_t, unsigned>> valueOperands;

    // An operand is "dead" if it is not consumed by another operation
    // and is not a model output.  Key is operand index; value is
    // operation index.
    std::map<uint32_t, uint32_t> deadOperands;

    // An operation is "dead" if all of its outputs are dead.
    std::set<uint32_t> deadOperations;

    // Collect the signatures of operations in this model.
    std::set<Signature> signatures;

    // For reporting purposes, keep track of the number of root
    // operations (those that do not consume results produced by other
    // operations).
    unsigned rootOperationCount = 0;

    // Track if we added operands with unknown dimensions. In this case,
    // partitioned compilation will fail if such an operand is read in a
    // different partition than it is written.
    bool hasUnknownDimensions = false;

    // Generate operations.
    for (unsigned i = 0; i < numOperations; i++) {
        const unsigned operationPatternIndex = randUInt(std::size(kOperationPatterns));
        const auto& operationPattern = kOperationPatterns[operationPatternIndex];

        // INPUTS //////////////////////////////////////////////////////////////////////////////////

        std::vector<uint32_t> operationInputs(operationPattern.mNumInputs, ~0U);

        // First, process activation function and special inputs, and
        // keep track of which inputs remain.
        std::vector<uint32_t> normalOperationInputIndexes;
        int32_t activationFunction = -1;
        for (unsigned operationInputIndex = 0; operationInputIndex < operationPattern.mNumInputs;
             operationInputIndex++) {
            if (int(operationInputIndex) == operationPattern.mActivationFunctionInputIndex) {
                const uint32_t operandIndex = model.addOperand(&activationFunctionType);
                activationFunction = randUInt(4);
                if (activationFunction == ANEURALNETWORKS_FUSED_RELU1) {
                    // workaround for http://b/69011131
                    activationFunction = ANEURALNETWORKS_FUSED_NONE;
                }
                model.setOperandValue(operandIndex, activationFunction);
                operationInputs[operationInputIndex] = operandIndex;
                continue;
            }
            if (operationPattern.mMakeSpecialInput != nullptr) {
                const int operandIndex = (this->*(operationPattern.mMakeSpecialInput))(
                    problemSize, &model, operationInputIndex);
                if (operandIndex >= 0) {
                    operationInputs[operationInputIndex] = operandIndex;
                    continue;
                }
            }
            normalOperationInputIndexes.push_back(operationInputIndex);
        }
        CHECK(!normalOperationInputIndexes.empty());
        signatures.insert(Signature(operationPattern.mOperationType, activationFunction));

        // A (normal) operation input can be one of:
        // - a new or existing model input
        // - an output of an existing operation
        // - an OperandValue
        // - an OperandValueFromMemory
        // Some guidelines:
        // - We generally don't want all of an operation's inputs to be values (constants)
        const unsigned normalOperationInputCount = normalOperationInputIndexes.size();
        //     How many of this operation's inputs are constants?
        unsigned normalOperationInputConstantCount = 0;
        //     How many of this operation's inputs are model inputs?
        unsigned normalOperationInputModelInputCount = 0;
        // We begin by deciding what kind of input each (normal) operation will be; we don't
        // actually pick input operand indexes at this time, because we might override this
        // decision later.
        enum InputKind { IK_MODEL_INPUT, IK_OPERATION_OUTPUT, IK_VALUE };
        std::vector<InputKind> normalOperationInputKinds(normalOperationInputCount);
        std::generate(normalOperationInputKinds.begin(), normalOperationInputKinds.end(),
                      [this, &model,
                       numOperations,
                       normalOperationInputCount,
                       &normalOperationInputConstantCount,
                       &normalOperationInputModelInputCount]() -> InputKind {
                          // Constant?  Becomes less likely the more
                          // constants we already have as inputs to
                          // this operation.
                          if (randFrac() < 0.3 * (1 - double(normalOperationInputConstantCount) /
                                                   normalOperationInputCount)) {
                              normalOperationInputConstantCount++;
                              return IK_VALUE;
                          }

                          // Model input?  Becomes less likely the
                          // more model inputs we already have as
                          // inputs to this operation, and the further
                          // along we are in generating this model
                          // (i.e., the more operations we have
                          // generated).
                          if ((model.operationCount() == 0) ||
                              (randFrac() < 0.5 *
                               (1 - double(normalOperationInputModelInputCount) /
                                normalOperationInputCount) *
                               std::min(0.3, (1 - double(model.operationCount()) /
                                              numOperations)))) {
                              normalOperationInputModelInputCount++;
                              return IK_MODEL_INPUT;
                          }

                          // Else output of an existing operation.
                          return IK_OPERATION_OUTPUT;
                      });

        // Now force common root or model input, if necessary.  (A
        // model must have at least one input.)
        auto force =
                [this, &normalOperationInputKinds, normalOperationInputCount](InputKind forceKind){
            if (std::none_of(normalOperationInputKinds.begin(),
                             normalOperationInputKinds.end(),
                             [forceKind](InputKind kind){ return kind == forceKind; })) {
                normalOperationInputKinds[randUInt(normalOperationInputCount)] = forceKind;
            }
        };
        if (forceCommonRoot && (model.operationCount() != 0)) {
            force(IK_OPERATION_OUTPUT);
        }
        if (modelInputs.empty()) {
            CHECK(model.operationCount() == 0);
            force(IK_MODEL_INPUT);
        }

        // Finally create the normal inputs.
        bool isRootOperation = true;
        for (unsigned i = 0; i < normalOperationInputCount; i++) {
            uint32_t operandIndex = ~0U;
            switch (normalOperationInputKinds[i]) {
                case IK_MODEL_INPUT: {
                    if (!modelInputs.empty() && (randFrac() < 0.5)) {
                        operandIndex = modelInputs[randUInt(modelInputs.size())];
                    } else {
                        operandIndex = model.addOperand(&problemType);
                        modelInputs.push_back(operandIndex);
                    }
                    break;
                }
                case IK_OPERATION_OUTPUT: {
                    decltype(deadOperands.begin()) deadOperandI;
                    if (!deadOperands.empty() && (randFrac() < 0.5)) {
                        deadOperandI = deadOperands.begin();
                        std::advance(deadOperandI, randUInt(deadOperands.size()));
                        operandIndex = deadOperandI->first;
                    } else {
                        const uint32_t existingOperationIndex = randUInt(model.operationCount());
                        const auto& existingOperationOutputs =
                                model.getOperationOutputs(existingOperationIndex);
                        operandIndex =
                            existingOperationOutputs[randUInt(existingOperationOutputs.size())];
                        deadOperandI = deadOperands.find(operandIndex);
                        CHECK(deadOperandI == deadOperands.end() ||
                              deadOperandI->second == existingOperationIndex);
                    }
                    if (deadOperandI != deadOperands.end()) {
                        const uint32_t correspondingOperation = deadOperandI->second;
                        deadOperands.erase(deadOperandI);

                        auto deadOperationI = deadOperations.find(correspondingOperation);
                        if (deadOperationI != deadOperations.end()) {
                            deadOperations.erase(deadOperationI);
                        }
                    }
                    isRootOperation = false;
                    break;
                }
                case IK_VALUE: {
                    if (!valueOperands.empty() && (randFrac() < 0.25)) {
                        operandIndex = valueOperands[randUInt(valueOperands.size())].first;
                    } else {
                        operandIndex = model.addOperand(&problemType);
                        if (randFrac() < 0.5) {
                            std::vector<float> value(problemSize * problemSize);
                            std::generate(value.begin(), value.end(), [this]{ return randFrac(); });
                            model.setOperandValue(operandIndex, value);
                            valueOperands.push_back(std::make_pair(operandIndex, ~0U));
                        } else {
                            unsigned memoryIndex = ~0U;
                            if ((weights.memoryCount() != 0) &&
                                (kAllWeightsInOnePool || (randFrac() < 0.5))) {
                                memoryIndex = randUInt(weights.memoryCount());
                            } else {
                                memoryIndex = weights.addMemory();
                            }
                            const size_t length = problemSize * problemSize * sizeof(float);
                            const unsigned regionIndex = weights.addRegion(memoryIndex, length);
                            valueOperands.push_back(std::make_pair(operandIndex, regionIndex));
                        }
                    }
                    break;
                }
                default:
                    FAIL();
            }
            operationInputs[normalOperationInputIndexes[i]] = operandIndex;
        }
        if (isRootOperation) {
            rootOperationCount++;
        }

        // OUTPUTS /////////////////////////////////////////////////////////////////////////////////

        std::vector<uint32_t> operationOutputs(operationPattern.mNumOutputs);
        std::generate(operationOutputs.begin(), operationOutputs.end(),
                      [&model, &problemType, &unknownDimensionsType, &hasUnknownDimensions,
                       allowUnknownDimensions, this]{
                          // 3% unknowns causes ~35% of partitionings to fail
                          // (determined by commenting out the fallback code,
                          // running tests and noting number of failures).
                          if (allowUnknownDimensions && randFrac() < 0.03) {
                              hasUnknownDimensions = true;
                              return model.addOperand(&unknownDimensionsType);
                          } else {
                              return model.addOperand(&problemType);
                          }
                      });

        // OPERATION ///////////////////////////////////////////////////////////////////////////////

        const uint32_t operationIndex =
                model.addOperation(operationPattern.mOperationType,
                                   operationInputs, operationOutputs);
        deadOperations.insert(operationIndex);
        std::for_each(operationOutputs.begin(), operationOutputs.end(),
                      [&deadOperands, operationIndex](uint32_t operandIndex) {
                          deadOperands.insert(std::make_pair(operandIndex, operationIndex));
                      });
    }

    // Now finalize the weights.
    weights.layout();
    for (const auto& valueOperand : valueOperands) {
        const uint32_t operandIndex = valueOperand.first;
        const unsigned regionIndex = valueOperand.second;

        if (regionIndex == ~0U) {
            continue;
        }

        const WrapperMemory* memory;
        uint32_t offset, length;
        float* region =
                static_cast<float*>(weights.getRegion(regionIndex, &memory, &offset, &length));
        CHECK(length == problemSize * problemSize * sizeof(float));
        std::generate(region, region + problemSize * problemSize, [this]{ return randFrac(); });
        model.setOperandValueFromMemory(operandIndex, memory, offset, length);
    }

    // Now select model outputs.
    for (uint32_t operationIdx = 0, operationCount = model.operationCount();
         operationIdx < operationCount; operationIdx++) {
        const auto& outputs = model.getOperationOutputs(operationIdx);
        for (uint32_t outputIdx = 0, outputCount = outputs.size(); outputIdx < outputCount;
             outputIdx++) {
            bool modelOutput = false;
            const uint32_t operandIndex = outputs[outputIdx];
            const auto deadOperandI = deadOperands.find(operandIndex);
            if (deadOperandI != deadOperands.end()) {
                // This is not consumed within the model, so unless we
                // make it an output of the model, it's dead.  The
                // further along we are in generating this model
                // (i.e., the more operations we have generated), the
                // more likely we are to classify this operation
                // output as a model output.
                const double probabilityOfModelOutput =
                        0.50 * [](double x){ return x*x; }((operationIdx + 1) / operationCount);
                modelOutput = (randFrac() < probabilityOfModelOutput);
            } else {
                // This is consumed within the model, so we'll rarely
                // make it an output of the model.
                modelOutput = (randFrac() < 0.05);
            }
            if (!modelOutput) {
                continue;
            }
            modelOutputs.push_back(operandIndex);
            if (deadOperandI != deadOperands.end()) {
                deadOperands.erase(deadOperandI);
                const auto deadOperationI = deadOperations.find(operationIdx);
                if (deadOperationI != deadOperations.end()) {
                    deadOperations.erase(deadOperationI);
                }
            }
        }
    }
    if (!allowDeadOperations) {
        // For each dead operation, pick a random output to become a model output.
        for (uint32_t deadOperationIndex : deadOperations) {
            const auto& deadOperationOutputs = model.getOperationOutputs(deadOperationIndex);
            const uint32_t deadOperandIndex =
                    deadOperationOutputs[randUInt(deadOperationOutputs.size())];
            modelOutputs.push_back(deadOperandIndex);
        }
    }
    // A model must have at least one output.
    if (modelOutputs.empty()) {
        const auto& outputs = model.getOperationOutputs(randUInt(model.operationCount()));
        modelOutputs.push_back(outputs[randUInt(outputs.size())]);
    }

    model.identifyInputsAndOutputs(modelInputs, modelOutputs);
#ifdef VERBOSE
    {
        std::cout << "Original model: " << ModelStats(&model) << std::endl;
        std::cout << "rootOperationCount = " << rootOperationCount
                  << ", deadOperations = ";
        if (allowDeadOperations) {
            std::cout << deadOperations.size();
        } else {
            std::cout << "forbidden (converted " << deadOperations.size() << ")";
        }
        std::cout << std::endl;
    }
#endif
    ASSERT_EQ(model.finish(), Result::NO_ERROR);

    // Non-partitioned compilation.
    TestCompilation c(&model);
    ASSERT_EQ(c.setPartitioning(DeviceManager::kPartitioningNo), Result::NO_ERROR);
    ASSERT_EQ(c.finish(), Result::NO_ERROR);

    // Create some drivers for partitioned compilation.
    CHECK(!signatures.empty());
    std::vector<std::set<Signature>> signaturesForDriver(signatures.size());
    //     First assign each signature to a random driver (a driver is
    //     just represented as an entry in the signaturesForDriver
    //     vector).
    for (Signature signature : signatures) {
        signaturesForDriver[randUInt(signatures.size())].insert(signature);
    }
    //     Now remove each entry that has no signatures.
    auto firstExtra =
        std::remove_if(signaturesForDriver.begin(), signaturesForDriver.end(),
                       [](const std::set<Signature>& sigSet) { return sigSet.empty(); });
    if (firstExtra != signaturesForDriver.end()) {
        signaturesForDriver.erase(firstExtra, signaturesForDriver.end());
    }
    //     Now actually create the drivers.
    std::vector<std::shared_ptr<Device>> devices;
    for (unsigned i = 0; i < signaturesForDriver.size(); i++) {
        const auto& signaturesForThisDriver = signaturesForDriver[i];
        // Minimum HAL version for this driver is highest minimum HAL version of
        // any operation supported by this driver.
        const HalVersion minHalVersion = getMinHalVersion(
                std::max_element(signaturesForThisDriver.begin(), signaturesForThisDriver.end(),
                                 [](const Signature& a, const Signature& b) {
                                     return getMinHalVersion(a.first) < getMinHalVersion(b.first);
                                 })
                        ->first);
        const HalVersion actualHalVersion =
                static_cast<HalVersion>(static_cast<int32_t>(minHalVersion) +
                                        randUInt(static_cast<int32_t>(HalVersion::LATEST) -
                                                 static_cast<int32_t>(minHalVersion) + 1));
        const std::string name =
                "TestDriver(" + std::to_string(i) + "){" + to_string(actualHalVersion) + "}";
#ifdef VERBOSE
        std::cout << "Creating " + name + " for collection of signatures that requires HAL " +
                             to_string(minHalVersion)
                  << std::endl;
#endif
        auto device = DeviceManager::forTest_makeDriverDevice(
                name, makeTestDriver(actualHalVersion, name.c_str(), signaturesForThisDriver));
        devices.push_back(device);
    }
    // CPU fallback device
    devices.push_back(DeviceManager::getCpuDevice());

    // Partitioned compilation.
    // For test cases without unknown intermediate operand sizes we require the
    // partitioning to succeed without CPU fallback. With unknown sizes we
    // retry with a fallback if the non-fallback partitioning fails and require
    // the fallback to succeed.
    TestCompilation cNoFallback(&model, devices);
    TestCompilation cWithFallback(&model, devices);
    TestCompilation *c2 = nullptr;
    ASSERT_EQ(cNoFallback.setPartitioning(DeviceManager::kPartitioningWithoutFallback),
              Result::NO_ERROR);
    auto compilationResult = cNoFallback.finish();
    if (hasUnknownDimensions && compilationResult == Result::OP_FAILED &&
        cNoFallback.getExecutionPlan().forTest_hasSubModelOutputsOfUnknownSize()) {
        ASSERT_EQ(cWithFallback.setPartitioning(DeviceManager::kPartitioningWithFallback),
                  Result::NO_ERROR);
        ASSERT_EQ(cWithFallback.finish(), Result::NO_ERROR);
        ASSERT_EQ(cWithFallback.getExecutionPlan().forTest_getKind(), ExecutionPlan::Kind::SIMPLE);
        ASSERT_EQ(cWithFallback.getExecutionPlan().forTest_simpleGetDevice(),
                  DeviceManager::getCpuDevice());
        c2 = &cWithFallback;
    } else {
        ASSERT_EQ(compilationResult, Result::NO_ERROR);
        c2 = &cNoFallback;
    }

#ifdef VERBOSE
    {
        std::cout << "signatures = " << signatures.size()
                  << ", devices = " << devices.size() << std::endl;
        const ExecutionPlan& plan = c2->getExecutionPlan();
        switch (plan.forTest_getKind()) {
            case ExecutionPlan::Kind::SIMPLE:
                std::cout << "plan: simple" << std::endl;
                break;
            case ExecutionPlan::Kind::COMPOUND: {
                const auto& steps = plan.forTest_compoundGetSteps();
                std::set<const Device*> devicesInPlan;
                for (const auto& step : steps) {
                    devicesInPlan.insert(step->getDevice().get());
                }
                std::cout << "plan: compound, " << steps.size() << " steps over "
                          << devicesInPlan.size() << " devices" << std::endl;
                for (unsigned i = 0; i < steps.size(); i++) {
                    std::cout << "Step " << i << ": " << ModelStats(steps[i]->getSubModel())
                              << ", device = " << steps[i]->getDevice()->getName() << std::endl;
                }
                break;
            }
            default:
                std::cout << "Unexpected plan kind: "
                    << static_cast<unsigned>(plan.forTest_getKind());
                break;
        }
    }
#endif

    // For execution:
    // - create master inputs (one long vector) and master output value
    //   - master inputs will be copied to actual inputs before each
    //     of the two executions
    //   - master output will be used to fill actual outputs before each
    //     of the two executions
    // - create actual inputs and outputs
    // - first execution (non-partitioned)
    //   - initialize inputs and (to avoid unrelated oddities) outputs
    //   - execute
    //   - copy outputs to a save area (one long vector)
    // - second execution (partitioned)
    //   - (to avoid unrelated oddities) initialize inputs and outputs
    //   - execute
    //   - compare outputs to save area

    // If the runtime and drivers are working properly, execution
    // should not change the inputs.  Nonetheless, we reinitialize the
    // inputs for each execution, so as to avoid unrelated problems
    // appearing to be problems related to unpartitioned execution
    // versus partitioned execution.  Similarly, execution behavior
    // should not be dependent on the outputs; but we'll initialize the
    // outputs anyway.
    std::vector<float> masterInputs(problemSize * problemSize * model.inputCount());
    std::generate(masterInputs.begin(), masterInputs.end(), [this]{ return randFrac(); });
#ifdef VERBOSE
    {
        std::cout << "flat inputs = ";
        dump(masterInputs.begin(), masterInputs.end());
    }
#endif
    const float masterOutput = randFrac();

    // Create the memory for the actual inputs and outputs.
    struct InputOutputDescriptor {
        enum Kind { INPUT, OUTPUT };
        Kind mKind;

        // The input or output either resides in a local buffer
        // (mVector, in which case mMemoryRegion is ignored); or in a
        // shared memory region within a TestMemories instance
        // (mMemoryRegion, in which case mVector is ignored).
        enum Location { VECTOR, REGION };
        Location getLocation() const { return !mVector.empty() ? VECTOR : REGION; }

        std::vector<float> mVector;
        unsigned mMemoryRegion;
    };
    std::vector<InputOutputDescriptor> ioDescriptors(model.inputCount() + model.outputCount());
    for (unsigned i = 0; i < ioDescriptors.size(); i++) {
        ioDescriptors[i].mKind = (i < model.inputCount()
                                  ? InputOutputDescriptor::INPUT
                                  : InputOutputDescriptor::OUTPUT);
    }
    //     We randomly interleave inputs and outputs in creation
    //     order, because when we we create memory regions in a
    //     TestMemories instance, the order in which regions are
    //     created within a single Memory is the order they'll be laid
    //     out in that memory; and when we have inputs and outputs
    //     within the same Memory, we want the possibility that
    //     they'll be interleaved.
    std::shuffle(ioDescriptors.begin(), ioDescriptors.end(), mRandNumEng);
    TestMemories ioMemories;
    for (auto &desc : ioDescriptors) {
        if (randFrac() < 0.5) {
            desc.mVector.resize(problemSize * problemSize);
        } else {
            // TODO: common this with the way we create IK_VALUE inputs?
            unsigned memoryIndex = ~0U;
            if ((ioMemories.memoryCount() != 0) && (randFrac() < 0.5)) {
                memoryIndex = randUInt(ioMemories.memoryCount());
            } else {
                memoryIndex = ioMemories.addMemory();
            }
            const size_t length = problemSize * problemSize * sizeof(float);
            desc.mMemoryRegion = ioMemories.addRegion(memoryIndex, length);
        }
    }
    ioMemories.layout();

    // Function to set up actual inputs and outputs (initializing them
    // and telling the WrapperExecution about them).
    auto prepareForExecution =
            [&model, &ioDescriptors, &ioMemories,
             &masterInputs, &masterOutput, problemSize, &problemType](WrapperExecution *e) {
        uint32_t inputIndex = 0, outputIndex = 0;
        for (auto &desc : ioDescriptors) {
            if (desc.getLocation() == InputOutputDescriptor::VECTOR) {
                if (desc.mKind == InputOutputDescriptor::INPUT) {
                    const size_t inputOffset = inputIndex * problemSize * problemSize;
                    std::copy(masterInputs.begin() + inputOffset,
                              masterInputs.begin() + inputOffset + problemSize * problemSize,
                              desc.mVector.begin());
                    e->setInput(inputIndex++, desc.mVector.data(),
                                desc.mVector.size() * sizeof(float));
                } else {
                    std::fill(desc.mVector.begin(),
                              desc.mVector.begin() + problemSize * problemSize,
                              masterOutput);
                    e->setOutput(outputIndex++, desc.mVector.data(),
                                 desc.mVector.size() * sizeof(float),
                                 &problemType.operandType);
                }
            } else {
                const WrapperMemory* memory;
                uint32_t offset, length;
                float* region =
                        static_cast<float*>(ioMemories.getRegion(desc.mMemoryRegion,
                                                                 &memory, &offset, &length));
                CHECK(length == problemSize * problemSize * sizeof(float));
                if (desc.mKind == InputOutputDescriptor::INPUT) {
                    const size_t inputOffset = inputIndex * problemSize * problemSize;
                    std::copy(masterInputs.begin() + inputOffset,
                              masterInputs.begin() + inputOffset + problemSize * problemSize,
                              region);
                    e->setInputFromMemory(inputIndex++, memory, offset, length);
                } else {
                    std::fill(region,
                              region + problemSize * problemSize,
                              masterOutput);
                    e->setOutputFromMemory(outputIndex++, memory, offset, length,
                                           &problemType.operandType);
                }
            }
        };
        CHECK(inputIndex == model.inputCount());
        CHECK(outputIndex == model.outputCount());
    };

    // Non-partitioned execution.
    WrapperExecution e(&c);
    ASSERT_NO_FATAL_FAILURE(prepareForExecution(&e));
    ASSERT_EQ(e.compute(), Result::NO_ERROR);

    // Copy the outputs of the non-partitioned execution to a save area.
    std::vector<float> nonPartitionedOutputs(problemSize * problemSize * model.outputCount());
    {
        uint32_t outputIndex = 0;
        for (const auto& desc : ioDescriptors) {
            if (desc.mKind != InputOutputDescriptor::OUTPUT) {
                continue;
            }
            const size_t outputOffset = outputIndex * problemSize * problemSize;
            if (desc.getLocation() == InputOutputDescriptor::VECTOR) {
                std::copy(desc.mVector.begin(),
                          desc.mVector.end(),
                          nonPartitionedOutputs.begin() + outputOffset);
            } else {
                float* region = static_cast<float*>(ioMemories.getRegion(desc.mMemoryRegion));
                std::copy(region,
                          region + problemSize * problemSize,
                          nonPartitionedOutputs.begin() + outputOffset);
            }
#ifdef VERBOSE
            {
                std::cout << "nonpartitioned output[" << outputIndex << "] = ";
                dump(nonPartitionedOutputs.begin() + outputOffset,
                     nonPartitionedOutputs.begin() + outputOffset + problemSize * problemSize);
            }
#endif
            outputIndex++;
        }
    }

    // Partitioned execution.
    WrapperExecution e2(c2);
    ASSERT_NO_FATAL_FAILURE(prepareForExecution(&e2));
    ASSERT_EQ(e2.compute(), Result::NO_ERROR);

    // Compare the outputs of the partitioned execution to the save
    // area containing the outpus of the non-partitioned execution.
    {
        uint32_t outputIndex = 0;
        for (const auto& desc : ioDescriptors) {
            if (desc.mKind != InputOutputDescriptor::OUTPUT) {
                continue;
            }
            SCOPED_TRACE(outputIndex);
            const size_t outputOffset = outputIndex * problemSize * problemSize;
            if (desc.getLocation() == InputOutputDescriptor::VECTOR) {
#ifdef VERBOSE
                std::cout << "   partitioned output[" << outputIndex << "] = ";
                dump(desc.mVector.begin(), desc.mVector.end());
#endif
                ASSERT_TRUE(std::equal(desc.mVector.begin(),
                                       desc.mVector.end(),
                                       nonPartitionedOutputs.begin() + outputOffset));
            } else {
                float* region = static_cast<float*>(ioMemories.getRegion(desc.mMemoryRegion));
#ifdef VERBOSE
                std::cout << "part output[" << outputIndex << "] = ";
                dump(region, region + problemSize * problemSize);
#endif
                ASSERT_TRUE(std::equal(region,
                                       region + problemSize * problemSize,
                                       nonPartitionedOutputs.begin() + outputOffset));
            }
            outputIndex++;
        }
    }
}

}  // namespace
}  // namespace android
