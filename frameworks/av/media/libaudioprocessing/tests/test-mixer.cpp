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

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <vector>
#include <audio_utils/primitives.h>
#include <audio_utils/sndfile.h>
#include <media/AudioBufferProvider.h>
#include <media/AudioMixer.h>
#include "test_utils.h"

/* Testing is typically through creation of an output WAV file from several
 * source inputs, to be later analyzed by an audio program such as Audacity.
 *
 * Sine or chirp functions are typically more useful as input to the mixer
 * as they show up as straight lines on a spectrogram if successfully mixed.
 *
 * A sample shell script is provided: mixer_to_wave_tests.sh
 */

using namespace android;

static void usage(const char* name) {
    fprintf(stderr, "Usage: %s [-f] [-m] [-c channels]"
                    " [-s sample-rate] [-o <output-file>] [-a <aux-buffer-file>] [-P csv]"
                    " (<input-file> | <command>)+\n", name);
    fprintf(stderr, "    -f    enable floating point input track by default\n");
    fprintf(stderr, "    -m    enable floating point mixer output\n");
    fprintf(stderr, "    -c    number of mixer output channels\n");
    fprintf(stderr, "    -s    mixer sample-rate\n");
    fprintf(stderr, "    -o    <output-file> WAV file, pcm16 (or float if -m specified)\n");
    fprintf(stderr, "    -a    <aux-buffer-file>\n");
    fprintf(stderr, "    -P    # frames provided per call to resample() in CSV format\n");
    fprintf(stderr, "    <input-file> is a WAV file\n");
    fprintf(stderr, "    <command> can be 'sine:[(i|f),]<channels>,<frequency>,<samplerate>'\n");
    fprintf(stderr, "                     'chirp:[(i|f),]<channels>,<samplerate>'\n");
}

static int writeFile(const char *filename, const void *buffer,
        uint32_t sampleRate, uint32_t channels, size_t frames, bool isBufferFloat) {
    if (filename == NULL) {
        return 0; // ok to pass in NULL filename
    }
    // write output to file.
    SF_INFO info;
    info.frames = 0;
    info.samplerate = sampleRate;
    info.channels = channels;
    info.format = SF_FORMAT_WAV | (isBufferFloat ? SF_FORMAT_FLOAT : SF_FORMAT_PCM_16);
    printf("saving file:%s  channels:%u  samplerate:%u  frames:%zu\n",
            filename, info.channels, info.samplerate, frames);
    SNDFILE *sf = sf_open(filename, SFM_WRITE, &info);
    if (sf == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }
    if (isBufferFloat) {
        (void) sf_writef_float(sf, (float*)buffer, frames);
    } else {
        (void) sf_writef_short(sf, (short*)buffer, frames);
    }
    sf_close(sf);
    return EXIT_SUCCESS;
}

const char *parseFormat(const char *s, bool *useFloat) {
    if (!strncmp(s, "f,", 2)) {
        *useFloat = true;
        return s + 2;
    }
    if (!strncmp(s, "i,", 2)) {
        *useFloat = false;
        return s + 2;
    }
    return s;
}

int main(int argc, char* argv[]) {
    const char* const progname = argv[0];
    bool useInputFloat = false;
    bool useMixerFloat = false;
    bool useRamp = true;
    uint32_t outputSampleRate = 48000;
    uint32_t outputChannels = 2; // stereo for now
    std::vector<int> Pvalues;
    const char* outputFilename = NULL;
    const char* auxFilename = NULL;
    std::vector<int32_t> names;
    std::vector<SignalProvider> providers;
    std::vector<audio_format_t> formats;

    for (int ch; (ch = getopt(argc, argv, "fmc:s:o:a:P:")) != -1;) {
        switch (ch) {
        case 'f':
            useInputFloat = true;
            break;
        case 'm':
            useMixerFloat = true;
            break;
        case 'c':
            outputChannels = atoi(optarg);
            break;
        case 's':
            outputSampleRate = atoi(optarg);
            break;
        case 'o':
            outputFilename = optarg;
            break;
        case 'a':
            auxFilename = optarg;
            break;
        case 'P':
            if (parseCSV(optarg, Pvalues) < 0) {
                fprintf(stderr, "incorrect syntax for -P option\n");
                return EXIT_FAILURE;
            }
            break;
        case '?':
        default:
            usage(progname);
            return EXIT_FAILURE;
        }
    }
    argc -= optind;
    argv += optind;

    if (argc == 0) {
        usage(progname);
        return EXIT_FAILURE;
    }

    size_t outputFrames = 0;

    // create providers for each track
    names.resize(argc);
    providers.resize(argc);
    formats.resize(argc);
    for (int i = 0; i < argc; ++i) {
        static const char chirp[] = "chirp:";
        static const char sine[] = "sine:";
        static const double kSeconds = 1;
        bool useFloat = useInputFloat;

        if (!strncmp(argv[i], chirp, strlen(chirp))) {
            std::vector<int> v;
            const char *s = parseFormat(argv[i] + strlen(chirp), &useFloat);

            parseCSV(s, v);
            if (v.size() == 2) {
                printf("creating chirp(%d %d)\n", v[0], v[1]);
                if (useFloat) {
                    providers[i].setChirp<float>(v[0], 0, v[1]/2, v[1], kSeconds);
                    formats[i] = AUDIO_FORMAT_PCM_FLOAT;
                } else {
                    providers[i].setChirp<int16_t>(v[0], 0, v[1]/2, v[1], kSeconds);
                    formats[i] = AUDIO_FORMAT_PCM_16_BIT;
                }
                providers[i].setIncr(Pvalues);
            } else {
                fprintf(stderr, "malformed input '%s'\n", argv[i]);
            }
        } else if (!strncmp(argv[i], sine, strlen(sine))) {
            std::vector<int> v;
            const char *s = parseFormat(argv[i] + strlen(sine), &useFloat);

            parseCSV(s, v);
            if (v.size() == 3) {
                printf("creating sine(%d %d %d)\n", v[0], v[1], v[2]);
                if (useFloat) {
                    providers[i].setSine<float>(v[0], v[1], v[2], kSeconds);
                    formats[i] = AUDIO_FORMAT_PCM_FLOAT;
                } else {
                    providers[i].setSine<int16_t>(v[0], v[1], v[2], kSeconds);
                    formats[i] = AUDIO_FORMAT_PCM_16_BIT;
                }
                providers[i].setIncr(Pvalues);
            } else {
                fprintf(stderr, "malformed input '%s'\n", argv[i]);
            }
        } else {
            printf("creating filename(%s)\n", argv[i]);
            if (useInputFloat) {
                providers[i].setFile<float>(argv[i]);
                formats[i] = AUDIO_FORMAT_PCM_FLOAT;
            } else {
                providers[i].setFile<short>(argv[i]);
                formats[i] = AUDIO_FORMAT_PCM_16_BIT;
            }
            providers[i].setIncr(Pvalues);
        }
        // calculate the number of output frames
        size_t nframes = (int64_t) providers[i].getNumFrames() * outputSampleRate
                / providers[i].getSampleRate();
        if (i == 0 || outputFrames > nframes) { // choose minimum for outputFrames
            outputFrames = nframes;
        }
    }

    // create the output buffer.
    const size_t outputFrameSize = outputChannels
            * (useMixerFloat ? sizeof(float) : sizeof(int16_t));
    const size_t outputSize = outputFrames * outputFrameSize;
    const audio_channel_mask_t outputChannelMask =
            audio_channel_out_mask_from_count(outputChannels);
    void *outputAddr = NULL;
    (void) posix_memalign(&outputAddr, 32, outputSize);
    memset(outputAddr, 0, outputSize);

    // create the aux buffer, if needed.
    const size_t auxFrameSize = sizeof(int32_t); // Q4.27 always
    const size_t auxSize = outputFrames * auxFrameSize;
    void *auxAddr = NULL;
    if (auxFilename) {
        (void) posix_memalign(&auxAddr, 32, auxSize);
        memset(auxAddr, 0, auxSize);
    }

    // create the mixer.
    const size_t mixerFrameCount = 320; // typical numbers may range from 240 or 960
    AudioMixer *mixer = new AudioMixer(mixerFrameCount, outputSampleRate);
    audio_format_t mixerFormat = useMixerFloat
            ? AUDIO_FORMAT_PCM_FLOAT : AUDIO_FORMAT_PCM_16_BIT;
    float f = AudioMixer::UNITY_GAIN_FLOAT / providers.size(); // normalize volume by # tracks
    static float f0; // zero

    // set up the tracks.
    for (size_t i = 0; i < providers.size(); ++i) {
        //printf("track %d out of %d\n", i, providers.size());
        uint32_t channelMask = audio_channel_out_mask_from_count(providers[i].getNumChannels());
        const int name = i;
        const status_t status = mixer->create(
                name, channelMask, formats[i], AUDIO_SESSION_OUTPUT_MIX);
        LOG_ALWAYS_FATAL_IF(status != OK);
        names[i] = name;
        mixer->setBufferProvider(name, &providers[i]);
        mixer->setParameter(name, AudioMixer::TRACK, AudioMixer::MAIN_BUFFER,
                (void *)outputAddr);
        mixer->setParameter(
                name,
                AudioMixer::TRACK,
                AudioMixer::MIXER_FORMAT,
                (void *)(uintptr_t)mixerFormat);
        mixer->setParameter(
                name,
                AudioMixer::TRACK,
                AudioMixer::FORMAT,
                (void *)(uintptr_t)formats[i]);
        mixer->setParameter(
                name,
                AudioMixer::TRACK,
                AudioMixer::MIXER_CHANNEL_MASK,
                (void *)(uintptr_t)outputChannelMask);
        mixer->setParameter(
                name,
                AudioMixer::TRACK,
                AudioMixer::CHANNEL_MASK,
                (void *)(uintptr_t)channelMask);
        mixer->setParameter(
                name,
                AudioMixer::RESAMPLE,
                AudioMixer::SAMPLE_RATE,
                (void *)(uintptr_t)providers[i].getSampleRate());
        if (useRamp) {
            mixer->setParameter(name, AudioMixer::VOLUME, AudioMixer::VOLUME0, &f0);
            mixer->setParameter(name, AudioMixer::VOLUME, AudioMixer::VOLUME1, &f0);
            mixer->setParameter(name, AudioMixer::RAMP_VOLUME, AudioMixer::VOLUME0, &f);
            mixer->setParameter(name, AudioMixer::RAMP_VOLUME, AudioMixer::VOLUME1, &f);
        } else {
            mixer->setParameter(name, AudioMixer::VOLUME, AudioMixer::VOLUME0, &f);
            mixer->setParameter(name, AudioMixer::VOLUME, AudioMixer::VOLUME1, &f);
        }
        if (auxFilename) {
            mixer->setParameter(name, AudioMixer::TRACK, AudioMixer::AUX_BUFFER,
                    (void *) auxAddr);
            mixer->setParameter(name, AudioMixer::VOLUME, AudioMixer::AUXLEVEL, &f0);
            mixer->setParameter(name, AudioMixer::RAMP_VOLUME, AudioMixer::AUXLEVEL, &f);
        }
        mixer->enable(name);
    }

    // pump the mixer to process data.
    size_t i;
    for (i = 0; i < outputFrames - mixerFrameCount; i += mixerFrameCount) {
        for (size_t j = 0; j < names.size(); ++j) {
            mixer->setParameter(names[j], AudioMixer::TRACK, AudioMixer::MAIN_BUFFER,
                    (char *) outputAddr + i * outputFrameSize);
            if (auxFilename) {
                mixer->setParameter(names[j], AudioMixer::TRACK, AudioMixer::AUX_BUFFER,
                        (char *) auxAddr + i * auxFrameSize);
            }
        }
        mixer->process();
    }
    outputFrames = i; // reset output frames to the data actually produced.

    // write to files
    writeFile(outputFilename, outputAddr,
            outputSampleRate, outputChannels, outputFrames, useMixerFloat);
    if (auxFilename) {
        // Aux buffer is always in q4_27 format for O and earlier.
        // memcpy_to_i16_from_q4_27((int16_t*)auxAddr, (const int32_t*)auxAddr, outputFrames);
        // Aux buffer is always in float format for P.
        memcpy_to_i16_from_float((int16_t*)auxAddr, (const float*)auxAddr, outputFrames);
        writeFile(auxFilename, auxAddr, outputSampleRate, 1, outputFrames, false);
    }

    delete mixer;
    free(outputAddr);
    free(auxAddr);
    return EXIT_SUCCESS;
}
