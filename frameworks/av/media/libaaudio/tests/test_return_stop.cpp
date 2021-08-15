/*
 * Copyright (C) 2018 The Android Open Source Project
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

/**
 * Return stop from the callback.
 * Expect the callback to cease.
 * Check the logcat for bad behavior.
 */

#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <aaudio/AAudio.h>

#define DEFAULT_TIMEOUT_NANOS  ((int64_t)1000000000)
#define STOP_AT_MSEC          1000
#define LOOP_DURATION_MSEC    4000
#define SLEEP_DURATION_MSEC    200

static void s_myErrorCallbackProc(
        AAudioStream *stream,
        void *userData,
        aaudio_result_t error);

struct AudioEngine {
    AAudioStreamBuilder *builder = nullptr;
    AAudioStream        *stream = nullptr;
    std::thread         *thread = nullptr;
    int32_t              stopAtFrame = 0;
    bool                 stopped = false;
    // These counters are read and written by the callback and the main thread.
    std::atomic<int32_t> framesRead{};
    std::atomic<int32_t> startingFramesRead{};
    std::atomic<int32_t> framesCalled{};
    std::atomic<int32_t> callbackCount{};
    std::atomic<int32_t> callbackCountAfterStop{};

    void reset() {
        framesRead.store(0);
        startingFramesRead.store(0);
        framesCalled.store(0);
        callbackCount.store(0);
        callbackCountAfterStop.store(0);
        stopped = false;
    }
};

// Callback function that fills the audio output buffer.
static aaudio_data_callback_result_t s_myDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) audioData;
    (void) numFrames;
    AudioEngine *engine = (struct AudioEngine *)userData;
    engine->callbackCount++;
    if (engine->stopped) {
        engine->callbackCountAfterStop++;
    }

    engine->framesRead = (int32_t)AAudioStream_getFramesRead(stream);
    if (engine->startingFramesRead == 0) {
        engine->startingFramesRead.store(engine->framesRead.load());
    }
    engine->framesCalled += numFrames;
    if (engine->framesCalled >= engine->stopAtFrame) {
        engine->stopped = true;
        return AAUDIO_CALLBACK_RESULT_STOP;
    } else {
        return AAUDIO_CALLBACK_RESULT_CONTINUE;
    }
}

static aaudio_result_t s_OpenAudioStream(struct AudioEngine *engine,
                                         aaudio_direction_t direction,
                                         aaudio_sharing_mode_t sharingMode,
                                         aaudio_performance_mode_t perfMode) {
    // Use an AAudioStreamBuilder to contain requested parameters.
    aaudio_result_t result = AAudio_createStreamBuilder(&engine->builder);
    if (result != AAUDIO_OK) {
        printf("AAudio_createStreamBuilder returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    // Request stream properties.
    AAudioStreamBuilder_setFormat(engine->builder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setPerformanceMode(engine->builder, perfMode);
    AAudioStreamBuilder_setSharingMode(engine->builder, sharingMode);
    AAudioStreamBuilder_setDirection(engine->builder, direction);
    AAudioStreamBuilder_setDataCallback(engine->builder, s_myDataCallbackProc, engine);
    AAudioStreamBuilder_setErrorCallback(engine->builder, s_myErrorCallbackProc, engine);

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(engine->builder, &engine->stream);
    if (result != AAUDIO_OK) {
        printf("AAudioStreamBuilder_openStream returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    return result;
}

static aaudio_result_t s_CloseAudioStream(struct AudioEngine *engine) {
    aaudio_result_t result = AAUDIO_OK;
    if (engine->stream != nullptr) {
        result = AAudioStream_close(engine->stream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_close returned %s\n",
                   AAudio_convertResultToText(result));
        }
        engine->stream = nullptr;
    }
    AAudioStreamBuilder_delete(engine->builder);
    engine->builder = nullptr;
    return result;
}

static void s_myErrorCallbackProc(
        AAudioStream *stream __unused,
        void *userData __unused,
        aaudio_result_t error) {
    printf("%s() - error = %d\n", __func__, error);
}

static void s_usage() {
    printf("test_return_stop [-i] [-x] [-n] [-c]\n");
    printf("     -i direction INPUT, otherwise OUTPUT\n");
    printf("     -x sharing mode EXCLUSIVE, otherwise SHARED\n");
    printf("     -n performance mode NONE, otherwise LOW_LATENCY\n");
    printf("     -c always return CONTINUE from callback, not STOP\n");
}

/**
 * @return 0 is OK, -1 for error
 */
static int s_checkEnginePositions(AudioEngine *engine) {
    const int64_t framesRead = AAudioStream_getFramesRead(engine->stream);
    const int64_t framesWritten = AAudioStream_getFramesWritten(engine->stream);
    const int32_t delta = (int32_t)(framesWritten - framesRead);
    printf("playing framesRead = %7d, framesWritten = %7d"
           ", delta = %4d, framesCalled = %6d, callbackCount = %4d\n",
           (int32_t) framesRead,
           (int32_t) framesWritten,
           delta,
           engine->framesCalled.load(),
           engine->callbackCount.load()
    );
    if (delta > AAudioStream_getBufferCapacityInFrames(engine->stream)) {
        printf("ERROR - delta > capacity\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct AudioEngine engine;
    aaudio_sharing_mode_t sharingMode = AAUDIO_SHARING_MODE_SHARED;
    aaudio_performance_mode_t perfMode = AAUDIO_PERFORMANCE_MODE_LOW_LATENCY;
    aaudio_direction_t direction = AAUDIO_DIRECTION_OUTPUT;
    aaudio_result_t result = AAUDIO_OK;
    bool alwaysContinue = false;
    int errorCount = 0;
    int callbackResult = EXIT_SUCCESS;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("Test Return Stop V1.0\n");
    printf("Wait for a few seconds.\n");
    printf("You should see callbackCount and framesRead stop advancing\n");
    printf("when callbackCount reaches %d msec\n", STOP_AT_MSEC);
    printf("\n");

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (arg[0] == '-') {
            char option = arg[1];
            switch (option) {
                case 'c':
                    alwaysContinue = true;
                    break;
                case 'i':
                    direction = AAUDIO_DIRECTION_INPUT;
                    break;
                case 'n':
                    perfMode = AAUDIO_PERFORMANCE_MODE_NONE;
                    break;
                case 'x':
                    sharingMode = AAUDIO_SHARING_MODE_EXCLUSIVE;
                    break;
                default:
                    s_usage();
                    exit(EXIT_FAILURE);
                    break;
            }
        } else {
            s_usage();
            exit(EXIT_FAILURE);
            break;
        }
    }

    result = s_OpenAudioStream(&engine, direction, sharingMode, perfMode);
    if (result != AAUDIO_OK) {
        printf("s_OpenAudioStream returned %s\n",
               AAudio_convertResultToText(result));
        errorCount++;
    }

    int32_t framesPerBurst = AAudioStream_getFramesPerBurst(engine.stream);
    // Use double buffered stream.
    const int32_t bufferSize = AAudioStream_setBufferSizeInFrames(engine.stream, 2 * framesPerBurst);
    if (bufferSize < 0) {
        printf("AAudioStream_setBufferSizeInFrames returned %s\n",
               AAudio_convertResultToText(bufferSize));
        errorCount++;
    }

    // Check to see what kind of stream we actually got.
    int32_t deviceId = AAudioStream_getDeviceId(engine.stream);
    aaudio_performance_mode_t actualPerfMode = AAudioStream_getPerformanceMode(engine.stream);
    printf("-------- opened: deviceId = %3d, framesPerBurst = %3d, perfMode = %d\n",
           deviceId, framesPerBurst, actualPerfMode);

    // Calculate how many callbacks needed.
    if (alwaysContinue) {
        engine.stopAtFrame = INT32_MAX;
    } else {
        int32_t sampleRate = AAudioStream_getSampleRate(engine.stream);
        engine.stopAtFrame = STOP_AT_MSEC * sampleRate / 1000;
    }

    for (int loops = 0; loops < 2 && result == AAUDIO_OK; loops++) {
        engine.reset();

        // Start stream.
        result = AAudioStream_requestStart(engine.stream);
        printf("AAudioStream_requestStart() returned %d >>>>>>>>>>>>>>>>>>>>>>\n", result);
        if (result != AAUDIO_OK) {
            errorCount++;
            break;
        }

        if (result == AAUDIO_OK) {
            const int watchLoops = LOOP_DURATION_MSEC / SLEEP_DURATION_MSEC;
            for (int i = watchLoops; i > 0; i--) {
                errorCount += s_checkEnginePositions(&engine) ? 1 : 0;
                usleep(SLEEP_DURATION_MSEC * 1000);
            }
        }

        if (engine.stopAtFrame != INT32_MAX) {
            callbackResult = (engine.callbackCountAfterStop == 0) ? EXIT_SUCCESS
                                                                  : EXIT_FAILURE;
            if (callbackResult) {
                printf("ERROR - Callback count after STOP = %d\n",
                       engine.callbackCountAfterStop.load());
                errorCount++;
            }
        }

        if (engine.startingFramesRead.load() == engine.framesRead.load()) {
            printf("ERROR - framesRead did not advance across callbacks\n");
            errorCount++;
        }

        result = AAudioStream_requestStop(engine.stream);
        printf("AAudioStream_requestStop() returned %d <<<<<<<<<<<<<<<<<<<<<\n", result);
        if (result != AAUDIO_OK) {
            errorCount++;
        }
        usleep(SLEEP_DURATION_MSEC * 1000);
        errorCount += s_checkEnginePositions(&engine) ? 1 : 0;
    }

    s_CloseAudioStream(&engine);

    printf("aaudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    printf("test %s\n", errorCount ? "FAILED" : "PASSED");

    return errorCount ? EXIT_FAILURE : EXIT_SUCCESS;
}
