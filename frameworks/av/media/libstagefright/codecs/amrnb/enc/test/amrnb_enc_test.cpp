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
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include "gsmamr_enc.h"

enum {
    kInputSize = 320, // 160 samples * 16-bit per sample.
    kOutputSize = 1024
};

struct AmrNbEncState {
    void *encCtx;
    void *pidSyncCtx;
};

void usage(void) {
    printf("Usage:\n");
    printf("AMRNBEnc [options] <input file> <output file>\n");
    printf("\n");
    printf("Options +M* for setting compression bitrate mode, default is 4.75 kbps\n");
    printf(" +M0 = 4.75 kbps\n");
    printf(" +M1 = 5.15 kbps\n");
    printf(" +M2 = 5.90 kbps\n");
    printf(" +M3 = 6.70 kbps\n");
    printf(" +M4 = 7.40 kbps\n");
    printf(" +M5 = 7.95 kbps\n");
    printf(" +M6 = 10.2 kbps\n");
    printf(" +M7 = 12.2 kbps\n");
    printf("\n");
}

int encode(int mode, const char *srcFile, const char *dstFile) {
    int           retVal     = EXIT_SUCCESS;
    FILE          *fSrc      = NULL;
    FILE          *fDst      = NULL;
    int           frameNum   = 0;
    bool          eofReached = false;
    uint16_t      *inputBuf  = NULL;
    uint8_t       *outputBuf = NULL;
    AmrNbEncState *amr       = NULL;

    clock_t   start, finish;
    double    duration = 0.0;

    // Open input file.
    fSrc = fopen(srcFile, "rb");
    if (fSrc == NULL) {
        fprintf(stderr, "Error opening input file\n");
        retVal = EXIT_FAILURE;
        goto safe_exit;
    }

    // Open output file.
    fDst = fopen(dstFile, "wb");
    if (fDst == NULL) {
        fprintf(stderr, "Error opening output file\n");
        retVal = EXIT_FAILURE;
        goto safe_exit;
    }

    // Allocate input buffer.
    inputBuf = (uint16_t*) malloc(kInputSize);
    assert(inputBuf != NULL);

    // Allocate output buffer.
    outputBuf = (uint8_t*) malloc(kOutputSize);
    assert(outputBuf != NULL);

    // Initialize encoder.
    amr = (AmrNbEncState*) malloc(sizeof(AmrNbEncState));
    AMREncodeInit(&amr->encCtx, &amr->pidSyncCtx, 0);

    // Write file header.
    fwrite("#!AMR\n", 1, 6, fDst);

    while (1) {
        // Read next input frame.
        int bytesRead;
        bytesRead = fread(inputBuf, 1, kInputSize, fSrc);
        if (bytesRead != kInputSize && !feof(fSrc)) {
            retVal = EXIT_FAILURE; // Invalid magic number.
            fprintf(stderr, "Error reading input file\n");
            goto safe_exit;
        } else if (feof(fSrc) && bytesRead == 0) {
            eofReached = true;
            break;
        }

        start = clock();

        // Encode the frame.
        Frame_Type_3GPP frame_type = (Frame_Type_3GPP) mode;
        int bytesGenerated;
        bytesGenerated = AMREncode(amr->encCtx, amr->pidSyncCtx, (Mode)mode,
                                   (Word16*)inputBuf, outputBuf, &frame_type,
                                   AMR_TX_WMF);

        // Convert from WMF to RFC 3267 format.
        if (bytesGenerated > 0) {
            outputBuf[0] = ((outputBuf[0] << 3) | 4) & 0x7c;
        }

        finish = clock();
        duration += finish - start;

        if (bytesGenerated < 0) {
            retVal = EXIT_FAILURE;
            fprintf(stderr, "Encoding error\n");
            goto safe_exit;
        }

        frameNum++;
        printf(" Frames processed: %d\n", frameNum);

        // Write the output.
        fwrite(outputBuf, 1, bytesGenerated, fDst);
    }

    // Dump the time taken by encode.
    printf("\n%2.5lf seconds\n", (double)duration/CLOCKS_PER_SEC);

safe_exit:

    // Free the encoder instance.
    if (amr) {
        AMREncodeExit(&amr->encCtx, &amr->pidSyncCtx);
        free(amr);
    }

    // Free input and output buffer.
    free(inputBuf);
    free(outputBuf);

    // Close the input and output files.
    if (fSrc) {
        fclose(fSrc);
    }
    if (fDst) {
        fclose(fDst);
    }

    return retVal;
}

int main(int argc, char *argv[]) {
    Mode  mode = MR475;
    int   retVal;
    char  *inFileName = NULL;
    char  *outFileName = NULL;
    int   arg, filename = 0;

    if (argc < 3) {
        usage();
        return EXIT_FAILURE;
    } else {
        for (arg = 1; arg < argc; arg++) {
            if (argv[arg][0] == '+') {
                if (argv[arg][1] == 'M') {
                    switch (argv[arg][2]) {
                    case '0': mode = MR475;
                        break;
                    case '1': mode = MR515;
                        break;
                    case '2': mode = MR59;
                        break;
                    case '3': mode = MR67;
                        break;
                    case '4': mode = MR74;
                        break;
                    case '5': mode = MR795;
                        break;
                    case '6': mode = MR102;
                        break;
                    case '7': mode = MR122;
                        break;
                    default:
                        usage();
                        fprintf(stderr, "Invalid parameter '%s'.\n", argv[arg]);
                        return EXIT_FAILURE;
                        break;
                    }
                } else {
                    usage();
                    fprintf(stderr, "Invalid parameter '%s'.\n", argv[arg]);
                    return EXIT_FAILURE;
                }
            } else {
                switch (filename) {
                case 0:
                    inFileName  = argv[arg];
                    break;
                case 1:
                    outFileName = argv[arg];
                    break;
                default:
                    usage();
                    fprintf(stderr, "Invalid parameter '%s'.\n", argv[arg]);
                    return EXIT_FAILURE;
                }
                filename++;
            }
        }
    }

    retVal = encode(mode, inFileName, outFileName);
    return retVal;
}

