/*
 * Copyright (C) 2017 The Android Open Source Project
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

// Try to trigger bugs by playing randomly on multiple streams.

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <aaudio/AAudio.h>
#include "AAudioArgsParser.h"
#include "AAudioExampleUtils.h"
#include "AAudioSimplePlayer.h"
#include "SineGenerator.h"

#define DEFAULT_TIMEOUT_NANOS  (1 * NANOS_PER_SECOND)

#define NUM_LOOPS          1000
#define MAX_MICROS_DELAY   (2 * 1000 * 1000)

// TODO Consider adding an input stream.
#define PROB_START   (0.20)
#define PROB_PAUSE   (PROB_START + 0.10)
#define PROB_FLUSH   (PROB_PAUSE + 0.10)
#define PROB_STOP    (PROB_FLUSH + 0.10)
#define PROB_CLOSE   (PROB_STOP + 0.10)
static_assert(PROB_CLOSE < 0.9, "Probability sum too high.");

aaudio_data_callback_result_t AAudioMonkeyDataCallback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames);

void AAudioMonkeyErrorCallbackProc(
        AAudioStream *stream __unused,
        void *userData __unused,
        aaudio_result_t error) {
    printf("Error Callback, error: %d\n",(int)error);
}

// This function is not thread safe. Only use this from a single thread.
double nextRandomDouble() {
    return drand48();
}

class AAudioMonkey : public AAudioSimplePlayer {
public:

    AAudioMonkey(int index, AAudioArgsParser *argParser)
            : mArgParser(argParser)
            , mIndex(index) {}

    aaudio_result_t open() {
        printf("Monkey # %d ---------------------------------------------- OPEN\n", mIndex);
        double offset = mIndex * 50;
        mSine1.setup(440.0, 48000);
        mSine1.setSweep(300.0 + offset, 600.0 + offset, 5.0);
        mSine2.setup(660.0, 48000);
        mSine2.setSweep(350.0 + offset, 900.0 + offset, 7.0);

        aaudio_result_t result = AAudioSimplePlayer::open(*mArgParser,
                                      AAudioMonkeyDataCallback,
                                      AAudioMonkeyErrorCallbackProc,
                                      this);
        if (result != AAUDIO_OK) {
            printf("ERROR -  player.open() returned %d\n", result);
        }

        mArgParser->compareWithStream(getStream());
        return result;
    }

    bool isOpen() {
        return (getStream() != nullptr);

    }
    /**
     *
     * @return true if stream passes tests
     */
    bool validate() {
        if (!isOpen()) return true; // closed is OK

        // update and query stream state
        aaudio_stream_state_t state = AAUDIO_STREAM_STATE_UNKNOWN;
        aaudio_result_t result = AAudioStream_waitForStateChange(getStream(),
            AAUDIO_STREAM_STATE_UNKNOWN, &state, 0);
        if (result != AAUDIO_OK) {
            printf("ERROR - AAudioStream_waitForStateChange returned %d\n", result);
            return false;
        }

        int64_t framesRead = AAudioStream_getFramesRead(getStream());
        int64_t framesWritten = AAudioStream_getFramesWritten(getStream());
        int32_t xRuns = AAudioStream_getXRunCount(getStream());
        // Print status
        printf("%30s, framesWritten = %8lld, framesRead = %8lld, xRuns = %d\n",
               AAudio_convertStreamStateToText(state),
               (unsigned long long) framesWritten,
               (unsigned long long) framesRead,
               xRuns);

        if (framesWritten < framesRead) {
            printf("WARNING - UNDERFLOW - diff = %d !!!!!!!!!!!!\n",
                   (int) (framesWritten - framesRead));
        }
        return true;
    }

    aaudio_result_t invoke() {
        aaudio_result_t result = AAUDIO_OK;
        if (!isOpen()) {
            result = open();
            if (result != AAUDIO_OK) return result;
        }

        if (!validate()) {
            return -1;
        }

        double dice = nextRandomDouble();
        // Select an action based on a weighted probability.
        if (dice < PROB_START) {
            printf("start\n");
            result = AAudioStream_requestStart(getStream());
        } else if (dice < PROB_PAUSE) {
            printf("pause\n");
            result = AAudioStream_requestPause(getStream());
        } else if (dice < PROB_FLUSH) {
            printf("flush\n");
            result = AAudioStream_requestFlush(getStream());
        } else if (dice < PROB_STOP) {
            printf("stop\n");
            result = AAudioStream_requestStop(getStream());
        } else if (dice < PROB_CLOSE) {
            printf("close\n");
            result = close();
        } else {
            printf("do nothing\n");
        }

        if (result == AAUDIO_ERROR_INVALID_STATE) {
            printf("    got AAUDIO_ERROR_INVALID_STATE - expected from a monkey\n");
            result = AAUDIO_OK;
        }
        if (result == AAUDIO_OK && isOpen()) {
            if (!validate()) {
                result = -1;
            }
        }
        return result;
    }

    aaudio_data_callback_result_t renderAudio(
            AAudioStream *stream,
            void *audioData,
            int32_t numFrames) {

        int32_t samplesPerFrame = AAudioStream_getChannelCount(stream);
        // This code only plays on the first one or two channels.
        // TODO Support arbitrary number of channels.
        switch (AAudioStream_getFormat(stream)) {
            case AAUDIO_FORMAT_PCM_I16: {
                int16_t *audioBuffer = (int16_t *) audioData;
                // Render sine waves as shorts to first channel.
                mSine1.render(&audioBuffer[0], samplesPerFrame, numFrames);
                // Render sine waves to second channel if there is one.
                if (samplesPerFrame > 1) {
                    mSine2.render(&audioBuffer[1], samplesPerFrame, numFrames);
                }
            }
                break;
            case AAUDIO_FORMAT_PCM_FLOAT: {
                float *audioBuffer = (float *) audioData;
                // Render sine waves as floats to first channel.
                mSine1.render(&audioBuffer[0], samplesPerFrame, numFrames);
                // Render sine waves to second channel if there is one.
                if (samplesPerFrame > 1) {
                    mSine2.render(&audioBuffer[1], samplesPerFrame, numFrames);
                }
            }
                break;
            default:
                return AAUDIO_CALLBACK_RESULT_STOP;
        }
        return AAUDIO_CALLBACK_RESULT_CONTINUE;
    }

private:
    const AAudioArgsParser  *mArgParser;
    const int                mIndex;
    SineGenerator            mSine1;
    SineGenerator            mSine2;
};

// Callback function that fills the audio output buffer.
aaudio_data_callback_result_t AAudioMonkeyDataCallback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    // should not happen but just in case...
    if (userData == nullptr) {
        printf("ERROR - AAudioMonkeyDataCallback needs userData\n");
        return AAUDIO_CALLBACK_RESULT_STOP;
    }
    AAudioMonkey *monkey = (AAudioMonkey *) userData;
    return monkey->renderAudio(stream, audioData, numFrames);
}


static void usage() {
    AAudioArgsParser::usage();
    printf("      -i{seed}  Initial random seed\n");
    printf("      -t{count} number of monkeys in the Troop\n");
}

int main(int argc, const char **argv) {
    AAudioArgsParser argParser;
    std::vector<AAudioMonkey> monkeys;
    aaudio_result_t result;
    int numMonkeys = 1;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("%s - Monkeys\n", argv[0]);

    long int seed = (long int)getNanoseconds();  // different every time by default

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (argParser.parseArg(arg)) {
            // Handle options that are not handled by the ArgParser
            if (arg[0] == '-') {
                char option = arg[1];
                switch (option) {
                    case 'i':
                        seed = atol(&arg[2]);
                        break;
                    case 't':
                        numMonkeys = atoi(&arg[2]);
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

    srand48(seed);
    printf("seed = %ld, nextRandomDouble() = %f\n", seed, nextRandomDouble());

    for (int m = 0; m < numMonkeys; m++) {
        monkeys.emplace_back(m, &argParser);
    }

    for (int i = 0; i < NUM_LOOPS; i++) {
        // pick a random monkey and invoke it
        double dice = nextRandomDouble();
        int monkeyIndex = floor(dice * numMonkeys);
        printf("----------- Monkey #%d\n", monkeyIndex);
        result = monkeys[monkeyIndex].invoke();
        if (result != AAUDIO_OK) {
            goto error;
        }

        // sleep some random time
        dice = nextRandomDouble();
        dice = dice * dice * dice; // skew towards smaller delays
        int micros = (int) (dice * MAX_MICROS_DELAY);
        usleep(micros);

        // TODO consider making this multi-threaded, one thread per monkey, to catch more bugs
    }

    printf("PASS\n");
    return EXIT_SUCCESS;

error:
    printf("FAIL - AAudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    usleep(1000 * 1000); // give me time to stop the logcat
    return EXIT_FAILURE;
}

