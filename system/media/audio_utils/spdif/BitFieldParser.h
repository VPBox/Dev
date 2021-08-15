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

#ifndef ANDROID_AUDIO_BIT_FIELD_PARSER_H
#define ANDROID_AUDIO_BIT_FIELD_PARSER_H

#include <stdint.h>

namespace android {

/**
 * Extract bit fields from a byte array.
 */
class BitFieldParser {
public:

    explicit BitFieldParser(uint8_t *data);
    virtual ~BitFieldParser();

    /**
     * Read numBits bits from the data array.
     * Fields may span byte boundaries but may not exceed 32-bits.
     * Note that the caller must ensure that there is suffcient data.
     * Assume data is organized as BigEndian format.
     */
    uint32_t readBits(uint32_t numBits);

    /*
     * When the cursor is zero it points to a position right before
     * the most significant bit.
     * When the cursor is seven it points to a position right before
     * the least significant bit.
     */
    uint32_t getBitCursor() const { return mBitCursor; }

private:
    uint8_t *mData;
    uint32_t mBitCursor;
};


}  // namespace android

#endif  // ANDROID_AUDIO_BIT_FIELD_PARSER_H
