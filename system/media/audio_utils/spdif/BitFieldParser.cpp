/*
 * Copyright 2015, The Android Open Source Project
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
//#define LOG_NDEBUG 0

#include <string.h>
#include <assert.h>

#include <log/log.h>
#include "BitFieldParser.h"

namespace android {

BitFieldParser::BitFieldParser(uint8_t *data)
 : mData(data)
 , mBitCursor(0)
{
}

BitFieldParser::~BitFieldParser()
{
}

uint32_t BitFieldParser::readBits(uint32_t numBits)
{
    ALOG_ASSERT(numBits <= 32);

    // Extract some bits from the current byte.
    uint32_t byteCursor = mBitCursor >> 3; // 8 bits per byte
    uint8_t byte = mData[byteCursor];

    uint32_t bitsLeftInByte = 8 - (mBitCursor & 7);
    uint32_t bitsFromByte = (bitsLeftInByte < numBits) ? bitsLeftInByte : numBits;
    uint32_t result = byte >> (bitsLeftInByte - bitsFromByte);
    result &= (1 << bitsFromByte) - 1; // mask
    mBitCursor += bitsFromByte;

    uint32_t bitsRemaining = numBits - bitsFromByte;
    if (bitsRemaining == 0) {
        return result;
    } else {
        // Use recursion to get remaining bits.
        return (result << bitsRemaining) | readBits(bitsRemaining);
    }
}

}  // namespace android
