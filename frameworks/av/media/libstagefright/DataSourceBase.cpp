/*
 * Copyright (C) 2009 The Android Open Source Project
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
//#define LOG_NDEBUG 0
#define LOG_TAG "DataSourceBase"

#include <media/DataSourceBase.h>
#include <media/stagefright/foundation/ByteUtils.h>
#include <media/stagefright/MediaErrors.h>
#include <utils/String8.h>

namespace android {

bool DataSourceBase::getUInt16(off64_t offset, uint16_t *x) {
    *x = 0;

    uint8_t byte[2];
    if (readAt(offset, byte, 2) != 2) {
        return false;
    }

    *x = (byte[0] << 8) | byte[1];

    return true;
}

bool DataSourceBase::getUInt24(off64_t offset, uint32_t *x) {
    *x = 0;

    uint8_t byte[3];
    if (readAt(offset, byte, 3) != 3) {
        return false;
    }

    *x = (byte[0] << 16) | (byte[1] << 8) | byte[2];

    return true;
}

bool DataSourceBase::getUInt32(off64_t offset, uint32_t *x) {
    *x = 0;

    uint32_t tmp;
    if (readAt(offset, &tmp, 4) != 4) {
        return false;
    }

    *x = ntohl(tmp);

    return true;
}

bool DataSourceBase::getUInt64(off64_t offset, uint64_t *x) {
    *x = 0;

    uint64_t tmp;
    if (readAt(offset, &tmp, 8) != 8) {
        return false;
    }

    *x = ntoh64(tmp);

    return true;
}

bool DataSourceBase::getUInt16Var(off64_t offset, uint16_t *x, size_t size) {
    if (size == 2) {
        return getUInt16(offset, x);
    }
    if (size == 1) {
        uint8_t tmp;
        if (readAt(offset, &tmp, 1) == 1) {
            *x = tmp;
            return true;
        }
    }
    return false;
}

bool DataSourceBase::getUInt32Var(off64_t offset, uint32_t *x, size_t size) {
    if (size == 4) {
        return getUInt32(offset, x);
    }
    if (size == 2) {
        uint16_t tmp;
        if (getUInt16(offset, &tmp)) {
            *x = tmp;
            return true;
        }
    }
    return false;
}

bool DataSourceBase::getUInt64Var(off64_t offset, uint64_t *x, size_t size) {
    if (size == 8) {
        return getUInt64(offset, x);
    }
    if (size == 4) {
        uint32_t tmp;
        if (getUInt32(offset, &tmp)) {
            *x = tmp;
            return true;
        }
    }
    return false;
}

status_t DataSourceBase::getSize(off64_t *size) {
    *size = 0;

    return ERROR_UNSUPPORTED;
}

bool DataSourceBase::getUri(char *uriString __unused, size_t bufferSize __unused) {
    return false;
}

}  // namespace android
