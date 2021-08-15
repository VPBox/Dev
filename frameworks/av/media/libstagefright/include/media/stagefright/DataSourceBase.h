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

#ifndef DATA_SOURCE_BASE_H_

#define DATA_SOURCE_BASE_H_

#include <sys/types.h>
#include <utils/Errors.h>

namespace android {

class String8;

class DataSourceBase {
public:
    enum Flags {
        kWantsPrefetching      = 1,
        kStreamedFromLocalHost = 2,
        kIsCachingDataSource   = 4,
        kIsHTTPBasedSource     = 8,
        kIsLocalFileSource     = 16,
    };

    DataSourceBase() {}

    virtual status_t initCheck() const = 0;

    // Returns the number of bytes read, or -1 on failure. It's not an error if
    // this returns zero; it just means the given offset is equal to, or
    // beyond, the end of the source.
    virtual ssize_t readAt(off64_t offset, void *data, size_t size) = 0;

    // Convenience methods:
    bool getUInt16(off64_t offset, uint16_t *x);
    bool getUInt24(off64_t offset, uint32_t *x); // 3 byte int, returned as a 32-bit int
    bool getUInt32(off64_t offset, uint32_t *x);
    bool getUInt64(off64_t offset, uint64_t *x);

    // read either int<N> or int<2N> into a uint<2N>_t, size is the int size in bytes.
    bool getUInt16Var(off64_t offset, uint16_t *x, size_t size);
    bool getUInt32Var(off64_t offset, uint32_t *x, size_t size);
    bool getUInt64Var(off64_t offset, uint64_t *x, size_t size);

    // May return ERROR_UNSUPPORTED.
    virtual status_t getSize(off64_t *size);

    virtual bool getUri(char *uriString, size_t bufferSize);

    virtual uint32_t flags() {
        return 0;
    }

    virtual void close() {};

    virtual status_t getAvailableSize(off64_t /*offset*/, off64_t * /*size*/) {
        return -1;
    }

protected:
    virtual ~DataSourceBase() {}

private:
    DataSourceBase(const DataSourceBase &);
    DataSourceBase &operator=(const DataSourceBase &);
};

}  // namespace android

#endif  // DATA_SOURCE_BASE_H_
