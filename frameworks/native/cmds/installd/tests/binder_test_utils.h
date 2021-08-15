/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <binder/Status.h>
#include <gtest/gtest.h>
#include <utils/String8.h>

#define ASSERT_BINDER_SUCCESS(expr)                                              \
    ({                                                                           \
        binder::Status expect_status = (expr);                                   \
        ASSERT_TRUE(expect_status.isOk()) << expect_status.toString8().c_str();  \
        expect_status;                                                           \
    })
#define ASSERT_BINDER_FAIL(expr)                \
    ({                                          \
        binder::Status expect_status = (expr);  \
        ASSERT_FALSE(expect_status.isOk());     \
        expect_status;                          \
    })
#define EXPECT_BINDER_SUCCESS(expr)                                              \
    ({                                                                           \
        binder::Status expect_status = (expr);                                   \
        EXPECT_TRUE(expect_status.isOk()) << expect_status.toString8().c_str();  \
        expect_status;                                                           \
    })
#define EXPECT_BINDER_FAIL(expr)                \
    ({                                          \
        binder::Status expect_status = (expr);  \
        EXPECT_FALSE(expect_status.isOk());     \
        expect_status;                          \
    })
