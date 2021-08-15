/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef MINIKIN_TEST_ICU_ENVIRONMENT_H
#define MINIKIN_TEST_ICU_ENVIRONMENT_H

// low level file access for mapping ICU data
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <cutils/log.h>
#include <gtest/gtest.h>
#include <unicode/uclean.h>
#include <unicode/udata.h>

namespace minikin {

class ICUEnvironment : public testing::Environment {
public:
    ICUEnvironment() : testing::Environment(), mData(nullptr), mSize(0) {}

    void* mData;
    size_t mSize;

    virtual void SetUp() override {
        const char* fn = "/apex/com.android.runtime/etc/icu/" U_ICUDATA_NAME ".dat";
        int fd = open(fn, O_RDONLY);
        LOG_ALWAYS_FATAL_IF(fd == -1);
        struct stat sb;
        LOG_ALWAYS_FATAL_IF(fstat(fd, &sb) != 0);

        mSize = sb.st_size;
        void* mData = mmap(nullptr, mSize, PROT_READ, MAP_SHARED, fd, 0);
        close(fd);

        UErrorCode errorCode = U_ZERO_ERROR;
        udata_setCommonData(mData, &errorCode);
        LOG_ALWAYS_FATAL_IF(U_FAILURE(errorCode));

        errorCode = U_ZERO_ERROR;
        u_init(&errorCode);
        LOG_ALWAYS_FATAL_IF(U_FAILURE(errorCode));
    }

    virtual void TearDown() override {
        u_cleanup();
        munmap(mData, mSize);
    }
};

}  // namespace minikin
#endif  //  MINIKIN_TEST_ICU_ENVIRONMENT_H
