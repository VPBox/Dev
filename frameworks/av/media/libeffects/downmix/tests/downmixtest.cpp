/*
 * Copyright (C) 2011 The Android Open Source Project
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
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <audio_effects/effect_downmix.h>
#include <audio_utils/channels.h>
#include <audio_utils/primitives.h>
#include <log/log.h>
#include <system/audio.h>

#include "EffectDownmix.h"
#define FRAME_LENGTH 256
#define MAX_NUM_CHANNELS 8

struct downmix_cntxt_s {
  effect_descriptor_t desc;
  effect_handle_t handle;
  effect_config_t config;

  int numFileChannels;
  int numProcessChannels;
};

extern audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM;

void printUsage() {
  printf("\nUsage:");
  printf("\n     downmixtest <input_file> <out_file> [options]\n");
  printf("\nwhere,");
  printf("\n     <input_file>  is the input file name");
  printf("\n                  on which LVM effects are applied");
  printf("\n     <output_file> processed output file");
  printf("\n     and options are mentioned below");
  printf("\n");
  printf("\n      -h");
  printf("\n           Prints this usage information");
  printf("\n");
  printf("\n     -ch_fmt:<format_of_input_audio>");
  printf("\n         0:AUDIO_CHANNEL_OUT_7POINT1(default)");
  printf("\n         1:AUDIO_CHANNEL_OUT_5POINT1_SIDE");
  printf("\n         2:AUDIO_CHANNEL_OUT_5POINT1_BACK");
  printf("\n         3:AUDIO_CHANNEL_OUT_QUAD_SIDE");
  printf("\n         4:AUDIO_CHANNEL_OUT_QUAD_BACK");
  printf("\n");
  printf("\n     -fch:<file_channels> (1 through 8)");
  printf("\n");
}

int32_t DownmixDefaultConfig(effect_config_t *pConfig) {
  pConfig->inputCfg.accessMode = EFFECT_BUFFER_ACCESS_READ;
  pConfig->inputCfg.format = AUDIO_FORMAT_PCM_FLOAT;
  pConfig->inputCfg.channels = AUDIO_CHANNEL_OUT_7POINT1;
  pConfig->inputCfg.bufferProvider.getBuffer = nullptr;
  pConfig->inputCfg.bufferProvider.releaseBuffer = nullptr;
  pConfig->inputCfg.bufferProvider.cookie = nullptr;
  pConfig->inputCfg.mask = EFFECT_CONFIG_ALL;

  pConfig->inputCfg.samplingRate = 44100;
  pConfig->outputCfg.samplingRate = pConfig->inputCfg.samplingRate;

  // set a default value for the access mode, but should be overwritten by caller
  pConfig->outputCfg.accessMode = EFFECT_BUFFER_ACCESS_WRITE;
  pConfig->outputCfg.format = AUDIO_FORMAT_PCM_FLOAT;
  pConfig->outputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
  pConfig->outputCfg.bufferProvider.getBuffer = nullptr;
  pConfig->outputCfg.bufferProvider.releaseBuffer = nullptr;
  pConfig->outputCfg.bufferProvider.cookie = nullptr;
  pConfig->outputCfg.mask = EFFECT_CONFIG_ALL;

  return 0;
}

int32_t DownmixConfiureAndEnable(downmix_cntxt_s *pDescriptor) {
  effect_handle_t *effectHandle = &pDescriptor->handle;
  downmix_module_t *downmixEffectHandle = (downmix_module_t *)*effectHandle;
  const struct effect_interface_s *Downmix_api = downmixEffectHandle->itfe;
  int32_t err = 0;
  uint32_t replySize = (uint32_t)sizeof(err);

  err = (Downmix_api->command)(*effectHandle, EFFECT_CMD_SET_CONFIG,
                               sizeof(effect_config_t), &(pDescriptor->config),
                               &replySize, &err);
  if (err != 0) {
    ALOGE("Downmix command to configure returned an error %d", err);
    return err;
  }

  err = ((Downmix_api->command))(*effectHandle, EFFECT_CMD_ENABLE, 0, nullptr,
                                 &replySize, &err);
  if (err != 0) {
    ALOGE("Downmix command to enable effect returned an error %d", err);
    return err;
  }
  return 0;
}

int32_t DownmixExecute(downmix_cntxt_s *pDescriptor, FILE *finp,
                       FILE *fout) {
  effect_handle_t *effectHandle = &pDescriptor->handle;
  downmix_module_t *downmixEffectHandle = (downmix_module_t *)*effectHandle;
  const struct effect_interface_s *Downmix_api = downmixEffectHandle->itfe;

  const int numFileChannels = pDescriptor->numFileChannels;
  const int numProcessChannels = pDescriptor->numProcessChannels;
  const int fileFrameSize = numFileChannels * sizeof(short);
  const unsigned int outputChannels =
      audio_channel_count_from_out_mask(AUDIO_CHANNEL_OUT_STEREO);

  std::vector<float> outFloat(FRAME_LENGTH * MAX_NUM_CHANNELS);
  std::vector<float> inFloat(FRAME_LENGTH * MAX_NUM_CHANNELS);

  audio_buffer_t inbuffer, outbuffer;
  inbuffer.f32 = inFloat.data();
  outbuffer.f32 = outFloat.data();
  inbuffer.frameCount = FRAME_LENGTH;
  outbuffer.frameCount = FRAME_LENGTH;

  audio_buffer_t *pinbuf, *poutbuf;
  pinbuf = &inbuffer;
  poutbuf = &outbuffer;

  int frameCounter = 0;
  std::vector<short> inS16(FRAME_LENGTH * MAX_NUM_CHANNELS);
  std::vector<short> outS16(FRAME_LENGTH * MAX_NUM_CHANNELS);

  while (fread(inS16.data(), fileFrameSize, FRAME_LENGTH, finp) ==
         FRAME_LENGTH) {
    if (numFileChannels != numProcessChannels) {
      adjust_channels(inS16.data(), numFileChannels, inS16.data(),
                      numProcessChannels, sizeof(short),
                      FRAME_LENGTH * fileFrameSize);
    }

    memcpy_to_float_from_i16(inFloat.data(), inS16.data(),
                             FRAME_LENGTH * numProcessChannels);

    const int32_t err = (Downmix_api->process)(*effectHandle, pinbuf, poutbuf);
    if (err != 0) {
      ALOGE("DownmixProcess returned an error %d", err);
      return -1;
    }

    memcpy_to_i16_from_float(outS16.data(), outFloat.data(),
                             FRAME_LENGTH * outputChannels);
    fwrite(outS16.data(), sizeof(short), (FRAME_LENGTH * outputChannels),
           fout);
    frameCounter++;
  }
  printf("frameCounter: [%d]\n", frameCounter);
  return 0;
}

int32_t DowmixMainProcess(downmix_cntxt_s *pDescriptor, FILE *finp,
                          FILE *fout) {
  effect_handle_t *effectHandle = &pDescriptor->handle;
  int32_t sessionId = 0, ioId = 0;
  const effect_uuid_t downmix_uuid = {
      0x93f04452, 0xe4fe, 0x41cc, 0x91f9, {0xe4, 0x75, 0xb6, 0xd1, 0xd6, 0x9f}};

  int32_t err = AUDIO_EFFECT_LIBRARY_INFO_SYM.create_effect(
      &downmix_uuid, sessionId, ioId,
      effectHandle);
  if (err != 0) {
    ALOGE("DownmixLib_Create returned an error %d", err);
    return -1;
  }

  // Passing the init config for time being.
  err = DownmixConfiureAndEnable(pDescriptor);
  if (err != 0) {
    ALOGE("DownmixConfigureAndEnable returned an error %d", err);
    return -1;
  }
  // execute call for downmix.
  err = DownmixExecute(pDescriptor, finp, fout);
  if (err != 0) {
    ALOGE("DownmixExecute returned an error %d", err);
    return -1;
  }
  // Release the library function.
  err = AUDIO_EFFECT_LIBRARY_INFO_SYM.release_effect(*effectHandle);
  if (err != 0) {
    ALOGE("DownmixRelease returned an error %d", err);
    return -1;
  }
  return 0;
}

int main(int argc, const char *argv[]) {
  int numFileChannels = 1, numProcessChannels = 8;
  downmix_cntxt_s descriptor = {};
  DownmixDefaultConfig(&(descriptor.config));

  const char *infile = nullptr;
  const char *outfile = nullptr;
  for (int i = 1; i < argc; i++) {
    printf("%s ", argv[i]);
    if (argv[i][0] != '-') {
      if (infile == nullptr) {
        infile = argv[i];
      } else if (outfile == nullptr) {
        outfile = argv[i];
      } else {
        printUsage();
        return -1;
      }
    } else if (!strncmp(argv[i], "-fs:", 4)) {
      // Add a check for all the supported streams.
      const int samplingFreq = atoi(argv[i] + 4);
      if (samplingFreq != 8000 && samplingFreq != 11025 &&
          samplingFreq != 12000 && samplingFreq != 16000 &&
          samplingFreq != 22050 && samplingFreq != 24000 &&
          samplingFreq != 32000 && samplingFreq != 44100 &&
          samplingFreq != 48000 && samplingFreq != 88200 &&
          samplingFreq != 96000 && samplingFreq != 176400 &&
          samplingFreq != 192000) {
        printf("Unsupported Sampling Frequency : %d", samplingFreq);
        printUsage();
        return -1;
      }

      descriptor.config.inputCfg.samplingRate = samplingFreq;
      descriptor.config.outputCfg.samplingRate = samplingFreq;
    } else if (!strncmp(argv[i], "-ch_fmt:", 8)) {
      const int format = atoi(argv[i] + 8);
      uint32_t *audioType = &descriptor.config.inputCfg.channels;
      switch (format) {
        case 0:
          *audioType = AUDIO_CHANNEL_OUT_7POINT1;
          break;
        case 1:
          *audioType = AUDIO_CHANNEL_OUT_5POINT1_SIDE;
          break;
        case 2:
          *audioType = AUDIO_CHANNEL_OUT_5POINT1_BACK;
          break;
        case 3:
          *audioType = AUDIO_CHANNEL_OUT_QUAD_SIDE;
          break;
        case 4:
          *audioType = AUDIO_CHANNEL_OUT_QUAD_BACK;
          break;
        default:
          *audioType = AUDIO_CHANNEL_OUT_7POINT1;
          break;
      }
      descriptor.numProcessChannels =
          audio_channel_count_from_out_mask(*audioType);
    } else if (!strncmp(argv[i], "-fch:", 5)) {
      const int fChannels = atoi(argv[i] + 5);
      if (fChannels > 8 || fChannels < 1) {
        printf("Unsupported number of file channels : %d", fChannels);
        printUsage();
        return -1;
      }
      descriptor.numFileChannels = fChannels;

    } else if (!strncmp(argv[i], "-h", 2)) {
      printUsage();
      return 0;
    }
  }

  if (/*infile == nullptr || */ outfile == nullptr) {
    printUsage();
    return -1;
  }

  FILE *finp = fopen(infile, "rb");
  if (finp == nullptr) {
    printf("Cannot open input file %s", infile);
    return -1;
  }
  FILE *fout = fopen(outfile, "wb");
  if (fout == nullptr) {
    printf("Cannot open output file %s", outfile);
    fclose(finp);
    return -1;
  }

  const int err = DowmixMainProcess(&descriptor, finp, fout);
  // close input and output files.
  fclose(finp);
  fclose(fout);
  if (err != 0) {
    printf("Error: %d\n", err);
  }
  return err;
}
