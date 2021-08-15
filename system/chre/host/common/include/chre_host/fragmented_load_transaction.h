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

#ifndef CHRE_HOST_FRAGMENTED_LOAD_TRANSACTION_H_
#define CHRE_HOST_FRAGMENTED_LOAD_TRANSACTION_H_

#include <cinttypes>
#include <vector>

namespace android {
namespace chre {

/**
 * A struct which represents a single fragmented request. The caller should use
 * this class along with FragmentedLoadTransaction to get global attributes for
 * the transaction and encode the load request using
 * HostProtocolHost::encodeFragmentedLoadNanoappRequest.
 */
struct FragmentedLoadRequest {
  size_t fragmentId;
  uint32_t transactionId;
  uint64_t appId;
  uint32_t appVersion;
  uint32_t targetApiVersion;
  size_t appTotalSizeBytes;
  std::vector<uint8_t> binary;

  FragmentedLoadRequest(
      size_t fragmentId, uint32_t transactionId,
      const std::vector<uint8_t>& binary) :
    FragmentedLoadRequest(fragmentId, transactionId, 0, 0, 0, 0, binary) {}

  FragmentedLoadRequest(
      size_t fragmentId, uint32_t transactionId, uint64_t appId,
      uint32_t appVersion, uint32_t targetApiVersion, size_t appTotalSizeBytes,
      const std::vector<uint8_t>& binary)
      : fragmentId(fragmentId),
        transactionId(transactionId),
        appId(appId),
        appVersion(appVersion),
        targetApiVersion(targetApiVersion),
        appTotalSizeBytes(appTotalSizeBytes),
        binary(binary) {}
};

/**
 * A class which splits a load transaction into separate requests with
 * fragmented binaries. This class can be used to send smaller chunks of data
 * when the kernel is under memory pressure and has limited contiguous memory.
 * The caller should use the getNextRequest() to retrieve the next available
 * fragment and send a load request with the fragmented binary and the fragment
 * ID.
 */
class FragmentedLoadTransaction {
 public:
   /**
    * @param transactionId the unique ID of the unfragmented load transaction
    * @param appId the unique ID of the nanoapp
    * @param appVersion the version of the nanoapp
    * @param targetApiVersion the API version this nanoapp is targeted for
    * @param appBinary the nanoapp binary data
    * @param fragmentSize the size of each fragment in bytes
    */
  FragmentedLoadTransaction(
      uint32_t transactionId, uint64_t appId, uint32_t appVersion,
      uint32_t targetApiVersion, const std::vector<uint8_t>& appBinary,
      size_t fragmentSize = kDefaultFragmentSize);

  /**
   * Retrieves the FragmentedLoadRequest including the next fragment of the
   * binary. Invoking getNextRequest() will prepare the next fragment for a
   * subsequent invocation.
   *
   * Invoking this method when there is no next request (i.e. isComplete()
   * returns true) is illegal.
   *
   * @return returns a reference to the next fragment.
   */
  const FragmentedLoadRequest& getNextRequest();

  /**
   * @return true if the last fragment has been retrieved by getNextRequest(),
   *         false otherwise.
   */
  bool isComplete() const;

  uint32_t getTransactionId() const {
    return mTransactionId;
  }

 private:
  std::vector<FragmentedLoadRequest> mFragmentRequests;
  size_t mCurrentRequestIndex = 0;
  uint32_t mTransactionId;

  static constexpr size_t kDefaultFragmentSize = 30 * 1024;
};

}  // namespace chre
}  // namespace android

#endif  // CHRE_HOST_FRAGMENTED_LOAD_TRANSACTION_H_
