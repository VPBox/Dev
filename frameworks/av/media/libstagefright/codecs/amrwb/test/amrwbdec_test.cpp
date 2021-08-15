/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "pvamrwbdecoder.h"
#include <audio_utils/sndfile.h>

// Constants for AMR-WB.
enum {
    kInputBufferSize = 64,
    kSamplesPerFrame = 320,
    kBitsPerSample = 16,
    kOutputBufferSize = kSamplesPerFrame * kBitsPerSample/8,
    kSampleRate = 16000,
    kChannels = 1,
    kFileHeaderSize = 9,
    kMaxSourceDataUnitSize = 477 * sizeof(int16_t)
};

const uint32_t kFrameSizes[] = { 17, 23, 32, 36, 40, 46, 50, 58, 60 };

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the input file.
    FILE* fpInput = fopen(argv[1], "rb");
    if (fpInput == NULL) {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Validate the input AMR file.
    char header[kFileHeaderSize];
    int bytesRead = fread(header, 1, kFileHeaderSize, fpInput);
    if ((bytesRead != kFileHeaderSize) ||
        (memcmp(header, "#!AMR-WB\n", kFileHeaderSize) != 0)) {
        fprintf(stderr, "Invalid AMR-WB file\n");
        fclose(fpInput);
        return EXIT_FAILURE;
    }

    // Open the output file.
    SF_INFO sfInfo;
    memset(&sfInfo, 0, sizeof(SF_INFO));
    sfInfo.channels = kChannels;
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfInfo.samplerate = kSampleRate;
    SNDFILE *handle = sf_open(argv[2], SFM_WRITE, &sfInfo);
    if (handle == NULL) {
        fprintf(stderr, "Could not create %s\n", argv[2]);
        fclose(fpInput);
        return EXIT_FAILURE;
    }

    // Allocate the decoder memory.
    uint32_t memRequirements = pvDecoder_AmrWbMemRequirements();
    void *decoderBuf = malloc(memRequirements);
    assert(decoderBuf != NULL);

    // Create AMR-WB decoder instance.
    void *amrHandle;
    int16_t *decoderCookie;
    pvDecoder_AmrWb_Init(&amrHandle, decoderBuf, &decoderCookie);

    // Allocate input buffer.
    uint8_t *inputBuf = (uint8_t*) malloc(kInputBufferSize);
    assert(inputBuf != NULL);

    // Allocate input sample buffer.
    int16_t *inputSampleBuf = (int16_t*) malloc(kMaxSourceDataUnitSize);
    assert(inputSampleBuf != NULL);

    // Allocate output buffer.
    int16_t *outputBuf = (int16_t*) malloc(kOutputBufferSize);
    assert(outputBuf != NULL);

    // Decode loop.
    int retVal = EXIT_SUCCESS;
    while (1) {
        // Read mode.
        uint8_t modeByte;
        bytesRead = fread(&modeByte, 1, 1, fpInput);
        if (bytesRead != 1) break;
        int16 mode = ((modeByte >> 3) & 0x0f);

        // AMR-WB file format cannot have mode 10, 11, 12 and 13.
        if (mode >= 10 && mode <= 13) {
            fprintf(stderr, "Encountered illegal frame type %d\n", mode);
            retVal = EXIT_FAILURE;
            break;
        }

        if (mode >= 9) {
            // Produce silence for comfort noise, speech lost and no data.
            memset(outputBuf, 0, kOutputBufferSize);
        } else /* if (mode < 9) */ {
            // Read rest of the frame.
            int32_t frameSize = kFrameSizes[mode];
            bytesRead = fread(inputBuf, 1, frameSize, fpInput);
            if (bytesRead != frameSize) break;

            int16 frameType, frameMode;
            RX_State_wb rx_state;
            frameMode = mode;
            mime_unsorting(
                    (uint8_t *)inputBuf,
                    inputSampleBuf,
                    &frameType, &frameMode, 1, &rx_state);

            int16_t numSamplesOutput;
            pvDecoder_AmrWb(
                    frameMode, inputSampleBuf,
                    outputBuf,
                    &numSamplesOutput,
                    decoderBuf, frameType, decoderCookie);

            if (numSamplesOutput != kSamplesPerFrame) {
                fprintf(stderr, "Decoder encountered error\n");
                retVal = EXIT_FAILURE;
                break;
            }

            for (int i = 0; i < kSamplesPerFrame; ++i) {
                outputBuf[i] &= 0xfffC;
            }
        }

        // Write output to wav.
        sf_writef_short(handle, outputBuf, kSamplesPerFrame / kChannels);
    }

    // Close input and output file.
    fclose(fpInput);
    sf_close(handle);

    // Free allocated memory.
    free(inputBuf);
    free(inputSampleBuf);
    free(outputBuf);

    return retVal;
}
