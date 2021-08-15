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
 *
 */

#include <gatekeeper/gatekeeper_messages.h>

#include <string.h>

namespace gatekeeper {

/**
 * Methods for serializing/deserializing SizedBuffers
 */

struct __attribute__((__packed__)) serial_header_t {
    uint32_t error;
    uint32_t user_id;
};

static inline uint32_t serialized_buffer_size(const SizedBuffer &buf) {
    return sizeof(buf.length) + buf.length;
}

static inline void append_to_buffer(uint8_t **buffer, const SizedBuffer *to_append) {
    memcpy(*buffer, &to_append->length, sizeof(to_append->length));
    *buffer += sizeof(to_append->length);
    if (to_append->length != 0) {
        memcpy(*buffer, to_append->buffer.get(), to_append->length);
        *buffer += to_append->length;
    }
}

static inline gatekeeper_error_t read_from_buffer(const uint8_t **buffer, const uint8_t *end,
        SizedBuffer *target) {
    if (*buffer + sizeof(target->length) > end) return ERROR_INVALID;

    memcpy(&target->length, *buffer, sizeof(target->length));
    *buffer += sizeof(target->length);
    if (target->length != 0) {
        const size_t buffer_size = end - *buffer;
        if (buffer_size < target->length) return ERROR_INVALID;

        target->buffer.reset(new uint8_t[target->length]);
        memcpy(target->buffer.get(), *buffer, target->length);
        *buffer += target->length;
    }
    return ERROR_NONE;
}


uint32_t GateKeeperMessage::GetSerializedSize() const {
    if (error == ERROR_NONE) {
        uint32_t size = sizeof(serial_header_t) + nonErrorSerializedSize();
        return size;
    } else {
        uint32_t size = sizeof(serial_header_t);
        if (error == ERROR_RETRY) {
            size += sizeof(retry_timeout);
        }
        return size;
    }
}

uint32_t GateKeeperMessage::Serialize(uint8_t *buffer, const uint8_t *end) const {
    uint32_t bytes_written = 0;
    if (buffer + GetSerializedSize() > end) {
        return 0;
    }

    serial_header_t *header = reinterpret_cast<serial_header_t *>(buffer);
    if (error != ERROR_NONE) {
        if (buffer + sizeof(serial_header_t) > end) return 0;
        header->error = error;
        header->user_id = user_id;
        bytes_written += sizeof(*header);
        if (error == ERROR_RETRY) {
            memcpy(buffer + sizeof(serial_header_t), &retry_timeout, sizeof(retry_timeout));
            bytes_written  += sizeof(retry_timeout);
        }
    } else {
        if (buffer + sizeof(serial_header_t) + nonErrorSerializedSize() > end)
            return 0;
        header->error = error;
        header->user_id = user_id;
        nonErrorSerialize(buffer + sizeof(*header));
        bytes_written += sizeof(*header) + nonErrorSerializedSize();
    }

    return bytes_written;
}

gatekeeper_error_t GateKeeperMessage::Deserialize(const uint8_t *payload, const uint8_t *end) {
    if (payload + sizeof(serial_header_t) > end) return ERROR_INVALID;
    const serial_header_t *header = reinterpret_cast<const serial_header_t *>(payload);
    payload += sizeof(serial_header_t);
    user_id = header->user_id;
    if (header->error == ERROR_NONE) {
        error = nonErrorDeserialize(payload, end);
    } else {
        error = static_cast<gatekeeper_error_t>(header->error);
        if (error == ERROR_RETRY) {
            if (payload < end) {
                if (payload + sizeof(retry_timeout) <= end) {
                    memcpy(&retry_timeout, payload, sizeof(retry_timeout));
                } else {
                    error = ERROR_INVALID;
                }
            } else {
                retry_timeout = 0;
            }
        }
    }

    return error;
}

void GateKeeperMessage::SetRetryTimeout(uint32_t retry_timeout) {
    this->retry_timeout = retry_timeout;
    this->error = ERROR_RETRY;
}

VerifyRequest::VerifyRequest(uint32_t user_id, uint64_t challenge,
        SizedBuffer *enrolled_password_handle, SizedBuffer *provided_password_payload) {
    this->user_id = user_id;
    this->challenge = challenge;
    this->password_handle.buffer.reset(enrolled_password_handle->buffer.release());
    this->password_handle.length = enrolled_password_handle->length;
    this->provided_password.buffer.reset(provided_password_payload->buffer.release());
    this->provided_password.length = provided_password_payload->length;
}

VerifyRequest::VerifyRequest() {
    memset_s(&password_handle, 0, sizeof(password_handle));
    memset_s(&provided_password, 0, sizeof(provided_password));
}

VerifyRequest::~VerifyRequest() {
    if (password_handle.buffer.get()) {
        password_handle.buffer.reset();
    }

    if (provided_password.buffer.get()) {
        memset_s(provided_password.buffer.get(), 0, provided_password.length);
        provided_password.buffer.reset();
    }
}

uint32_t VerifyRequest::nonErrorSerializedSize() const {
    return sizeof(challenge) + serialized_buffer_size(password_handle)
            + serialized_buffer_size(provided_password);
}

void VerifyRequest::nonErrorSerialize(uint8_t *buffer) const {
    memcpy(buffer, &challenge, sizeof(challenge));
    buffer += sizeof(challenge);
    append_to_buffer(&buffer, &password_handle);
    append_to_buffer(&buffer, &provided_password);
}

gatekeeper_error_t VerifyRequest::nonErrorDeserialize(const uint8_t *payload, const uint8_t *end) {
    gatekeeper_error_t error = ERROR_NONE;

    if (password_handle.buffer.get()) {
        password_handle.buffer.reset();
    }

    if (provided_password.buffer.get()) {
        memset_s(provided_password.buffer.get(), 0, provided_password.length);
        provided_password.buffer.reset();
    }

    if (payload + sizeof(challenge) > end) {
        return ERROR_INVALID;
    }
    memcpy(&challenge, payload, sizeof(challenge));
    payload += sizeof(challenge);

    error = read_from_buffer(&payload, end, &password_handle);
    if (error != ERROR_NONE) return error;

    return read_from_buffer(&payload, end, &provided_password);

}

VerifyResponse::VerifyResponse(uint32_t user_id, SizedBuffer *auth_token) {
    this->user_id = user_id;
    this->auth_token.buffer.reset(auth_token->buffer.release());
    this->auth_token.length = auth_token->length;
    this->request_reenroll = false;
}

VerifyResponse::VerifyResponse() {
    request_reenroll = false;
    memset_s(&auth_token, 0, sizeof(auth_token));
};

VerifyResponse::~VerifyResponse() {
    if (auth_token.length > 0) {
        auth_token.buffer.reset();
    }
}

void VerifyResponse::SetVerificationToken(SizedBuffer *auth_token) {
    this->auth_token.buffer.reset(auth_token->buffer.release());
    this->auth_token.length = auth_token->length;
}

uint32_t VerifyResponse::nonErrorSerializedSize() const {
    return serialized_buffer_size(auth_token) + sizeof(request_reenroll);
}

void VerifyResponse::nonErrorSerialize(uint8_t *buffer) const {
    append_to_buffer(&buffer, &auth_token);
    memcpy(buffer, &request_reenroll, sizeof(request_reenroll));
}

gatekeeper_error_t VerifyResponse::nonErrorDeserialize(const uint8_t *payload, const uint8_t *end) {
    if (auth_token.buffer.get()) {
        auth_token.buffer.reset();
    }

    gatekeeper_error_t err = read_from_buffer(&payload, end, &auth_token);
    if (err != ERROR_NONE) {
        return err;
    }

    if (payload + sizeof(request_reenroll) > end) {
        return ERROR_INVALID;
    }

    memcpy(&request_reenroll, payload, sizeof(request_reenroll));
    return ERROR_NONE;
}

EnrollRequest::EnrollRequest(uint32_t user_id, SizedBuffer *password_handle,
        SizedBuffer *provided_password,  SizedBuffer *enrolled_password) {
    this->user_id = user_id;
    this->provided_password.buffer.reset(provided_password->buffer.release());
    this->provided_password.length = provided_password->length;

    if (enrolled_password == NULL) {
        this->enrolled_password.buffer.reset();
        this->enrolled_password.length = 0;
    } else {
        this->enrolled_password.buffer.reset(enrolled_password->buffer.release());
        this->enrolled_password.length = enrolled_password->length;
    }

    if (password_handle == NULL) {
        this->password_handle.buffer.reset();
        this->password_handle.length = 0;
    } else {
        this->password_handle.buffer.reset(password_handle->buffer.release());
        this->password_handle.length = password_handle->length;
    }
}

EnrollRequest::EnrollRequest() {
    memset_s(&provided_password, 0, sizeof(provided_password));
    memset_s(&enrolled_password, 0, sizeof(enrolled_password));
    memset_s(&password_handle, 0, sizeof(password_handle));
}

EnrollRequest::~EnrollRequest() {
    if (provided_password.buffer.get()) {
        memset_s(provided_password.buffer.get(), 0, provided_password.length);
        provided_password.buffer.reset();
    }

    if (enrolled_password.buffer.get()) {
        memset_s(enrolled_password.buffer.get(), 0, enrolled_password.length);
        enrolled_password.buffer.reset();
    }

    if (password_handle.buffer.get()) {
        memset_s(password_handle.buffer.get(), 0, password_handle.length);
        password_handle.buffer.reset();
    }
}

uint32_t EnrollRequest::nonErrorSerializedSize() const {
   return serialized_buffer_size(provided_password) + serialized_buffer_size(enrolled_password)
       + serialized_buffer_size(password_handle);
}

void EnrollRequest::nonErrorSerialize(uint8_t *buffer) const {
    append_to_buffer(&buffer, &provided_password);
    append_to_buffer(&buffer, &enrolled_password);
    append_to_buffer(&buffer, &password_handle);
}

gatekeeper_error_t EnrollRequest::nonErrorDeserialize(const uint8_t *payload, const uint8_t *end) {
    gatekeeper_error_t ret;
    if (provided_password.buffer.get()) {
        memset_s(provided_password.buffer.get(), 0, provided_password.length);
        provided_password.buffer.reset();
    }

    if (enrolled_password.buffer.get()) {
        memset_s(enrolled_password.buffer.get(), 0, enrolled_password.length);
        enrolled_password.buffer.reset();
    }

    if (password_handle.buffer.get()) {
        memset_s(password_handle.buffer.get(), 0, password_handle.length);
        password_handle.buffer.reset();
    }

     ret = read_from_buffer(&payload, end, &provided_password);
     if (ret != ERROR_NONE) {
         return ret;
     }

     ret = read_from_buffer(&payload, end, &enrolled_password);
     if (ret != ERROR_NONE) {
         return ret;
     }

     return read_from_buffer(&payload, end, &password_handle);
}

EnrollResponse::EnrollResponse(uint32_t user_id, SizedBuffer *enrolled_password_handle) {
    this->user_id = user_id;
    this->enrolled_password_handle.buffer.reset(enrolled_password_handle->buffer.release());
    this->enrolled_password_handle.length = enrolled_password_handle->length;
}

EnrollResponse::EnrollResponse() {
    memset_s(&enrolled_password_handle, 0, sizeof(enrolled_password_handle));
}

EnrollResponse::~EnrollResponse() {
    if (enrolled_password_handle.buffer.get()) {
        enrolled_password_handle.buffer.reset();
    }
}

void EnrollResponse::SetEnrolledPasswordHandle(SizedBuffer *enrolled_password_handle) {
    this->enrolled_password_handle.buffer.reset(enrolled_password_handle->buffer.release());
    this->enrolled_password_handle.length = enrolled_password_handle->length;
}

uint32_t EnrollResponse::nonErrorSerializedSize() const {
    return serialized_buffer_size(enrolled_password_handle);
}

void EnrollResponse::nonErrorSerialize(uint8_t *buffer) const {
    append_to_buffer(&buffer, &enrolled_password_handle);
}

gatekeeper_error_t EnrollResponse::nonErrorDeserialize(const uint8_t *payload, const uint8_t *end) {
    if (enrolled_password_handle.buffer.get()) {
        enrolled_password_handle.buffer.reset();
    }

    return read_from_buffer(&payload, end, &enrolled_password_handle);
}

};

