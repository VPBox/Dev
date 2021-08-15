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

#include "chre/platform/slpi/smgr/smr_helper.h"

#include <inttypes.h>

#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/slpi/power_control_util.h"
#include "chre/util/lock_guard.h"
#include "chre/util/memory.h"

namespace chre {

smr_err SmrHelper::releaseSync(smr_client_hndl clientHandle,
                               Nanoseconds timeout) {
  // smr_client_release is synchronous for SMR services in the current
  // implementation, so we can't hold the lock while calling it otherwise we'll
  // deadlock in the callback.
  prepareForWait();

  smr_err result = smr_client_release(
      clientHandle, SmrHelper::smrReleaseCb, this);
  if (result != SMR_NO_ERR) {
    LOGE("SMR client release failed: %d", result);

    // Don't need to re-acquire the mutex for this, as we are assured that the
    // callback won't be invoked in the error case
    mWaiting = false;
  } else {
    LockGuard<Mutex> lock(mMutex);
    bool waitSuccess = true;
    while (mWaiting && waitSuccess) {
      waitSuccess = mCond.wait_for(mMutex, timeout);
    }

    if (!waitSuccess) {
      LOGE("Releasing SMR client timed out");
      result = SMR_TIMEOUT_ERR;
      mWaiting = false;
    }
  }

  return result;
}

smr_err SmrHelper::waitForService(qmi_idl_service_object_type serviceObj,
                                  Microseconds timeout) {
  // smr_client_check_ext is synchronous if the service already exists,
  // so don't hold the lock while calling to prevent deadlock in the callback.
  prepareForWait();

  smr_err result = smr_client_check_ext(serviceObj, SMR_CLIENT_INSTANCE_ANY,
                                        timeout.getMicroseconds(),
                                        SmrHelper::smrWaitForServiceCb, this);
  if (result != SMR_NO_ERR) {
    LOGE("Failed to wait for service: %d", result);

    // Don't need to re-acquire the mutex for this, as we are assured that the
    // callback won't be invoked in the error case
    mWaiting = false;
  } else {
    LockGuard<Mutex> lock(mMutex);
    while (mWaiting) {
      mCond.wait(mMutex);
    }

    if (mServiceTimedOut) {
      LOGE("Wait for SMR service timed out");
      result = SMR_TIMEOUT_ERR;
      mServiceTimedOut = false;
    }
  }

  return result;
}

smr_err SmrHelper::sendReqAsyncUntyped(
    smr_client_hndl client_handle, unsigned int msg_id,
    void *req_c_struct, unsigned int req_c_struct_len,
    void *resp_c_struct, unsigned int resp_c_struct_len,
    void *resp_cb_data, smr_client_resp_cb resp_cb) {
  // Force big image since smr_client_send_req is not supported in micro-image
  slpiForceBigImage();

  smr_err result = smr_client_send_req(client_handle, msg_id, req_c_struct,
                                       req_c_struct_len, resp_c_struct,
                                       resp_c_struct_len, resp_cb, resp_cb_data,
                                       nullptr /* txn_handle */);

  if (result != SMR_NO_ERR) {
    LOGE("Failed to send request (msg_id 0x%02x): %d", msg_id, result);
  }

  return result;
}

bool SmrHelper::sendReqSyncUntyped(
    smr_client_hndl client_handle, unsigned int msg_id,
    void *req_c_struct, unsigned int req_c_struct_len,
    void *resp_c_struct, unsigned int resp_c_struct_len,
    Nanoseconds timeout, smr_err *result) {
  // Set to false only in the event of timeout
  bool waitSuccess = true;

  // Force big image since smr_client_send_req is not supported in micro-image
  slpiForceBigImage();

  UniquePtr<SmrTransaction> txn = MakeUnique<SmrTransaction>();
  if (txn.isNull()) {
    FATAL_ERROR_OOM();
  }
  txn->parent = this;
  txn->reqBuf = req_c_struct;
  txn->rspBuf = resp_c_struct;
  txn->transactionId = mCurrentTransactionId;

  // Avoid holding the mutex while calling smr_client_send_req() - deadlock
  // could arise if we did, due to acquisition order for SMR's client_mutex in
  // the response flow
  prepareForWait();

  // Note that null txn_handle means we can't abandon the transaction, but it's
  // only supported for QMI (non-SMR) services, and we don't expect that anyway.
  // SMR itself does not support canceling transactions made to SMR services.
  *result = smr_client_send_req(
      client_handle, msg_id, req_c_struct, req_c_struct_len, resp_c_struct,
      resp_c_struct_len, SmrHelper::smrSyncRespCb, txn.get(),
      nullptr /* txn_handle */);
  if (*result != SMR_NO_ERR) {
    LOGE("Failed to send request (msg_id 0x%02x): %d", msg_id, *result);

    // Don't need to re-acquire the mutex for this, as we are assured that the
    // callback won't be invoked in the error case
    mWaiting = false;
  } else {
    LockGuard<Mutex> lock(mMutex);
    while (mWaiting && waitSuccess) {
      waitSuccess = mCond.wait_for(mMutex, timeout);
    }

    if (waitSuccess) {
      *result = mTranspErr;
    } else {
      LOGE("SMR request for msg_id 0x%02x timed out after %" PRIu64 " ms",
           msg_id, Milliseconds(timeout).getMilliseconds());
      *result = SMR_TIMEOUT_ERR;

      // If the callback comes later, we'll recognize the mismatched transaction
      // ID and drop the response. Note that we don't increment this in the
      // successful case, as it's not necessary in that case, and this gives us
      // a way to track the number of timeouts we've hit.
      mCurrentTransactionId++;
      mWaiting = false;
      txn.release();
    }
  }

  return waitSuccess;
}

void SmrHelper::handleResp(smr_client_hndl client_handle, unsigned int msg_id,
                           void *resp_c_struct, unsigned int resp_c_struct_len,
                           smr_err transp_err, SmrTransaction *txn) {
  LockGuard<Mutex> lock(mMutex);

  if (!mWaiting || txn->transactionId != mCurrentTransactionId) {
    LOGE("Got expired SMR response (my ID %" PRIu32 " vs current %" PRIu32 ")",
         txn->transactionId, mCurrentTransactionId);

    // If this happens, it means the requestor timed out, so it's depending on
    // us to release the memory
    memoryFree(txn->reqBuf);
    memoryFree(txn->rspBuf);
    memoryFree(txn);
  } else {
    // SMR will handle copying the response into the buffer passed in to
    // smr_client_send_req(), so we just need to unblock the waiting thread
    mTranspErr = transp_err;
    mWaiting = false;
    mCond.notify_one();
  }
}

void SmrHelper::prepareForWait() {
  LockGuard<Mutex> lock(mMutex);
  CHRE_ASSERT(!mWaiting);
  mWaiting = true;
}

void SmrHelper::smrReleaseCb(void *release_cb_data) {
  SmrHelper *obj = static_cast<SmrHelper *>(release_cb_data);
  LockGuard<Mutex> lock(obj->mMutex);
  obj->mWaiting = false;
  obj->mCond.notify_one();
}

void SmrHelper::smrSyncRespCb(
    smr_client_hndl client_handle, unsigned int msg_id, void *resp_c_struct,
    unsigned int resp_c_struct_len, void *resp_cb_data, smr_err transp_err) {
  auto *txn = static_cast<SmrTransaction *>(resp_cb_data);
  txn->parent->handleResp(
      client_handle, msg_id, resp_c_struct, resp_c_struct_len, transp_err, txn);
}

void SmrHelper::smrWaitForServiceCb(
    qmi_idl_service_object_type /* service_obj */,
    qmi_service_instance /* instance_id */, bool timeout_expired,
    void *wait_for_service_cb_data) {
  SmrHelper *obj = static_cast<SmrHelper *>(wait_for_service_cb_data);
  LockGuard<Mutex> lock(obj->mMutex);
  obj->mServiceTimedOut = timeout_expired;
  obj->mWaiting = false;
  obj->mCond.notify_one();
}

}  // namespace chre
