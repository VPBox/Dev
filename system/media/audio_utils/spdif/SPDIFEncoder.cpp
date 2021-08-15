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

#include <stdint.h>
#include <string.h>

#define LOG_TAG "AudioSPDIF"
#include <log/log.h>
#include <audio_utils/spdif/SPDIFEncoder.h>

#include "AC3FrameScanner.h"
#include "DTSFrameScanner.h"

namespace android {

// Burst Preamble defined in IEC61937-1
const uint16_t SPDIFEncoder::kSPDIFSync1 = 0xF872; // Pa
const uint16_t SPDIFEncoder::kSPDIFSync2 = 0x4E1F; // Pb

static int32_t sEndianDetector = 1;
#define isLittleEndian()  (*((uint8_t *)&sEndianDetector))

SPDIFEncoder::SPDIFEncoder(audio_format_t format)
  : mFramer(NULL)
  , mSampleRate(48000)
  , mBurstBuffer(NULL)
  , mBurstBufferSizeBytes(0)
  , mRateMultiplier(1)
  , mBurstFrames(0)
  , mByteCursor(0)
  , mBitstreamNumber(0)
  , mPayloadBytesPending(0)
  , mScanning(true)
{
    switch(format) {
        case AUDIO_FORMAT_AC3:
        case AUDIO_FORMAT_E_AC3:
            mFramer = new AC3FrameScanner(format);
            break;
        case AUDIO_FORMAT_DTS:
        case AUDIO_FORMAT_DTS_HD:
            mFramer = new DTSFrameScanner();
            break;
        default:
            break;
    }

    // This a programmer error. Call isFormatSupported() first.
    LOG_ALWAYS_FATAL_IF((mFramer == NULL),
        "SPDIFEncoder: invalid audio format = 0x%08X", format);

    mBurstBufferSizeBytes = sizeof(uint16_t)
            * SPDIF_ENCODED_CHANNEL_COUNT
            * mFramer->getMaxSampleFramesPerSyncFrame();

    ALOGI("SPDIFEncoder: mBurstBufferSizeBytes = %zu, littleEndian = %d",
            mBurstBufferSizeBytes, isLittleEndian());
    mBurstBuffer = new uint16_t[mBurstBufferSizeBytes >> 1];
    clearBurstBuffer();
}

SPDIFEncoder::SPDIFEncoder()
    : SPDIFEncoder(AUDIO_FORMAT_AC3)
{
}

SPDIFEncoder::~SPDIFEncoder()
{
    delete[] mBurstBuffer;
    delete mFramer;
}

bool SPDIFEncoder::isFormatSupported(audio_format_t format)
{
    switch(format) {
        case AUDIO_FORMAT_AC3:
        case AUDIO_FORMAT_E_AC3:
        case AUDIO_FORMAT_DTS:
        case AUDIO_FORMAT_DTS_HD:
            return true;
        default:
            return false;
    }
}

int SPDIFEncoder::getBytesPerOutputFrame()
{
    return SPDIF_ENCODED_CHANNEL_COUNT * sizeof(int16_t);
}

void SPDIFEncoder::writeBurstBufferShorts(const uint16_t *buffer, size_t numShorts)
{
    // avoid static analyser warning
    LOG_ALWAYS_FATAL_IF((mBurstBuffer == NULL), "mBurstBuffer never allocated");
    mByteCursor = (mByteCursor + 1) & ~1; // round up to even byte
    size_t bytesToWrite = numShorts * sizeof(uint16_t);
    if ((mByteCursor + bytesToWrite) > mBurstBufferSizeBytes) {
        ALOGE("SPDIFEncoder: Burst buffer overflow!");
        reset();
        return;
    }
    memcpy(&mBurstBuffer[mByteCursor >> 1], buffer, bytesToWrite);
    mByteCursor += bytesToWrite;
}

// Pack the bytes into the short buffer in the order:
//   byte[0] -> short[0] MSB
//   byte[1] -> short[0] LSB
//   byte[2] -> short[1] MSB
//   byte[3] -> short[1] LSB
//   etcetera
// This way they should come out in the correct order for SPDIF on both
// Big and Little Endian CPUs.
void SPDIFEncoder::writeBurstBufferBytes(const uint8_t *buffer, size_t numBytes)
{
    size_t bytesToWrite = numBytes;
    if ((mByteCursor + bytesToWrite) > mBurstBufferSizeBytes) {
        ALOGE("SPDIFEncoder: Burst buffer overflow!");
        clearBurstBuffer();
        return;
    }
    uint16_t pad = mBurstBuffer[mByteCursor >> 1];
    for (size_t i = 0; i < bytesToWrite; i++) {
        if (mByteCursor & 1 ) {
            pad |= *buffer++; // put second byte in LSB
            mBurstBuffer[mByteCursor >> 1] = pad;
            pad = 0;
        } else {
            pad |= (*buffer++) << 8; // put first byte in MSB
        }
        mByteCursor++;
    }
    // Save partially filled short.
    if (mByteCursor & 1 ){
        mBurstBuffer[mByteCursor >> 1] = pad;
    }
}

void SPDIFEncoder::sendZeroPad()
{
    // Pad remainder of burst with zeros.
    size_t burstSize = mFramer->getSampleFramesPerSyncFrame() * sizeof(uint16_t)
            * SPDIF_ENCODED_CHANNEL_COUNT;
    if (mByteCursor > burstSize) {
        ALOGE("SPDIFEncoder: Burst buffer, contents too large!");
        clearBurstBuffer();
    } else {
        // We don't have to write zeros because buffer already set to zero
        // by clearBurstBuffer(). Just pretend we wrote zeros by
        // incrementing cursor.
        mByteCursor = burstSize;
    }
}

void SPDIFEncoder::reset()
{
    ALOGV("SPDIFEncoder: reset()");
    clearBurstBuffer();
    if (mFramer != NULL) {
        mFramer->resetBurst();
    }
    mPayloadBytesPending = 0;
    mScanning = true;
}

void SPDIFEncoder::flushBurstBuffer()
{
    const int preambleSize = 4 * sizeof(uint16_t);
    if (mByteCursor > preambleSize) {
        // Set lengthCode for valid payload before zeroPad.
        uint16_t numBytes = (mByteCursor - preambleSize);
        mBurstBuffer[3] = mFramer->convertBytesToLengthCode(numBytes);

        sendZeroPad();
        writeOutput(mBurstBuffer, mByteCursor);
    }
    reset();
}

void SPDIFEncoder::clearBurstBuffer()
{
    if (mBurstBuffer) {
        memset(mBurstBuffer, 0, mBurstBufferSizeBytes);
    }
    mByteCursor = 0;
}

void SPDIFEncoder::startDataBurst()
{
    // Encode IEC61937-1 Burst Preamble
    uint16_t preamble[4];

    uint16_t burstInfo = (mBitstreamNumber << 13)
        | (mFramer->getDataTypeInfo() << 8)
        | mFramer->getDataType();

    mRateMultiplier = mFramer->getRateMultiplier();

    preamble[0] = kSPDIFSync1;
    preamble[1] = kSPDIFSync2;
    preamble[2] = burstInfo;
    preamble[3] = 0; // lengthCode - This will get set after the buffer is full.
    writeBurstBufferShorts(preamble, 4);
}

size_t SPDIFEncoder::startSyncFrame()
{
    // Write start of encoded frame that was buffered in frame detector.
    size_t syncSize = mFramer->getHeaderSizeBytes();
    writeBurstBufferBytes(mFramer->getHeaderAddress(), syncSize);
    return mFramer->getFrameSizeBytes() - syncSize;
}

// Wraps raw encoded data into a data burst.
ssize_t SPDIFEncoder::write( const void *buffer, size_t numBytes )
{
    size_t bytesLeft = numBytes;
    const uint8_t *data = (const uint8_t *)buffer;
    ALOGV("SPDIFEncoder: mScanning = %d, write(buffer[0] = 0x%02X, numBytes = %zu)",
        mScanning, (uint) *data, numBytes);
    while (bytesLeft > 0) {
        if (mScanning) {
        // Look for beginning of next encoded frame.
            if (mFramer->scan(*data)) {
                if (mByteCursor == 0) {
                    startDataBurst();
                } else if (mFramer->isFirstInBurst()) {
                    // Make sure that this frame is at the beginning of the data burst.
                    flushBurstBuffer();
                    startDataBurst();
                }
                mPayloadBytesPending = startSyncFrame();
                mScanning = false;
            }
            data++;
            bytesLeft--;
        } else {
            // Write payload until we hit end of frame.
            size_t bytesToWrite = bytesLeft;
            // Only write as many as we need to finish the frame.
            if (bytesToWrite > mPayloadBytesPending) {
                bytesToWrite = mPayloadBytesPending;
            }
            writeBurstBufferBytes(data, bytesToWrite);

            data += bytesToWrite;
            bytesLeft -= bytesToWrite;
            mPayloadBytesPending -= bytesToWrite;

            // If we have all the payload then send a data burst.
            if (mPayloadBytesPending == 0) {
                if (mFramer->isLastInBurst()) {
                    flushBurstBuffer();
                }
                mScanning = true;
            }
        }
    }
    return numBytes;
}

}  // namespace android
