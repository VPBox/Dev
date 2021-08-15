/*
 * Copyright 2014, The Android Open Source Project
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

#define LOG_TAG "AudioSPDIF"

#include <string.h>
#include <assert.h>

#include <log/log.h>
#include <audio_utils/spdif/FrameScanner.h>

namespace android {

FrameScanner::FrameScanner(int dataType,
            const uint8_t *syncBytes,
            uint32_t syncLength,
            uint32_t headerLength)
 : mBytesSkipped(0)
 , mSyncBytes(syncBytes)
 , mSyncLength(syncLength)
 , mHeaderLength(headerLength)
 , mCursor(0)
 , mFormatDumpCount(0)
 , mSampleRate(0)
 , mRateMultiplier(1)
 , mFrameSizeBytes(0)
 , mDataType(dataType)
 , mDataTypeInfo(0)
{
}

FrameScanner::~FrameScanner()
{
}

// State machine that scans for headers in a byte stream.
// @return true if we have detected a complete and valid header.
bool FrameScanner::scan(uint8_t byte)
{
    bool result = false;
    ALOGV("FrameScanner: byte = 0x%02X, mCursor = %d", byte, mCursor);
    assert(mCursor < sizeof(mHeaderBuffer));
    if (mCursor < mSyncLength) {
        // match sync word
        if (byte == mSyncBytes[mCursor]) {
            mHeaderBuffer[mCursor++] = byte;
        } else {
            mBytesSkipped += 1; // skip unsynchronized data
            mCursor = 0;
        }
    } else if (mCursor < mHeaderLength) {
        // gather header for parsing
        mHeaderBuffer[mCursor++] = byte;
        if (mCursor >= mHeaderLength) {
            if (parseHeader()) {
                result = true;
            } else {
                ALOGE("FrameScanner: ERROR - parseHeader() failed.");
            }
            mCursor = 0;
        }
    }
    return result;
}

}  // namespace android
