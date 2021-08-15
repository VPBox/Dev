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

#ifndef CHRE_PLATFORM_SLPI_NANOAPP_LOAD_MANAGER_H_
#define CHRE_PLATFORM_SLPI_NANOAPP_LOAD_MANAGER_H_

#include <cstddef>
#include <cstdint>

#include "chre/core/nanoapp.h"
#include "chre/util/non_copyable.h"
#include "chre/util/unique_ptr.h"

namespace chre {

/**
 * A struct that holds metadata of a fragmented nanoapp load transaction.
 */
struct FragmentedLoadInfo {
  //! The ID of the client that initiated this transaction.
  uint16_t hostClientId;
  //! The unique ID of this load transaction.
  uint32_t transactionId;
  //! The next fragment ID that is expected to be received for this transaction.
  uint32_t nextFragmentId;
};

/**
 * A class which handles loading a (possibly fragmented) nanoapp binary.
 */
class NanoappLoadManager : public NonCopyable {
 public:
   /**
    * Prepares for a (possibly fragmented) load transaction. If an ongoing
    * transaction exists, the transaction will be overwritten by the new
    * incoming transaction.
    *
    * @param hostClientId the ID of client that originated this transaction
    * @param transactionId the ID of the transaction
    * @param appId the ID of the app to load
    * @param appVersion the version of the app to load
    * @param totalBinaryLen the total nanoapp binary length
    *
    * @return true if the preparation was successful, false otherwise
    */
  bool prepareForLoad(
      uint16_t hostClientId, uint32_t transactionId, uint64_t appId,
      uint32_t appVersion, size_t totalBinaryLen);

  /**
   * Copies a fragment of a nanoapp binary. If the parameters do not match the
   * expected load transaction, the transaction is marked as a failure.
   *
   * @param hostClientId the ID of client that originated this transaction
   * @param transactionId the ID of the transaction
   * @param fragmentId the ID of the fragment
   * @param buffer the pointer to the buffer binary
   * @param bufferLen the size of the buffer in bytes
   *
   * @return true if the copy was successful, false otherwise
   */
  bool copyNanoappFragment(
      uint16_t hostClientId, uint32_t transactionId, uint32_t fragmentId,
      const void *buffer, size_t bufferLen);

  /**
   * Invalidates an ongoing load transaction. After this method is invoked,
   * hasPendingLoadTransaction() will return false, and a new transaction must
   * be started by invoking prepareForLoad.
   */
  void markFailure() {
    mNanoapp.reset(nullptr);
  }

  /**
   * @return true if a pending transaction exists, false otherwise
   */
  bool hasPendingLoadTransaction() const {
    return !mNanoapp.isNull();
  }

  /**
   * @return true if a pending transaction exists and the nanoapp is fully
   *         loaded, false otherwise
   */
  bool isLoadComplete() const {
    return hasPendingLoadTransaction() && mNanoapp->isLoaded();
  }

  /**
   * @return the currently ongoing load transaction, invalid if
   *         hasPendingLoadTransaction() returns false
   */
  FragmentedLoadInfo getTransactionInfo() const {
    return mCurrentLoadInfo;
  }

  /**
   * Releases the underlying nanoapp of a currently ongoing load transaction,
   * regardless of completion status. After this method is called, the ownership
   * of the nanoapp is transferred to the caller. This method should only be
   * called if hasPendingLoadTransaction() is true.
   *
   * @return the UniquePtr<Nanoapp> of the ongoing transaction, or null if no
   *             transaction exists
   */
  UniquePtr<Nanoapp> releaseNanoapp() {
    return mNanoapp.release();
  }

 private:
  //! The currently managed fragmented load.
  FragmentedLoadInfo mCurrentLoadInfo;

  //! The underlying nanoapp that is being loaded.
  UniquePtr<Nanoapp> mNanoapp;

  /**
   * Validates an incoming fragment against the next expected one. An error is
   * logged if invalid arguments are passed.
   *
   * @return true if the arguments represent the next fragment, false otherwise
   */
  bool validateFragment(
      uint16_t hostClientId, uint32_t transactionId, uint32_t fragmentId) const;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_NANOAPP_LOAD_MANAGER_H_
