/*
 * Copyright 2015 The Android Open Source Project
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
#include <gatekeeper/UniquePtr.h>
#include <gatekeeper/gatekeeper.h>

#include <endian.h>

#define DAY_IN_MS (1000 * 60 * 60 * 24)

namespace gatekeeper {

void GateKeeper::Enroll(const EnrollRequest &request, EnrollResponse *response) {
    if (response == NULL) return;

    if (!request.provided_password.buffer.get()) {
        response->error = ERROR_INVALID;
        return;
    }

    secure_id_t user_id = 0;// todo: rename to policy
    uint32_t uid = request.user_id;

    if (request.password_handle.buffer.get() == NULL) {
        // Password handle does not match what is stored, generate new SecureID
        GetRandom(&user_id, sizeof(secure_id_t));
    } else {
        if (request.password_handle.length < sizeof(password_handle_t)) {
            response->error = ERROR_INVALID;
            return;
        }
        password_handle_t *pw_handle =
            reinterpret_cast<password_handle_t *>(request.password_handle.buffer.get());

        if (pw_handle->version > HANDLE_VERSION) {
            response->error = ERROR_INVALID;
            return;
        }

        user_id = pw_handle->user_id;

        uint64_t timestamp = GetMillisecondsSinceBoot();

        uint32_t timeout = 0;
        bool throttle = (pw_handle->version >= HANDLE_VERSION_THROTTLE);
        if (throttle) {
            bool throttle_secure = pw_handle->flags & HANDLE_FLAG_THROTTLE_SECURE;
            failure_record_t record;
            if (!GetFailureRecord(uid, user_id, &record, throttle_secure)) {
                response->error = ERROR_UNKNOWN;
                return;
            }

            if (ThrottleRequest(uid, timestamp, &record, throttle_secure, response)) return;

            if (!IncrementFailureRecord(uid, user_id, timestamp, &record, throttle_secure)) {
                response->error = ERROR_UNKNOWN;
                return;
            }

            timeout = ComputeRetryTimeout(&record);
        }

        if (!DoVerify(pw_handle, request.enrolled_password)) {
            // incorrect old password
            if (throttle && timeout > 0) {
                response->SetRetryTimeout(timeout);
            } else {
                response->error = ERROR_INVALID;
            }
            return;
        }
    }

    uint64_t flags = 0;
    if (ClearFailureRecord(uid, user_id, true)) {
        flags |= HANDLE_FLAG_THROTTLE_SECURE;
    } else {
        ClearFailureRecord(uid, user_id, false);
    }

    salt_t salt;
    GetRandom(&salt, sizeof(salt));

    SizedBuffer password_handle;
    if (!CreatePasswordHandle(&password_handle,
            salt, user_id, flags, HANDLE_VERSION, request.provided_password.buffer.get(),
            request.provided_password.length)) {
        response->error = ERROR_INVALID;
        return;
    }

    response->SetEnrolledPasswordHandle(&password_handle);
}

void GateKeeper::Verify(const VerifyRequest &request, VerifyResponse *response) {
    if (response == NULL) return;

    if (!request.provided_password.buffer.get() || !request.password_handle.buffer.get()) {
        response->error = ERROR_INVALID;
        return;
    }

    if (request.password_handle.length < sizeof(password_handle_t)) {
        response->error = ERROR_INVALID;
        return;
    }

    password_handle_t *password_handle = reinterpret_cast<password_handle_t *>(
            request.password_handle.buffer.get());

    if (password_handle->version > HANDLE_VERSION) {
        response->error = ERROR_INVALID;
        return;
    }

    secure_id_t user_id = password_handle->user_id;
    secure_id_t authenticator_id = 0;
    uint32_t uid = request.user_id;

    uint64_t timestamp = GetMillisecondsSinceBoot();

    uint32_t timeout = 0;
    bool throttle = (password_handle->version >= HANDLE_VERSION_THROTTLE);
    bool throttle_secure = password_handle->flags & HANDLE_FLAG_THROTTLE_SECURE;
    if (throttle) {
        failure_record_t record;
        if (!GetFailureRecord(uid, user_id, &record, throttle_secure)) {
            response->error = ERROR_UNKNOWN;
            return;
        }

        if (ThrottleRequest(uid, timestamp, &record, throttle_secure, response)) return;

        if (!IncrementFailureRecord(uid, user_id, timestamp, &record, throttle_secure)) {
            response->error = ERROR_UNKNOWN;
            return;
        }

        timeout = ComputeRetryTimeout(&record);
    } else {
        response->request_reenroll = true;
    }

    if (DoVerify(password_handle, request.provided_password)) {
        // Signature matches
        UniquePtr<uint8_t> auth_token_buffer;
        uint32_t auth_token_len;
        MintAuthToken(&auth_token_buffer, &auth_token_len, timestamp,
                user_id, authenticator_id, request.challenge);

        SizedBuffer auth_token(auth_token_len);
        memcpy(auth_token.buffer.get(), auth_token_buffer.get(), auth_token_len);
        response->SetVerificationToken(&auth_token);
        if (throttle) ClearFailureRecord(uid, user_id, throttle_secure);
    } else {
        // compute the new timeout given the incremented record
        if (throttle && timeout > 0) {
            response->SetRetryTimeout(timeout);
        } else {
            response->error = ERROR_INVALID;
        }
    }
}

bool GateKeeper::CreatePasswordHandle(SizedBuffer *password_handle_buffer, salt_t salt,
        secure_id_t user_id, uint64_t flags, uint8_t handle_version, const uint8_t *password,
        uint32_t password_length) {
    password_handle_buffer->buffer.reset(new uint8_t[sizeof(password_handle_t)]);
    password_handle_buffer->length = sizeof(password_handle_t);

    password_handle_t *password_handle = reinterpret_cast<password_handle_t *>(
            password_handle_buffer->buffer.get());
    password_handle->version = handle_version;
    password_handle->salt = salt;
    password_handle->user_id = user_id;
    password_handle->flags = flags;
    password_handle->hardware_backed = IsHardwareBacked();

    uint32_t metadata_length = sizeof(user_id) + sizeof(flags) + sizeof(HANDLE_VERSION);
    const size_t to_sign_size = password_length + metadata_length;
    UniquePtr<uint8_t[]> to_sign(new uint8_t[to_sign_size]);

    if (to_sign.get() == nullptr) {
        return false;
    }

    memcpy(to_sign.get(), password_handle, metadata_length);
    memcpy(to_sign.get() + metadata_length, password, password_length);

    const uint8_t *password_key = NULL;
    uint32_t password_key_length = 0;
    GetPasswordKey(&password_key, &password_key_length);

    if (!password_key || password_key_length == 0) {
        return false;
    }

    ComputePasswordSignature(password_handle->signature, sizeof(password_handle->signature),
            password_key, password_key_length, to_sign.get(), to_sign_size, salt);
    return true;
}

bool GateKeeper::DoVerify(const password_handle_t *expected_handle, const SizedBuffer &password) {
    if (!password.buffer.get()) return false;

    SizedBuffer provided_handle;
    if (!CreatePasswordHandle(&provided_handle, expected_handle->salt, expected_handle->user_id,
            expected_handle->flags, expected_handle->version,
            password.buffer.get(), password.length)) {
        return false;
    }

    password_handle_t *generated_handle =
            reinterpret_cast<password_handle_t *>(provided_handle.buffer.get());
    return memcmp_s(generated_handle->signature, expected_handle->signature,
            sizeof(expected_handle->signature)) == 0;
}

void GateKeeper::MintAuthToken(UniquePtr<uint8_t> *auth_token, uint32_t *length,
        uint64_t timestamp, secure_id_t user_id, secure_id_t authenticator_id,
        uint64_t challenge) {
    if (auth_token == NULL) return;

    hw_auth_token_t *token = new hw_auth_token_t;
    SizedBuffer serialized_auth_token;

    token->version = HW_AUTH_TOKEN_VERSION;
    token->challenge = challenge;
    token->user_id = user_id;
    token->authenticator_id = authenticator_id;
    token->authenticator_type = htobe32(HW_AUTH_PASSWORD);
    token->timestamp = htobe64(timestamp);

    const uint8_t *auth_token_key = NULL;
    uint32_t key_len = 0;
    if (GetAuthTokenKey(&auth_token_key, &key_len)) {
        uint32_t hash_len = (uint32_t)((uint8_t *)&token->hmac - (uint8_t *)token);
        ComputeSignature(token->hmac, sizeof(token->hmac), auth_token_key, key_len,
                reinterpret_cast<uint8_t *>(token), hash_len);
    } else {
        memset(token->hmac, 0, sizeof(token->hmac));
    }

    if (length != NULL) *length = sizeof(*token);
    auth_token->reset(reinterpret_cast<uint8_t *>(token));
}

/*
 * Calculates the timeout in milliseconds as a function of the failure
 * counter 'x' as follows:
 *
 * [0, 4] -> 0
 * 5 -> 30
 * [6, 10] -> 0
 * [11, 29] -> 30
 * [30, 139] -> 30 * (2^((x - 30)/10))
 * [140, inf) -> 1 day
 *
 */
uint32_t GateKeeper::ComputeRetryTimeout(const failure_record_t *record) {
    static const int failure_timeout_ms = 30000;
    if (record->failure_counter == 0) return 0;

    if (record->failure_counter > 0 && record->failure_counter <= 10) {
        if (record->failure_counter % 5 == 0) {
            return failure_timeout_ms;
        }  else {
            return 0;
        }
    } else if (record->failure_counter < 30) {
        return failure_timeout_ms;
    } else if (record->failure_counter < 140) {
        return failure_timeout_ms << ((record->failure_counter - 30) / 10);
    }

    return DAY_IN_MS;
}

bool GateKeeper::ThrottleRequest(uint32_t uid, uint64_t timestamp,
        failure_record_t *record, bool secure, GateKeeperMessage *response) {

    uint64_t last_checked = record->last_checked_timestamp;
    uint32_t timeout = ComputeRetryTimeout(record);

    if (timeout > 0) {
        // we have a pending timeout
        if (timestamp < last_checked + timeout && timestamp > last_checked) {
            // attempt before timeout expired, return remaining time
            response->SetRetryTimeout(timeout - (timestamp - last_checked));
            return true;
        } else if (timestamp <= last_checked) {
            // device was rebooted or timer reset, don't count as new failure but
            // reset timeout
            record->last_checked_timestamp = timestamp;
            if (!WriteFailureRecord(uid, record, secure)) {
                response->error = ERROR_UNKNOWN;
                return true;
            }
            response->SetRetryTimeout(timeout);
            return true;
        }
    }

    return false;
}

bool GateKeeper::IncrementFailureRecord(uint32_t uid, secure_id_t user_id, uint64_t timestamp,
            failure_record_t *record, bool secure) {
    record->secure_user_id = user_id;
    record->failure_counter++;
    record->last_checked_timestamp = timestamp;

    return WriteFailureRecord(uid, record, secure);
}
} // namespace gatekeeper

