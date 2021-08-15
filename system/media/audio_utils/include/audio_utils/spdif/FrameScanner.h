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

#ifndef ANDROID_AUDIO_FRAME_SCANNER_H
#define ANDROID_AUDIO_FRAME_SCANNER_H

#include <stdint.h>

namespace android {


/**
 * Scan a byte stream looking for the start of an encoded frame.
 * Parse the sample rate and the size of the encoded frame.
 * Buffer the sync header so it can be prepended to the remaining data.
 *
 * This is used directly by the SPDIFEncoder. External clients will
 * generally not call this class.
 */
class FrameScanner {
public:
    FrameScanner(int dataType,
            const uint8_t *syncBytes,
            uint32_t syncLength,
            uint32_t headerLength
            );
    virtual ~FrameScanner();

    /**
     * Pass each byte of the encoded stream to this scanner.
     * @return true if a complete and valid header was detected
     */
    virtual bool scan(uint8_t byte);

    /**
     * @return address of where the sync header was stored by scan()
     */
    const uint8_t *getHeaderAddress() const { return mHeaderBuffer; }

    /**
     * @return number of bytes in sync header stored by scan()
     */
    size_t getHeaderSizeBytes() const { return mHeaderLength; }

    /**
     * @return sample rate of the encoded audio
     */
    uint32_t getSampleRate()   const { return mSampleRate; }

    /**
     * Some formats, for example EAC3, are wrapped in data bursts that have
     * a sample rate that is a multiple of the encoded sample rate.
     * The default multiplier is 1.
     * @return sample rate multiplier for the SP/DIF PCM data bursts
     */
    uint32_t getRateMultiplier()   const { return mRateMultiplier; }

    size_t getFrameSizeBytes()     const { return mFrameSizeBytes; }

    /**
     * dataType is defined by the SPDIF standard for each format
     */
    int getDataType()      const { return mDataType; }
    int getDataTypeInfo()  const { return mDataTypeInfo; }

    virtual int getMaxChannels() const = 0;

    virtual void resetBurst() = 0;

    /**
     * @return the number of pcm frames that correspond to one encoded frame
     */
    virtual int getMaxSampleFramesPerSyncFrame() const = 0;
    virtual int getSampleFramesPerSyncFrame()    const = 0;

    /**
     * @return true if this parsed frame must be the first frame in a data burst.
     */
    virtual bool isFirstInBurst() = 0;

    /**
     * If this returns false then the previous frame may or may not be the last frame.
     * @return true if this parsed frame is definitely the last frame in a data burst.
     */
    virtual bool isLastInBurst()  = 0;

    /**
     * Most compression types use a lengthCode expressed in bits.
     */
    virtual uint16_t convertBytesToLengthCode(uint16_t numBytes) const { return numBytes * 8; }

protected:
    uint32_t  mBytesSkipped;     // how many bytes were skipped looking for the start of a frame
    const uint8_t *mSyncBytes;   // pointer to the sync word specific to a format
    uint32_t  mSyncLength;       // number of bytes in sync word
    uint8_t   mHeaderBuffer[32]; // a place to gather the relevant header bytes for parsing
    uint32_t  mHeaderLength;     // the number of bytes we need to parse
    uint32_t  mCursor;           // position in the mHeaderBuffer
    uint32_t  mFormatDumpCount;  // used to thin out the debug dumps
    uint32_t  mSampleRate;       // encoded sample rate
    uint32_t  mRateMultiplier;   // SPDIF output data burst rate = msampleRate * mRateMultiplier
    size_t    mFrameSizeBytes;   // encoded frame size
    int       mDataType;         // as defined in IEC61937-2 paragraph 4.2
    int       mDataTypeInfo;     // as defined in IEC61937-2 paragraph 4.1

    /**
     * Parse data in mHeaderBuffer.
     * Sets mDataType, mFrameSizeBytes, mSampleRate, mRateMultiplier.
     * @return true if the header is valid.
     */
    virtual bool parseHeader() = 0;

};


}  // namespace android
#endif  // ANDROID_AUDIO_FRAME_SCANNER_H
