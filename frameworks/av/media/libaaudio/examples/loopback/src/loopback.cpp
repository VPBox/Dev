/*
 * Copyright (C) 2016 The Android Open Source Project
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

// Audio loopback tests to measure the round trip latency and glitches.

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>

#include "AAudioSimplePlayer.h"
#include "AAudioSimpleRecorder.h"
#include "AAudioExampleUtils.h"
#include "LoopbackAnalyzer.h"
#include "../../utils/AAudioExampleUtils.h"

// V0.4.00 = rectify and low-pass filter the echos, auto-correlate entire echo
// V0.4.01 = add -h hang option
//           fix -n option to set output buffer for -tm
//           plot first glitch
// V0.4.02 = allow -n0 for minimal buffer size
#define APP_VERSION             "0.4.02"

// Tag for machine readable results as property = value pairs
#define RESULT_TAG              "RESULT: "
#define FILENAME_ALL            "/data/loopback_all.wav"
#define FILENAME_ECHOS          "/data/loopback_echos.wav"
#define FILENAME_PROCESSED      "/data/loopback_processed.wav"

constexpr int kLogPeriodMillis       = 1000;
constexpr int kNumInputChannels      = 1;
constexpr int kNumCallbacksToDrain   = 20;
constexpr int kNumCallbacksToNotRead = 0; // let input fill back up
constexpr int kNumCallbacksToDiscard = 20;
constexpr int kDefaultHangTimeMillis = 50;
constexpr int kMaxGlitchEventsToSave = 32;

struct LoopbackData {
    AAudioStream      *inputStream = nullptr;
    AAudioStream      *outputStream = nullptr;
    int32_t            inputFramesMaximum = 0;
    int16_t           *inputShortData = nullptr;
    float             *inputFloatData = nullptr;
    aaudio_format_t    actualInputFormat = AAUDIO_FORMAT_INVALID;
    int32_t            actualInputChannelCount = 0;
    int32_t            actualOutputChannelCount = 0;
    int32_t            numCallbacksToDrain = kNumCallbacksToDrain;
    int32_t            numCallbacksToNotRead = kNumCallbacksToNotRead;
    int32_t            numCallbacksToDiscard = kNumCallbacksToDiscard;
    int32_t            minNumFrames = INT32_MAX;
    int32_t            maxNumFrames = 0;
    int32_t            insufficientReadCount = 0;
    int32_t            insufficientReadFrames = 0;
    int32_t            framesReadTotal = 0;
    int32_t            framesWrittenTotal = 0;
    int32_t            hangPeriodMillis = 5 * 1000; // time between hangs
    int32_t            hangCountdownFrames = 5 * 48000; // frames til next hang
    int32_t            hangTimeMillis = 0; // 0 for no hang
    bool               isDone = false;

    aaudio_result_t    inputError = AAUDIO_OK;
    aaudio_result_t    outputError = AAUDIO_OK;

    SineAnalyzer       sineAnalyzer;
    EchoAnalyzer       echoAnalyzer;
    AudioRecording     audioRecording;
    LoopbackProcessor *loopbackProcessor;

    int32_t            glitchFrames[kMaxGlitchEventsToSave];
    int32_t            numGlitchEvents = 0;

    void hangIfRequested(int32_t numFrames) {
        if (hangTimeMillis > 0) {
            hangCountdownFrames -= numFrames;
            if (hangCountdownFrames <= 0) {
                const int64_t startNanos = getNanoseconds();
                usleep(hangTimeMillis * 1000);
                const int64_t endNanos = getNanoseconds();
                const int32_t elapsedMicros = (int32_t)
                        ((endNanos - startNanos) / 1000);
                printf("callback hanging for %d millis, actual = %d micros\n",
                       hangTimeMillis, elapsedMicros);
                hangCountdownFrames = (int64_t) hangPeriodMillis
                        * AAudioStream_getSampleRate(outputStream)
                        / 1000;
            }
        }


    }
};

static void convertPcm16ToFloat(const int16_t *source,
                                float *destination,
                                int32_t numSamples) {
    constexpr float scaler = 1.0f / 32768.0f;
    for (int i = 0; i < numSamples; i++) {
        destination[i] = source[i] * scaler;
    }
}

// ====================================================================================
// ========================= CALLBACK =================================================
// ====================================================================================
// Callback function that fills the audio output buffer.

static int32_t readFormattedData(LoopbackData *myData, int32_t numFrames) {
    int32_t framesRead = AAUDIO_ERROR_INVALID_FORMAT;
    if (myData->actualInputFormat == AAUDIO_FORMAT_PCM_I16) {
        framesRead = AAudioStream_read(myData->inputStream, myData->inputShortData,
                                       numFrames,
                                       0 /* timeoutNanoseconds */);
    } else if (myData->actualInputFormat == AAUDIO_FORMAT_PCM_FLOAT) {
        framesRead = AAudioStream_read(myData->inputStream, myData->inputFloatData,
                                       numFrames,
                                       0 /* timeoutNanoseconds */);
    } else {
        printf("ERROR actualInputFormat = %d\n", myData->actualInputFormat);
        assert(false);
    }
    if (framesRead < 0) {
        // Expect INVALID_STATE if STATE_STARTING
        if (myData->framesReadTotal > 0) {
            myData->inputError = framesRead;
            printf("ERROR in read = %d = %s\n", framesRead,
                   AAudio_convertResultToText(framesRead));
        } else {
            framesRead = 0;
        }
    } else {
        myData->framesReadTotal += framesRead;
    }
    return framesRead;
}

static aaudio_data_callback_result_t MyDataCallbackProc(
        AAudioStream *outputStream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) outputStream;
    aaudio_data_callback_result_t result = AAUDIO_CALLBACK_RESULT_CONTINUE;
    LoopbackData *myData = (LoopbackData *) userData;
    float  *outputData = (float  *) audioData;

    // Read audio data from the input stream.
    int32_t actualFramesRead;

    if (numFrames > myData->inputFramesMaximum) {
        myData->inputError = AAUDIO_ERROR_OUT_OF_RANGE;
        return AAUDIO_CALLBACK_RESULT_STOP;
    }

    if (numFrames > myData->maxNumFrames) {
        myData->maxNumFrames = numFrames;
    }
    if (numFrames < myData->minNumFrames) {
        myData->minNumFrames = numFrames;
    }

    // Silence the output.
    int32_t numBytes = numFrames * myData->actualOutputChannelCount * sizeof(float);
    memset(audioData, 0 /* value */, numBytes);

    if (myData->numCallbacksToDrain > 0) {
        // Drain the input.
        int32_t totalFramesRead = 0;
        do {
            actualFramesRead = readFormattedData(myData, numFrames);
            if (actualFramesRead > 0) {
                totalFramesRead += actualFramesRead;
            } else if (actualFramesRead < 0) {
                result = AAUDIO_CALLBACK_RESULT_STOP;
            }
            // Ignore errors because input stream may not be started yet.
        } while (actualFramesRead > 0);
        // Only counts if we actually got some data.
        if (totalFramesRead > 0) {
            myData->numCallbacksToDrain--;
        }

    } else if (myData->numCallbacksToNotRead > 0) {
        // Let the input fill up a bit so we are not so close to the write pointer.
        myData->numCallbacksToNotRead--;
    } else if (myData->numCallbacksToDiscard > 0) {
        // Ignore. Allow the input to fill back up to equilibrium with the output.
        actualFramesRead = readFormattedData(myData, numFrames);
        if (actualFramesRead < 0) {
            result = AAUDIO_CALLBACK_RESULT_STOP;
        }
        myData->numCallbacksToDiscard--;

    } else {
        myData->hangIfRequested(numFrames);

        int32_t numInputBytes = numFrames * myData->actualInputChannelCount * sizeof(float);
        memset(myData->inputFloatData, 0 /* value */, numInputBytes);

        // Process data after equilibrium.
        int64_t inputFramesWritten = AAudioStream_getFramesWritten(myData->inputStream);
        int64_t inputFramesRead = AAudioStream_getFramesRead(myData->inputStream);
        int64_t framesAvailable = inputFramesWritten - inputFramesRead;

        actualFramesRead = readFormattedData(myData, numFrames); // READ
        if (actualFramesRead < 0) {
            result = AAUDIO_CALLBACK_RESULT_STOP;
        } else {

            if (actualFramesRead < numFrames) {
                if(actualFramesRead < (int32_t) framesAvailable) {
                    printf("insufficient for no reason, numFrames = %d"
                                   ", actualFramesRead = %d"
                                   ", inputFramesWritten = %d"
                                   ", inputFramesRead = %d"
                                   ", available = %d\n",
                           numFrames,
                           actualFramesRead,
                           (int) inputFramesWritten,
                           (int) inputFramesRead,
                           (int) framesAvailable);
                }
                myData->insufficientReadCount++;
                myData->insufficientReadFrames += numFrames - actualFramesRead; // deficit
                // printf("Error insufficientReadCount = %d\n",(int)myData->insufficientReadCount);
            }

            int32_t numSamples = actualFramesRead * myData->actualInputChannelCount;

            if (myData->actualInputFormat == AAUDIO_FORMAT_PCM_I16) {
                convertPcm16ToFloat(myData->inputShortData, myData->inputFloatData, numSamples);
            }

            // Analyze the data.
            LoopbackProcessor::process_result procResult = myData->loopbackProcessor->process(myData->inputFloatData,
                                               myData->actualInputChannelCount,
                                               outputData,
                                               myData->actualOutputChannelCount,
                                               numFrames);

            if (procResult == LoopbackProcessor::PROCESS_RESULT_GLITCH) {
                if (myData->numGlitchEvents < kMaxGlitchEventsToSave) {
                    myData->glitchFrames[myData->numGlitchEvents++] = myData->audioRecording.size();
                }
            }

            // Save for later.
            myData->audioRecording.write(myData->inputFloatData,
                                         myData->actualInputChannelCount,
                                         actualFramesRead);

            myData->isDone = myData->loopbackProcessor->isDone();
            if (myData->isDone) {
                result = AAUDIO_CALLBACK_RESULT_STOP;
            }
        }
    }
    myData->framesWrittenTotal += numFrames;

    return result;
}

static void MyErrorCallbackProc(
        AAudioStream *stream __unused,
        void *userData __unused,
        aaudio_result_t error) {
    printf("Error Callback, error: %d\n",(int)error);
    LoopbackData *myData = (LoopbackData *) userData;
    myData->outputError = error;
}

static void usage() {
    printf("Usage: aaudio_loopback [OPTION]...\n\n");
    AAudioArgsParser::usage();
    printf("      -B{frames}        input capacity in frames\n");
    printf("      -C{channels}      number of input channels\n");
    printf("      -D{deviceId}      input device ID\n");
    printf("      -F{0,1,2}         input format, 1=I16, 2=FLOAT\n");
    printf("      -g{gain}          recirculating loopback gain\n");
    printf("      -h{hangMillis}    occasionally hang in the callback\n");
    printf("      -P{inPerf}        set input AAUDIO_PERFORMANCE_MODE*\n");
    printf("          n for _NONE\n");
    printf("          l for _LATENCY\n");
    printf("          p for _POWER_SAVING\n");
    printf("      -t{test}          select test mode\n");
    printf("          m for sine magnitude\n");
    printf("          e for echo latency (default)\n");
    printf("          f for file latency, analyzes %s\n\n", FILENAME_ECHOS);
    printf("      -X  use EXCLUSIVE mode for input\n");
    printf("Example:  aaudio_loopback -n2 -pl -Pl -x\n");
}

static aaudio_performance_mode_t parsePerformanceMode(char c) {
    aaudio_performance_mode_t mode = AAUDIO_ERROR_ILLEGAL_ARGUMENT;
    c = tolower(c);
    switch (c) {
        case 'n':
            mode = AAUDIO_PERFORMANCE_MODE_NONE;
            break;
        case 'l':
            mode = AAUDIO_PERFORMANCE_MODE_LOW_LATENCY;
            break;
        case 'p':
            mode = AAUDIO_PERFORMANCE_MODE_POWER_SAVING;
            break;
        default:
            printf("ERROR in value performance mode %c\n", c);
            break;
    }
    return mode;
}

enum {
    TEST_SINE_MAGNITUDE = 0,
    TEST_ECHO_LATENCY,
    TEST_FILE_LATENCY,
};

static int parseTestMode(char c) {
    int testMode = TEST_ECHO_LATENCY;
    c = tolower(c);
    switch (c) {
        case 'm':
            testMode = TEST_SINE_MAGNITUDE;
            break;
        case 'e':
            testMode = TEST_ECHO_LATENCY;
            break;
        case 'f':
            testMode = TEST_FILE_LATENCY;
            break;
        default:
            printf("ERROR in value test mode %c\n", c);
            break;
    }
    return testMode;
}

void printAudioGraphRegion(AudioRecording &recording, int32_t start, int32_t end) {
    if (end >= recording.size()) {
        end = recording.size() - 1;
    }
    float *data = recording.getData();
    // Normalize data so we can see it better.
    float maxSample = 0.01;
    for (int32_t i = start; i < end; i++) {
        float samplePos = fabs(data[i]);
        if (samplePos > maxSample) {
            maxSample = samplePos;
        }
    }
    float gain = 0.98f / maxSample;

    for (int32_t i = start; i < end; i++) {
        float sample = data[i];
        printf("%6d: %7.4f ", i, sample); // actual value
        sample *= gain;
        printAudioScope(sample);
    }
}


// ====================================================================================
// TODO break up this large main() function into smaller functions
int main(int argc, const char **argv)
{

    AAudioArgsParser      argParser;
    AAudioSimplePlayer    player;
    AAudioSimpleRecorder  recorder;
    LoopbackData          loopbackData;
    AAudioStream         *inputStream                = nullptr;
    AAudioStream         *outputStream               = nullptr;

    aaudio_result_t       result = AAUDIO_OK;
    int32_t               requestedInputDeviceId     = AAUDIO_UNSPECIFIED;
    aaudio_sharing_mode_t requestedInputSharingMode  = AAUDIO_SHARING_MODE_SHARED;
    int                   requestedInputChannelCount = kNumInputChannels;
    aaudio_format_t       requestedInputFormat       = AAUDIO_FORMAT_UNSPECIFIED;
    int32_t               requestedInputCapacity     = AAUDIO_UNSPECIFIED;
    aaudio_performance_mode_t inputPerformanceLevel  = AAUDIO_PERFORMANCE_MODE_LOW_LATENCY;

    int32_t               outputFramesPerBurst       = 0;

    aaudio_format_t       actualOutputFormat         = AAUDIO_FORMAT_INVALID;
    int32_t               actualSampleRate           = 0;
    int                   written                    = 0;

    int                   testMode                   = TEST_ECHO_LATENCY;
    double                gain                       = 1.0;
    int                   hangTimeMillis             = 0;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, NULL, _IONBF, (size_t) 0);

    printf("%s - Audio loopback using AAudio V" APP_VERSION "\n", argv[0]);

    // Use LOW_LATENCY as the default to match input default.
    argParser.setPerformanceMode(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (argParser.parseArg(arg)) {
            // Handle options that are not handled by the ArgParser
            if (arg[0] == '-') {
                char option = arg[1];
                switch (option) {
                    case 'B':
                        requestedInputCapacity = atoi(&arg[2]);
                        break;
                    case 'C':
                        requestedInputChannelCount = atoi(&arg[2]);
                        break;
                    case 'D':
                        requestedInputDeviceId = atoi(&arg[2]);
                        break;
                    case 'F':
                        requestedInputFormat = atoi(&arg[2]);
                        break;
                    case 'g':
                        gain = atof(&arg[2]);
                        break;
                    case 'h':
                        // Was there a number after the "-h"?
                        if (arg[2]) {
                            hangTimeMillis = atoi(&arg[2]);
                        } else {
                            // If no number then use the default.
                            hangTimeMillis = kDefaultHangTimeMillis;
                        }
                        break;
                    case 'P':
                        inputPerformanceLevel = parsePerformanceMode(arg[2]);
                        break;
                    case 'X':
                        requestedInputSharingMode = AAUDIO_SHARING_MODE_EXCLUSIVE;
                        break;
                    case 't':
                        testMode = parseTestMode(arg[2]);
                        break;
                    default:
                        usage();
                        exit(EXIT_FAILURE);
                        break;
                }
            } else {
                usage();
                exit(EXIT_FAILURE);
                break;
            }
        }

    }

    if (inputPerformanceLevel < 0) {
        printf("illegal inputPerformanceLevel = %d\n", inputPerformanceLevel);
        exit(EXIT_FAILURE);
    }

    int32_t requestedDuration = argParser.getDurationSeconds();
    int32_t requestedDurationMillis = requestedDuration * kMillisPerSecond;
    int32_t timeMillis = 0;
    int32_t recordingDuration = std::min(60 * 5, requestedDuration);

    int32_t requestedOutputBursts = argParser.getNumberOfBursts();

    switch(testMode) {
        case TEST_SINE_MAGNITUDE:
            loopbackData.loopbackProcessor = &loopbackData.sineAnalyzer;
            break;
        case TEST_ECHO_LATENCY:
            loopbackData.echoAnalyzer.setGain(gain);
            loopbackData.loopbackProcessor = &loopbackData.echoAnalyzer;
            break;
        case TEST_FILE_LATENCY: {
            loopbackData.echoAnalyzer.setGain(gain);

            loopbackData.loopbackProcessor = &loopbackData.echoAnalyzer;
            int read = loopbackData.loopbackProcessor->load(FILENAME_ECHOS);
            printf("main() read %d mono samples from %s on Android device, rate = %d\n",
                   read, FILENAME_ECHOS,
                   loopbackData.loopbackProcessor->getSampleRate());
            loopbackData.loopbackProcessor->report();
            goto report_result;
        }
            break;
        default:
            exit(1);
            break;
    }

    printf("OUTPUT stream ----------------------------------------\n");
    result = player.open(argParser, MyDataCallbackProc, MyErrorCallbackProc, &loopbackData);
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  player.open() returned %d\n", result);
        exit(1);
    }
    outputStream = loopbackData.outputStream = player.getStream();

    actualOutputFormat = AAudioStream_getFormat(outputStream);
    if (actualOutputFormat != AAUDIO_FORMAT_PCM_FLOAT) {
        fprintf(stderr, "ERROR - only AAUDIO_FORMAT_PCM_FLOAT supported\n");
        exit(1);
    }

    actualSampleRate = AAudioStream_getSampleRate(outputStream);
    loopbackData.audioRecording.allocate(recordingDuration * actualSampleRate);
    loopbackData.audioRecording.setSampleRate(actualSampleRate);
    outputFramesPerBurst = AAudioStream_getFramesPerBurst(outputStream);

    argParser.compareWithStream(outputStream);

    printf("INPUT  stream ----------------------------------------\n");
    // Use different parameters for the input.
    argParser.setDeviceId(requestedInputDeviceId);
    argParser.setNumberOfBursts(AAudioParameters::kDefaultNumberOfBursts);
    argParser.setFormat(requestedInputFormat);
    argParser.setPerformanceMode(inputPerformanceLevel);
    argParser.setChannelCount(requestedInputChannelCount);
    argParser.setSharingMode(requestedInputSharingMode);
    if (requestedInputCapacity != AAUDIO_UNSPECIFIED) {
        printf("Warning! If you set input capacity then maybe no FAST track on Legacy path!\n");
    }
    argParser.setBufferCapacity(requestedInputCapacity);

    result = recorder.open(argParser);
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.open() returned %d\n", result);
        goto finish;
    }
    inputStream = loopbackData.inputStream = recorder.getStream();

    {
        int32_t actualCapacity = AAudioStream_getBufferCapacityInFrames(inputStream);
        (void) AAudioStream_setBufferSizeInFrames(inputStream, actualCapacity);

        if (testMode == TEST_SINE_MAGNITUDE
                && requestedOutputBursts == AAUDIO_UNSPECIFIED) {
            result = AAudioStream_setBufferSizeInFrames(outputStream, actualCapacity);
            if (result < 0) {
                fprintf(stderr, "ERROR -  AAudioStream_setBufferSizeInFrames(output) returned %d\n",
                        result);
                goto finish;
            } else {
                printf("Output buffer size set to match input capacity = %d frames!\n", result);
            }
        }

        // If the input stream is too small then we cannot satisfy the output callback.
        if (actualCapacity < 2 * outputFramesPerBurst) {
            fprintf(stderr, "ERROR - input capacity < 2 * outputFramesPerBurst\n");
            goto finish;
        }
    }

    argParser.compareWithStream(inputStream);

    // ------- Setup loopbackData -----------------------------
    loopbackData.actualInputFormat = AAudioStream_getFormat(inputStream);

    loopbackData.actualInputChannelCount = recorder.getChannelCount();
    loopbackData.actualOutputChannelCount = player.getChannelCount();

    // Allocate a buffer for the audio data.
    loopbackData.inputFramesMaximum = 32 * AAudioStream_getFramesPerBurst(inputStream);

    if (loopbackData.actualInputFormat == AAUDIO_FORMAT_PCM_I16) {
        loopbackData.inputShortData = new int16_t[loopbackData.inputFramesMaximum
                                                  * loopbackData.actualInputChannelCount]{};
    }
    loopbackData.inputFloatData = new float[loopbackData.inputFramesMaximum *
                                              loopbackData.actualInputChannelCount]{};

    loopbackData.loopbackProcessor->reset();

    loopbackData.hangTimeMillis = hangTimeMillis;

    // Start OUTPUT first so INPUT does not overflow.
    result = player.start();
    if (result != AAUDIO_OK) {
        goto finish;
    }

    result = recorder.start();
    if (result != AAUDIO_OK) {
        goto finish;
    }

    printf("------- sleep and log while the callback runs --------------\n");
    while (timeMillis <= requestedDurationMillis) {
        if (loopbackData.inputError != AAUDIO_OK) {
            printf("  ERROR on input stream\n");
            break;
        } else if (loopbackData.outputError != AAUDIO_OK) {
                printf("  ERROR on output stream\n");
                break;
        } else if (loopbackData.isDone) {
                printf("  Test says it is DONE!\n");
                break;
        } else {
            // Log a line of stream data.
            printf("%7.3f: ", 0.001 * timeMillis); // display in seconds
            loopbackData.loopbackProcessor->printStatus();
            printf(" insf %3d,", (int) loopbackData.insufficientReadCount);

            int64_t inputFramesWritten = AAudioStream_getFramesWritten(inputStream);
            int64_t inputFramesRead = AAudioStream_getFramesRead(inputStream);
            int64_t outputFramesWritten = AAudioStream_getFramesWritten(outputStream);
            int64_t outputFramesRead = AAudioStream_getFramesRead(outputStream);
            static const int textOffset = strlen("AAUDIO_STREAM_STATE_"); // strip this off
            printf(" | INPUT: wr %7lld - rd %7lld = %5lld, st %8s, oruns %3d",
                   (long long) inputFramesWritten,
                   (long long) inputFramesRead,
                   (long long) (inputFramesWritten - inputFramesRead),
                   &AAudio_convertStreamStateToText(
                           AAudioStream_getState(inputStream))[textOffset],
                   AAudioStream_getXRunCount(inputStream));

            printf(" | OUTPUT: wr %7lld - rd %7lld = %5lld, st %8s, uruns %3d\n",
                   (long long) outputFramesWritten,
                   (long long) outputFramesRead,
                    (long long) (outputFramesWritten - outputFramesRead),
                   &AAudio_convertStreamStateToText(
                           AAudioStream_getState(outputStream))[textOffset],
                   AAudioStream_getXRunCount(outputStream)
            );
        }
        int32_t periodMillis = (timeMillis < 2000) ? kLogPeriodMillis / 4 : kLogPeriodMillis;
        usleep(periodMillis * 1000);
        timeMillis += periodMillis;
    }

    result = player.stop();
    if (result != AAUDIO_OK) {
        printf("ERROR - player.stop() returned %d = %s\n",
               result, AAudio_convertResultToText(result));
        goto finish;
    }

    result = recorder.stop();
    if (result != AAUDIO_OK) {
        printf("ERROR - recorder.stop() returned %d = %s\n",
               result, AAudio_convertResultToText(result));
        goto finish;
    }

    printf("input error = %d = %s\n",
           loopbackData.inputError, AAudio_convertResultToText(loopbackData.inputError));

    written = loopbackData.loopbackProcessor->save(FILENAME_ECHOS);
    if (written > 0) {
        printf("main() wrote %8d mono samples to \"%s\" on Android device\n",
               written, FILENAME_ECHOS);
    }

    written = loopbackData.audioRecording.save(FILENAME_ALL);
    if (written > 0) {
        printf("main() wrote %8d mono samples to \"%s\" on Android device\n",
               written, FILENAME_ALL);
    }

    if (loopbackData.inputError == AAUDIO_OK) {
        if (testMode == TEST_SINE_MAGNITUDE) {
            if (loopbackData.numGlitchEvents > 0) {
                // Graph around the first glitch if there is one.
                const int32_t start = loopbackData.glitchFrames[0] - 8;
                const int32_t end = start + outputFramesPerBurst + 8 + 8;
                printAudioGraphRegion(loopbackData.audioRecording, start, end);
            } else {
                // Or graph the middle of the signal.
                const int32_t start = loopbackData.audioRecording.size() / 2;
                const int32_t end = start + 200;
                printAudioGraphRegion(loopbackData.audioRecording, start, end);
            }
        }

        loopbackData.loopbackProcessor->report();
    }

    {
        int32_t framesRead = AAudioStream_getFramesRead(inputStream);
        int32_t framesWritten = AAudioStream_getFramesWritten(inputStream);
        const int64_t framesAvailable = framesWritten - framesRead;
        printf("Callback Results ---------------------------------------- INPUT\n");
        printf("  input overruns   = %8d\n", AAudioStream_getXRunCount(inputStream));
        printf("  framesWritten    = %8d\n", framesWritten);
        printf("  framesRead       = %8d\n", framesRead);
        printf("  myFramesRead     = %8d\n", (int) loopbackData.framesReadTotal);
        printf("  written - read   = %8d\n", (int) framesAvailable);
        printf("  insufficient #   = %8d\n", (int) loopbackData.insufficientReadCount);
        if (loopbackData.insufficientReadCount > 0) {
            printf("  insuffic. frames = %8d\n", (int) loopbackData.insufficientReadFrames);
        }
        int32_t actualInputCapacity = AAudioStream_getBufferCapacityInFrames(inputStream);
        if (framesAvailable > 2 * actualInputCapacity) {
            printf("  WARNING: written - read > 2*capacity !\n");
        }
    }

    {
        int32_t framesRead = AAudioStream_getFramesRead(outputStream);
        int32_t framesWritten = AAudioStream_getFramesWritten(outputStream);
        printf("Callback Results ---------------------------------------- OUTPUT\n");
        printf("  output underruns = %8d\n", AAudioStream_getXRunCount(outputStream));
        printf("  myFramesWritten  = %8d\n", (int) loopbackData.framesWrittenTotal);
        printf("  framesWritten    = %8d\n", framesWritten);
        printf("  framesRead       = %8d\n", framesRead);
        printf("  min numFrames    = %8d\n", (int) loopbackData.minNumFrames);
        printf("  max numFrames    = %8d\n", (int) loopbackData.maxNumFrames);
    }

    if (loopbackData.insufficientReadCount > 3) {
        printf("ERROR: LOOPBACK PROCESSING FAILED. insufficientReadCount too high\n");
        result = AAUDIO_ERROR_UNAVAILABLE;
    }

finish:
    player.close();
    recorder.close();
    delete[] loopbackData.inputFloatData;
    delete[] loopbackData.inputShortData;

report_result:

    for (int i = 0; i < loopbackData.numGlitchEvents; i++) {
        printf("  glitch at frame %d\n", loopbackData.glitchFrames[i]);
    }

    written = loopbackData.loopbackProcessor->save(FILENAME_PROCESSED);
    if (written > 0) {
        printf("main() wrote %8d processed samples to \"%s\" on Android device\n",
               written, FILENAME_PROCESSED);
    }

    if (loopbackData.loopbackProcessor->getResult() < 0) {
        result = loopbackData.loopbackProcessor->getResult();
    }
    printf(RESULT_TAG "result = %d \n", result); // machine readable
    printf("result is %s\n", AAudio_convertResultToText(result)); // human readable
    if (result != AAUDIO_OK) {
        printf("TEST FAILED\n");
        return EXIT_FAILURE;
    } else {
        printf("TEST PASSED\n");
        return EXIT_SUCCESS;
    }
}
