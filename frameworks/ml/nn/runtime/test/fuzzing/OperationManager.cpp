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

#include "fuzzing/OperationManager.h"

#include <vector>

#include "TestNeuralNetworksWrapper.h"
#include "fuzzing/RandomGraphGeneratorUtils.h"

namespace android {
namespace nn {
namespace fuzzing_test {

template <typename T>
inline bool hasValue(const std::vector<T>& vec, const T& val) {
    // Empty vector indicates "no filter", i.e. always true.
    return vec.empty() || std::count(vec.begin(), vec.end(), val) > 0;
}

bool OperationSignature::matchFilter(const OperationFilter& filter) {
    if (!hasValue(filter.opcodes, opType) || !hasValue(filter.versions, version)) {
        return false;
    }

    // Match data types.
    std::vector<Type> combinedDataTypes;
    for (auto dataType : supportedDataTypes) {
        if (hasValue(filter.dataTypes, dataType)) combinedDataTypes.push_back(dataType);
    }
    if (combinedDataTypes.empty()) return false;
    supportedDataTypes = combinedDataTypes;

    // Match rank.
    std::vector<uint32_t> combinedRanks;
    for (auto rank : supportedRanks) {
        if (hasValue(filter.ranks, rank)) combinedRanks.push_back(rank);
    }
    if (combinedRanks.empty()) return false;
    supportedRanks = combinedRanks;
    return true;
}

OperationManager* OperationManager::get() {
    static OperationManager instance;
    return &instance;
}

void OperationManager::addSignature(const std::string& name, const OperationSignature& signature) {
    mOperationSignatures.emplace(name, signature);
}

void OperationManager::applyFilter(const OperationFilter& filter) {
    mFilteredSignatures.clear();
    for (const auto& pair : mOperationSignatures) {
        mFilteredSignatures.push_back(pair.second);
        if (!mFilteredSignatures.back().matchFilter(filter)) mFilteredSignatures.pop_back();
    }
}

const OperationSignature& OperationManager::getRandomOperation() const {
    NN_FUZZER_CHECK(!mFilteredSignatures.empty());
    return getRandomChoice(mFilteredSignatures);
}

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
