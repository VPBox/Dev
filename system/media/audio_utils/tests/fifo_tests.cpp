/*
 * Copyright (C) 2015 The Android Open Source Project
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

// Test program for audio_utils FIFO library.
// This only tests the single-threaded aspects, not the barriers.

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <audio_utils/fifo.h>
#include <audio_utils/sndfile.h>

#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

int main(int argc, char **argv)
{
    size_t frameCount = 0;
    size_t maxFramesPerRead = 0;
    size_t maxFramesPerWrite = 0;
    bool readerThrottlesWriter = true;
    bool verbose = false;
    int i;
    for (i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] != '-')
            break;
        switch (arg[1]) {
        case 'f':   // FIFO frame count
            frameCount = atoi(&arg[2]);
            break;
        case 'r':   // maximum frame count per read from FIFO
            maxFramesPerRead = atoi(&arg[2]);
            break;
        case 't':   // disable throttling of writer by reader
            readerThrottlesWriter = false;
            break;
        case 'v':   // enable verbose logging
            verbose = true;
            break;
        case 'w':   // maximum frame count per write to FIFO
            maxFramesPerWrite = atoi(&arg[2]);
            break;
        default:
            fprintf(stderr, "%s: unknown option %s\n", argv[0], arg);
            goto usage;
        }
    }
    if (frameCount == 0) {
        frameCount = 256;
    }
    if (maxFramesPerRead == 0) {
        maxFramesPerRead = frameCount;
    }
    if (maxFramesPerWrite == 0) {
        maxFramesPerWrite = frameCount;
    }

    if (argc - i != 2) {
usage:
        fprintf(stderr, "usage: %s [-f#] [-r#] [-t] [-v] [-w#] in.wav out.wav\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *inputFile = argv[i];
    char *outputFile = argv[i+1];

    SF_INFO sfinfoin;
    memset(&sfinfoin, 0, sizeof(sfinfoin));
    SNDFILE *sfin = sf_open(inputFile, SFM_READ, &sfinfoin);
    if (sfin == NULL) {
        perror(inputFile);
        return EXIT_FAILURE;
    }
    switch (sfinfoin.format & (SF_FORMAT_TYPEMASK | SF_FORMAT_SUBMASK)) {
    case SF_FORMAT_WAV | SF_FORMAT_PCM_16:
    case SF_FORMAT_WAV | SF_FORMAT_PCM_U8:
        break;
    default:
        fprintf(stderr, "%s: unsupported format\n", inputFile);
        sf_close(sfin);
        return EXIT_FAILURE;
    }
    size_t frameSize = sizeof(int16_t) * sfinfoin.channels;
    int16_t *inputBuffer = new int16_t[sfinfoin.frames * sfinfoin.channels];
    sf_count_t actualRead = sf_readf_short(sfin, inputBuffer, sfinfoin.frames);
    if (actualRead != sfinfoin.frames) {
        fprintf(stderr, "%s: unexpected EOF or error\n", inputFile);
        sf_close(sfin);
        return EXIT_FAILURE;
    }
    sf_close(sfin);

    int16_t *outputBuffer = new int16_t[sfinfoin.frames * sfinfoin.channels];
    size_t framesWritten = 0;
    size_t framesRead = 0;
    int16_t *fifoBuffer = new int16_t[frameCount * sfinfoin.channels];
    audio_utils_fifo fifo(frameCount, frameSize, fifoBuffer, readerThrottlesWriter);
    audio_utils_fifo_writer fifoWriter(fifo);
    audio_utils_fifo_reader fifoReader(fifo, readerThrottlesWriter);
    int fifoWriteCount = 0, fifoReadCount = 0;
    int fifoFillLevel = 0, minFillLevel = INT_MAX, maxFillLevel = INT_MIN;
    for (;;) {
        size_t framesToWrite = sfinfoin.frames - framesWritten;
        size_t framesToRead = sfinfoin.frames - framesRead;
        if (framesToWrite == 0 && (framesToRead == 0 || !readerThrottlesWriter)) {
            break;
        }

        if (framesToWrite > maxFramesPerWrite) {
            framesToWrite = maxFramesPerWrite;
        }
        framesToWrite = rand() % (framesToWrite + 1);
        ssize_t actualWritten = fifoWriter.write(
                &inputBuffer[framesWritten * sfinfoin.channels], framesToWrite);
        if (verbose) {
            printf("wrote %d out of %d\n", (int) actualWritten, (int) framesToWrite);
        }
        if (actualWritten < 0 || (size_t) actualWritten > framesToWrite) {
            fprintf(stderr, "write to FIFO failed\n");
            break;
        }
        if (actualWritten < min((int) frameCount - fifoFillLevel, (int) framesToWrite)) {
            fprintf(stderr, "only wrote %d when should have written min(%d, %d)\n",
                    (int) actualWritten, (int) frameCount - fifoFillLevel, (int) framesToWrite);
        }
        framesWritten += actualWritten;
        if (actualWritten > 0) {
            fifoWriteCount++;
        }
        fifoFillLevel += actualWritten;
        if (verbose) {
            printf("fill level after write %d\n", fifoFillLevel);
        }
        if (fifoFillLevel > maxFillLevel) {
            maxFillLevel = fifoFillLevel;
            if (maxFillLevel > (int) frameCount) {
                if (readerThrottlesWriter) {
                    printf("maxFillLevel=%d > frameCount=%d\n", maxFillLevel, (int) frameCount);
                    abort();
                }
            }
        }

        if (framesToRead > maxFramesPerRead) {
            framesToRead = maxFramesPerRead;
        }
        framesToRead = rand() % (framesToRead + 1);
        ssize_t actualRead = fifoReader.read(
                &outputBuffer[framesRead * sfinfoin.channels], framesToRead);
        if (verbose) {
            printf("read %d out of %d\n", (int) actualRead, (int) framesToRead);
        }
        if (actualRead < 0 || (size_t) actualRead > framesToRead) {
            switch (actualRead) {
            case -EIO:
                fprintf(stderr, "read from FIFO failed: corrupted indices\n");
                abort();
                break;
            case -EOVERFLOW:
                if (readerThrottlesWriter) {
                    fprintf(stderr, "read from FIFO failed: unexpected overflow\n");
                    abort();
                }
                printf("warning: reader lost frames\n");
                actualRead = 0;
                break;
            default:
                if (actualRead < 0) {
                    fprintf(stderr, "read from FIFO failed: unexpected error code %d\n",
                            (int) actualRead);
                } else {
                    fprintf(stderr, "read from FIFO failed: actualRead=%d > framesToRead=%d\n",
                            (int) actualRead, (int) framesToRead);
                }
                abort();
            }
        }
        if (actualRead < min(fifoFillLevel, (int) framesToRead)) {
            //fprintf(stderr, "only read %d when should have read min(%d, %d)\n",
            //        (int) actualRead, fifoFillLevel, (int) framesToRead);
        }
        framesRead += actualRead;
        if (actualRead > 0) {
            fifoReadCount++;
        }
        fifoFillLevel -= actualRead;
        if (verbose) {
            printf("fill level after read %d\n", fifoFillLevel);
        }
        if (fifoFillLevel < minFillLevel) {
            minFillLevel = fifoFillLevel;
            if (minFillLevel < 0) {
                printf("minFillLevel=%d < 0\n", minFillLevel);
                abort();
            }
        }
    }
    delete[] inputBuffer;
    inputBuffer = NULL;
    delete[] fifoBuffer;
    fifoBuffer = NULL;

    printf("FIFO non-empty writes: %d, non-empty reads: %d\n", fifoWriteCount, fifoReadCount);
    printf("fill=%d, min=%d, max=%d\n", fifoFillLevel, minFillLevel, maxFillLevel);

    printf("writing output\n");
    SF_INFO sfinfoout;
    memset(&sfinfoout, 0, sizeof(sfinfoout));
    sfinfoout.samplerate = sfinfoin.samplerate;
    sfinfoout.channels = sfinfoin.channels;
    sfinfoout.format = sfinfoin.format;
    SNDFILE *sfout = sf_open(outputFile, SFM_WRITE, &sfinfoout);
    if (sfout == NULL) {
        perror(outputFile);
        return EXIT_FAILURE;
    }
    sf_count_t actualWritten = sf_writef_short(sfout, outputBuffer, framesRead);
    delete[] outputBuffer;
    outputBuffer = NULL;

    if (actualWritten != (sf_count_t) framesRead) {
        fprintf(stderr, "%s: unexpected error\n", outputFile);
        sf_close(sfout);
        return EXIT_FAILURE;
    }
    sf_close(sfout);
    printf("done\n");

    return EXIT_SUCCESS;
}
