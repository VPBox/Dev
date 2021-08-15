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
#ifndef GATEKEEPER_MESSAGES_H_
#define GATEKEEPER_MESSAGES_H_

#include <stdint.h>
#include <gatekeeper/UniquePtr.h>


#include "gatekeeper_utils.h"
/**
 * Message serialization objects for communicating with the hardware gatekeeper.
 */
namespace gatekeeper {

const uint32_t ENROLL = 0;
const uint32_t VERIFY = 1;

typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID = 1,
    ERROR_RETRY = 2,
    ERROR_UNKNOWN = 3,
} gatekeeper_error_t;

struct SizedBuffer {
    SizedBuffer() {
        length = 0;
    }

    /*
     * Constructs a SizedBuffer of a provided
     * length.
     */
    explicit SizedBuffer(uint32_t length) {
        if (length != 0) {
            buffer.reset(new uint8_t[length]);
        } else {
            buffer.reset();
        }
        this->length = length;
    }

    /*
     * Constructs a SizedBuffer out of a pointer and a length
     * Takes ownership of the buf pointer, and deallocates it
     * when destructed.
     */
    SizedBuffer(uint8_t buf[], uint32_t len) {
        buffer.reset(buf);
        length = len;
    }

    UniquePtr<uint8_t[]> buffer;
    uint32_t length;
};

/*
 * Abstract base class of all message objects. Handles serialization of common
 * elements like the error and user ID. Delegates specialized serialization
 * to protected pure virtual functions implemented by subclasses.
 */
struct GateKeeperMessage {
    GateKeeperMessage() : error(ERROR_NONE) {}
    explicit GateKeeperMessage(gatekeeper_error_t error) : error(error) {}
    virtual ~GateKeeperMessage() {}

    /**
     * Returns serialized size in bytes of the current state of the
     * object.
     */
    uint32_t GetSerializedSize() const;
    /**
     * Converts the object into its serialized representation.
     *
     * Expects payload to be allocated with GetSerializedSize bytes.
     *
     * Returns the number of bytes written or 0 on error.
     */
    uint32_t Serialize(uint8_t *payload, const uint8_t *end) const;

    /**
     * Inflates the object from its serial representation.
     */
    gatekeeper_error_t Deserialize(const uint8_t *payload, const uint8_t *end);

    /**
     * Calls may fail due to throttling. If so, this sets a timeout in milliseconds
     * for when the caller should attempt the call again. Additionally, sets the
     * error to ERROR_RETRY.
     */
    void SetRetryTimeout(uint32_t retry_timeout);

    /**
     * The following methods are intended to be implemented by subclasses.
     * They are hooks to serialize the elements specific to each particular
     * specialization.
     */

    /**
     * Returns the size of serializing only the elements specific to the
     * current sublclass.
     */
    virtual uint32_t nonErrorSerializedSize() const { return 0; } ;
    /**
     * Takes a pointer to a buffer prepared by Serialize and writes
     * the subclass specific data into it. The size of the buffer must be exactly
     * that returned by nonErrorSerializedSize() in bytes.
     */
    virtual void nonErrorSerialize(uint8_t *) const { }

    /**
     * Deserializes subclass specific data from payload without reading past end.
     */
    virtual gatekeeper_error_t nonErrorDeserialize(const uint8_t *, const uint8_t *) {
        return ERROR_NONE;
    }

    gatekeeper_error_t error;
    uint32_t user_id;
    uint32_t retry_timeout;
};

struct VerifyRequest : public GateKeeperMessage {
    VerifyRequest(
            uint32_t user_id,
            uint64_t challenge,
            SizedBuffer *enrolled_password_handle,
            SizedBuffer *provided_password_payload);
    VerifyRequest();
    ~VerifyRequest();

    virtual uint32_t nonErrorSerializedSize() const;
    virtual void nonErrorSerialize(uint8_t *buffer) const;
    virtual gatekeeper_error_t nonErrorDeserialize(const uint8_t *payload, const uint8_t *end);

    uint64_t challenge;
    SizedBuffer password_handle;
    SizedBuffer provided_password;
};

struct VerifyResponse : public GateKeeperMessage {
    VerifyResponse(uint32_t user_id, SizedBuffer *auth_token);
    VerifyResponse();
    ~VerifyResponse();

    void SetVerificationToken(SizedBuffer *auth_token);

    virtual uint32_t nonErrorSerializedSize() const;
    virtual void nonErrorSerialize(uint8_t *buffer) const;
    virtual gatekeeper_error_t nonErrorDeserialize(const uint8_t *payload, const uint8_t *end);

    SizedBuffer auth_token;
    bool request_reenroll;
};

struct EnrollRequest : public GateKeeperMessage {
    EnrollRequest(uint32_t user_id, SizedBuffer *password_handle,
            SizedBuffer *provided_password, SizedBuffer *enrolled_password);
    EnrollRequest();
    ~EnrollRequest();

    virtual uint32_t nonErrorSerializedSize() const;
    virtual void nonErrorSerialize(uint8_t *buffer) const;
    virtual gatekeeper_error_t nonErrorDeserialize(const uint8_t *payload, const uint8_t *end);

    /**
     * The password handle returned from the previous call to enroll or NULL
     * if none
     */
    SizedBuffer password_handle;
    /**
     * The currently enrolled password as entered by the user
     */
    SizedBuffer enrolled_password;
    /**
     * The password desired by the user
     */
    SizedBuffer provided_password;
};

struct EnrollResponse : public GateKeeperMessage {
public:
    EnrollResponse(uint32_t user_id, SizedBuffer *enrolled_password_handle);
    EnrollResponse();
    ~EnrollResponse();

    void SetEnrolledPasswordHandle(SizedBuffer *enrolled_password_handle);

    virtual uint32_t nonErrorSerializedSize() const;
    virtual void nonErrorSerialize(uint8_t *buffer) const;
    virtual gatekeeper_error_t nonErrorDeserialize(const uint8_t *payload, const uint8_t *end);

   SizedBuffer enrolled_password_handle;
};
}

#endif // GATEKEEPER_MESSAGES_H_
