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

#include "chre_host/fragmented_load_transaction.h"

#include <algorithm>

namespace android {
namespace chre {

namespace {

/**
 * Returns a vector containing a specific subarray of the original vector.
 *
 * If the ending index of the subarray exceeds that of the source vector,
 * the size will be truncated to the last element of the source vector.
 *
 * @param source the source vector
 * @param start the starting index of the subarray
 * @param size the size of the subarray
 *
 * @return the vector containing the subarray
 */
inline std::vector<uint8_t> getSubVector(
    const std::vector<uint8_t>& source, size_t start, size_t size) {
  size_t end = std::min(source.size(), start + size);
  return (source.size() == 0) ?
      std::vector<uint8_t>() :
      std::vector<uint8_t>(
          source.begin() + start, source.begin() + end); // [start, end)
}

} // anonymous namespace

FragmentedLoadTransaction::FragmentedLoadTransaction(
    uint32_t transactionId, uint64_t appId, uint32_t appVersion,
    uint32_t targetApiVersion, const std::vector<uint8_t>& appBinary,
    size_t fragmentSize) {
  mTransactionId = transactionId;

  // Start with fragmentId at 1 since 0 is used to indicate
  // legacy behavior at CHRE
  size_t fragmentId = 1;
  size_t byteIndex = 0;
  do {
    if (fragmentId == 1) {
      mFragmentRequests.emplace_back(
          fragmentId++, transactionId, appId, appVersion, targetApiVersion,
          appBinary.size(), getSubVector(appBinary, byteIndex, fragmentSize));
    } else {
      mFragmentRequests.emplace_back(
          fragmentId++, transactionId,
          getSubVector(appBinary, byteIndex, fragmentSize));
    }

    byteIndex += fragmentSize;
  } while (byteIndex < appBinary.size());
}

const FragmentedLoadRequest& FragmentedLoadTransaction::getNextRequest() {
  return mFragmentRequests.at(mCurrentRequestIndex++);
}

bool FragmentedLoadTransaction::isComplete() const {
  return (mCurrentRequestIndex >= mFragmentRequests.size());
}

}  // namespace chre
}  // namespace android
