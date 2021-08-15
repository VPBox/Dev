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

#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "gsmamr_dec.h"
#include <audio_utils/sndfile.h>

// Constants for AMR-NB
enum {
    kInputBufferSize = 64,
    kSamplesPerFrame = 160,
    kBitsPerSample = 16,
    kOutputBufferSize = kSamplesPerFrame * kBitsPerSample/8,
    kSampleRate = 8000,
    kChannels = 1,
    kFileHeaderSize = 6
};
const uint32_t kFrameSizes[] = {12, 13, 15, 17, 19, 20, 26, 31};


int main(int argc, char *argv[]) {

    if(argc != 3) {
        fprintf(stderr, "Usage %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* fpInput = fopen(argv[1], "rb");
    if (!fpInput) {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 1;
    }

    // Validate the input AMR file
    char header[kFileHeaderSize];
    int bytesRead = fread(header, 1, kFileHeaderSize, fpInput);
    if (bytesRead != kFileHeaderSize || memcmp(header, "#!AMR\n", kFileHeaderSize)) {
        fprintf(stderr, "Invalid AMR-NB file\n");
        fclose(fpInput);
        return 1;
    }

    // Open the output file
    SF_INFO sfInfo;
    memset(&sfInfo, 0, sizeof(SF_INFO));
    sfInfo.channels = kChannels;
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfInfo.samplerate = kSampleRate;
    SNDFILE *handle = sf_open(argv[2], SFM_WRITE, &sfInfo);
    if(!handle){
        fprintf(stderr, "Could not create %s\n", argv[2]);
        fclose(fpInput);
        return 1;
    }

    // Create AMR-NB decoder instance
    void* amrHandle;
    int err = GSMInitDecode(&amrHandle, (Word8*)"AMRNBDecoder");
    if(err != 0){
        fprintf(stderr, "Error creating AMR-NB decoder instance\n");
        fclose(fpInput);
        sf_close(handle);
        return 1;
    }

    //Allocate input buffer
    void *inputBuf = malloc(kInputBufferSize);
    assert(inputBuf != NULL);

    //Allocate output buffer
    void *outputBuf = malloc(kOutputBufferSize);
    assert(outputBuf != NULL);


    // Decode loop
    uint32_t retVal = 0;
    while (1) {
        // Read mode
        uint8_t mode;
        bytesRead = fread(&mode, 1, 1, fpInput);
        if (bytesRead != 1) break;

        // Find frame type
        Frame_Type_3GPP frameType = (Frame_Type_3GPP)((mode >> 3) & 0x0f);
        if (frameType >= AMR_SID){
            fprintf(stderr, "Frame type %d not supported\n",frameType);
            retVal = 1;
            break;
        }

        // Find frame type
        int32_t frameSize = kFrameSizes[frameType];
        bytesRead = fread(inputBuf, 1, frameSize, fpInput);
        if (bytesRead != frameSize) break;

        //Decode frame
        int32_t decodeStatus;
        decodeStatus = AMRDecode(amrHandle, frameType, (uint8_t*)inputBuf,
                                 (int16_t*)outputBuf, MIME_IETF);
        if(decodeStatus == -1) {
            fprintf(stderr, "Decoder encountered error\n");
            retVal = 1;
            break;
        }

        //Write output to wav
        sf_writef_short(handle, (int16_t*)outputBuf, kSamplesPerFrame);

    }

    // Close input and output file
    fclose(fpInput);
    sf_close(handle);

    //Free allocated memory
    free(inputBuf);
    free(outputBuf);

    // Close decoder instance
    GSMDecodeFrameExit(&amrHandle);

    return retVal;
}
