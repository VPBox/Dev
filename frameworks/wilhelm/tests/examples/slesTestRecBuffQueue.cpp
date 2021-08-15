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

/* Audio Record Test

First run the program from shell:
  % adb shell slesTest_recBuffQueue /sdcard/myrec.wav

These use adb on host to retrive the file:
  % adb pull /sdcard/myrec.wav

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <system/audio.h>
#include <audio_utils/primitives.h>
#include <audio_utils/sndfile.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

audio_format_t transferFormat = AUDIO_FORMAT_DEFAULT;
uint32_t sampleRate = 48000;
int channelCount = 1;
bool useIndexChannelMask = false;
size_t frameSize;
uint32_t performanceMode = SL_ANDROID_PERFORMANCE_LATENCY;
bool aec = false;
bool agc = false;
bool ns = false;

/* Preset number to use for recording */
SLuint32 presetValue = SL_ANDROID_RECORDING_PRESET_NONE;

/* Explicitly requesting SL_IID_ANDROIDSIMPLEBUFFERQUEUE and SL_IID_ANDROIDCONFIGURATION
 * on the AudioRecorder object */
#define NUM_EXPLICIT_INTERFACES_FOR_RECORDER 2

/* Size of the recording buffer queue */
#define NB_BUFFERS_IN_QUEUE 1
/* Size of each buffer in the queue */
#define BUFFER_SIZE_IN_BYTES 2048

/* Local storage for Audio data */
int8_t pcmData[NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES];

/* destination for recorded data */
SNDFILE *sndfile;

//-----------------------------------------------------------------
/* Exits the application if an error is encountered */
#define ExitOnError(x) ExitOnErrorFunc(x,__LINE__)

void ExitOnErrorFunc( SLresult result , int line)
{
    if (SL_RESULT_SUCCESS != result) {
        fprintf(stderr, "%u error code encountered at line %d, exiting\n", result, line);
        exit(EXIT_FAILURE);
    }
}

//-----------------------------------------------------------------
/* Structure for passing information to callback function */
typedef struct CallbackCntxt_ {
    SLPlayItf  playItf;
    SLuint32   size;
    SLint8*   pDataBase;    // Base address of local audio data storage
    SLint8*   pData;        // Current address of local audio data storage
} CallbackCntxt;


//-----------------------------------------------------------------
/* Callback for recording buffer queue events */
void RecCallback(
        SLRecordItf caller,
        void *pContext __unused,
        SLuint32 event)
{
    if (SL_RECORDEVENT_HEADATNEWPOS & event) {
        SLmillisecond pMsec = 0;
        (*caller)->GetPosition(caller, &pMsec);
        printf("SL_RECORDEVENT_HEADATNEWPOS current position=%ums\n", pMsec);
    }

    if (SL_RECORDEVENT_HEADATMARKER & event) {
        SLmillisecond pMsec = 0;
        (*caller)->GetPosition(caller, &pMsec);
        printf("SL_RECORDEVENT_HEADATMARKER current position=%ums\n", pMsec);
    }
}

//-----------------------------------------------------------------
/* Callback for recording buffer queue events */
void RecBufferQueueCallback(
        SLAndroidSimpleBufferQueueItf queueItf,
        void *pContext)
{
    //printf("RecBufferQueueCallback called\n");

    CallbackCntxt *pCntxt = (CallbackCntxt*)pContext;

    /* Save the recorded data  */
    sf_count_t frameCount = BUFFER_SIZE_IN_BYTES / frameSize;
    switch (transferFormat) {
    case AUDIO_FORMAT_PCM_8_BIT: {
        short temp[BUFFER_SIZE_IN_BYTES];
        memcpy_to_i16_from_u8(temp, (const unsigned char *) pCntxt->pDataBase,
                              BUFFER_SIZE_IN_BYTES);
        (void) sf_writef_short(sndfile, (const short *) temp, frameCount);
        } break;
    case AUDIO_FORMAT_PCM_16_BIT:
        (void) sf_writef_short(sndfile, (const short *) pCntxt->pDataBase, frameCount);
        break;
    case AUDIO_FORMAT_PCM_32_BIT:
        (void) sf_writef_int(sndfile, (const int *) pCntxt->pDataBase, frameCount);
        break;
    case AUDIO_FORMAT_PCM_FLOAT:
        (void) sf_writef_float(sndfile, (const float *) pCntxt->pDataBase, frameCount);
        break;
    default:
        fprintf(stderr, "Unsupported transfer format %d\n", transferFormat);
        exit(EXIT_FAILURE);
    }

    /* Increase data pointer by buffer size */
    pCntxt->pData += BUFFER_SIZE_IN_BYTES;

    if (pCntxt->pData >= pCntxt->pDataBase + (NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES)) {
        pCntxt->pData = pCntxt->pDataBase;
    }

    ExitOnError( (*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES) );

    SLAndroidSimpleBufferQueueState recQueueState;
    ExitOnError( (*queueItf)->GetState(queueItf, &recQueueState) );

    /*fprintf(stderr, "\tRecBufferQueueCallback now has pCntxt->pData=%p queue: "
            "count=%u playIndex=%u\n",
            pCntxt->pData, recQueueState.count, recQueueState.index);*/
    //printf("RecBufferQueueCallback returned\n");
}

//-----------------------------------------------------------------

/* Record to an audio path by opening a file descriptor on that path  */
void TestRecToBuffQueue( SLObjectItf sl, const char* path, SLAint64 durationInSeconds)
{
    SF_INFO info;
    info.frames = 0;
    info.samplerate = sampleRate;
    info.channels = channelCount;
    switch (transferFormat) {
    case AUDIO_FORMAT_PCM_8_BIT:
        info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
        break;
    case AUDIO_FORMAT_PCM_16_BIT:
        info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        break;
    case AUDIO_FORMAT_PCM_32_BIT:
        info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
        break;
    case AUDIO_FORMAT_PCM_FLOAT:
        info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
        break;
    default:
        fprintf(stderr, "Unsupported transfer format %d\n", transferFormat);
        exit(EXIT_FAILURE);
    }
    sndfile = sf_open(path, SFM_WRITE, &info);
    if (sndfile == NULL) {
        ExitOnError(SL_RESULT_RESOURCE_ERROR);
    }

    SLresult  result;
    SLEngineItf EngineItf;

    /* Objects this application uses: one audio recorder */
    SLObjectItf  recorder;

    /* Interfaces for the audio recorder */
    SLAndroidSimpleBufferQueueItf recBuffQueueItf;
    SLRecordItf               recordItf;
    SLAndroidConfigurationItf configItf;

    /* Source of audio data for the recording */
    SLDataSource           recSource;
    SLDataLocator_IODevice ioDevice;

    /* Data sink for recorded audio */
    SLDataSink                recDest;
    SLDataLocator_AndroidSimpleBufferQueue recBuffQueue;
    /* As mentioned in the Android extension API documentation this is identical to
     * OpenSL ES 1.1 SLDataFormat_PCM_EX, and can be used for an instance of SLDataFormat_PCM.
     */
    SLAndroidDataFormat_PCM_EX pcm;

    int numInterfaces = NUM_EXPLICIT_INTERFACES_FOR_RECORDER;
    if (aec) numInterfaces++;
    if (agc) numInterfaces++;
    if (ns) numInterfaces++;

    SLboolean required[numInterfaces];
    SLInterfaceID iidArray[numInterfaces];

    /* Get the SL Engine Interface which is implicit */
    result = (*sl)->GetInterface(sl, SL_IID_ENGINE, (void*)&EngineItf);
    ExitOnError(result);

    /* Initialize arrays required[] and iidArray[] */
    for (int i=0 ; i < numInterfaces ; i++) {
        required[i] = SL_BOOLEAN_FALSE;
        iidArray[i] = SL_IID_NULL;
    }


    /* ------------------------------------------------------ */
    /* Configuration of the recorder  */

    /* Request the AndroidSimpleBufferQueue and AndroidConfiguration interfaces */
    int index = 0;
    required[index] = SL_BOOLEAN_TRUE;
    iidArray[index++] = SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
    required[index] = SL_BOOLEAN_TRUE;
    iidArray[index++] = SL_IID_ANDROIDCONFIGURATION;
    if (aec) {
        iidArray[index++] = SL_IID_ANDROIDACOUSTICECHOCANCELLATION;
    }
    if (agc) {
        iidArray[index++] = SL_IID_ANDROIDAUTOMATICGAINCONTROL;
    }
    if (ns) {
        iidArray[index++] = SL_IID_ANDROIDNOISESUPPRESSION;
    }

    /* Setup the data source */
    ioDevice.locatorType = SL_DATALOCATOR_IODEVICE;
    ioDevice.deviceType = SL_IODEVICE_AUDIOINPUT;
    ioDevice.deviceID = SL_DEFAULTDEVICEID_AUDIOINPUT;
    ioDevice.device = NULL;
    recSource.pLocator = (void *) &ioDevice;
    recSource.pFormat = NULL;

    /* Setup the data sink */
    recBuffQueue.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    recBuffQueue.numBuffers = NB_BUFFERS_IN_QUEUE;
    /*    set up the format of the data in the buffer queue */
    pcm.formatType = transferFormat == AUDIO_FORMAT_PCM_FLOAT ||
            transferFormat == AUDIO_FORMAT_PCM_8_BIT ?
            SL_ANDROID_DATAFORMAT_PCM_EX : SL_DATAFORMAT_PCM;
    pcm.numChannels = channelCount;
    pcm.sampleRate = sampleRate * 1000; // milliHz
    pcm.representation = SL_ANDROID_PCM_REPRESENTATION_SIGNED_INT;
    switch (transferFormat) {
    case AUDIO_FORMAT_PCM_16_BIT:
        pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
        pcm.containerSize = 16;
        break;
    case AUDIO_FORMAT_PCM_32_BIT:
        pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_32;
        pcm.containerSize = 32;
        break;
    case AUDIO_FORMAT_PCM_8_BIT:
        pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
        pcm.containerSize = 8;
        pcm.representation = SL_ANDROID_PCM_REPRESENTATION_UNSIGNED_INT;
        break;
    case AUDIO_FORMAT_PCM_FLOAT:
        pcm.bitsPerSample = 32;
        pcm.containerSize = 32;
        pcm.representation = SL_ANDROID_PCM_REPRESENTATION_FLOAT;
        break;
    default:
        fprintf(stderr, "Unsupported transfer format %d\n", transferFormat);
        exit(EXIT_FAILURE);
    }
    if (useIndexChannelMask) {
        pcm.channelMask = (1 << channelCount) - 1;
    } else {
        switch (channelCount) {
        case 1:
            pcm.channelMask = SL_SPEAKER_FRONT_LEFT;
            break;
        case 2:
            pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
            break;
        default:
            fprintf(stderr, "Unsupported channel count %d\n", channelCount);
            exit(EXIT_FAILURE);
        }
    }
    pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    recDest.pLocator = (void *) &recBuffQueue;
    recDest.pFormat = (void * ) &pcm;

    /* Create the audio recorder */
    result = (*EngineItf)->CreateAudioRecorder(EngineItf, &recorder, &recSource, &recDest,
                                               numInterfaces, iidArray, required);
    ExitOnError(result);
    printf("Recorder created\n");

    /* Get the Android configuration interface which is explicit */
    result = (*recorder)->GetInterface(recorder, SL_IID_ANDROIDCONFIGURATION, (void*)&configItf);
    ExitOnError(result);

    /* Use the configuration interface to configure the recorder before it's realized */
    if (presetValue != SL_ANDROID_RECORDING_PRESET_NONE) {
        result = (*configItf)->SetConfiguration(configItf, SL_ANDROID_KEY_RECORDING_PRESET,
                &presetValue, sizeof(SLuint32));
        ExitOnError(result);
        printf("Recorder configured with preset %u\n", presetValue);
    } else {
        printf("Using default record preset\n");
    }

    if (performanceMode != SL_ANDROID_PERFORMANCE_LATENCY) {
        result = (*configItf)->SetConfiguration(configItf, SL_ANDROID_KEY_PERFORMANCE_MODE,
                &performanceMode, sizeof(SLuint32));
        ExitOnError(result);
        printf("Recorder configured with performance mode %u\n", performanceMode);
    } else {
        printf("Using default performance mode\n");
    }

    SLuint32 presetRetrieved = SL_ANDROID_RECORDING_PRESET_NONE;
    SLuint32 presetSize = 2*sizeof(SLuint32); // intentionally too big
    result = (*configItf)->GetConfiguration(configItf, SL_ANDROID_KEY_RECORDING_PRESET,
            &presetSize, (void*)&presetRetrieved);
    ExitOnError(result);
    if (presetValue == SL_ANDROID_RECORDING_PRESET_NONE) {
        printf("The default record preset appears to be %u\n", presetRetrieved);
    } else if (presetValue != presetRetrieved) {
        fprintf(stderr, "Error retrieving recording preset as %u instead of %u\n", presetRetrieved,
                presetValue);
        ExitOnError(SL_RESULT_INTERNAL_ERROR);
    }

    /* Realize the recorder in synchronous mode. */
    result = (*recorder)->Realize(recorder, SL_BOOLEAN_FALSE);
    ExitOnError(result);
    printf("Recorder realized\n");

    /* Check actual performance mode granted*/
    SLuint32 modeRetrieved = SL_ANDROID_PERFORMANCE_NONE;
    SLuint32 modeSize = sizeof(SLuint32);
    result = (*configItf)->GetConfiguration(configItf, SL_ANDROID_KEY_PERFORMANCE_MODE,
            &modeSize, (void*)&modeRetrieved);
    ExitOnError(result);
    printf("Actual performance mode is %u\n", modeRetrieved);

    /* Get the record interface which is implicit */
    result = (*recorder)->GetInterface(recorder, SL_IID_RECORD, (void*)&recordItf);
    ExitOnError(result);

    /* Set up the recorder callback to get events during the recording */
    result = (*recordItf)->SetMarkerPosition(recordItf, 2000);
    ExitOnError(result);
    result = (*recordItf)->SetPositionUpdatePeriod(recordItf, 500);
    ExitOnError(result);
    result = (*recordItf)->SetCallbackEventsMask(recordItf,
            SL_RECORDEVENT_HEADATMARKER | SL_RECORDEVENT_HEADATNEWPOS);
    ExitOnError(result);
    result = (*recordItf)->RegisterCallback(recordItf, RecCallback, NULL);
    ExitOnError(result);
    printf("Recorder callback registered\n");

    /* Enable AEC if requested */
    if (aec) {
        SLAndroidAcousticEchoCancellationItf aecItf;
        result = (*recorder)->GetInterface(
                recorder, SL_IID_ANDROIDACOUSTICECHOCANCELLATION, (void*)&aecItf);
        printf("AEC is %savailable\n", SL_RESULT_SUCCESS == result ? "" : "not ");
        if (SL_RESULT_SUCCESS == result) {
            result = (*aecItf)->SetEnabled(aecItf, true);
            ExitOnError(result);
            SLboolean enabled;
            result = (*aecItf)->IsEnabled(aecItf, &enabled);
            ExitOnError(result);
            printf("AEC is %s\n", enabled ? "enabled" : "not enabled");
        }
    }

    /* Enable AGC if requested */
    if (agc) {
        SLAndroidAutomaticGainControlItf agcItf;
        result = (*recorder)->GetInterface(
                recorder, SL_IID_ANDROIDAUTOMATICGAINCONTROL, (void*)&agcItf);
        printf("AGC is %savailable\n", SL_RESULT_SUCCESS == result ? "" : "not ");
        if (SL_RESULT_SUCCESS == result) {
            result = (*agcItf)->SetEnabled(agcItf, true);
            ExitOnError(result);
            SLboolean enabled;
            result = (*agcItf)->IsEnabled(agcItf, &enabled);
            ExitOnError(result);
            printf("AGC is %s\n", enabled ? "enabled" : "not enabled");
        }
    }

    /* Enable NS if requested */
    if (ns) {
        SLAndroidNoiseSuppressionItf nsItf;
        result = (*recorder)->GetInterface(
                recorder, SL_IID_ANDROIDNOISESUPPRESSION, (void*)&nsItf);
        printf("NS is %savailable\n", SL_RESULT_SUCCESS == result ? "" : "not ");
        if (SL_RESULT_SUCCESS == result) {
            result = (*nsItf)->SetEnabled(nsItf, true);
            ExitOnError(result);
            SLboolean enabled;
            result = (*nsItf)->IsEnabled(nsItf, &enabled);
            ExitOnError(result);
            printf("NS is %s\n", enabled ? "enabled" : "not enabled");
        }
    }

    /* Get the buffer queue interface which was explicitly requested */
    result = (*recorder)->GetInterface(recorder, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
            (void*)&recBuffQueueItf);
    ExitOnError(result);

    /* ------------------------------------------------------ */
    /* Initialize the callback and its context for the recording buffer queue */
    CallbackCntxt cntxt;
    cntxt.pDataBase = (int8_t*)&pcmData;
    cntxt.pData = cntxt.pDataBase;
    cntxt.size = sizeof(pcmData);
    result = (*recBuffQueueItf)->RegisterCallback(recBuffQueueItf, RecBufferQueueCallback, &cntxt);
    ExitOnError(result);

    /* Enqueue buffers to map the region of memory allocated to store the recorded data */
    printf("Enqueueing buffer ");
    for(int i = 0 ; i < NB_BUFFERS_IN_QUEUE ; i++) {
        printf("%d ", i);
        result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, cntxt.pData, BUFFER_SIZE_IN_BYTES);
        ExitOnError(result);
        cntxt.pData += BUFFER_SIZE_IN_BYTES;
    }
    printf("\n");
    cntxt.pData = cntxt.pDataBase;

    /* ------------------------------------------------------ */
    /* Start recording */
    result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);
    ExitOnError(result);
    printf("Starting to record\n");

    /* Record for at least a second */
    if (durationInSeconds < 1) {
        durationInSeconds = 1;
    }
    usleep(durationInSeconds * 1000 * 1000);

    /* ------------------------------------------------------ */
    /* End of recording */

    /* Stop recording */
    result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
    ExitOnError(result);
    printf("Stopped recording\n");

    /* Destroy the AudioRecorder object */
    (*recorder)->Destroy(recorder);

    sf_close(sndfile);
}

//-----------------------------------------------------------------
int main(int argc, char* const argv[])
{
    int durationInSeconds = 10;
    SLresult    result;
    SLObjectItf sl;

    const char *prog = argv[0];
    printf("OpenSL ES test %s: exercises SLRecordItf and SLAndroidSimpleBufferQueueItf ",
            prog);
    printf("on an AudioRecorder object\n");

    int i;
    for (i = 1; i < argc; ++i) {
        const char *arg = argv[i];
        if (arg[0] != '-') {
            break;
        }
        switch (arg[1]) {
        case 'c':   // channel count
            channelCount = atoi(&arg[2]);
            break;
        case 'd':   // duration in seconds
            durationInSeconds = atoi(&arg[2]);
            break;
        case 'f':
            transferFormat = AUDIO_FORMAT_PCM_FLOAT;
            break;
        case 'i':
            useIndexChannelMask = true;
            break;
        case 'p':   // preset number
            presetValue = atoi(&arg[2]);
            break;
        case 'r':
            sampleRate = atoi(&arg[2]);
            break;
        case '1':
            transferFormat = AUDIO_FORMAT_PCM_8_BIT;
            break;
        case '2':
            transferFormat = AUDIO_FORMAT_PCM_16_BIT;
            break;
        case '4':
            transferFormat = AUDIO_FORMAT_PCM_32_BIT;
            break;
        case 'm':
            performanceMode = atoi(&arg[2]);
            break;
        case 'x':
            if (strstr(arg, "e") != NULL) {
                aec = true;
            }
            if (strstr(arg, "a") != NULL) {
                agc = true;
            }
            if (strstr(arg, "n") != NULL) {
                ns = true;
            }
            break;
        default:
            fprintf(stderr, "%s: unknown option %s\n", prog, arg);
            break;
        }
    }

    if (transferFormat == AUDIO_FORMAT_DEFAULT) {
        transferFormat = AUDIO_FORMAT_PCM_16_BIT;
    }
    frameSize = audio_bytes_per_sample(transferFormat) * channelCount;

    if (argc-i != 1) {
        printf("Usage: \t%s [-c#] [-d#] [-i] [-p#] [-r#] [-1/2/4/f] destination_file\n", prog);
        printf("  -c# channel count, defaults to 1\n");
        printf("  -d# duration in seconds, default to 10\n");
        printf("  -i  index channel mask, not yet implemented\n");
        printf("  -p# is the preset value which defaults to SL_ANDROID_RECORDING_PRESET_NONE\n");
        printf("  possible values are:\n");
        printf("    -p%d SL_ANDROID_RECORDING_PRESET_NONE\n",
                SL_ANDROID_RECORDING_PRESET_NONE);
        printf("    -p%d SL_ANDROID_RECORDING_PRESET_GENERIC\n",
                SL_ANDROID_RECORDING_PRESET_GENERIC);
        printf("    -p%d SL_ANDROID_RECORDING_PRESET_CAMCORDER\n",
                SL_ANDROID_RECORDING_PRESET_CAMCORDER);
        printf("    -p%d SL_ANDROID_RECORDING_PRESET_VOICE_RECOGNITION\n",
                SL_ANDROID_RECORDING_PRESET_VOICE_RECOGNITION);
        printf("    -p%d SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION\n",
                SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION);
        printf("  -r# sample rate in Hz, defaults to 48000\n");
        printf("  -[1/2/4/f] sample format: 8-bit unsigned, 16-bit signed, 32-bit signed, float, "
                "defaults to 16-bit signed\n");
        printf("  -m# is the performance mode value which defaults to"
                " SL_ANDROID_PERFORMANCE_LATENCY\n");
        printf("  possible values are:\n");
        printf("    -m%d SL_ANDROID_PERFORMANCE_NONE\n",
               SL_ANDROID_PERFORMANCE_NONE);
        printf("    -m%d SL_ANDROID_PERFORMANCE_LATENCY\n",
               SL_ANDROID_PERFORMANCE_LATENCY);
        printf("    -m%d SL_ANDROID_PERFORMANCE_LATENCY_EFFECTS\n",
               SL_ANDROID_PERFORMANCE_LATENCY_EFFECTS);
        printf("    -m%d SL_ANDROID_PERFORMANCE_POWER_SAVING\n",
               SL_ANDROID_PERFORMANCE_POWER_SAVING);
        printf("  -x[e][a][n] for pre processing:\n"
                " - e: Echo canceler\n"
                " - a: Automatic Gain Control\n"
                " - n: Noise Suppression\n");
        printf("Example: \"%s /sdcard/myrec.wav\" \n", prog);
        exit(EXIT_FAILURE);
    }

    SLEngineOption EngineOption[] = {
            {(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE}
    };

    result = slCreateEngine( &sl, 1, EngineOption, 0, NULL, NULL);
    ExitOnError(result);

    /* Realizing the SL Engine in synchronous mode. */
    result = (*sl)->Realize(sl, SL_BOOLEAN_FALSE);
    ExitOnError(result);

    TestRecToBuffQueue(sl, argv[i], durationInSeconds);

    /* Shutdown OpenSL ES */
    (*sl)->Destroy(sl);

    return EXIT_SUCCESS;
}
