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

#ifndef KEYSTORE_KEYMASTER_WORKER_H_
#define KEYSTORE_KEYMASTER_WORKER_H_

#include <condition_variable>
#include <functional>
#include <keymasterV4_0/Keymaster.h>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <tuple>

#include <keystore/ExportResult.h>
#include <keystore/KeyCharacteristics.h>
#include <keystore/KeymasterBlob.h>
#include <keystore/OperationResult.h>
#include <keystore/keystore_return_types.h>

#include "blob.h"
#include "operation.h"

namespace keystore {

using android::sp;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using android::hardware::keymaster::V4_0::ErrorCode;
using android::hardware::keymaster::V4_0::HardwareAuthToken;
using android::hardware::keymaster::V4_0::HmacSharingParameters;
using android::hardware::keymaster::V4_0::KeyCharacteristics;
using android::hardware::keymaster::V4_0::KeyFormat;
using android::hardware::keymaster::V4_0::KeyParameter;
using android::hardware::keymaster::V4_0::KeyPurpose;
using android::hardware::keymaster::V4_0::VerificationToken;
using android::hardware::keymaster::V4_0::support::Keymaster;
// using KeystoreCharacteristics = ::android::security::keymaster::KeyCharacteristics;
using ::android::security::keymaster::KeymasterBlob;

class KeyStore;

class Worker {

    /*
     * NonCopyableFunction works similar to std::function in that it wraps callable objects and
     * erases their type. The rationale for using a custom class instead of
     * std::function is that std::function requires the wrapped object to be copy contructible.
     * NonCopyableFunction is itself not copyable and never attempts to copy the wrapped object.
     * TODO use similar optimization as std::function to remove the extra make_unique allocation.
     */
    template <typename Fn> class NonCopyableFunction;

    template <typename Ret, typename... Args> class NonCopyableFunction<Ret(Args...)> {

        class NonCopyableFunctionBase {
          public:
            NonCopyableFunctionBase() = default;
            virtual ~NonCopyableFunctionBase() {}
            virtual Ret operator()(Args... args) = 0;
            NonCopyableFunctionBase(const NonCopyableFunctionBase&) = delete;
            NonCopyableFunctionBase& operator=(const NonCopyableFunctionBase&) = delete;
        };

        template <typename Fn>
        class NonCopyableFunctionTypeEraser : public NonCopyableFunctionBase {
          private:
            Fn f_;

          public:
            NonCopyableFunctionTypeEraser() = default;
            explicit NonCopyableFunctionTypeEraser(Fn f) : f_(std::move(f)) {}
            Ret operator()(Args... args) override { return f_(std::move(args)...); }
        };

      private:
        std::unique_ptr<NonCopyableFunctionBase> f_;

      public:
        NonCopyableFunction() = default;
        // NOLINTNEXTLINE(google-explicit-constructor)
        template <typename F> NonCopyableFunction(F f) {
            f_ = std::make_unique<NonCopyableFunctionTypeEraser<F>>(std::move(f));
        }
        NonCopyableFunction(NonCopyableFunction&& other) = default;
        NonCopyableFunction& operator=(NonCopyableFunction&& other) = default;
        NonCopyableFunction(const NonCopyableFunction& other) = delete;
        NonCopyableFunction& operator=(const NonCopyableFunction& other) = delete;

        Ret operator()(Args... args) {
            if (f_) return (*f_)(std::move(args)...);
        }
    };

    using WorkerTask = NonCopyableFunction<void()>;

    std::queue<WorkerTask> pending_requests_;
    std::mutex pending_requests_mutex_;
    std::condition_variable pending_requests_cond_var_;
    bool running_ = false;

  public:
    Worker();
    ~Worker();
    void addRequest(WorkerTask request);
};

template <typename... Args> struct MakeKeymasterWorkerCB;

template <typename ErrorType, typename... Args>
struct MakeKeymasterWorkerCB<ErrorType, std::function<void(Args...)>> {
    using type = std::function<void(ErrorType, std::tuple<std::decay_t<Args>...>&&)>;
};

template <typename ErrorType> struct MakeKeymasterWorkerCB<ErrorType> {
    using type = std::function<void(ErrorType)>;
};

template <typename... Args>
using MakeKeymasterWorkerCB_t = typename MakeKeymasterWorkerCB<Args...>::type;

class KeymasterWorker : protected Worker {
  private:
    sp<Keymaster> keymasterDevice_;
    OperationMap operationMap_;
    KeyStore* keyStore_;

    template <typename KMFn, typename ErrorType, typename... Args, size_t... I>
    void unwrap_tuple(KMFn kmfn, std::function<void(ErrorType)> cb,
                      const std::tuple<Args...>& tuple, std::index_sequence<I...>) {
        cb(((*keymasterDevice_).*kmfn)(std::get<I>(tuple)...));
    }

    template <typename KMFn, typename ErrorType, typename... ReturnTypes, typename... Args,
              size_t... I>
    void unwrap_tuple(KMFn kmfn, std::function<void(ErrorType, std::tuple<ReturnTypes...>&&)> cb,
                      const std::tuple<Args...>& tuple, std::index_sequence<I...>) {
        std::tuple<ReturnTypes...> returnValue;
        auto result = ((*keymasterDevice_).*kmfn)(
            std::get<I>(tuple)...,
            [&returnValue](const ReturnTypes&... args) { returnValue = std::make_tuple(args...); });
        cb(std::move(result), std::move(returnValue));
    }

    template <typename KMFn, typename ErrorType, typename... Args>
    void addRequest(KMFn kmfn, std::function<void(ErrorType)> cb, Args&&... args) {
        Worker::addRequest([this, kmfn, cb = std::move(cb),
                            tuple = std::make_tuple(std::forward<Args>(args)...)]() {
            unwrap_tuple(kmfn, std::move(cb), tuple, std::index_sequence_for<Args...>{});
        });
    }

    template <typename KMFn, typename ErrorType, typename... ReturnTypes, typename... Args>
    void addRequest(KMFn kmfn, std::function<void(ErrorType, std::tuple<ReturnTypes...>&&)> cb,
                    Args&&... args) {
        Worker::addRequest([this, kmfn, cb = std::move(cb),
                            tuple = std::make_tuple(std::forward<Args>(args)...)]() {
            unwrap_tuple(kmfn, std::move(cb), tuple, std::index_sequence_for<Args...>{});
        });
    }
    std::tuple<KeyStoreServiceReturnCode, Blob>
    upgradeKeyBlob(const LockedKeyBlobEntry& lockedEntry, const AuthorizationSet& params);
    std::tuple<KeyStoreServiceReturnCode, KeyCharacteristics, Blob, Blob>
    createKeyCharacteristicsCache(const LockedKeyBlobEntry& lockedEntry,
                                  const hidl_vec<uint8_t>& clientId,
                                  const hidl_vec<uint8_t>& appData, Blob keyBlob, Blob charBlob);

    /**
     * Get the auth token for this operation from the auth token table.
     *
     * Returns NO_ERROR if the auth token was found or none was required.  If not needed, the
     *             token will be empty (which keymaster interprets as no auth token).
     *         OP_AUTH_NEEDED if it is a per op authorization, no authorization token exists for
     *             that operation and  failOnTokenMissing is false.
     *         KM_ERROR_KEY_USER_NOT_AUTHENTICATED if there is no valid auth token for the operation
     */
    std::pair<KeyStoreServiceReturnCode, HardwareAuthToken>
    getAuthToken(const KeyCharacteristics& characteristics, uint64_t handle, KeyPurpose purpose,
                 bool failOnTokenMissing = true);

    KeyStoreServiceReturnCode abort(const sp<IBinder>& token);

    bool pruneOperation();

    KeyStoreServiceReturnCode getOperationAuthTokenIfNeeded(std::shared_ptr<Operation> op);

    void appendConfirmationTokenIfNeeded(const KeyCharacteristics& keyCharacteristics,
                                         hidl_vec<KeyParameter>* params);

  public:
    KeymasterWorker(sp<Keymaster> keymasterDevice, KeyStore* keyStore);

    void logIfKeymasterVendorError(ErrorCode ec) const;

    using worker_begin_cb = std::function<void(::android::security::keymaster::OperationResult)>;
    void begin(LockedKeyBlobEntry, sp<IBinder> appToken, Blob keyBlob, Blob charBlob,
               bool pruneable, KeyPurpose purpose, AuthorizationSet opParams,
               hidl_vec<uint8_t> entropy, worker_begin_cb worker_cb);

    using update_cb = std::function<void(::android::security::keymaster::OperationResult)>;
    void update(sp<IBinder> token, AuthorizationSet params, hidl_vec<uint8_t> data,
                update_cb _hidl_cb);

    using finish_cb = std::function<void(::android::security::keymaster::OperationResult)>;
    void finish(sp<IBinder> token, AuthorizationSet params, hidl_vec<uint8_t> input,
                hidl_vec<uint8_t> signature, hidl_vec<uint8_t> entorpy, finish_cb worker_cb);

    using abort_cb = std::function<void(KeyStoreServiceReturnCode)>;
    void abort(sp<IBinder> token, abort_cb _hidl_cb);

    using getHardwareInfo_cb = MakeKeymasterWorkerCB_t<Return<void>, Keymaster::getHardwareInfo_cb>;
    void getHardwareInfo(getHardwareInfo_cb _hidl_cb);

    using getHmacSharingParameters_cb =
        MakeKeymasterWorkerCB_t<Return<void>, Keymaster::getHmacSharingParameters_cb>;
    void getHmacSharingParameters(getHmacSharingParameters_cb _hidl_cb);

    using computeSharedHmac_cb =
        MakeKeymasterWorkerCB_t<Return<void>, Keymaster::computeSharedHmac_cb>;
    void computeSharedHmac(hidl_vec<HmacSharingParameters> params, computeSharedHmac_cb _hidl_cb);

    using verifyAuthorization_cb =
        std::function<void(KeyStoreServiceReturnCode ec, HardwareAuthToken, VerificationToken)>;
    void verifyAuthorization(uint64_t challenge, hidl_vec<KeyParameter> params,
                             HardwareAuthToken token, verifyAuthorization_cb _hidl_cb);

    using addRngEntropy_cb = MakeKeymasterWorkerCB_t<Return<ErrorCode>>;
    void addRngEntropy(hidl_vec<uint8_t> data, addRngEntropy_cb _hidl_cb);

    using generateKey_cb = std::function<void(
        KeyStoreServiceReturnCode, ::android::hardware::keymaster::V4_0::KeyCharacteristics)>;
    void generateKey(LockedKeyBlobEntry, hidl_vec<KeyParameter> keyParams,
                     hidl_vec<uint8_t> entropy, int flags, generateKey_cb _hidl_cb);

    using generateKey2_cb = MakeKeymasterWorkerCB_t<Return<void>, Keymaster::generateKey_cb>;
    void generateKey(hidl_vec<KeyParameter> keyParams, generateKey2_cb _hidl_cb);

    using getKeyCharacteristics_cb = std::function<void(
        KeyStoreServiceReturnCode, ::android::hardware::keymaster::V4_0::KeyCharacteristics)>;
    void getKeyCharacteristics(LockedKeyBlobEntry lockedEntry, hidl_vec<uint8_t> clientId,
                               hidl_vec<uint8_t> appData, Blob keyBlob, Blob charBlob,
                               getKeyCharacteristics_cb _hidl_cb);

    using importKey_cb = std::function<void(
        KeyStoreServiceReturnCode, ::android::hardware::keymaster::V4_0::KeyCharacteristics)>;
    void importKey(LockedKeyBlobEntry lockedEntry, hidl_vec<KeyParameter> params,
                   KeyFormat keyFormat, hidl_vec<uint8_t> keyData, int flags,
                   importKey_cb _hidl_cb);

    using importWrappedKey_cb = std::function<void(
        KeyStoreServiceReturnCode, ::android::hardware::keymaster::V4_0::KeyCharacteristics)>;
    void importWrappedKey(LockedKeyBlobEntry wrappingLockedEntry,
                          LockedKeyBlobEntry wrapppedLockedEntry, hidl_vec<uint8_t> wrappedKeyData,
                          hidl_vec<uint8_t> maskingKey, hidl_vec<KeyParameter> unwrappingParams,
                          Blob wrappingBlob, Blob wrappingCharBlob, uint64_t passwordSid,
                          uint64_t biometricSid, importWrappedKey_cb worker_cb);

    using exportKey_cb = std::function<void(::android::security::keymaster::ExportResult)>;
    void exportKey(LockedKeyBlobEntry lockedEntry, KeyFormat exportFormat,
                   hidl_vec<uint8_t> clientId, hidl_vec<uint8_t> appData, Blob keyBlob,
                   Blob charBlob, exportKey_cb _hidl_cb);

    using attestKey_cb = MakeKeymasterWorkerCB_t<Return<void>, Keymaster::attestKey_cb>;
    void attestKey(hidl_vec<uint8_t> keyToAttest, hidl_vec<KeyParameter> attestParams,
                   attestKey_cb _hidl_cb);

    using deleteKey_cb = MakeKeymasterWorkerCB_t<Return<ErrorCode>>;
    void deleteKey(hidl_vec<uint8_t> keyBlob, deleteKey_cb _hidl_cb);

    using begin_cb = MakeKeymasterWorkerCB_t<Return<void>, Keymaster::begin_cb>;
    void begin(KeyPurpose purpose, hidl_vec<uint8_t> key, hidl_vec<KeyParameter> inParams,
               HardwareAuthToken authToken, begin_cb _hidl_cb);

    void binderDied(android::wp<IBinder> who);

    const Keymaster::VersionResult& halVersion() { return keymasterDevice_->halVersion(); }
};

}  // namespace keystore

#endif  // KEYSTORE_KEYMASTER_WORKER_H_
