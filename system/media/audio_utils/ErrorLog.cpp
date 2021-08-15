/*
 * Copyright 2017 The Android Open Source Project
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

// #define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_ErrorLog"
#include <log/log.h>

#include <audio_utils/ErrorLog.h>

using namespace android;

error_log_t *error_log_create(size_t entries, int64_t aggregate_ns)
{
    return reinterpret_cast<error_log_t *>
            (new(std::nothrow) ErrorLog<int32_t>(entries, aggregate_ns));
}

void error_log_log(error_log_t *error_log, int32_t code, int64_t now_ns)
{
    if (error_log == nullptr) {
        return;
    }
    reinterpret_cast<ErrorLog<int32_t> *>(error_log)->log(code, now_ns);
}

int error_log_dump(
        error_log_t *error_log, int fd, const char *prefix, size_t lines, int64_t limit_ns)
{
    if (error_log == nullptr) {
        return BAD_VALUE;
    }
    return reinterpret_cast<ErrorLog<int32_t> *>(error_log)->dump(fd, prefix, lines, limit_ns);
}

void error_log_destroy(error_log_t *error_log)
{
    delete reinterpret_cast<ErrorLog<int32_t> *>(error_log);
}
