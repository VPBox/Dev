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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pvmp3decoder_api.h"
#include "mp3reader.h"
#include <audio_utils/sndfile.h>

using namespace std;

enum {
    kInputBufferSize = 10 * 1024,
    kOutputBufferSize = 4608 * 2,
};

int main(int argc, const char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Usage %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize the config.
    tPVMP3DecoderExternal config;
    config.equalizerType = flat;
    config.crcEnabled = false;

    // Allocate the decoder memory.
    uint32_t memRequirements = pvmp3_decoderMemRequirements();
    void *decoderBuf = malloc(memRequirements);
    assert(decoderBuf != NULL);

    // Initialize the decoder.
    pvmp3_InitDecoder(&config, decoderBuf);

    // Open the input file.
    Mp3Reader mp3Reader;
    bool success = mp3Reader.init(argv[1]);
    if (!success) {
        fprintf(stderr, "Encountered error reading %s\n", argv[1]);
        free(decoderBuf);
        return EXIT_FAILURE;
    }

    // Open the output file.
    SF_INFO sfInfo;
    memset(&sfInfo, 0, sizeof(SF_INFO));
    sfInfo.channels = mp3Reader.getNumChannels();
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfInfo.samplerate = mp3Reader.getSampleRate();
    SNDFILE *handle = sf_open(argv[2], SFM_WRITE, &sfInfo);
    if (handle == NULL) {
        fprintf(stderr, "Encountered error writing %s\n", argv[2]);
        mp3Reader.close();
        free(decoderBuf);
        return EXIT_FAILURE;
    }

    // Allocate input buffer.
    uint8_t *inputBuf = static_cast<uint8_t*>(malloc(kInputBufferSize));
    assert(inputBuf != NULL);

    // Allocate output buffer.
    int16_t *outputBuf = static_cast<int16_t*>(malloc(kOutputBufferSize));
    assert(outputBuf != NULL);

    // Decode loop.
    int retVal = EXIT_SUCCESS;
    while (1) {
        // Read input from the file.
        uint32_t bytesRead;
        bool success = mp3Reader.getFrame(inputBuf, &bytesRead);
        if (!success) break;

        // Set the input config.
        config.inputBufferCurrentLength = bytesRead;
        config.inputBufferMaxLength = 0;
        config.inputBufferUsedLength = 0;
        config.pInputBuffer = inputBuf;
        config.pOutputBuffer = outputBuf;
        config.outputFrameSize = kOutputBufferSize / sizeof(int16_t);

        ERROR_CODE decoderErr;
        decoderErr = pvmp3_framedecoder(&config, decoderBuf);
        if (decoderErr != NO_DECODING_ERROR) {
            fprintf(stderr, "Decoder encountered error\n");
            retVal = EXIT_FAILURE;
            break;
        }
        sf_writef_short(handle, outputBuf,
                        config.outputFrameSize / sfInfo.channels);
    }

    // Close input reader and output writer.
    mp3Reader.close();
    sf_close(handle);

    // Free allocated memory.
    free(inputBuf);
    free(outputBuf);
    free(decoderBuf);

    return retVal;
}
