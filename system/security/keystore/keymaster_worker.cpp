/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#define LOG_TAG "keymaster_worker"

#include "keymaster_worker.h"

#include "keystore_utils.h"

#include <android-base/logging.h>

#include "KeyStore.h"
#include "keymaster_enforcement.h"

#include "key_proto_handler.h"
#include "keystore_utils.h"

namespace keystore {

constexpr size_t kMaxOperations = 15;

using AndroidKeymasterArguments = android::security::keymaster::KeymasterArguments;
using android::security::keymaster::ExportResult;
using android::security::keymaster::operationFailed;
using android::security::keymaster::OperationResult;

Worker::Worker() {}
Worker::~Worker() {
    std::unique_lock<std::mutex> lock(pending_requests_mutex_);
    pending_requests_cond_var_.wait(lock, [this] { return pending_requests_.empty(); });
}
void Worker::addRequest(WorkerTask request) {
    std::unique_lock<std::mutex> lock(pending_requests_mutex_);
    bool start_thread = pending_requests_.empty();
    pending_requests_.push(std::move(request));
    lock.unlock();
    if (start_thread) {
        auto worker = std::thread([this] {
            std::unique_lock<std::mutex> lock(pending_requests_mutex_);
            running_ = true;
            while (!pending_requests_.empty()) {
                auto request = std::move(pending_requests_.front());
                lock.unlock();
                request();
                lock.lock();
                pending_requests_.pop();
                pending_requests_cond_var_.notify_all();
            }
        });
        worker.detach();
    }
}

KeymasterWorker::KeymasterWorker(sp<Keymaster> keymasterDevice, KeyStore* keyStore)
    : keymasterDevice_(std::move(keymasterDevice)), operationMap_(keyStore), keyStore_(keyStore) {
    // make sure that hal version is cached.
    if (keymasterDevice_) keymasterDevice_->halVersion();
}

void KeymasterWorker::logIfKeymasterVendorError(ErrorCode ec) const {
    keymasterDevice_->logIfKeymasterVendorError(ec);
}

std::tuple<KeyStoreServiceReturnCode, Blob>
KeymasterWorker::upgradeKeyBlob(const LockedKeyBlobEntry& lockedEntry,
                                const AuthorizationSet& params) {
    LOG(INFO) << "upgradeKeyBlob " << lockedEntry->alias() << " " << (uint32_t)lockedEntry->uid();

    std::tuple<KeyStoreServiceReturnCode, Blob> result;

    auto userState = keyStore_->getUserStateDB().getUserStateByUid(lockedEntry->uid());

    Blob& blob = std::get<1>(result);
    KeyStoreServiceReturnCode& error = std::get<0>(result);

    Blob charBlob;
    ResponseCode rc;

    std::tie(rc, blob, charBlob) =
        lockedEntry.readBlobs(userState->getEncryptionKey(), userState->getState());

    userState = {};

    if (rc != ResponseCode::NO_ERROR) {
        return error = rc, result;
    }

    auto hidlKey = blob2hidlVec(blob);
    auto& dev = keymasterDevice_;

    auto hidlCb = [&](ErrorCode ret, const ::std::vector<uint8_t>& upgradedKeyBlob) {
        dev->logIfKeymasterVendorError(ret);
        error = ret;
        if (!error.isOk()) {
            if (error == ErrorCode::INVALID_KEY_BLOB) {
                log_key_integrity_violation(lockedEntry->alias().c_str(), lockedEntry->uid());
            }
            return;
        }

        error = keyStore_->del(lockedEntry);
        if (!error.isOk()) {
            ALOGI("upgradeKeyBlob keystore->del failed %d", error.getErrorCode());
            return;
        }

        Blob newBlob(&upgradedKeyBlob[0], upgradedKeyBlob.size(), nullptr /* info */,
                     0 /* infoLength */, ::TYPE_KEYMASTER_10);
        newBlob.setSecurityLevel(blob.getSecurityLevel());
        newBlob.setEncrypted(blob.isEncrypted());
        newBlob.setSuperEncrypted(blob.isSuperEncrypted());
        newBlob.setCriticalToDeviceEncryption(blob.isCriticalToDeviceEncryption());

        error = keyStore_->put(lockedEntry, newBlob, charBlob);
        if (!error.isOk()) {
            ALOGI("upgradeKeyBlob keystore->put failed %d", error.getErrorCode());
            return;
        }
        blob = std::move(newBlob);
    };

    KeyStoreServiceReturnCode error2;
    error2 = KS_HANDLE_HIDL_ERROR(dev, dev->upgradeKey(hidlKey, params.hidl_data(), hidlCb));
    if (!error2.isOk()) {
        return error = error2, result;
    }

    return result;
}

std::tuple<KeyStoreServiceReturnCode, KeyCharacteristics, Blob, Blob>
KeymasterWorker::createKeyCharacteristicsCache(const LockedKeyBlobEntry& lockedEntry,
                                               const hidl_vec<uint8_t>& clientId,
                                               const hidl_vec<uint8_t>& appData, Blob keyBlob,
                                               Blob charBlob) {
    std::tuple<KeyStoreServiceReturnCode, KeyCharacteristics, Blob, Blob> result;

#if __cplusplus == 201703L
    auto& [rc, resultCharacteristics, outBlob, charOutBlob] = result;
#else
    KeyStoreServiceReturnCode& rc = std::get<0>(result);
    KeyCharacteristics& resultCharacteristics = std::get<1>(result);
    Blob& outBlob = std::get<2>(result);
    Blob& charOutBlob = std::get<3>(result);
#endif

    rc = ResponseCode::SYSTEM_ERROR;
    if (!keyBlob) return result;
    auto hidlKeyBlob = blob2hidlVec(keyBlob);
    auto& dev = keymasterDevice_;

    KeyStoreServiceReturnCode error;

    AuthorizationSet hwEnforced, swEnforced;
    bool success = true;

    if (charBlob) {
        std::tie(success, hwEnforced, swEnforced) = charBlob.getKeyCharacteristics();
    }
    if (!success) {
        LOG(ERROR) << "Failed to read cached key characteristics";
        return rc = ResponseCode::SYSTEM_ERROR, result;
    }

    auto hidlCb = [&](ErrorCode ret, const KeyCharacteristics& keyCharacteristics) {
        dev->logIfKeymasterVendorError(ret);
        error = ret;
        if (!error.isOk()) {
            if (error == ErrorCode::INVALID_KEY_BLOB) {
                log_key_integrity_violation(lockedEntry->alias().c_str(), lockedEntry->uid());
            }
            return;
        }

        // Replace the sw_enforced set with those persisted to disk, minus hw_enforced
        AuthorizationSet softwareEnforced = keyCharacteristics.softwareEnforced;
        hwEnforced = keyCharacteristics.hardwareEnforced;
        swEnforced.Union(softwareEnforced);
        softwareEnforced.Subtract(hwEnforced);

        // We only get the characteristics from keymaster if there was no cache file or the
        // the chach file was a legacy cache file. So lets write a new cache file for the next time.
        Blob newCharBlob;
        success = newCharBlob.putKeyCharacteristics(hwEnforced, swEnforced);
        if (!success) {
            error = ResponseCode::SYSTEM_ERROR;
            LOG(ERROR) << "Failed to serialize cached key characteristics";
            return;
        }

        error = keyStore_->put(lockedEntry, {}, newCharBlob);
        if (!error.isOk()) {
            ALOGE("Failed to write key characteristics cache");
            return;
        }
        charBlob = std::move(newCharBlob);
    };

    if (!charBlob || charBlob.getType() == TYPE_KEY_CHARACTERISTICS) {
        // this updates the key characteristics cache file to the new format or creates one in
        // in the first place
        rc = KS_HANDLE_HIDL_ERROR(
            dev, dev->getKeyCharacteristics(hidlKeyBlob, clientId, appData, hidlCb));
        if (!rc.isOk()) {
            return result;
        }

        if (error == ErrorCode::KEY_REQUIRES_UPGRADE) {
            AuthorizationSet upgradeParams;
            if (clientId.size()) {
                upgradeParams.push_back(TAG_APPLICATION_ID, clientId);
            }
            if (appData.size()) {
                upgradeParams.push_back(TAG_APPLICATION_DATA, appData);
            }
            std::tie(rc, keyBlob) = upgradeKeyBlob(lockedEntry, upgradeParams);
            if (!rc.isOk()) {
                return result;
            }

            auto upgradedHidlKeyBlob = blob2hidlVec(keyBlob);

            rc = KS_HANDLE_HIDL_ERROR(
                dev, dev->getKeyCharacteristics(upgradedHidlKeyBlob, clientId, appData, hidlCb));
            if (!rc.isOk()) {
                return result;
            }
        }
    }

    resultCharacteristics.hardwareEnforced = hwEnforced.hidl_data();
    resultCharacteristics.softwareEnforced = swEnforced.hidl_data();

    outBlob = std::move(keyBlob);
    charOutBlob = std::move(charBlob);
    rc = error;
    return result;
}

/**
 * Get the auth token for this operation from the auth token table.
 *
 * Returns ResponseCode::NO_ERROR if the auth token was set or none was required.
 *         ::OP_AUTH_NEEDED if it is a per op authorization, no
 *         authorization token exists for that operation and
 *         failOnTokenMissing is false.
 *         KM_ERROR_KEY_USER_NOT_AUTHENTICATED if there is no valid auth
 *         token for the operation
 */
std::pair<KeyStoreServiceReturnCode, HardwareAuthToken>
KeymasterWorker::getAuthToken(const KeyCharacteristics& characteristics, uint64_t handle,
                              KeyPurpose purpose, bool failOnTokenMissing) {

    AuthorizationSet allCharacteristics(characteristics.softwareEnforced);
    allCharacteristics.append(characteristics.hardwareEnforced.begin(),
                              characteristics.hardwareEnforced.end());

    HardwareAuthToken authToken;
    AuthTokenTable::Error err;
    std::tie(err, authToken) = keyStore_->getAuthTokenTable().FindAuthorization(
        allCharacteristics, static_cast<KeyPurpose>(purpose), handle);

    KeyStoreServiceReturnCode rc;

    switch (err) {
    case AuthTokenTable::OK:
    case AuthTokenTable::AUTH_NOT_REQUIRED:
        rc = ResponseCode::NO_ERROR;
        break;

    case AuthTokenTable::AUTH_TOKEN_NOT_FOUND:
    case AuthTokenTable::AUTH_TOKEN_EXPIRED:
    case AuthTokenTable::AUTH_TOKEN_WRONG_SID:
        ALOGE("getAuthToken failed: %d", err);  // STOPSHIP: debug only, to be removed
        rc = ErrorCode::KEY_USER_NOT_AUTHENTICATED;
        break;

    case AuthTokenTable::OP_HANDLE_REQUIRED:
        rc = failOnTokenMissing ? KeyStoreServiceReturnCode(ErrorCode::KEY_USER_NOT_AUTHENTICATED)
                                : KeyStoreServiceReturnCode(ResponseCode::OP_AUTH_NEEDED);
        break;

    default:
        ALOGE("Unexpected FindAuthorization return value %d", err);
        rc = ErrorCode::INVALID_ARGUMENT;
    }

    return {rc, std::move(authToken)};
}

KeyStoreServiceReturnCode KeymasterWorker::abort(const sp<IBinder>& token) {
    auto op = operationMap_.removeOperation(token, false /* wasOpSuccessful */);
    if (op) {
        keyStore_->getAuthTokenTable().MarkCompleted(op->handle);
        return KS_HANDLE_HIDL_ERROR(keymasterDevice_, keymasterDevice_->abort(op->handle));
    } else {
        return ErrorCode::INVALID_OPERATION_HANDLE;
    }
}

/**
 * Prune the oldest pruneable operation.
 */
bool KeymasterWorker::pruneOperation() {
    sp<IBinder> oldest = operationMap_.getOldestPruneableOperation();
    ALOGD("Trying to prune operation %p", oldest.get());
    size_t op_count_before_abort = operationMap_.getOperationCount();
    // We mostly ignore errors from abort() because all we care about is whether at least
    // one operation has been removed.
    auto rc = abort(oldest);
    keyStore_->removeOperationDevice(oldest);
    if (operationMap_.getOperationCount() >= op_count_before_abort) {
        ALOGE("Failed to abort pruneable operation %p, error: %d", oldest.get(), rc.getErrorCode());
        return false;
    }
    return true;
}

// My IDE defines "CAPTURE_MOVE(x) x" because it does not understand generalized lambda captures.
// It should never be redefined by a build system though.
#ifndef CAPTURE_MOVE
#define CAPTURE_MOVE(x) x = std::move(x)
#endif

void KeymasterWorker::begin(LockedKeyBlobEntry lockedEntry, sp<IBinder> appToken, Blob keyBlob,
                            Blob charBlob, bool pruneable, KeyPurpose purpose,
                            AuthorizationSet opParams, hidl_vec<uint8_t> entropy,
                            worker_begin_cb worker_cb) {

    Worker::addRequest([this, CAPTURE_MOVE(lockedEntry), CAPTURE_MOVE(appToken),
                        CAPTURE_MOVE(keyBlob), CAPTURE_MOVE(charBlob), pruneable, purpose,
                        CAPTURE_MOVE(opParams), CAPTURE_MOVE(entropy),
                        CAPTURE_MOVE(worker_cb)]() mutable {
        // Concurrently executed

        auto& dev = keymasterDevice_;

        KeyCharacteristics characteristics;

        {
            hidl_vec<uint8_t> clientId;
            hidl_vec<uint8_t> appData;
            for (const auto& param : opParams) {
                if (param.tag == Tag::APPLICATION_ID) {
                    clientId = authorizationValue(TAG_APPLICATION_ID, param).value();
                } else if (param.tag == Tag::APPLICATION_DATA) {
                    appData = authorizationValue(TAG_APPLICATION_DATA, param).value();
                }
            }
            KeyStoreServiceReturnCode error;
            std::tie(error, characteristics, keyBlob, charBlob) = createKeyCharacteristicsCache(
                lockedEntry, clientId, appData, std::move(keyBlob), std::move(charBlob));
            if (!error.isOk()) {
                worker_cb(operationFailed(error));
                return;
            }
        }

        KeyStoreServiceReturnCode rc, authRc;
        HardwareAuthToken authToken;
        std::tie(authRc, authToken) = getAuthToken(characteristics, 0 /* no challenge */, purpose,
                                                   /*failOnTokenMissing*/ false);

        // If per-operation auth is needed we need to begin the operation and
        // the client will need to authorize that operation before calling
        // update. Any other auth issues stop here.
        if (!authRc.isOk() && authRc != ResponseCode::OP_AUTH_NEEDED) {
            return worker_cb(operationFailed(authRc));
        }

        // Add entropy to the device first.
        if (entropy.size()) {
            rc = KS_HANDLE_HIDL_ERROR(dev, dev->addRngEntropy(entropy));
            if (!rc.isOk()) {
                return worker_cb(operationFailed(rc));
            }
        }

        // Create a keyid for this key.
        auto keyid = KeymasterEnforcement::CreateKeyId(blob2hidlVec(keyBlob));
        if (!keyid) {
            ALOGE("Failed to create a key ID for authorization checking.");
            return worker_cb(operationFailed(ErrorCode::UNKNOWN_ERROR));
        }

        // Check that all key authorization policy requirements are met.
        AuthorizationSet key_auths = characteristics.hardwareEnforced;
        key_auths.append(characteristics.softwareEnforced.begin(),
                         characteristics.softwareEnforced.end());

        rc = keyStore_->getEnforcementPolicy().AuthorizeOperation(
            purpose, *keyid, key_auths, opParams, authToken, 0 /* op_handle */,
            true /* is_begin_operation */);
        if (!rc.isOk()) {
            return worker_cb(operationFailed(rc));
        }

        // If there are more than kMaxOperations, abort the oldest operation that was started as
        // pruneable.
        while (operationMap_.getOperationCount() >= kMaxOperations) {
            ALOGD("Reached or exceeded concurrent operations limit");
            if (!pruneOperation()) {
                break;
            }
        }

        android::security::keymaster::OperationResult result;

        auto hidlCb = [&](ErrorCode ret, const hidl_vec<KeyParameter>& outParams,
                          uint64_t operationHandle) {
            dev->logIfKeymasterVendorError(ret);
            result.resultCode = ret;
            if (!result.resultCode.isOk()) {
                if (result.resultCode == ErrorCode::INVALID_KEY_BLOB) {
                    log_key_integrity_violation(lockedEntry->alias().c_str(), lockedEntry->uid());
                }
                return;
            }
            result.handle = operationHandle;
            result.outParams = outParams;
        };

        do {
            rc = KS_HANDLE_HIDL_ERROR(dev, dev->begin(purpose, blob2hidlVec(keyBlob),
                                                      opParams.hidl_data(), authToken, hidlCb));
            if (!rc.isOk()) {
                LOG(ERROR) << "Got error " << rc << " from begin()";
                return worker_cb(operationFailed(ResponseCode::SYSTEM_ERROR));
            }

            if (result.resultCode == ErrorCode::KEY_REQUIRES_UPGRADE) {
                std::tie(rc, keyBlob) = upgradeKeyBlob(lockedEntry, opParams);
                if (!rc.isOk()) {
                    return worker_cb(operationFailed(rc));
                }

                rc = KS_HANDLE_HIDL_ERROR(dev, dev->begin(purpose, blob2hidlVec(keyBlob),
                                                          opParams.hidl_data(), authToken, hidlCb));
                if (!rc.isOk()) {
                    LOG(ERROR) << "Got error " << rc << " from begin()";
                    return worker_cb(operationFailed(ResponseCode::SYSTEM_ERROR));
                }
            }
            // If there are too many operations abort the oldest operation that was
            // started as pruneable and try again.
        } while (result.resultCode == ErrorCode::TOO_MANY_OPERATIONS && pruneOperation());

        rc = result.resultCode;
        if (!rc.isOk()) {
            return worker_cb(operationFailed(rc));
        }

        // Note: The operation map takes possession of the contents of "characteristics".
        // It is safe to use characteristics after the following line but it will be empty.
        sp<IBinder> operationToken =
            operationMap_.addOperation(result.handle, *keyid, purpose, dev, appToken,
                                       std::move(characteristics), opParams.hidl_data(), pruneable);
        assert(characteristics.hardwareEnforced.size() == 0);
        assert(characteristics.softwareEnforced.size() == 0);
        result.token = operationToken;

        auto operation = operationMap_.getOperation(operationToken);
        if (!operation) {
            return worker_cb(operationFailed(ResponseCode::SYSTEM_ERROR));
        }

        if (authRc.isOk() && authToken.mac.size() &&
            dev->halVersion().securityLevel == SecurityLevel::STRONGBOX) {
            operation->authTokenFuture = operation->authTokenPromise.get_future();
            std::weak_ptr<Operation> weak_operation = operation;

            auto verifyTokenCB = [weak_operation](KeyStoreServiceReturnCode rc,
                                                  HardwareAuthToken authToken,
                                                  VerificationToken verificationToken) {
                auto operation = weak_operation.lock();
                if (!operation) {
                    // operation aborted, nothing to do
                    return;
                }
                if (rc.isOk()) {
                    operation->authToken = std::move(authToken);
                    operation->verificationToken = std::move(verificationToken);
                }
                operation->authTokenPromise.set_value(rc);
            };
            auto teeKmDevice = keyStore_->getDevice(SecurityLevel::TRUSTED_ENVIRONMENT);
            teeKmDevice->verifyAuthorization(result.handle, {}, std::move(authToken),
                                             std::move(verifyTokenCB));
        }

        // Return the authentication lookup result. If this is a per operation
        // auth'd key then the resultCode will be ::OP_AUTH_NEEDED and the
        // application should get an auth token using the handle before the
        // first call to update, which will fail if keystore hasn't received the
        // auth token.
        if (result.resultCode.isOk()) {
            result.resultCode = authRc;
        }
        return worker_cb(result);
    });
}

KeyStoreServiceReturnCode
KeymasterWorker::getOperationAuthTokenIfNeeded(std::shared_ptr<Operation> op) {
    if (!op) return ErrorCode::INVALID_OPERATION_HANDLE;

    if (op->authTokenFuture.valid()) {
        LOG(INFO) << "Waiting for verification token";
        op->authTokenFuture.wait();
        auto rc = op->authTokenFuture.get();
        if (!rc.isOk()) {
            return rc;
        }
        op->authTokenFuture = {};
    } else if (!op->hasAuthToken()) {
        KeyStoreServiceReturnCode rc;
        HardwareAuthToken found;
        std::tie(rc, found) = getAuthToken(op->characteristics, op->handle, op->purpose);
        if (!rc.isOk()) return rc;
        op->authToken = std::move(found);
    }

    return ResponseCode::NO_ERROR;
}

namespace {

class Finalize {
  private:
    std::function<void()> f_;

  public:
    explicit Finalize(std::function<void()> f) : f_(f) {}
    ~Finalize() {
        if (f_) f_();
    }
    void release() { f_ = {}; }
};

}  // namespace

void KeymasterWorker::update(sp<IBinder> token, AuthorizationSet params, hidl_vec<uint8_t> data,
                             update_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(token), CAPTURE_MOVE(params), CAPTURE_MOVE(data),
                        CAPTURE_MOVE(worker_cb)]() {
        KeyStoreServiceReturnCode rc;
        auto op = operationMap_.getOperation(token);
        if (!op) {
            return worker_cb(operationFailed(ErrorCode::INVALID_OPERATION_HANDLE));
        }

        Finalize abort_operation_in_case_of_error([&] {
            operationMap_.removeOperation(token, false);
            keyStore_->getAuthTokenTable().MarkCompleted(op->handle);
            KS_HANDLE_HIDL_ERROR(keymasterDevice_, keymasterDevice_->abort(op->handle));
        });

        rc = getOperationAuthTokenIfNeeded(op);
        if (!rc.isOk()) return worker_cb(operationFailed(rc));

        // Check that all key authorization policy requirements are met.
        AuthorizationSet key_auths(op->characteristics.hardwareEnforced);
        key_auths.append(op->characteristics.softwareEnforced.begin(),
                         op->characteristics.softwareEnforced.end());

        rc = keyStore_->getEnforcementPolicy().AuthorizeOperation(op->purpose, op->keyid, key_auths,
                                                                  params, op->authToken, op->handle,
                                                                  false /* is_begin_operation */);
        if (!rc.isOk()) return worker_cb(operationFailed(rc));

        OperationResult result;
        auto hidlCb = [&](ErrorCode ret, uint32_t inputConsumed,
                          const hidl_vec<KeyParameter>& outParams,
                          const ::std::vector<uint8_t>& output) {
            op->device->logIfKeymasterVendorError(ret);
            result.resultCode = ret;
            if (result.resultCode.isOk()) {
                result.inputConsumed = inputConsumed;
                result.outParams = outParams;
                result.data = output;
            }
        };

        rc = KS_HANDLE_HIDL_ERROR(op->device,
                                  op->device->update(op->handle, params.hidl_data(), data,
                                                     op->authToken, op->verificationToken, hidlCb));

        // just a reminder: on success result->resultCode was set in the callback. So we only
        // overwrite it if there was a communication error indicated by the ErrorCode.
        if (!rc.isOk()) result.resultCode = rc;
        if (result.resultCode.isOk()) {
            // if everything went well we don't abort the operation.
            abort_operation_in_case_of_error.release();
        }
        return worker_cb(std::move(result));
    });
}

/**
 * Check that all KeyParameters provided by the application are allowed. Any parameter that keystore
 * adds itself should be disallowed here.
 */
template <typename ParamsIter>
static bool checkAllowedOperationParams(ParamsIter begin, const ParamsIter end) {
    while (begin != end) {
        switch (begin->tag) {
        case Tag::ATTESTATION_APPLICATION_ID:
        case Tag::RESET_SINCE_ID_ROTATION:
            return false;
        default:
            break;
        }
        ++begin;
    }
    return true;
}

void KeymasterWorker::finish(sp<IBinder> token, AuthorizationSet params, hidl_vec<uint8_t> input,
                             hidl_vec<uint8_t> signature, hidl_vec<uint8_t> entropy,
                             finish_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(token), CAPTURE_MOVE(params), CAPTURE_MOVE(input),
                        CAPTURE_MOVE(signature), CAPTURE_MOVE(entropy),
                        CAPTURE_MOVE(worker_cb)]() mutable {
        KeyStoreServiceReturnCode rc;
        auto op = operationMap_.getOperation(token);
        if (!op) {
            return worker_cb(operationFailed(ErrorCode::INVALID_OPERATION_HANDLE));
        }

        bool finished = false;
        Finalize abort_operation_in_case_of_error([&] {
            operationMap_.removeOperation(token, finished && rc.isOk());
            keyStore_->getAuthTokenTable().MarkCompleted(op->handle);
            if (!finished)
                KS_HANDLE_HIDL_ERROR(keymasterDevice_, keymasterDevice_->abort(op->handle));
        });

        if (!checkAllowedOperationParams(params.begin(), params.end())) {
            return worker_cb(operationFailed(ErrorCode::INVALID_ARGUMENT));
        }

        rc = getOperationAuthTokenIfNeeded(op);
        if (!rc.isOk()) return worker_cb(operationFailed(rc));

        // Check that all key authorization policy requirements are met.
        AuthorizationSet key_auths(op->characteristics.hardwareEnforced);
        key_auths.append(op->characteristics.softwareEnforced.begin(),
                         op->characteristics.softwareEnforced.end());

        if (key_auths.Contains(Tag::TRUSTED_CONFIRMATION_REQUIRED)) {
            hidl_vec<uint8_t> confirmationToken =
                keyStore_->getConfirmationManager().getLatestConfirmationToken();
            if (confirmationToken.size() == 0) {
                LOG(ERROR) << "Confirmation token required but none found";
                return worker_cb(operationFailed(ErrorCode::NO_USER_CONFIRMATION));
            }
            params.push_back(keymaster::TAG_CONFIRMATION_TOKEN, std::move(confirmationToken));
        }

        rc = keyStore_->getEnforcementPolicy().AuthorizeOperation(op->purpose, op->keyid, key_auths,
                                                                  params, op->authToken, op->handle,
                                                                  false /* is_begin_operation */);
        if (!rc.isOk()) return worker_cb(operationFailed(rc));

        if (entropy.size()) {
            rc = KS_HANDLE_HIDL_ERROR(op->device, op->device->addRngEntropy(entropy));
            if (!rc.isOk()) {
                return worker_cb(operationFailed(rc));
            }
        }

        OperationResult result;
        auto hidlCb = [&](ErrorCode ret, const hidl_vec<KeyParameter>& outParams,
                          const ::std::vector<uint8_t>& output) {
            op->device->logIfKeymasterVendorError(ret);
            result.resultCode = ret;
            if (result.resultCode.isOk()) {
                result.outParams = outParams;
                result.data = output;
            }
        };

        rc = KS_HANDLE_HIDL_ERROR(op->device, op->device->finish(op->handle, params.hidl_data(),
                                                                 input, signature, op->authToken,
                                                                 op->verificationToken, hidlCb));

        if (rc.isOk()) {
            // inform the finalizer that the finish call went through
            finished = true;
            // and what the result was
            rc = result.resultCode;
        } else {
            return worker_cb(operationFailed(rc));
        }
        return worker_cb(std::move(result));
    });
}

void KeymasterWorker::abort(sp<IBinder> token, abort_cb worker_cb) {
    Worker::addRequest(
        [this, CAPTURE_MOVE(token), CAPTURE_MOVE(worker_cb)]() { return worker_cb(abort(token)); });
}

void KeymasterWorker::verifyAuthorization(uint64_t challenge, hidl_vec<KeyParameter> params,
                                          HardwareAuthToken token,
                                          verifyAuthorization_cb worker_cb) {
    Worker::addRequest([this, challenge, CAPTURE_MOVE(params), CAPTURE_MOVE(token),
                        CAPTURE_MOVE(worker_cb)]() {
        KeyStoreServiceReturnCode error;
        VerificationToken verificationToken;
        KeyStoreServiceReturnCode rc = KS_HANDLE_HIDL_ERROR(
            keymasterDevice_,
            keymasterDevice_->verifyAuthorization(
                challenge, params, token, [&](ErrorCode ret, const VerificationToken& vToken) {
                    keymasterDevice_->logIfKeymasterVendorError(ret);
                    error = ret;
                    verificationToken = vToken;
                }));
        worker_cb(rc.isOk() ? error : rc, std::move(token), std::move(verificationToken));
    });
}

void KeymasterWorker::addRngEntropy(hidl_vec<uint8_t> data, addRngEntropy_cb _hidl_cb) {
    addRequest(&Keymaster::addRngEntropy, std::move(_hidl_cb), std::move(data));
}

namespace {
bool containsTag(const hidl_vec<KeyParameter>& params, Tag tag) {
    return params.end() !=
           std::find_if(params.begin(), params.end(),
                        [&](const KeyParameter& param) { return param.tag == tag; });
}

bool isAuthenticationBound(const hidl_vec<KeyParameter>& params) {
    return !containsTag(params, Tag::NO_AUTH_REQUIRED);
}
}  // namespace

void KeymasterWorker::generateKey(LockedKeyBlobEntry lockedEntry, hidl_vec<KeyParameter> keyParams,
                                  hidl_vec<uint8_t> entropy, int flags, generateKey_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(lockedEntry), CAPTURE_MOVE(keyParams),
                        CAPTURE_MOVE(entropy), CAPTURE_MOVE(worker_cb), flags]() mutable {
        KeyStoreServiceReturnCode rc =
            KS_HANDLE_HIDL_ERROR(keymasterDevice_, keymasterDevice_->addRngEntropy(entropy));
        if (!rc.isOk()) {
            return worker_cb(rc, {});
        }

        SecurityLevel securityLevel = keymasterDevice_->halVersion().securityLevel;

        // Fallback cannot be considered for Strongbox. Further versions restrictions are enforced
        // by KeyStore::getFallbackDevice()
        bool consider_fallback = securityLevel == SecurityLevel::TRUSTED_ENVIRONMENT;

        Finalize logOnFail(
            [&] { uploadKeyCharacteristicsAsProto(keyParams, false /* wasCreationSuccessful */); });

        KeyCharacteristics outCharacteristics;
        KeyStoreServiceReturnCode error;
        auto hidl_cb = [&](ErrorCode ret, const hidl_vec<uint8_t>& hidlKeyBlob,
                           const KeyCharacteristics& keyCharacteristics) {
            keymasterDevice_->logIfKeymasterVendorError(ret);
            error = ret;
            if (!error.isOk()) {
                return;
            }
            consider_fallback = false;
            outCharacteristics = keyCharacteristics;

            Blob keyBlob(&hidlKeyBlob[0], hidlKeyBlob.size(), nullptr, 0, ::TYPE_KEYMASTER_10);
            keyBlob.setSecurityLevel(securityLevel);
            keyBlob.setCriticalToDeviceEncryption(flags &
                                                  KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION);
            if (isAuthenticationBound(keyParams) && !keyBlob.isCriticalToDeviceEncryption()) {
                keyBlob.setSuperEncrypted(true);
            }
            keyBlob.setEncrypted(flags & KEYSTORE_FLAG_ENCRYPTED);

            AuthorizationSet sw_enforced = keyParams;
            sw_enforced.Subtract(outCharacteristics.hardwareEnforced);
            sw_enforced.Union(outCharacteristics.softwareEnforced);
            sw_enforced.Filter([](const KeyParameter& param) -> bool {
                return !(param.tag == Tag::APPLICATION_DATA || param.tag == Tag::APPLICATION_ID);
            });
            if (!sw_enforced.Contains(Tag::USER_ID)) {
                // Most Java processes don't have access to this tag
                sw_enforced.push_back(keymaster::TAG_USER_ID, get_user_id(lockedEntry->uid()));
            }
            Blob keyCharBlob;
            keyCharBlob.putKeyCharacteristics(outCharacteristics.hardwareEnforced, sw_enforced);
            error = keyStore_->put(lockedEntry, std::move(keyBlob), std::move(keyCharBlob));
        };

        rc = KS_HANDLE_HIDL_ERROR(keymasterDevice_,
                                  keymasterDevice_->generateKey(keyParams, hidl_cb));
        if (!rc.isOk()) {
            return worker_cb(rc, {});
        }

        if (consider_fallback && !error.isOk()) {
            auto fallback = keyStore_->getFallbackDevice();
            if (!fallback) {
                return worker_cb(error, {});
            }
            // No fallback for 3DES
            for (auto& param : keyParams) {
                auto algorithm = authorizationValue(TAG_ALGORITHM, param);
                if (algorithm.isOk() && algorithm.value() == Algorithm::TRIPLE_DES) {
                    return worker_cb(ErrorCode::UNSUPPORTED_ALGORITHM, {});
                }
            }

            // delegate to fallback worker
            fallback->generateKey(std::move(lockedEntry), std::move(keyParams), std::move(entropy),
                                  flags, std::move(worker_cb));
            // let fallback do the logging
            logOnFail.release();
            return;
        }

        if (!error.isOk()) return worker_cb(error, {});

        // log on success
        logOnFail.release();
        uploadKeyCharacteristicsAsProto(keyParams, true /* wasCreationSuccessful */);

        return worker_cb(error, std::move(outCharacteristics));
    });
}

void KeymasterWorker::generateKey(hidl_vec<KeyParameter> keyParams, generateKey2_cb worker_cb) {
    addRequest(&Keymaster::generateKey, std::move(worker_cb), std::move(keyParams));
}

void KeymasterWorker::getKeyCharacteristics(LockedKeyBlobEntry lockedEntry,
                                            hidl_vec<uint8_t> clientId, hidl_vec<uint8_t> appData,
                                            Blob keyBlob, Blob charBlob,
                                            getKeyCharacteristics_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(lockedEntry), CAPTURE_MOVE(clientId),
                        CAPTURE_MOVE(appData), CAPTURE_MOVE(keyBlob), CAPTURE_MOVE(charBlob),
                        CAPTURE_MOVE(worker_cb)]() {
        auto result = createKeyCharacteristicsCache(lockedEntry, clientId, appData,
                                                    std::move(keyBlob), std::move(charBlob));
        return worker_cb(std::get<0>(result), std::move(std::get<1>(result)));
    });
}

void KeymasterWorker::importKey(LockedKeyBlobEntry lockedEntry, hidl_vec<KeyParameter> keyParams,
                                KeyFormat keyFormat, hidl_vec<uint8_t> keyData, int flags,
                                importKey_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(lockedEntry), CAPTURE_MOVE(keyParams), keyFormat,
                        CAPTURE_MOVE(keyData), flags, CAPTURE_MOVE(worker_cb)]() mutable {
        SecurityLevel securityLevel = keymasterDevice_->halVersion().securityLevel;

        // Fallback cannot be considered for Strongbox. Further versions restrictions are enforced
        // by KeyStore::getFallbackDevice()
        bool consider_fallback = securityLevel == SecurityLevel::TRUSTED_ENVIRONMENT;

        Finalize logOnFail(
            [&] { uploadKeyCharacteristicsAsProto(keyParams, false /* wasCreationSuccessful */); });

        KeyCharacteristics outCharacteristics;
        KeyStoreServiceReturnCode error;
        auto hidl_cb = [&](ErrorCode ret, const hidl_vec<uint8_t>& hidlKeyBlob,
                           const KeyCharacteristics& keyCharacteristics) {
            keymasterDevice_->logIfKeymasterVendorError(ret);
            error = ret;
            if (!error.isOk()) {
                LOG(INFO) << "importKey failed";
                return;
            }
            consider_fallback = false;
            outCharacteristics = keyCharacteristics;

            Blob keyBlob(&hidlKeyBlob[0], hidlKeyBlob.size(), nullptr, 0, ::TYPE_KEYMASTER_10);
            keyBlob.setSecurityLevel(securityLevel);
            keyBlob.setCriticalToDeviceEncryption(flags &
                                                  KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION);
            if (isAuthenticationBound(keyParams) && !keyBlob.isCriticalToDeviceEncryption()) {
                keyBlob.setSuperEncrypted(true);
            }
            keyBlob.setEncrypted(flags & KEYSTORE_FLAG_ENCRYPTED);

            AuthorizationSet sw_enforced = keyParams;
            sw_enforced.Subtract(outCharacteristics.hardwareEnforced);
            sw_enforced.Union(outCharacteristics.softwareEnforced);
            sw_enforced.Filter([](const KeyParameter& param) -> bool {
                return !(param.tag == Tag::APPLICATION_DATA || param.tag == Tag::APPLICATION_ID);
            });
            if (!sw_enforced.Contains(Tag::USER_ID)) {
                // Most Java processes don't have access to this tag
                sw_enforced.push_back(keymaster::TAG_USER_ID, get_user_id(lockedEntry->uid()));
            }
            Blob keyCharBlob;
            keyCharBlob.putKeyCharacteristics(outCharacteristics.hardwareEnforced, sw_enforced);
            error = keyStore_->put(lockedEntry, std::move(keyBlob), std::move(keyCharBlob));
        };

        KeyStoreServiceReturnCode rc = KS_HANDLE_HIDL_ERROR(
            keymasterDevice_, keymasterDevice_->importKey(keyParams, keyFormat, keyData, hidl_cb));
        if (!rc.isOk()) {
            return worker_cb(rc, {});
        }

        if (consider_fallback && !error.isOk()) {
            auto fallback = keyStore_->getFallbackDevice();
            if (!fallback) {
                return worker_cb(error, {});
            }
            // No fallback for 3DES
            for (auto& param : keyParams) {
                auto algorithm = authorizationValue(TAG_ALGORITHM, param);
                if (algorithm.isOk() && algorithm.value() == Algorithm::TRIPLE_DES) {
                    return worker_cb(ErrorCode::UNSUPPORTED_ALGORITHM, {});
                }
            }

            // delegate to fallback worker
            fallback->importKey(std::move(lockedEntry), std::move(keyParams), keyFormat,
                                std::move(keyData), flags, std::move(worker_cb));
            // let fallback to the logging
            logOnFail.release();
            return;
        }

        if (!error.isOk()) return worker_cb(error, {});

        // log on success
        logOnFail.release();
        uploadKeyCharacteristicsAsProto(keyParams, true /* wasCreationSuccessful */);

        return worker_cb(error, std::move(outCharacteristics));
    });
}

void KeymasterWorker::importWrappedKey(LockedKeyBlobEntry wrappingLockedEntry,
                                       LockedKeyBlobEntry wrapppedLockedEntry,
                                       hidl_vec<uint8_t> wrappedKeyData,
                                       hidl_vec<uint8_t> maskingKey,
                                       hidl_vec<KeyParameter> unwrappingParams, Blob wrappingBlob,
                                       Blob wrappingCharBlob, uint64_t passwordSid,
                                       uint64_t biometricSid, importWrappedKey_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(wrappingLockedEntry), CAPTURE_MOVE(wrapppedLockedEntry),
                        CAPTURE_MOVE(wrappedKeyData), CAPTURE_MOVE(maskingKey),
                        CAPTURE_MOVE(unwrappingParams), CAPTURE_MOVE(wrappingBlob),
                        CAPTURE_MOVE(wrappingCharBlob), passwordSid, biometricSid,
                        CAPTURE_MOVE(worker_cb)]() mutable {
        auto hidlWrappingKey = blob2hidlVec(wrappingBlob);

        SecurityLevel securityLevel = keymasterDevice_->halVersion().securityLevel;

        KeyCharacteristics outCharacteristics;
        KeyStoreServiceReturnCode error;

        auto hidlCb = [&](ErrorCode ret, const hidl_vec<uint8_t>& hidlKeyBlob,
                          const KeyCharacteristics& keyCharacteristics) {
            keymasterDevice_->logIfKeymasterVendorError(ret);
            error = ret;
            if (!error.isOk()) {
                return;
            }
            outCharacteristics = keyCharacteristics;

            Blob keyBlob(hidlKeyBlob.data(), hidlKeyBlob.size(), nullptr, 0, ::TYPE_KEYMASTER_10);
            keyBlob.setSecurityLevel(securityLevel);
            if (isAuthenticationBound(keyCharacteristics.hardwareEnforced)) {
                keyBlob.setSuperEncrypted(true);
            }

            AuthorizationSet sw_enforced = outCharacteristics.softwareEnforced;
            if (!sw_enforced.Contains(Tag::USER_ID)) {
                // Most Java processes don't have access to this tag
                sw_enforced.push_back(keymaster::TAG_USER_ID,
                                      get_user_id(wrapppedLockedEntry->uid()));
            }
            Blob keyCharBlob;
            keyCharBlob.putKeyCharacteristics(outCharacteristics.hardwareEnforced, sw_enforced);
            error = keyStore_->put(wrapppedLockedEntry, std::move(keyBlob), std::move(keyCharBlob));
        };

        KeyStoreServiceReturnCode rc = KS_HANDLE_HIDL_ERROR(
            keymasterDevice_, keymasterDevice_->importWrappedKey(
                                  wrappedKeyData, hidlWrappingKey, maskingKey, unwrappingParams,
                                  passwordSid, biometricSid, hidlCb));

        // possible hidl error
        if (!rc.isOk()) {
            return worker_cb(rc, {});
        }

        if (error == ErrorCode::KEY_REQUIRES_UPGRADE) {
            std::tie(rc, wrappingBlob) = upgradeKeyBlob(wrappingLockedEntry, {});
            if (!rc.isOk()) {
                return worker_cb(rc, {});
            }

            auto upgradedHidlKeyBlob = blob2hidlVec(wrappingBlob);

            rc = KS_HANDLE_HIDL_ERROR(keymasterDevice_,
                                      keymasterDevice_->importWrappedKey(
                                          wrappedKeyData, upgradedHidlKeyBlob, maskingKey,
                                          unwrappingParams, passwordSid, biometricSid, hidlCb));
            if (!rc.isOk()) {
                error = rc;
            }
        }
        return worker_cb(error, std::move(outCharacteristics));
    });
}

void KeymasterWorker::exportKey(LockedKeyBlobEntry lockedEntry, KeyFormat exportFormat,
                                hidl_vec<uint8_t> clientId, hidl_vec<uint8_t> appData, Blob keyBlob,
                                Blob charBlob, exportKey_cb worker_cb) {
    Worker::addRequest([this, CAPTURE_MOVE(lockedEntry), exportFormat, CAPTURE_MOVE(clientId),
                        CAPTURE_MOVE(appData), CAPTURE_MOVE(keyBlob), CAPTURE_MOVE(charBlob),
                        CAPTURE_MOVE(worker_cb)]() mutable {
        auto key = blob2hidlVec(keyBlob);

        ExportResult result;
        auto hidlCb = [&](ErrorCode ret,
                          const ::android::hardware::hidl_vec<uint8_t>& keyMaterial) {
            keymasterDevice_->logIfKeymasterVendorError(ret);
            result.resultCode = ret;
            if (!result.resultCode.isOk()) {
                if (result.resultCode == ErrorCode::INVALID_KEY_BLOB) {
                    log_key_integrity_violation(lockedEntry->alias().c_str(), lockedEntry->uid());
                }
                return;
            }
            result.exportData = keyMaterial;
        };
        KeyStoreServiceReturnCode rc = KS_HANDLE_HIDL_ERROR(
            keymasterDevice_,
            keymasterDevice_->exportKey(exportFormat, key, clientId, appData, hidlCb));

        // Overwrite result->resultCode only on HIDL error. Otherwise we want the result set in the
        // callback hidlCb.
        if (!rc.isOk()) {
            result.resultCode = rc;
        }

        if (result.resultCode == ErrorCode::KEY_REQUIRES_UPGRADE) {
            AuthorizationSet upgradeParams;
            if (clientId.size()) {
                upgradeParams.push_back(TAG_APPLICATION_ID, clientId);
            }
            if (appData.size()) {
                upgradeParams.push_back(TAG_APPLICATION_DATA, appData);
            }
            std::tie(rc, keyBlob) = upgradeKeyBlob(lockedEntry, upgradeParams);
            if (!rc.isOk()) {
                return worker_cb(std::move(result));
            }

            auto upgradedHidlKeyBlob = blob2hidlVec(keyBlob);

            rc = KS_HANDLE_HIDL_ERROR(keymasterDevice_,
                                      keymasterDevice_->exportKey(exportFormat, upgradedHidlKeyBlob,
                                                                  clientId, appData, hidlCb));
            if (!rc.isOk()) {
                result.resultCode = rc;
            }
        }
        return worker_cb(std::move(result));
    });
}
void KeymasterWorker::attestKey(hidl_vec<uint8_t> keyToAttest, hidl_vec<KeyParameter> attestParams,
                                attestKey_cb worker_cb) {
    addRequest(&Keymaster::attestKey, std::move(worker_cb), std::move(keyToAttest),
               std::move(attestParams));
}

void KeymasterWorker::deleteKey(hidl_vec<uint8_t> keyBlob, deleteKey_cb _hidl_cb) {
    addRequest(&Keymaster::deleteKey, std::move(_hidl_cb), std::move(keyBlob));
}

void KeymasterWorker::binderDied(android::wp<IBinder> who) {
    Worker::addRequest([this, who]() {
        auto operations = operationMap_.getOperationsForToken(who.unsafe_get());
        for (const auto& token : operations) {
            abort(token);
            keyStore_->removeOperationDevice(token);
        }
    });
}

}  // namespace keystore
