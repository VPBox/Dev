/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_TEST_UTILS_H
#define ANDROID_AUDIO_TEST_UTILS_H

#ifndef LOG_TAG
#define LOG_TAG "test_utils"
#endif

#include <log/log.h>

#include <android-base/macros.h>
#include <audio_utils/sndfile.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

template<typename T, typename U>
struct is_same
{
    static const bool value = false;
};

template<typename T>
struct is_same<T, T>  // partial specialization
{
    static const bool value = true;
};

template<typename T>
static inline T convertValue(double val)
{
    if (is_same<T, int16_t>::value) {
        return floor(val * 32767.0 + 0.5);
    } else if (is_same<T, int32_t>::value) {
        return floor(val * (1UL<<31) + 0.5);
    }
    return val; // assume float or double
}

// Convert a list of integers in CSV format to a Vector of those values.
// Returns the number of elements in the list, or -1 on error.
static inline int parseCSV(const char *string, std::vector<int>& values)
{
    // pass 1: count the number of values and do syntax check
    size_t numValues = 0;
    bool hadDigit = false;
    for (const char *p = string; ; ) {
        switch (*p++) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            hadDigit = true;
            break;
        case '\0':
            if (hadDigit) {
                // pass 2: allocate and initialize vector of values
                values.resize(++numValues);
                values[0] = atoi(p = string);
                for (size_t i = 1; i < numValues; ) {
                    if (*p++ == ',') {
                        values[i++] = atoi(p);
                    }
                }
                return numValues;
            }
            FALLTHROUGH_INTENDED;
        case ',':
            if (hadDigit) {
                hadDigit = false;
                numValues++;
                break;
            }
            FALLTHROUGH_INTENDED;
        default:
            return -1;
        }
    }
}

/* Creates a type-independent audio buffer provider from
 * a buffer base address, size, framesize, and input increment array.
 *
 * No allocation or deallocation of the provided buffer is done.
 */
class TestProvider : public android::AudioBufferProvider {
public:
    TestProvider(void* addr, size_t frames, size_t frameSize,
            const std::vector<int>& inputIncr)
    : mAddr(addr),
      mNumFrames(frames),
      mFrameSize(frameSize),
      mNextFrame(0), mUnrel(0), mInputIncr(inputIncr), mNextIdx(0)
    {
    }

    TestProvider()
    : mAddr(NULL), mNumFrames(0), mFrameSize(0),
      mNextFrame(0), mUnrel(0), mNextIdx(0)
    {
    }

    void setIncr(const std::vector<int>& inputIncr) {
        mInputIncr = inputIncr;
        mNextIdx = 0;
    }

    virtual android::status_t getNextBuffer(Buffer* buffer)
    {
        size_t requestedFrames = buffer->frameCount;
        if (requestedFrames > mNumFrames - mNextFrame) {
            buffer->frameCount = mNumFrames - mNextFrame;
        }
        if (!mInputIncr.empty()) {
            size_t provided = mInputIncr[mNextIdx++];
            ALOGV("getNextBuffer() mValue[%zu]=%zu not %zu",
                    mNextIdx-1, provided, buffer->frameCount);
            if (provided < buffer->frameCount) {
                buffer->frameCount = provided;
            }
            if (mNextIdx >= mInputIncr.size()) {
                mNextIdx = 0;
            }
        }
        ALOGV("getNextBuffer() requested %zu frames out of %zu frames available"
                " and returned %zu frames",
                requestedFrames, mNumFrames - mNextFrame, buffer->frameCount);
        mUnrel = buffer->frameCount;
        if (buffer->frameCount > 0) {
            buffer->raw = (char *)mAddr + mFrameSize * mNextFrame;
            return android::NO_ERROR;
        } else {
            buffer->raw = NULL;
            return android::NOT_ENOUGH_DATA;
        }
    }

    virtual void releaseBuffer(Buffer* buffer)
    {
        if (buffer->frameCount > mUnrel) {
            ALOGE("releaseBuffer() released %zu frames but only %zu available "
                    "to release", buffer->frameCount, mUnrel);
            mNextFrame += mUnrel;
            mUnrel = 0;
        } else {

            ALOGV("releaseBuffer() released %zu frames out of %zu frames available "
                    "to release", buffer->frameCount, mUnrel);
            mNextFrame += buffer->frameCount;
            mUnrel -= buffer->frameCount;
        }
        buffer->frameCount = 0;
        buffer->raw = NULL;
    }

    void reset()
    {
        mNextFrame = 0;
    }

    size_t getNumFrames()
    {
        return mNumFrames;
    }


protected:
    void* mAddr;   // base address
    size_t mNumFrames;   // total frames
    int mFrameSize;      // frame size (# channels * bytes per sample)
    size_t mNextFrame;   // index of next frame to provide
    size_t mUnrel;       // number of frames not yet released
    std::vector<int> mInputIncr; // number of frames provided per call
    size_t mNextIdx;     // index of next entry in mInputIncr to use
};

/* Creates a buffer filled with a sine wave.
 */
template<typename T>
static void createSine(void *vbuffer, size_t frames,
        size_t channels, double sampleRate, double freq)
{
    double tscale = 1. / sampleRate;
    T* buffer = reinterpret_cast<T*>(vbuffer);
    for (size_t i = 0; i < frames; ++i) {
        double t = i * tscale;
        double y = sin(2. * M_PI * freq * t);
        T yt = convertValue<T>(y);

        for (size_t j = 0; j < channels; ++j) {
            buffer[i*channels + j] = yt / T(j + 1);
        }
    }
}

/* Creates a buffer filled with a chirp signal (a sine wave sweep).
 *
 * When creating the Chirp, note that the frequency is the true sinusoidal
 * frequency not the sampling rate.
 *
 * http://en.wikipedia.org/wiki/Chirp
 */
template<typename T>
static void createChirp(void *vbuffer, size_t frames,
        size_t channels, double sampleRate,  double minfreq, double maxfreq)
{
    double tscale = 1. / sampleRate;
    T *buffer = reinterpret_cast<T*>(vbuffer);
    // note the chirp constant k has a divide-by-two.
    double k = (maxfreq - minfreq) / (2. * tscale * frames);
    for (size_t i = 0; i < frames; ++i) {
        double t = i * tscale;
        double y = sin(2. * M_PI * (k * t + minfreq) * t);
        T yt = convertValue<T>(y);

        for (size_t j = 0; j < channels; ++j) {
            buffer[i*channels + j] = yt / T(j + 1);
        }
    }
}

/* This derived class creates a buffer provider of datatype T,
 * consisting of an input signal, e.g. from createChirp().
 * The number of frames can be obtained from the base class
 * TestProvider::getNumFrames().
 */

class SignalProvider : public TestProvider {
public:
    SignalProvider()
    : mSampleRate(0),
      mChannels(0)
    {
    }

    virtual ~SignalProvider()
    {
        free(mAddr);
        mAddr = NULL;
    }

    template <typename T>
    void setChirp(size_t channels, double minfreq, double maxfreq, double sampleRate, double time)
    {
        createBufferByFrames<T>(channels, sampleRate, sampleRate*time);
        createChirp<T>(mAddr, mNumFrames, mChannels, mSampleRate, minfreq, maxfreq);
    }

    template <typename T>
    void setSine(size_t channels,
            double freq, double sampleRate, double time)
    {
        createBufferByFrames<T>(channels, sampleRate, sampleRate*time);
        createSine<T>(mAddr, mNumFrames,  mChannels, mSampleRate, freq);
    }

    template <typename T>
    void setFile(const char *file_in)
    {
        SF_INFO info;
        info.format = 0;
        SNDFILE *sf = sf_open(file_in, SFM_READ, &info);
        if (sf == NULL) {
            perror(file_in);
            return;
        }
        createBufferByFrames<T>(info.channels, info.samplerate, info.frames);
        if (is_same<T, float>::value) {
            (void) sf_readf_float(sf, (float *) mAddr, mNumFrames);
        } else if (is_same<T, short>::value) {
            (void) sf_readf_short(sf, (short *) mAddr, mNumFrames);
        }
        sf_close(sf);
    }

    template <typename T>
    void createBufferByFrames(size_t channels, uint32_t sampleRate, size_t frames)
    {
        mNumFrames = frames;
        mChannels = channels;
        mFrameSize = mChannels * sizeof(T);
        free(mAddr);
        mAddr = malloc(mFrameSize * mNumFrames);
        mSampleRate = sampleRate;
    }

    uint32_t getSampleRate() const {
        return mSampleRate;
    }

    uint32_t getNumChannels() const {
        return mChannels;
    }

protected:
    uint32_t mSampleRate;
    uint32_t mChannels;
};

#endif // ANDROID_AUDIO_TEST_UTILS_H
