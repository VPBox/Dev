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

// Record input using AAudio and display the peak amplitudes.

#ifndef AAUDIO_SIMPLE_RECORDER_H
#define AAUDIO_SIMPLE_RECORDER_H

#include <aaudio/AAudio.h>
#include "AAudioArgsParser.h"

//#define SHARING_MODE  AAUDIO_SHARING_MODE_EXCLUSIVE
#define SHARING_MODE  AAUDIO_SHARING_MODE_SHARED
#define PERFORMANCE_MODE AAUDIO_PERFORMANCE_MODE_NONE

/**
 * Simple wrapper for AAudio that opens an input stream either in callback or blocking read mode.
 */
class AAudioSimpleRecorder {
public:
    AAudioSimpleRecorder() {}
    ~AAudioSimpleRecorder() {
        close();
    };

    /**
     * Call this before calling open().
     * @param requestedSharingMode
     */
    void setSharingMode(aaudio_sharing_mode_t requestedSharingMode) {
        mRequestedSharingMode = requestedSharingMode;
    }

    /**
     * Call this before calling open().
     * @param requestedPerformanceMode
     */
    void setPerformanceMode(aaudio_performance_mode_t requestedPerformanceMode) {
        mRequestedPerformanceMode = requestedPerformanceMode;
    }

    /**
     * Also known as "sample rate"
     * Only call this after open() has been called.
     */
    int32_t getFramesPerSecond() const {
        return getSampleRate(); // alias
    }

    /**
     * Only call this after open() has been called.
     */
    int32_t getSampleRate() const {
        if (mStream == nullptr) {
            return AAUDIO_ERROR_INVALID_STATE;
        }
        return AAudioStream_getSampleRate(mStream);
    }

    /**
     * Only call this after open() has been called.
     */
    int32_t getChannelCount() {
        if (mStream == nullptr) {
            return AAUDIO_ERROR_INVALID_STATE;
        }
        return AAudioStream_getChannelCount(mStream);;
    }

    /**
     * @deprecated use getChannelCount()
     */
    int32_t getSamplesPerFrame() {
        return getChannelCount();
    }

    /**
     * Only call this after open() has been called.
     */
    int64_t getFramesRead() {
        if (mStream == nullptr) {
            return AAUDIO_ERROR_INVALID_STATE;
        }
        return AAudioStream_getFramesRead(mStream);
    }

    aaudio_result_t open(const AAudioParameters &parameters,
                         AAudioStream_dataCallback dataCallback = nullptr,
                         AAudioStream_errorCallback errorCallback = nullptr,
                         void *userContext = nullptr) {
        aaudio_result_t result = AAUDIO_OK;

        // Use an AAudioStreamBuilder to contain requested parameters.
        AAudioStreamBuilder *builder = nullptr;
        result = AAudio_createStreamBuilder(&builder);
        if (result != AAUDIO_OK) return result;

        parameters.applyParameters(builder); // apply args

        AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);

        if (dataCallback != nullptr) {
            AAudioStreamBuilder_setDataCallback(builder, dataCallback, userContext);
        }
        if (errorCallback != nullptr) {
            AAudioStreamBuilder_setErrorCallback(builder, errorCallback, userContext);
        }

        // Open an AAudioStream using the Builder.
        result = AAudioStreamBuilder_openStream(builder, &mStream);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStreamBuilder_openStream() returned %d %s\n",
                    result, AAudio_convertResultToText(result));
        }

        if (result == AAUDIO_OK) {
            int32_t sizeInBursts = parameters.getNumberOfBursts();
            if (sizeInBursts > 0) {
                int32_t framesPerBurst = AAudioStream_getFramesPerBurst(mStream);
                AAudioStream_setBufferSizeInFrames(mStream, sizeInBursts * framesPerBurst);
            }
        }

        AAudioStreamBuilder_delete(builder);
        return result;
    }

    /**
     * Open a stream
     */
    aaudio_result_t open(int channelCount, int sampSampleRate, aaudio_format_t format,
                         AAudioStream_dataCallback dataProc,
                         AAudioStream_errorCallback errorProc,
                         void *userContext) {
        aaudio_result_t result = AAUDIO_OK;

        // Use an AAudioStreamBuilder to contain requested parameters.
        AAudioStreamBuilder *builder = nullptr;
        result = AAudio_createStreamBuilder(&builder);
        if (result != AAUDIO_OK) return result;

        AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);
        AAudioStreamBuilder_setPerformanceMode(builder, mRequestedPerformanceMode);
        AAudioStreamBuilder_setSharingMode(builder, mRequestedSharingMode);
        if (dataProc != nullptr) {
            AAudioStreamBuilder_setDataCallback(builder, dataProc, userContext);
        }
        if (errorProc != nullptr) {
            AAudioStreamBuilder_setErrorCallback(builder, errorProc, userContext);
        }
        AAudioStreamBuilder_setChannelCount(builder, channelCount);
        AAudioStreamBuilder_setSampleRate(builder, sampSampleRate);
        AAudioStreamBuilder_setFormat(builder, format);

        // Open an AAudioStream using the Builder.
        result = AAudioStreamBuilder_openStream(builder, &mStream);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStreamBuilder_openStream() returned %d %s\n",
                    result, AAudio_convertResultToText(result));
        }

        AAudioStreamBuilder_delete(builder);
        return result;
    }

    aaudio_result_t close() {
        if (mStream != nullptr) {
            AAudioStream_close(mStream);
            mStream = nullptr;
        }
        return AAUDIO_OK;
    }

    // Write zero data to fill up the buffer and prevent underruns.
    aaudio_result_t prime() {
        int32_t samplesPerFrame = AAudioStream_getChannelCount(mStream);
        const int numFrames = 32; // arbitrary
        float zeros[numFrames * samplesPerFrame];
        memset(zeros, 0, sizeof(zeros));
        aaudio_result_t result = numFrames;
        while (result == numFrames) {
            result = AAudioStream_write(mStream, zeros, numFrames, 0);
        }
        return result;
    }

    // Start the stream. AAudio will start calling your callback function.
     aaudio_result_t start() {
        aaudio_result_t result = AAudioStream_requestStart(mStream);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStream_requestStart(input) returned %d %s\n",
                    result, AAudio_convertResultToText(result));
            fprintf(stderr, "        Did you remember to enter:   adb root    ????\n");

        }
        return result;
    }

    // Stop the stream. AAudio will stop calling your callback function.
    aaudio_result_t stop() {
        aaudio_result_t result = AAudioStream_requestStop(mStream);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStream_requestStop(input) returned %d %s\n",
                    result, AAudio_convertResultToText(result));

        }
        return result;
    }

    // Pause the stream. AAudio will stop calling your callback function.
    aaudio_result_t pause() {
        aaudio_result_t result = AAudioStream_requestPause(mStream);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStream_requestPause(input) returned %d %s\n",
                    result, AAudio_convertResultToText(result));
        }
        return result;
    }

    AAudioStream *getStream() const {
        return mStream;
    }

private:
    AAudioStream             *mStream = nullptr;
    aaudio_sharing_mode_t     mRequestedSharingMode = SHARING_MODE;
    aaudio_performance_mode_t mRequestedPerformanceMode = PERFORMANCE_MODE;
};

// Application data that gets passed to the callback.
typedef struct PeakTrackerData {
    float peakLevel;
} PeakTrackerData_t;

#define DECAY_FACTOR   0.999

// Callback function that fills the audio output buffer.
aaudio_data_callback_result_t SimpleRecorderDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
        ) {

    // should not happen but just in case...
    if (userData == nullptr) {
        fprintf(stderr, "ERROR - SimpleRecorderDataCallbackProc needs userData\n");
        return AAUDIO_CALLBACK_RESULT_STOP;
    }
    PeakTrackerData_t *data = (PeakTrackerData_t *) userData;
    // printf("MyCallbackProc(): frameCount = %d\n", numFrames);
    int32_t samplesPerFrame = AAudioStream_getChannelCount(stream);
    float sample;
    // This code assume mono or stereo.
    switch (AAudioStream_getFormat(stream)) {
        case AAUDIO_FORMAT_PCM_I16: {
            int16_t *audioBuffer = (int16_t *) audioData;
            // Peak follower
            for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
                sample = audioBuffer[frameIndex * samplesPerFrame] * (1.0/32768);
                data->peakLevel *= DECAY_FACTOR;
                if (sample > data->peakLevel) {
                    data->peakLevel = sample;
                }
            }
        }
        break;
        case AAUDIO_FORMAT_PCM_FLOAT: {
            float *audioBuffer = (float *) audioData;
            // Peak follower
            for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
                sample = audioBuffer[frameIndex * samplesPerFrame];
                data->peakLevel *= DECAY_FACTOR;
                if (sample > data->peakLevel) {
                    data->peakLevel = sample;
                }
            }
        }
        break;
        default:
            return AAUDIO_CALLBACK_RESULT_STOP;
    }

    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void SimpleRecorderErrorCallbackProc(
        AAudioStream *stream __unused,
        void *userData __unused,
        aaudio_result_t error)
{
    printf("Error Callback, error: %d\n",(int)error);
}

#endif //AAUDIO_SIMPLE_RECORDER_H
