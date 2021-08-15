/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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
#include <assert.h>
#include <stdlib.h>

#include "mp4enc_api.h"

// Constants.
enum {
    kMaxWidth         = 720,
    kMaxHeight        = 480,
    kMaxFrameRate     = 30,
    kMaxBitrate       = 2048, // in kbps.
    kOutputBufferSize = 250 * 1024,
    kIDRFrameRefreshIntervalInSec = 1, // in seconds.
};

int main(int argc, char *argv[]) {

    if (argc < 8) {
        fprintf(stderr, "Usage %s <input yuv> <output file> <mode> <width> "
                        "<height> <frame rate> <bitrate in kbps>\n", argv[0]);
        fprintf(stderr, "mode : h263 or mpeg4\n");
        fprintf(stderr, "Max width %d\n", kMaxWidth);
        fprintf(stderr, "Max height %d\n", kMaxHeight);
        fprintf(stderr, "Max framerate %d\n", kMaxFrameRate);
        fprintf(stderr, "Max bitrate %d kbps\n", kMaxBitrate);
        return EXIT_FAILURE;
    }

    // Read mode.
    bool isH263mode;
    if (strcmp(argv[3], "mpeg4") == 0) {
        isH263mode = false;
    } else if (strcmp(argv[3], "h263") == 0) {
        isH263mode = true;
    } else {
        fprintf(stderr, "Unsupported mode %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    // Read height and width.
    int32_t width;
    int32_t height;
    width = atoi(argv[4]);
    height = atoi(argv[5]);
    if (width > kMaxWidth || height > kMaxHeight || width <= 0 || height <= 0) {
        fprintf(stderr, "Unsupported dimensions %dx%d\n", width, height);
        return EXIT_FAILURE;
    }

    if (width % 16 != 0 || height % 16 != 0) {
        fprintf(stderr, "Video frame size %dx%d must be a multiple of 16\n",
            width, height);
        return EXIT_FAILURE;
    }

    // Read frame rate.
    int32_t frameRate;
    frameRate = atoi(argv[6]);
    if (frameRate > kMaxFrameRate || frameRate <= 0) {
        fprintf(stderr, "Unsupported frame rate %d\n", frameRate);
        return EXIT_FAILURE;
    }

    // Read bitrate.
    int32_t bitrate;
    bitrate = atoi(argv[7]);
    if (bitrate > kMaxBitrate || bitrate <= 0) {
        fprintf(stderr, "Unsupported bitrate %d\n", bitrate);
        return EXIT_FAILURE;
    }

    // Allocate input buffer.
    uint8_t *inputBuf = (uint8_t *)malloc((width * height * 3) / 2);
    assert(inputBuf != NULL);

    // Allocate output buffer.
    uint8_t *outputBuf = (uint8_t *)malloc(kOutputBufferSize);
    assert(outputBuf != NULL);

    // Open the input file.
    FILE *fpInput = fopen(argv[1], "rb");
    if (fpInput == NULL) {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        free(inputBuf);
        free(outputBuf);
        return EXIT_FAILURE;
    }

    // Open the output file.
    FILE *fpOutput = fopen(argv[2], "wb");
    if (fpOutput == NULL) {
        fprintf(stderr, "Could not open %s\n", argv[2]);
        free(inputBuf);
        free(outputBuf);
        fclose(fpInput);
        return EXIT_FAILURE;
    }

    // Initialize the encoder parameters.
    tagvideoEncOptions encParams;
    memset(&encParams, 0, sizeof(tagvideoEncOptions));
    if (!PVGetDefaultEncOption(&encParams, 0)) {
        fprintf(stderr, "Failed to get default encoding parameters\n");
        free(inputBuf);
        free(outputBuf);
        fclose(fpInput);
        fclose(fpOutput);
        return EXIT_FAILURE;
    }

    if (isH263mode == false) {
        encParams.encMode = COMBINE_MODE_WITH_ERR_RES;
    } else {
        encParams.encMode = H263_MODE;
    }
    encParams.encWidth[0] = width;
    encParams.encHeight[0] = height;
    encParams.encFrameRate[0] = frameRate;
    encParams.rcType = VBR_1;
    encParams.vbvDelay = 5.0f;
    encParams.profile_level = CORE_PROFILE_LEVEL2;
    encParams.packetSize = 32;
    encParams.rvlcEnable = PV_OFF;
    encParams.numLayers = 1;
    encParams.timeIncRes = 1000;
    encParams.tickPerSrc = encParams.timeIncRes / frameRate;

    encParams.bitRate[0] = bitrate * 1024;
    encParams.iQuant[0] = 15;
    encParams.pQuant[0] = 12;
    encParams.quantType[0] = 0;
    encParams.noFrameSkipped = PV_OFF;

    int32_t  IDRFrameRefreshIntervalInSec = kIDRFrameRefreshIntervalInSec;
    if (IDRFrameRefreshIntervalInSec == 0) {
        encParams.intraPeriod = 1;  // All I frames.
    } else {
        encParams.intraPeriod = (IDRFrameRefreshIntervalInSec * frameRate);
    }

    encParams.numIntraMB = 0;
    encParams.sceneDetect = PV_ON;
    encParams.searchRange = 16;
    encParams.mv8x8Enable = PV_OFF;
    encParams.gobHeaderInterval = 0;
    encParams.useACPred = PV_ON;
    encParams.intraDCVlcTh = 0;

    // Initialize the handle.
    tagvideoEncControls handle;
    memset(&handle, 0, sizeof(tagvideoEncControls));

    // Initialize the encoder.
    if (!PVInitVideoEncoder(&handle, &encParams)) {
        fprintf(stderr, "Failed to initialize the encoder\n");
        fclose(fpInput);
        fclose(fpOutput);
        free(inputBuf);
        free(outputBuf);
        return EXIT_FAILURE;
    }

    // Generate the header.
    int32_t headerLength = kOutputBufferSize;
    if (!PVGetVolHeader(&handle, outputBuf, &headerLength, 0)) {
        fprintf(stderr, "Failed to get VOL header\n");
        fclose(fpInput);
        fclose(fpOutput);
        free(inputBuf);
        free(outputBuf);
        return EXIT_FAILURE;
    }
    fwrite(outputBuf, 1, headerLength, fpOutput);

    // Core loop.
    int32_t retVal = EXIT_SUCCESS;
    int32_t frameSize = (width * height * 3) / 2;
    int32_t numFramesEncoded = 0;

    while (1) {
        // Read the input frame.
        int32_t bytesRead;
        bytesRead = fread(inputBuf, 1, frameSize, fpInput);
        if (bytesRead != frameSize) {
            break; // End of file.
        }

        // Encode the input frame.
        VideoEncFrameIO vin, vout;
        memset(&vin, 0, sizeof(vin));
        memset(&vout, 0, sizeof(vout));
        vin.height = height; // height is multiple of 16.
        vin.pitch = width; // width is multiple of 16.
        vin.timestamp = (numFramesEncoded * 1000) / frameRate;  // in ms.
        vin.yChan = inputBuf;
        vin.uChan = vin.yChan + vin.height * vin.pitch;
        vin.vChan = vin.uChan + ((vin.height * vin.pitch) >> 2);

        uint32_t modTimeMs = 0;
        int32_t nLayer = 0;
        MP4HintTrack hintTrack;
        int32_t dataLength = kOutputBufferSize;
        if (!PVEncodeVideoFrame(&handle, &vin, &vout,
                &modTimeMs, outputBuf, &dataLength, &nLayer) ||
            !PVGetHintTrack(&handle, &hintTrack)) {
            fprintf(stderr, "Failed to encode frame or get hink track at "
                    " frame %d\n", numFramesEncoded);
            retVal = EXIT_FAILURE;
            break;
        }
        PVGetOverrunBuffer(&handle);
        numFramesEncoded++;

        // Write the output.
        fwrite(outputBuf, 1, dataLength, fpOutput);
    }

    // Close input and output file.
    fclose(fpInput);
    fclose(fpOutput);

    // Free allocated memory.
    free(inputBuf);
    free(outputBuf);

    // Close encoder instance.
    PVCleanUpVideoEncoder(&handle);
    return retVal;
}
