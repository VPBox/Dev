/*
 * Copyright (C) 2010 The Android Open Source Project
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

// Play an audio file using buffer queue

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <system/audio.h>
#include <audio_utils/fifo.h>
#include <audio_utils/primitives.h>
#include <audio_utils/sndfile.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

unsigned numBuffers = 2;
int framesPerBuffer = 512;
SNDFILE *sndfile;
SF_INFO sfinfo;
unsigned which; // which buffer to use next
SLboolean eof;  // whether we have hit EOF on input yet
void *buffers;
SLuint32 byteOrder; // desired to use for PCM buffers
SLuint32 nativeByteOrder;   // of platform
audio_format_t transferFormat = AUDIO_FORMAT_DEFAULT;
size_t sfframesize = 0;

static audio_utils_fifo *fifo;
static audio_utils_fifo_reader *fifoReader;
static audio_utils_fifo_writer *fifoWriter;
static unsigned underruns = 0;

static SLuint32 squeeze(void *buffer, SLuint32 nbytes)
{
    if (byteOrder != nativeByteOrder) {
        // FIXME does not work for non 16-bit
        swab(buffer, buffer, nbytes);
    }
    if (transferFormat == AUDIO_FORMAT_PCM_8_BIT) {
        memcpy_to_u8_from_i16((uint8_t *) buffer, (const int16_t *) buffer,
                nbytes / sizeof(int16_t));
        nbytes /= 2;
    } else if (transferFormat == AUDIO_FORMAT_PCM_24_BIT_PACKED) {
        memcpy_to_p24_from_i32((uint8_t *) buffer, (const int32_t *) buffer,
                nbytes / sizeof(int32_t));
        nbytes = nbytes * 3 / 4;
    }
    return nbytes;
}

// This callback is called each time a buffer finishes playing

static void callback(SLBufferQueueItf bufq, void *param)
{
    assert(NULL == param);
    if (!eof) {
        void *buffer = (char *)buffers + framesPerBuffer * sfframesize * which;
        ssize_t count = fifoReader->read(buffer, framesPerBuffer);
        // on underrun from pipe, substitute silence
        if (0 >= count) {
            memset(buffer, 0, framesPerBuffer * sfframesize);
            count = framesPerBuffer;
            ++underruns;
        }
        if (count > 0) {
            SLuint32 nbytes = count * sfframesize;
            nbytes = squeeze(buffer, nbytes);
            SLresult result = (*bufq)->Enqueue(bufq, buffer, nbytes);
            assert(SL_RESULT_SUCCESS == result);
            if (++which >= numBuffers)
                which = 0;
        }
    }
}

// This thread reads from a (slow) filesystem with unpredictable latency and writes to pipe

static void *file_reader_loop(void *arg __unused)
{
#define READ_FRAMES 256
    void *temp = malloc(READ_FRAMES * sfframesize);
    sf_count_t total = 0;
    sf_count_t count;
    for (;;) {
        switch (transferFormat) {
        case AUDIO_FORMAT_PCM_FLOAT:
            count = sf_readf_float(sndfile, (float *) temp, READ_FRAMES);
            break;
        case AUDIO_FORMAT_PCM_32_BIT:
        case AUDIO_FORMAT_PCM_24_BIT_PACKED:
            count = sf_readf_int(sndfile, (int *) temp, READ_FRAMES);
            break;
        case AUDIO_FORMAT_PCM_16_BIT:
        case AUDIO_FORMAT_PCM_8_BIT:
            count = sf_readf_short(sndfile, (short *) temp, READ_FRAMES);
            break;
        default:
            count = 0;
            break;
        }
        if (0 >= count) {
            eof = SL_BOOLEAN_TRUE;
            break;
        }
        const unsigned char *ptr = (unsigned char *) temp;
        while (count > 0) {
            ssize_t actual = fifoWriter->write(ptr, (size_t) count);
            if (actual < 0) {
                break;
            }
            if ((sf_count_t) actual < count) {
                usleep(10000);
            }
            ptr += actual * sfframesize;
            count -= actual;
            total += actual;
        }
        // simulate occasional filesystem latency
        if ((total & 0xFF00) == 0xFF00) {
            usleep(100000);
        }
    }
    free(temp);
    return NULL;
}

// Main program

int main(int argc, char **argv)
{
    // Determine the native byte order (SL_BYTEORDER_NATIVE not available until 1.1)
    union {
        short s;
        char c[2];
    } u;
    u.s = 0x1234;
    if (u.c[0] == 0x34) {
        nativeByteOrder = SL_BYTEORDER_LITTLEENDIAN;
    } else if (u.c[0] == 0x12) {
        nativeByteOrder = SL_BYTEORDER_BIGENDIAN;
    } else {
        fprintf(stderr, "Unable to determine native byte order\n");
        return EXIT_FAILURE;
    }
    byteOrder = nativeByteOrder;

    SLboolean enableReverb = SL_BOOLEAN_FALSE;
    SLboolean enablePlaybackRate = SL_BOOLEAN_FALSE;
    SLpermille initialRate = 0;
    SLpermille finalRate = 0;
    SLpermille deltaRate = 1;
    SLmillisecond deltaRateMs = 0;

    // process command-line options
    int i;
    for (i = 1; i < argc; ++i) {
        char *arg = argv[i];
        if (arg[0] != '-') {
            break;
        }
        if (!strcmp(arg, "-b")) {
            byteOrder = SL_BYTEORDER_BIGENDIAN;
        } else if (!strcmp(arg, "-l")) {
            byteOrder = SL_BYTEORDER_LITTLEENDIAN;
        } else if (!strcmp(arg, "-8")) {
            transferFormat = AUDIO_FORMAT_PCM_8_BIT;
        } else if (!strcmp(arg, "-16")) {
            transferFormat = AUDIO_FORMAT_PCM_16_BIT;
        } else if (!strcmp(arg, "-24")) {
            transferFormat = AUDIO_FORMAT_PCM_24_BIT_PACKED;
        } else if (!strcmp(arg, "-32")) {
            transferFormat = AUDIO_FORMAT_PCM_32_BIT;
        } else if (!strcmp(arg, "-32f")) {
            transferFormat = AUDIO_FORMAT_PCM_FLOAT;
        } else if (!strncmp(arg, "-f", 2)) {
            framesPerBuffer = atoi(&arg[2]);
        } else if (!strncmp(arg, "-n", 2)) {
            numBuffers = atoi(&arg[2]);
        } else if (!strncmp(arg, "-p", 2)) {
            initialRate = atoi(&arg[2]);
            enablePlaybackRate = SL_BOOLEAN_TRUE;
        } else if (!strncmp(arg, "-P", 2)) {
            finalRate = atoi(&arg[2]);
            enablePlaybackRate = SL_BOOLEAN_TRUE;
        } else if (!strncmp(arg, "-q", 2)) {
            deltaRate = atoi(&arg[2]);
            // deltaRate is a magnitude, so take absolute value
            if (deltaRate < 0) {
                deltaRate = -deltaRate;
            }
            enablePlaybackRate = SL_BOOLEAN_TRUE;
        } else if (!strncmp(arg, "-Q", 2)) {
            deltaRateMs = atoi(&arg[2]);
            enablePlaybackRate = SL_BOOLEAN_TRUE;
        } else if (!strcmp(arg, "-r")) {
            enableReverb = SL_BOOLEAN_TRUE;
        } else {
            fprintf(stderr, "option %s ignored\n", arg);
        }
    }

    if (argc - i != 1) {
        fprintf(stderr, "usage: [-b/l] [-8 | | -16 | -24 | -32 | -32f] [-f#] [-n#] [-p#] [-r]"
                " %s filename\n", argv[0]);
        fprintf(stderr, "    -b  force big-endian byte order (default is native byte order)\n");
        fprintf(stderr, "    -l  force little-endian byte order (default is native byte order)\n");
        fprintf(stderr, "    -8  output 8-bits per sample (default is that of input file)\n");
        fprintf(stderr, "    -16 output 16-bits per sample\n");
        fprintf(stderr, "    -24 output 24-bits per sample\n");
        fprintf(stderr, "    -32 output 32-bits per sample\n");
        fprintf(stderr, "    -32f output float 32-bits per sample\n");
        fprintf(stderr, "    -f# frames per buffer (default 512)\n");
        fprintf(stderr, "    -n# number of buffers (default 2)\n");
        fprintf(stderr, "    -p# initial playback rate in per mille (default 1000)\n");
        fprintf(stderr, "    -P# final playback rate in per mille (default same as -p#)\n");
        fprintf(stderr, "    -q# magnitude of playback rate changes in per mille (default 1)\n");
        fprintf(stderr, "    -Q# period between playback rate changes in ms (default 50)\n");
        fprintf(stderr, "    -r  enable reverb (default disabled)\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[i];
    //memset(&sfinfo, 0, sizeof(SF_INFO));
    sfinfo.format = 0;
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (NULL == sndfile) {
        perror(filename);
        return EXIT_FAILURE;
    }

    // verify the file format
    switch (sfinfo.channels) {
    case 1:
    case 2:
        break;
    default:
        fprintf(stderr, "unsupported channel count %d\n", sfinfo.channels);
        goto close_sndfile;
    }

    if (sfinfo.samplerate < 8000 || sfinfo.samplerate > 192000) {
        fprintf(stderr, "unsupported sample rate %d\n", sfinfo.samplerate);
        goto close_sndfile;
    }

    switch (sfinfo.format & SF_FORMAT_TYPEMASK) {
    case SF_FORMAT_WAV:
        break;
    default:
        fprintf(stderr, "unsupported format type 0x%x\n", sfinfo.format & SF_FORMAT_TYPEMASK);
        goto close_sndfile;
    }

    switch (sfinfo.format & SF_FORMAT_SUBMASK) {
    case SF_FORMAT_FLOAT:
        if (transferFormat == AUDIO_FORMAT_DEFAULT) {
            transferFormat = AUDIO_FORMAT_PCM_FLOAT;
        }
        break;
    case SF_FORMAT_PCM_32:
        if (transferFormat == AUDIO_FORMAT_DEFAULT) {
            transferFormat = AUDIO_FORMAT_PCM_32_BIT;
        }
        break;
    case SF_FORMAT_PCM_16:
        if (transferFormat == AUDIO_FORMAT_DEFAULT) {
            transferFormat = AUDIO_FORMAT_PCM_16_BIT;
        }
        break;
    case SF_FORMAT_PCM_U8:
        if (transferFormat == AUDIO_FORMAT_DEFAULT) {
            transferFormat = AUDIO_FORMAT_PCM_8_BIT;
        }
        break;
    case SF_FORMAT_PCM_24:
        if (transferFormat == AUDIO_FORMAT_DEFAULT) {
            transferFormat = AUDIO_FORMAT_PCM_24_BIT_PACKED;
        }
        break;
    default:
        fprintf(stderr, "unsupported sub-format 0x%x\n", sfinfo.format & SF_FORMAT_SUBMASK);
        goto close_sndfile;
    }

    SLuint32 bitsPerSample;
    switch (transferFormat) {
    case AUDIO_FORMAT_PCM_FLOAT:
        bitsPerSample = 32;
        sfframesize = sfinfo.channels * sizeof(float);
        break;
    case AUDIO_FORMAT_PCM_32_BIT:
        bitsPerSample = 32;
        sfframesize = sfinfo.channels * sizeof(int);
        break;
    case AUDIO_FORMAT_PCM_24_BIT_PACKED:
        bitsPerSample = 24;
        sfframesize = sfinfo.channels * sizeof(int); // use int size
        break;
    case AUDIO_FORMAT_PCM_16_BIT:
        bitsPerSample = 16;
        sfframesize = sfinfo.channels * sizeof(short);
        break;
    case AUDIO_FORMAT_PCM_8_BIT:
        bitsPerSample = 8;
        sfframesize = sfinfo.channels * sizeof(short); // use short size
        break;
    default:
        fprintf(stderr, "unsupported transfer format %#x\n", transferFormat);
        goto close_sndfile;
    }

    {
    buffers = malloc(framesPerBuffer * sfframesize * numBuffers);

    // create engine
    SLresult result;
    SLObjectItf engineObject;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    SLEngineItf engineEngine;
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);

    // create output mix
    SLObjectItf outputMixObject;
    SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, enableReverb ? 1 : 0,
            ids, req);
    assert(SL_RESULT_SUCCESS == result);
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // configure environmental reverb on output mix
    SLEnvironmentalReverbItf mixEnvironmentalReverb = NULL;
    if (enableReverb) {
        result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                &mixEnvironmentalReverb);
        assert(SL_RESULT_SUCCESS == result);
        SLEnvironmentalReverbSettings settings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
        result = (*mixEnvironmentalReverb)->SetEnvironmentalReverbProperties(mixEnvironmentalReverb,
                &settings);
        assert(SL_RESULT_SUCCESS == result);
    }

    // configure audio source
    SLDataLocator_BufferQueue loc_bufq;
    loc_bufq.locatorType = SL_DATALOCATOR_BUFFERQUEUE;
    loc_bufq.numBuffers = numBuffers;
    SLAndroidDataFormat_PCM_EX format_pcm;
    format_pcm.formatType = transferFormat == AUDIO_FORMAT_PCM_FLOAT
            ? SL_ANDROID_DATAFORMAT_PCM_EX : SL_DATAFORMAT_PCM;
    format_pcm.numChannels = sfinfo.channels;
    format_pcm.sampleRate = sfinfo.samplerate * 1000;
    format_pcm.bitsPerSample = bitsPerSample;
    format_pcm.containerSize = format_pcm.bitsPerSample;
    format_pcm.channelMask = 1 == format_pcm.numChannels ? SL_SPEAKER_FRONT_CENTER :
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    format_pcm.endianness = byteOrder;
    format_pcm.representation = transferFormat == AUDIO_FORMAT_PCM_FLOAT
            ? SL_ANDROID_PCM_REPRESENTATION_FLOAT : transferFormat == AUDIO_FORMAT_PCM_8_BIT
                    ? SL_ANDROID_PCM_REPRESENTATION_UNSIGNED_INT
                            : SL_ANDROID_PCM_REPRESENTATION_SIGNED_INT;
    SLDataSource audioSrc;
    audioSrc.pLocator = &loc_bufq;
    audioSrc.pFormat = &format_pcm;

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix;
    loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    loc_outmix.outputMix = outputMixObject;
    SLDataSink audioSnk;
    audioSnk.pLocator = &loc_outmix;
    audioSnk.pFormat = NULL;

    // create audio player
    SLInterfaceID ids2[3] = {SL_IID_BUFFERQUEUE, SL_IID_PLAYBACKRATE, SL_IID_EFFECTSEND};
    SLboolean req2[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    SLObjectItf playerObject;
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc,
            &audioSnk, enableReverb ? 3 : (enablePlaybackRate ? 2 : 1), ids2, req2);
    if (SL_RESULT_SUCCESS != result) {
        fprintf(stderr, "can't create audio player\n");
        goto no_player;
    }

    {

    // realize the player
    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the effect send interface and enable effect send reverb for this player
    if (enableReverb) {
        SLEffectSendItf playerEffectSend;
        result = (*playerObject)->GetInterface(playerObject, SL_IID_EFFECTSEND, &playerEffectSend);
        assert(SL_RESULT_SUCCESS == result);
        result = (*playerEffectSend)->EnableEffectSend(playerEffectSend, mixEnvironmentalReverb,
                SL_BOOLEAN_TRUE, (SLmillibel) 0);
        assert(SL_RESULT_SUCCESS == result);
    }

    // get the playback rate interface and configure the rate
    SLPlaybackRateItf playerPlaybackRate;
    SLpermille currentRate = 0;
    if (enablePlaybackRate) {
        result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAYBACKRATE,
                &playerPlaybackRate);
        assert(SL_RESULT_SUCCESS == result);
        SLpermille defaultRate;
        result = (*playerPlaybackRate)->GetRate(playerPlaybackRate, &defaultRate);
        assert(SL_RESULT_SUCCESS == result);
        SLuint32 defaultProperties;
        result = (*playerPlaybackRate)->GetProperties(playerPlaybackRate, &defaultProperties);
        assert(SL_RESULT_SUCCESS == result);
        printf("default playback rate %d per mille, properties 0x%x\n", defaultRate,
                defaultProperties);
        if (initialRate <= 0) {
            initialRate = defaultRate;
        }
        if (finalRate <= 0) {
            finalRate = initialRate;
        }
        currentRate = defaultRate;
        if (finalRate == initialRate) {
            deltaRate = 0;
        } else if (finalRate < initialRate) {
            deltaRate = -deltaRate;
        }
        if (initialRate != defaultRate) {
            result = (*playerPlaybackRate)->SetRate(playerPlaybackRate, initialRate);
            if (SL_RESULT_FEATURE_UNSUPPORTED == result) {
                fprintf(stderr, "initial playback rate %d is unsupported\n", initialRate);
                deltaRate = 0;
            } else if (SL_RESULT_PARAMETER_INVALID == result) {
                fprintf(stderr, "initial playback rate %d is invalid\n", initialRate);
                deltaRate = 0;
            } else {
                assert(SL_RESULT_SUCCESS == result);
                currentRate = initialRate;
            }
        }
    }

    // get the play interface
    SLPlayItf playerPlay;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    assert(SL_RESULT_SUCCESS == result);

    // get the buffer queue interface
    SLBufferQueueItf playerBufferQueue;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE,
            &playerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);

    // loop until EOF or no more buffers
    for (which = 0; which < numBuffers; ++which) {
        void *buffer = (char *)buffers + framesPerBuffer * sfframesize * which;
        sf_count_t frames = framesPerBuffer;
        sf_count_t count;
        switch (transferFormat) {
        case AUDIO_FORMAT_PCM_FLOAT:
            count = sf_readf_float(sndfile, (float *) buffer, frames);
            break;
        case AUDIO_FORMAT_PCM_32_BIT:
            count = sf_readf_int(sndfile, (int *) buffer, frames);
            break;
        case AUDIO_FORMAT_PCM_24_BIT_PACKED:
            count = sf_readf_int(sndfile, (int *) buffer, frames);
            break;
        case AUDIO_FORMAT_PCM_16_BIT:
        case AUDIO_FORMAT_PCM_8_BIT:
            count = sf_readf_short(sndfile, (short *) buffer, frames);
            break;
        default:
            count = 0;
            break;
        }
        if (0 >= count) {
            eof = SL_BOOLEAN_TRUE;
            break;
        }

        // enqueue a buffer
        SLuint32 nbytes = count * sfframesize;
        nbytes = squeeze(buffer, nbytes);
        result = (*playerBufferQueue)->Enqueue(playerBufferQueue, buffer, nbytes);
        assert(SL_RESULT_SUCCESS == result);
    }
    if (which >= numBuffers) {
        which = 0;
    }

    // register a callback on the buffer queue
    result = (*playerBufferQueue)->RegisterCallback(playerBufferQueue, callback, NULL);
    assert(SL_RESULT_SUCCESS == result);

#define FIFO_FRAMES 16384
    void *fifoBuffer = malloc(FIFO_FRAMES * sfframesize);
    fifo = new audio_utils_fifo(FIFO_FRAMES, sfframesize, fifoBuffer);
    fifoReader = new audio_utils_fifo_reader(*fifo, true /*throttlesWriter*/);
    fifoWriter = new audio_utils_fifo_writer(*fifo);

    // create thread to read from file
    pthread_t thread;
    int ok = pthread_create(&thread, (const pthread_attr_t *) NULL, file_reader_loop, NULL);
    assert(0 == ok);

    // give thread a head start so that the pipe is initially filled
    sleep(1);

    // set the player's state to playing
    result = (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);

    // get the initial time
    struct timespec prevTs;
    clock_gettime(CLOCK_MONOTONIC, &prevTs);
    long elapsedNs = 0;
    long deltaRateNs = deltaRateMs * 1000000;

    // wait until the buffer queue is empty
    SLBufferQueueState bufqstate;
    for (;;) {
        result = (*playerBufferQueue)->GetState(playerBufferQueue, &bufqstate);
        assert(SL_RESULT_SUCCESS == result);
        if (0 >= bufqstate.count) {
            break;
        }
        if (!enablePlaybackRate || deltaRate == 0) {
            sleep(1);
        } else {
            struct timespec curTs;
            clock_gettime(CLOCK_MONOTONIC, &curTs);
            elapsedNs += (curTs.tv_sec - prevTs.tv_sec) * 1000000000 +
                    // this term can be negative
                    (curTs.tv_nsec - prevTs.tv_nsec);
            prevTs = curTs;
            if (elapsedNs < deltaRateNs) {
                usleep((deltaRateNs - elapsedNs) / 1000);
                continue;
            }
            elapsedNs -= deltaRateNs;
            SLpermille nextRate = currentRate + deltaRate;
            result = (*playerPlaybackRate)->SetRate(playerPlaybackRate, nextRate);
            if (SL_RESULT_SUCCESS != result) {
                fprintf(stderr, "next playback rate %d is unsupported\n", nextRate);
            } else if (SL_RESULT_PARAMETER_INVALID == result) {
                fprintf(stderr, "next playback rate %d is invalid\n", nextRate);
            } else {
                assert(SL_RESULT_SUCCESS == result);
            }
            currentRate = nextRate;
            if (currentRate >= max(initialRate, finalRate)) {
                currentRate = max(initialRate, finalRate);
                deltaRate = -abs(deltaRate);
            } else if (currentRate <= min(initialRate, finalRate)) {
                currentRate = min(initialRate, finalRate);
                deltaRate = abs(deltaRate);
            }
        }

    }

    // wait for reader thread to exit
    ok = pthread_join(thread, (void **) NULL);
    assert(0 == ok);

    // set the player's state to stopped
    result = (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
    assert(SL_RESULT_SUCCESS == result);

    // destroy audio player
    (*playerObject)->Destroy(playerObject);

    delete fifoWriter;
    fifoWriter = NULL;
    delete fifoReader;
    fifoReader = NULL;
    delete fifo;
    fifo = NULL;
    free(fifoBuffer);
    fifoBuffer = NULL;

    }

no_player:

    // destroy output mix
    (*outputMixObject)->Destroy(outputMixObject);

    // destroy engine
    (*engineObject)->Destroy(engineObject);

    }

close_sndfile:

    (void) sf_close(sndfile);

    return EXIT_SUCCESS;
}
