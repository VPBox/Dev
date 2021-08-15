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

//#define LOG_NDEBUG 0
#define LOG_TAG "audioflinger_resampler_tests"

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <log/log.h>
#include <media/AudioBufferProvider.h>

#include <media/AudioResampler.h>
#include "../AudioResamplerDyn.h"
#include "../AudioResamplerFirGen.h"
#include "test_utils.h"

template <typename T>
static void printData(T *data, size_t size) {
    const size_t stride = 8;
    for (size_t i = 0; i < size; ) {
        for (size_t j = 0; j < stride && i < size; ++j) {
            std::cout << data[i++] << ' ';  // extra space before newline
        }
        std::cout << '\n'; // or endl
    }
}

void resample(int channels, void *output,
        size_t outputFrames, const std::vector<size_t> &outputIncr,
        android::AudioBufferProvider *provider, android::AudioResampler *resampler)
{
    for (size_t i = 0, j = 0; i < outputFrames; ) {
        size_t thisFrames = outputIncr[j++];
        if (j >= outputIncr.size()) {
            j = 0;
        }
        if (thisFrames == 0 || thisFrames > outputFrames - i) {
            thisFrames = outputFrames - i;
        }
        size_t framesResampled = resampler->resample(
                (int32_t*) output + channels*i, thisFrames, provider);
        // we should have enough buffer space, so there is no short count.
        ASSERT_EQ(thisFrames, framesResampled);
        i += thisFrames;
    }
}

void buffercmp(const void *reference, const void *test,
        size_t outputFrameSize, size_t outputFrames)
{
    for (size_t i = 0; i < outputFrames; ++i) {
        int check = memcmp((const char*)reference + i * outputFrameSize,
                (const char*)test + i * outputFrameSize, outputFrameSize);
        if (check) {
            ALOGE("Failure at frame %zu", i);
            ASSERT_EQ(check, 0); /* fails */
        }
    }
}

void testBufferIncrement(size_t channels, bool useFloat,
        unsigned inputFreq, unsigned outputFreq,
        enum android::AudioResampler::src_quality quality)
{
    const audio_format_t format = useFloat ? AUDIO_FORMAT_PCM_FLOAT : AUDIO_FORMAT_PCM_16_BIT;
    // create the provider
    std::vector<int> inputIncr;
    SignalProvider provider;
    if (useFloat) {
        provider.setChirp<float>(channels,
                0., outputFreq/2., outputFreq, outputFreq/2000.);
    } else {
        provider.setChirp<int16_t>(channels,
                0., outputFreq/2., outputFreq, outputFreq/2000.);
    }
    provider.setIncr(inputIncr);

    // calculate the output size
    size_t outputFrames = ((int64_t) provider.getNumFrames() * outputFreq) / inputFreq;
    size_t outputFrameSize = (channels == 1 ? 2 : channels) * (useFloat ? sizeof(float) : sizeof(int32_t));
    size_t outputSize = outputFrameSize * outputFrames;
    outputSize &= ~7;

    // create the resampler
    android::AudioResampler* resampler;

    resampler = android::AudioResampler::create(format, channels, outputFreq, quality);
    resampler->setSampleRate(inputFreq);
    resampler->setVolume(android::AudioResampler::UNITY_GAIN_FLOAT,
            android::AudioResampler::UNITY_GAIN_FLOAT);

    // set up the reference run
    std::vector<size_t> refIncr;
    refIncr.push_back(outputFrames);
    void* reference = calloc(outputFrames, outputFrameSize);
    resample(channels, reference, outputFrames, refIncr, &provider, resampler);

    provider.reset();

#if 0
    /* this test will fail - API interface issue: reset() does not clear internal buffers */
    resampler->reset();
#else
    delete resampler;
    resampler = android::AudioResampler::create(format, channels, outputFreq, quality);
    resampler->setSampleRate(inputFreq);
    resampler->setVolume(android::AudioResampler::UNITY_GAIN_FLOAT,
            android::AudioResampler::UNITY_GAIN_FLOAT);
#endif

    // set up the test run
    std::vector<size_t> outIncr;
    outIncr.push_back(1);
    outIncr.push_back(2);
    outIncr.push_back(3);
    void* test = calloc(outputFrames, outputFrameSize);
    inputIncr.push_back(1);
    inputIncr.push_back(3);
    provider.setIncr(inputIncr);
    resample(channels, test, outputFrames, outIncr, &provider, resampler);

    // check
    buffercmp(reference, test, outputFrameSize, outputFrames);

    free(reference);
    free(test);
    delete resampler;
}

template <typename T>
inline double sqr(T v)
{
    double dv = static_cast<double>(v);
    return dv * dv;
}

template <typename T>
double signalEnergy(T *start, T *end, unsigned stride)
{
    double accum = 0;

    for (T *p = start; p < end; p += stride) {
        accum += sqr(*p);
    }
    unsigned count = (end - start + stride - 1) / stride;
    return accum / count;
}

// TI = resampler input type, int16_t or float
// TO = resampler output type, int32_t or float
template <typename TI, typename TO>
void testStopbandDownconversion(size_t channels,
        unsigned inputFreq, unsigned outputFreq,
        unsigned passband, unsigned stopband,
        enum android::AudioResampler::src_quality quality)
{
    // create the provider
    std::vector<int> inputIncr;
    SignalProvider provider;
    provider.setChirp<TI>(channels,
            0., inputFreq/2., inputFreq, inputFreq/2000.);
    provider.setIncr(inputIncr);

    // calculate the output size
    size_t outputFrames = ((int64_t) provider.getNumFrames() * outputFreq) / inputFreq;
    size_t outputFrameSize = (channels == 1 ? 2 : channels) * sizeof(TO);
    size_t outputSize = outputFrameSize * outputFrames;
    outputSize &= ~7;

    // create the resampler
    android::AudioResampler* resampler;

    resampler = android::AudioResampler::create(
            is_same<TI, int16_t>::value ? AUDIO_FORMAT_PCM_16_BIT : AUDIO_FORMAT_PCM_FLOAT,
            channels, outputFreq, quality);
    resampler->setSampleRate(inputFreq);
    resampler->setVolume(android::AudioResampler::UNITY_GAIN_FLOAT,
            android::AudioResampler::UNITY_GAIN_FLOAT);

    // set up the reference run
    std::vector<size_t> refIncr;
    refIncr.push_back(outputFrames);
    void* reference = calloc(outputFrames, outputFrameSize);
    resample(channels, reference, outputFrames, refIncr, &provider, resampler);

    TO *out = reinterpret_cast<TO *>(reference);

    // check signal energy in passband
    const unsigned passbandFrame = passband * outputFreq / 1000.;
    const unsigned stopbandFrame = stopband * outputFreq / 1000.;

    // check each channel separately
    if (channels == 1) channels = 2; // workaround (mono duplicates output channel)

    for (size_t i = 0; i < channels; ++i) {
        double passbandEnergy = signalEnergy(out, out + passbandFrame * channels, channels);
        double stopbandEnergy = signalEnergy(out + stopbandFrame * channels,
                out + outputFrames * channels, channels);
        double dbAtten = -10. * log10(stopbandEnergy / passbandEnergy);
        ASSERT_GT(dbAtten, 60.);

#if 0
        // internal verification
        printf("if:%d  of:%d  pbf:%d  sbf:%d  sbe: %f  pbe: %f  db: %.2f\n",
                provider.getNumFrames(), outputFrames,
                passbandFrame, stopbandFrame, stopbandEnergy, passbandEnergy, dbAtten);
        for (size_t i = 0; i < 10; ++i) {
            std::cout << out[i+passbandFrame*channels] << std::endl;
        }
        for (size_t i = 0; i < 10; ++i) {
            std::cout << out[i+stopbandFrame*channels] << std::endl;
        }
#endif
    }

    free(reference);
    delete resampler;
}

void testFilterResponse(
        size_t channels, unsigned inputFreq, unsigned outputFreq,
        android::AudioResampler::src_quality quality = android::AudioResampler::DYN_HIGH_QUALITY)
{
    // create resampler
    using ResamplerType = android::AudioResamplerDyn<float, float, float>;
    std::unique_ptr<ResamplerType> rdyn(
            static_cast<ResamplerType *>(
                    android::AudioResampler::create(
                            AUDIO_FORMAT_PCM_FLOAT,
                            channels,
                            outputFreq,
                            quality)));
    rdyn->setSampleRate(inputFreq);

    // get design parameters
    const int phases = rdyn->getPhases();
    const int halfLength = rdyn->getHalfLength();
    const float *coefs = rdyn->getFilterCoefs();
    const double fcr = rdyn->getNormalizedCutoffFrequency();
    const double tbw = rdyn->getNormalizedTransitionBandwidth();
    const double attenuation = rdyn->getFilterAttenuation();
    const double stopbandDb = rdyn->getStopbandAttenuationDb();
    const double passbandDb = rdyn->getPassbandRippleDb();
    const double fp = fcr - tbw * 0.5;
    const double fs = fcr + tbw * 0.5;
    const double idealfs = inputFreq <= outputFreq
        ? 0.5                            // upsample
        : 0.5 * outputFreq  / inputFreq; // downsample

    printf("inputFreq:%d outputFreq:%d design quality %d"
            " phases:%d halfLength:%d"
            " fcr:%lf fp:%lf fs:%lf tbw:%lf fcrp:%lf"
            " attenuation:%lf stopRipple:%.lf passRipple:%lf"
            "\n",
            inputFreq, outputFreq, quality,
            phases, halfLength,
            fcr, fp, fs, tbw, fcr * 100. / idealfs,
            attenuation, stopbandDb, passbandDb);

    // verify design parameters
    constexpr int32_t passSteps = 1000;
    double passMin, passMax, passRipple, stopMax, stopRipple;
    android::testFir(coefs, phases, halfLength, fp / phases, fs / phases,
            passSteps, phases * passSteps /* stopSteps */,
            passMin, passMax, passRipple,
            stopMax, stopRipple);
    printf("inputFreq:%d outputFreq:%d verify"
            " passMin:%lf passMax:%lf passRipple:%lf stopMax:%lf stopRipple:%lf"
            "\n",
            inputFreq, outputFreq,
            passMin, passMax, passRipple, stopMax, stopRipple);

    ASSERT_GT(stopRipple, 60.);  // enough stopband attenuation
    ASSERT_LT(passRipple, 0.2);  // small passband ripple
    ASSERT_GT(passMin, 0.99);    // we do not attenuate the signal (ideally 1.)
}

/* Buffer increment test
 *
 * We compare a reference output, where we consume and process the entire
 * buffer at a time, and a test output, where we provide small chunks of input
 * data and process small chunks of output (which may not be equivalent in size).
 *
 * Two subtests - fixed phase (3:2 down) and interpolated phase (147:320 up)
 */
TEST(audioflinger_resampler, bufferincrement_fixedphase) {
    // all of these work
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            android::AudioResampler::LOW_QUALITY,
            android::AudioResampler::MED_QUALITY,
            android::AudioResampler::HIGH_QUALITY,
            android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testBufferIncrement(2, false, 48000, 32000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, bufferincrement_interpolatedphase) {
    // all of these work except low quality
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
//           android::AudioResampler::LOW_QUALITY,
            android::AudioResampler::MED_QUALITY,
            android::AudioResampler::HIGH_QUALITY,
            android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testBufferIncrement(2, false, 22050, 48000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, bufferincrement_fixedphase_multi) {
    // only dynamic quality
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testBufferIncrement(4, false, 48000, 32000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, bufferincrement_interpolatedphase_multi_float) {
    // only dynamic quality
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testBufferIncrement(8, true, 22050, 48000, kQualityArray[i]);
    }
}

/* Simple aliasing test
 *
 * This checks stopband response of the chirp signal to make sure frequencies
 * are properly suppressed.  It uses downsampling because the stopband can be
 * clearly isolated by input frequencies exceeding the output sample rate (nyquist).
 */
TEST(audioflinger_resampler, stopbandresponse_integer) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                2, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                2, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, stopbandresponse_integer_mono) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                1, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                1, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, stopbandresponse_integer_multichannel) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                8, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<int16_t, int32_t>(
                8, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, stopbandresponse_float) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                2, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                2, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, stopbandresponse_float_mono) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                1, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                1, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

TEST(audioflinger_resampler, stopbandresponse_float_multichannel) {
    // not all of these may work (old resamplers fail on downsampling)
    static const enum android::AudioResampler::src_quality kQualityArray[] = {
            //android::AudioResampler::LOW_QUALITY,
            //android::AudioResampler::MED_QUALITY,
            //android::AudioResampler::HIGH_QUALITY,
            //android::AudioResampler::VERY_HIGH_QUALITY,
            android::AudioResampler::DYN_LOW_QUALITY,
            android::AudioResampler::DYN_MED_QUALITY,
            android::AudioResampler::DYN_HIGH_QUALITY,
    };

    // in this test we assume a maximum transition band between 12kHz and 20kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                8, 48000, 32000, 12000, 20000, kQualityArray[i]);
    }

    // in this test we assume a maximum transition band between 7kHz and 15kHz.
    // there must be at least 60dB relative attenuation between stopband and passband.
    // (the weird ratio triggers interpolative resampling)
    for (size_t i = 0; i < ARRAY_SIZE(kQualityArray); ++i) {
        testStopbandDownconversion<float, float>(
                8, 48000, 22101, 7000, 15000, kQualityArray[i]);
    }
}

// Selected downsampling responses for various frequencies relating to hearing aid.
TEST(audioflinger_resampler, downsamplingresponse) {
    static constexpr android::AudioResampler::src_quality qualities[] = {
        android::AudioResampler::DYN_LOW_QUALITY,
        android::AudioResampler::DYN_MED_QUALITY,
        android::AudioResampler::DYN_HIGH_QUALITY,
    };
    static constexpr int inSampleRates[] = {
        32000,
        44100,
        48000,
    };
    static constexpr int outSampleRates[] = {
        16000,
        24000,
    };

    for (auto quality : qualities) {
        for (int outSampleRate : outSampleRates) {
            for (int inSampleRate : inSampleRates) {
                testFilterResponse(2 /* channels */, inSampleRate, outSampleRate, quality);
            }
        }
    }
}

// General responses for typical output device scenarios - 44.1, 48, 96 kHz
// (48, 96 are part of the same resampler generation family).
TEST(audioflinger_resampler, generalresponse) {
    static constexpr int inSampleRates[] = {
        8000,
        11025,
        12000,
        16000,
        22050,
        24000,
        32000,
        44100,
        48000,
        88200,
        96000,
        176400,
        192000,
    };
    static constexpr int outSampleRates[] = {
        44100,
        48000,
        96000,
    };

    for (int outSampleRate : outSampleRates) {
        for (int inSampleRate : inSampleRates) {
            testFilterResponse(2 /* channels */, inSampleRate, outSampleRate);
        }
    }
}
