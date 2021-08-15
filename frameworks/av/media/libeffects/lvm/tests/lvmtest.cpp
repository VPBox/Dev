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
#include <assert.h>
#include <inttypes.h>
#include <iterator>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <audio_utils/channels.h>
#include <audio_utils/primitives.h>
#include <log/log.h>
#include <system/audio.h>

#include "EffectBundle.h"
#include "LVM_Private.h"

#ifdef VERY_VERY_VERBOSE_LOGGING
#define ALOGVV ALOGV
#else
#define ALOGVV(a...) \
  do {               \
  } while (false)
#endif

#define CHECK_ARG(cond)                                \
  {                                                    \
    if (!(cond)) {                                     \
      ALOGE("\tLVM_ERROR : Invalid argument: " #cond); \
      return -EINVAL;                                  \
    }                                                  \
  \
}

#define LVM_ERROR_CHECK(LvmStatus, callingFunc, calledFunc)     \
  {                                                             \
    if ((LvmStatus) == LVM_NULLADDRESS) {                       \
      ALOGE(                                                    \
          "\tLVM_ERROR : Parameter error - "                    \
          "null pointer returned by %s in %s\n\n\n\n",          \
          callingFunc, calledFunc);                             \
    }                                                           \
    if ((LvmStatus) == LVM_ALIGNMENTERROR) {                    \
      ALOGE(                                                    \
          "\tLVM_ERROR : Parameter error - "                    \
          "bad alignment returned by %s in %s\n\n\n\n",         \
          callingFunc, calledFunc);                             \
    }                                                           \
    if ((LvmStatus) == LVM_INVALIDNUMSAMPLES) {                 \
      ALOGE(                                                    \
          "\tLVM_ERROR : Parameter error - "                    \
          "bad number of samples returned by %s in %s\n\n\n\n", \
          callingFunc, calledFunc);                             \
    }                                                           \
    if ((LvmStatus) == LVM_OUTOFRANGE) {                        \
      ALOGE(                                                    \
          "\tLVM_ERROR : Parameter error - "                    \
          "out of range returned by %s in %s\n",                \
          callingFunc, calledFunc);                             \
    }                                                           \
  }

struct lvmConfigParams_t {
  int              samplingFreq    = 44100;
  int              nrChannels      = 2;
  int              chMask          = AUDIO_CHANNEL_OUT_STEREO;
  int              vcBal           = 0;
  int              fChannels       = 2;
  bool             monoMode        = false;
  int              bassEffectLevel = 0;
  int              eqPresetLevel   = 0;
  int              frameLength     = 256;
  LVM_BE_Mode_en   bassEnable      = LVM_BE_OFF;
  LVM_TE_Mode_en   trebleEnable    = LVM_TE_OFF;
  LVM_EQNB_Mode_en eqEnable        = LVM_EQNB_OFF;
  LVM_Mode_en      csEnable        = LVM_MODE_OFF;
};

constexpr audio_channel_mask_t lvmConfigChMask[] = {
    AUDIO_CHANNEL_OUT_MONO,
    AUDIO_CHANNEL_OUT_STEREO,
    AUDIO_CHANNEL_OUT_2POINT1,
    AUDIO_CHANNEL_OUT_2POINT0POINT2,
    AUDIO_CHANNEL_OUT_QUAD,
    AUDIO_CHANNEL_OUT_QUAD_BACK,
    AUDIO_CHANNEL_OUT_QUAD_SIDE,
    AUDIO_CHANNEL_OUT_SURROUND,
    (1 << 4) - 1,
    AUDIO_CHANNEL_OUT_2POINT1POINT2,
    AUDIO_CHANNEL_OUT_3POINT0POINT2,
    AUDIO_CHANNEL_OUT_PENTA,
    (1 << 5) - 1,
    AUDIO_CHANNEL_OUT_3POINT1POINT2,
    AUDIO_CHANNEL_OUT_5POINT1,
    AUDIO_CHANNEL_OUT_5POINT1_BACK,
    AUDIO_CHANNEL_OUT_5POINT1_SIDE,
    (1 << 6) - 1,
    AUDIO_CHANNEL_OUT_6POINT1,
    (1 << 7) - 1,
    AUDIO_CHANNEL_OUT_5POINT1POINT2,
    AUDIO_CHANNEL_OUT_7POINT1,
    (1 << 8) - 1,
};


void printUsage() {
  printf("\nUsage: ");
  printf("\n     <executable> -i:<input_file> -o:<out_file> [options]\n");
  printf("\nwhere, \n     <inputfile>  is the input file name");
  printf("\n                  on which LVM effects are applied");
  printf("\n     <outputfile> processed output file");
  printf("\n     and options are mentioned below");
  printf("\n");
  printf("\n     -help (or) -h");
  printf("\n           Prints this usage information");
  printf("\n");
  printf("\n     -chMask:<channel_mask>\n");
  printf("\n         0  - AUDIO_CHANNEL_OUT_MONO");
  printf("\n         1  - AUDIO_CHANNEL_OUT_STEREO");
  printf("\n         2  - AUDIO_CHANNEL_OUT_2POINT1");
  printf("\n         3  - AUDIO_CHANNEL_OUT_2POINT0POINT2");
  printf("\n         4  - AUDIO_CHANNEL_OUT_QUAD");
  printf("\n         5  - AUDIO_CHANNEL_OUT_QUAD_BACK");
  printf("\n         6  - AUDIO_CHANNEL_OUT_QUAD_SIDE");
  printf("\n         7  - AUDIO_CHANNEL_OUT_SURROUND");
  printf("\n         8  - canonical channel index mask for 4 ch: (1 << 4) - 1");
  printf("\n         9  - AUDIO_CHANNEL_OUT_2POINT1POINT2");
  printf("\n         10 - AUDIO_CHANNEL_OUT_3POINT0POINT2");
  printf("\n         11 - AUDIO_CHANNEL_OUT_PENTA");
  printf("\n         12 - canonical channel index mask for 5 ch: (1 << 5) - 1");
  printf("\n         13 - AUDIO_CHANNEL_OUT_3POINT1POINT2");
  printf("\n         14 - AUDIO_CHANNEL_OUT_5POINT1");
  printf("\n         15 - AUDIO_CHANNEL_OUT_5POINT1_BACK");
  printf("\n         16 - AUDIO_CHANNEL_OUT_5POINT1_SIDE");
  printf("\n         17 - canonical channel index mask for 6 ch: (1 << 6) - 1");
  printf("\n         18 - AUDIO_CHANNEL_OUT_6POINT1");
  printf("\n         19 - canonical channel index mask for 7 ch: (1 << 7) - 1");
  printf("\n         20 - AUDIO_CHANNEL_OUT_5POINT1POINT2");
  printf("\n         21 - AUDIO_CHANNEL_OUT_7POINT1");
  printf("\n         22 - canonical channel index mask for 8 ch: (1 << 8) - 1");
  printf("\n         default 0");
  printf("\n     -vcBal:<Left Right Balance control in dB [-96 to 96 dB]>");
  printf("\n            -ve values reduce Right channel while +ve value reduces Left channel");
  printf("\n                 default 0");
  printf("\n     -fch:<file_channels> (1 through 8)\n\n");
  printf("\n     -M");
  printf("\n           Mono mode (force all input audio channels to be identical)");
  printf("\n     -basslvl:<effect_level>");
  printf("\n           A value that ranges between %d - %d default 0", LVM_BE_MIN_EFFECTLEVEL,
    LVM_BE_MAX_EFFECTLEVEL);
  printf("\n");
  printf("\n     -eqPreset:<preset Value>");
  const size_t numPresetLvls  = std::size(gEqualizerPresets);
  for (size_t i = 0; i < numPresetLvls; ++i) {
    printf("\n           %zu - %s", i, gEqualizerPresets[i].name);
  }
  printf("\n           default - 0");
  printf("\n     -bE ");
  printf("\n           Enable Dynamic Bass Enhancement");
  printf("\n");
  printf("\n     -tE ");
  printf("\n           Enable Treble Boost");
  printf("\n");
  printf("\n     -csE ");
  printf("\n           Enable Concert Surround");
  printf("\n");
  printf("\n     -eqE ");
  printf("\n           Enable Equalizer");
}

//----------------------------------------------------------------------------
// LvmEffect_free()
//----------------------------------------------------------------------------
// Purpose: Free all memory associated with the Bundle.
//
// Inputs:
//  pContext:   effect engine context
//
// Outputs:
//
//----------------------------------------------------------------------------

void LvmEffect_free(struct EffectContext *pContext) {
  LVM_ReturnStatus_en LvmStatus = LVM_SUCCESS; /* Function call status */
  LVM_MemTab_t MemTab;

  /* Free the algorithm memory */
  LvmStatus = LVM_GetMemoryTable(pContext->pBundledContext->hInstance, &MemTab,
                                 LVM_NULL);

  LVM_ERROR_CHECK(LvmStatus, "LVM_GetMemoryTable", "LvmEffect_free")

  for (int i = 0; i < LVM_NR_MEMORY_REGIONS; i++) {
    if (MemTab.Region[i].Size != 0) {
      if (MemTab.Region[i].pBaseAddress != NULL) {
        ALOGV("\tLvmEffect_free - START freeing %" PRIu32
              " bytes for region %u at %p\n",
              MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);

        free(MemTab.Region[i].pBaseAddress);

        ALOGV("\tLvmEffect_free - END   freeing %" PRIu32
              " bytes for region %u at %p\n",
              MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
      } else {
        ALOGE(
            "\tLVM_ERROR : LvmEffect_free - trying to free with NULL pointer "
            "%" PRIu32 " bytes for region %u at %p ERROR\n",
            MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
      }
    }
  }
} /* end LvmEffect_free */

//----------------------------------------------------------------------------
// LvmBundle_init()
//----------------------------------------------------------------------------
// Purpose: Initialize engine with default configuration, creates instance
// with all effects disabled.
//
// Inputs:
//  pContext:   effect engine context
//
// Outputs:
//
//----------------------------------------------------------------------------

int LvmBundle_init(struct EffectContext *pContext, LVM_ControlParams_t *params) {
  ALOGV("\tLvmBundle_init start");

  pContext->config.inputCfg.accessMode = EFFECT_BUFFER_ACCESS_READ;
  pContext->config.inputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
  pContext->config.inputCfg.format = EFFECT_BUFFER_FORMAT;
  pContext->config.inputCfg.samplingRate = 44100;
  pContext->config.inputCfg.bufferProvider.getBuffer = NULL;
  pContext->config.inputCfg.bufferProvider.releaseBuffer = NULL;
  pContext->config.inputCfg.bufferProvider.cookie = NULL;
  pContext->config.inputCfg.mask = EFFECT_CONFIG_ALL;
  pContext->config.outputCfg.accessMode = EFFECT_BUFFER_ACCESS_ACCUMULATE;
  pContext->config.outputCfg.channels = AUDIO_CHANNEL_OUT_STEREO;
  pContext->config.outputCfg.format = EFFECT_BUFFER_FORMAT;
  pContext->config.outputCfg.samplingRate = 44100;
  pContext->config.outputCfg.bufferProvider.getBuffer = NULL;
  pContext->config.outputCfg.bufferProvider.releaseBuffer = NULL;
  pContext->config.outputCfg.bufferProvider.cookie = NULL;
  pContext->config.outputCfg.mask = EFFECT_CONFIG_ALL;

  if (pContext->pBundledContext->hInstance != NULL) {
    ALOGV(
        "\tLvmBundle_init pContext->pBassBoost != NULL "
        "-> Calling pContext->pBassBoost->free()");

    LvmEffect_free(pContext);

    ALOGV(
        "\tLvmBundle_init pContext->pBassBoost != NULL "
        "-> Called pContext->pBassBoost->free()");
  }

  LVM_ReturnStatus_en LvmStatus = LVM_SUCCESS; /* Function call status */
  LVM_InstParams_t InstParams;                 /* Instance parameters */
  LVM_EQNB_BandDef_t BandDefs[MAX_NUM_BANDS];  /* Equaliser band definitions */
  LVM_HeadroomParams_t HeadroomParams;         /* Headroom parameters */
  LVM_HeadroomBandDef_t HeadroomBandDef[LVM_HEADROOM_MAX_NBANDS];
  LVM_MemTab_t MemTab; /* Memory allocation table */
  bool bMallocFailure = LVM_FALSE;

  /* Set the capabilities */
  InstParams.BufferMode = LVM_UNMANAGED_BUFFERS;
  InstParams.MaxBlockSize = MAX_CALL_SIZE;
  InstParams.EQNB_NumBands = MAX_NUM_BANDS;
  InstParams.PSA_Included = LVM_PSA_ON;

  /* Allocate memory, forcing alignment */
  LvmStatus = LVM_GetMemoryTable(LVM_NULL, &MemTab, &InstParams);

  LVM_ERROR_CHECK(LvmStatus, "LVM_GetMemoryTable", "LvmBundle_init");
  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  ALOGV("\tCreateInstance Succesfully called LVM_GetMemoryTable\n");

  /* Allocate memory */
  for (int i = 0; i < LVM_NR_MEMORY_REGIONS; i++) {
    if (MemTab.Region[i].Size != 0) {
      MemTab.Region[i].pBaseAddress = malloc(MemTab.Region[i].Size);

      if (MemTab.Region[i].pBaseAddress == LVM_NULL) {
        ALOGE(
            "\tLVM_ERROR :LvmBundle_init CreateInstance Failed to allocate "
            "%" PRIu32 " bytes for region %u\n",
            MemTab.Region[i].Size, i);
        bMallocFailure = LVM_TRUE;
        break;
      } else {
        ALOGV("\tLvmBundle_init CreateInstance allocated %" PRIu32
              " bytes for region %u at %p\n",
              MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
      }
    }
  }

  /* If one or more of the memory regions failed to allocate, free the regions
   * that were
   * succesfully allocated and return with an error
   */
  if (bMallocFailure == LVM_TRUE) {
    for (int i = 0; i < LVM_NR_MEMORY_REGIONS; i++) {
      if (MemTab.Region[i].pBaseAddress == LVM_NULL) {
        ALOGE(
            "\tLVM_ERROR :LvmBundle_init CreateInstance Failed to allocate "
            "%" PRIu32 " bytes for region %u Not freeing\n",
            MemTab.Region[i].Size, i);
      } else {
        ALOGE(
            "\tLVM_ERROR :LvmBundle_init CreateInstance Failed: but allocated "
            "%" PRIu32 " bytes for region %u at %p- free\n",
            MemTab.Region[i].Size, i, MemTab.Region[i].pBaseAddress);
        free(MemTab.Region[i].pBaseAddress);
      }
    }
    return -EINVAL;
  }
  ALOGV("\tLvmBundle_init CreateInstance Succesfully malloc'd memory\n");

  /* Initialise */
  pContext->pBundledContext->hInstance = LVM_NULL;

  /* Init sets the instance handle */
  LvmStatus = LVM_GetInstanceHandle(&pContext->pBundledContext->hInstance,
                                    &MemTab, &InstParams);

  LVM_ERROR_CHECK(LvmStatus, "LVM_GetInstanceHandle", "LvmBundle_init");
  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  ALOGV(
      "\tLvmBundle_init CreateInstance Succesfully called "
      "LVM_GetInstanceHandle\n");

  /* Set the initial process parameters */
  /* General parameters */
  params->OperatingMode = LVM_MODE_ON;
  params->SampleRate = LVM_FS_44100;
  params->SourceFormat = LVM_STEREO;
  params->ChMask       = AUDIO_CHANNEL_OUT_STEREO;
  params->SpeakerType = LVM_HEADPHONES;

  pContext->pBundledContext->SampleRate = LVM_FS_44100;

  /* Concert Sound parameters */
  params->VirtualizerOperatingMode = LVM_MODE_OFF;
  params->VirtualizerType = LVM_CONCERTSOUND;
  params->VirtualizerReverbLevel = 100;
  params->CS_EffectLevel = LVM_CS_EFFECT_NONE;

  /* N-Band Equaliser parameters */
  params->EQNB_OperatingMode = LVM_EQNB_ON;
  params->EQNB_NBands = FIVEBAND_NUMBANDS;
  params->pEQNB_BandDefinition = &BandDefs[0];

  for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
    BandDefs[i].Frequency = EQNB_5BandPresetsFrequencies[i];
    BandDefs[i].QFactor = EQNB_5BandPresetsQFactors[i];
    BandDefs[i].Gain = EQNB_5BandSoftPresets[i];
  }

  /* Volume Control parameters */
  params->VC_EffectLevel = 0;
  params->VC_Balance = 0;

  /* Treble Enhancement parameters */
  params->TE_OperatingMode = LVM_TE_OFF;
  params->TE_EffectLevel = 0;

  /* PSA Control parameters */
  params->PSA_Enable = LVM_PSA_OFF;
  params->PSA_PeakDecayRate = (LVM_PSA_DecaySpeed_en)0;

  /* Bass Enhancement parameters */
  params->BE_OperatingMode = LVM_BE_ON;
  params->BE_EffectLevel = 0;
  params->BE_CentreFreq = LVM_BE_CENTRE_90Hz;
  params->BE_HPF = LVM_BE_HPF_ON;

  /* PSA Control parameters */
  params->PSA_Enable = LVM_PSA_OFF;
  params->PSA_PeakDecayRate = LVM_PSA_SPEED_MEDIUM;

  /* TE Control parameters */
  params->TE_OperatingMode = LVM_TE_OFF;
  params->TE_EffectLevel = 0;

  /* Activate the initial settings */
  LvmStatus =
      LVM_SetControlParameters(pContext->pBundledContext->hInstance, params);

  LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "LvmBundle_init");
  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  ALOGV(
      "\tLvmBundle_init CreateInstance Succesfully called "
      "LVM_SetControlParameters\n");

  /* Set the headroom parameters */
  HeadroomBandDef[0].Limit_Low = 20;
  HeadroomBandDef[0].Limit_High = 4999;
  HeadroomBandDef[0].Headroom_Offset = 0;
  HeadroomBandDef[1].Limit_Low = 5000;
  HeadroomBandDef[1].Limit_High = 24000;
  HeadroomBandDef[1].Headroom_Offset = 0;
  HeadroomParams.pHeadroomDefinition = &HeadroomBandDef[0];
  HeadroomParams.Headroom_OperatingMode = LVM_HEADROOM_ON;
  HeadroomParams.NHeadroomBands = 2;

  LvmStatus = LVM_SetHeadroomParams(pContext->pBundledContext->hInstance,
                                    &HeadroomParams);

  LVM_ERROR_CHECK(LvmStatus, "LVM_SetHeadroomParams", "LvmBundle_init");
  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  ALOGV(
      "\tLvmBundle_init CreateInstance Succesfully called "
      "LVM_SetHeadroomParams\n");
  ALOGV("\tLvmBundle_init End");
  return 0;
} /* end LvmBundle_init */

int lvmCreate(struct EffectContext *pContext,
              lvmConfigParams_t    *plvmConfigParams,
              LVM_ControlParams_t  *params) {
  int ret = 0;
  pContext->pBundledContext = NULL;
  pContext->pBundledContext = (BundledEffectContext *)malloc(sizeof(struct BundledEffectContext));
  if (NULL == pContext->pBundledContext) {
    return -EINVAL;
  }

  pContext->pBundledContext->SessionNo = 0;
  pContext->pBundledContext->SessionId = 0;
  pContext->pBundledContext->hInstance = NULL;
  pContext->pBundledContext->bVolumeEnabled = LVM_FALSE;
  pContext->pBundledContext->bEqualizerEnabled = LVM_FALSE;
  pContext->pBundledContext->bBassEnabled = LVM_FALSE;
  pContext->pBundledContext->bBassTempDisabled = LVM_FALSE;
  pContext->pBundledContext->bVirtualizerEnabled = LVM_FALSE;
  pContext->pBundledContext->bVirtualizerTempDisabled = LVM_FALSE;
  pContext->pBundledContext->nOutputDevice = AUDIO_DEVICE_NONE;
  pContext->pBundledContext->nVirtualizerForcedDevice = AUDIO_DEVICE_NONE;
  pContext->pBundledContext->NumberEffectsEnabled = 0;
  pContext->pBundledContext->NumberEffectsCalled = 0;
  pContext->pBundledContext->firstVolume = LVM_TRUE;
  pContext->pBundledContext->volume = 0;

  /* Saved strength is used to return the exact strength that was used in the
   * set to the get
   * because we map the original strength range of 0:1000 to 1:15, and this will
   * avoid
   * quantisation like effect when returning
   */
  pContext->pBundledContext->BassStrengthSaved = 0;
  pContext->pBundledContext->VirtStrengthSaved = 0;
  pContext->pBundledContext->CurPreset = PRESET_CUSTOM;
  pContext->pBundledContext->levelSaved = 0;
  pContext->pBundledContext->bMuteEnabled = LVM_FALSE;
  pContext->pBundledContext->bStereoPositionEnabled = LVM_FALSE;
  pContext->pBundledContext->positionSaved = 0;
  pContext->pBundledContext->workBuffer = NULL;
  pContext->pBundledContext->frameCount = -1;
  pContext->pBundledContext->SamplesToExitCountVirt = 0;
  pContext->pBundledContext->SamplesToExitCountBb = 0;
  pContext->pBundledContext->SamplesToExitCountEq = 0;
#if defined(BUILD_FLOAT) && !defined(NATIVE_FLOAT_BUFFER)
  pContext->pBundledContext->pInputBuffer = NULL;
  pContext->pBundledContext->pOutputBuffer = NULL;
#endif
  for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
    pContext->pBundledContext->bandGaindB[i] = EQNB_5BandSoftPresets[i];
  }
  pContext->config.inputCfg.channels = plvmConfigParams->nrChannels;
  ALOGV("\tEffectCreate - Calling LvmBundle_init");
  ret = LvmBundle_init(pContext, params);

  if (ret < 0) {
    ALOGE("\tLVM_ERROR : lvmCreate() Bundle init failed");
    return ret;
  }
  return 0;
}

int lvmControl(struct EffectContext *pContext,
               lvmConfigParams_t    *plvmConfigParams,
               LVM_ControlParams_t  *params) {
  LVM_ReturnStatus_en LvmStatus = LVM_SUCCESS; /* Function call status */

  /* Set the initial process parameters */
  /* General parameters */
  params->OperatingMode = LVM_MODE_ON;
  params->SpeakerType = LVM_HEADPHONES;

  params->ChMask     = plvmConfigParams->chMask;
  params->NrChannels = plvmConfigParams->nrChannels;
  if (params->NrChannels == 1) {
    params->SourceFormat = LVM_MONO;
  } else if (params->NrChannels == 2) {
    params->SourceFormat = LVM_STEREO;
  } else if (params->NrChannels > 2 && params->NrChannels <= 8) { // FCC_2 FCC_8
    params->SourceFormat = LVM_MULTICHANNEL;
  } else {
      return -EINVAL;
  }

  LVM_Fs_en sampleRate;
  switch (plvmConfigParams->samplingFreq) {
    case 8000:
      sampleRate = LVM_FS_8000;
      break;
    case 11025:
      sampleRate = LVM_FS_11025;
      break;
    case 12000:
      sampleRate = LVM_FS_12000;
      break;
    case 16000:
      sampleRate = LVM_FS_16000;
      break;
    case 22050:
      sampleRate = LVM_FS_22050;
      break;
    case 24000:
      sampleRate = LVM_FS_24000;
      break;
    case 32000:
      sampleRate = LVM_FS_32000;
      break;
    case 44100:
      sampleRate = LVM_FS_44100;
      break;
    case 48000:
      sampleRate = LVM_FS_48000;
      break;
    case 88200:
      sampleRate = LVM_FS_88200;
      break;
    case 96000:
      sampleRate = LVM_FS_96000;
      break;
    case 176400:
      sampleRate = LVM_FS_176400;
      break;
    case 192000:
      sampleRate = LVM_FS_192000;
      break;
    default:
      return -EINVAL;
  }
  params->SampleRate = sampleRate;

  /* Concert Sound parameters */
  params->VirtualizerOperatingMode = plvmConfigParams->csEnable;
  params->VirtualizerType = LVM_CONCERTSOUND;
  params->VirtualizerReverbLevel = 100;
  params->CS_EffectLevel = LVM_CS_EFFECT_NONE;

  /* N-Band Equaliser parameters */
  const int eqPresetLevel = plvmConfigParams->eqPresetLevel;
  LVM_EQNB_BandDef_t BandDefs[MAX_NUM_BANDS];  /* Equaliser band definitions */
  for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
    BandDefs[i].Frequency = EQNB_5BandPresetsFrequencies[i];
    BandDefs[i].QFactor = EQNB_5BandPresetsQFactors[i];
    BandDefs[i].Gain =
        EQNB_5BandSoftPresets[(FIVEBAND_NUMBANDS * eqPresetLevel) + i];
  }
  params->EQNB_OperatingMode = plvmConfigParams->eqEnable;
 // Caution: raw pointer to stack data, stored in instance by LVM_SetControlParameters.
  params->pEQNB_BandDefinition = &BandDefs[0];

  /* Volume Control parameters */
  params->VC_EffectLevel = 0;
  params->VC_Balance = plvmConfigParams->vcBal;

  /* Treble Enhancement parameters */
  params->TE_OperatingMode = plvmConfigParams->trebleEnable;

  /* PSA Control parameters */
  params->PSA_Enable = LVM_PSA_ON;

  /* Bass Enhancement parameters */
  params->BE_OperatingMode = plvmConfigParams->bassEnable;

  /* Activate the initial settings */
  LvmStatus =
      LVM_SetControlParameters(pContext->pBundledContext->hInstance, params);

  LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "LvmBundle_init");
  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  LvmStatus = LVM_ApplyNewSettings(pContext->pBundledContext->hInstance);

  if (LvmStatus != LVM_SUCCESS) return -EINVAL;

  return 0;
}

int lvmExecute(float *floatIn, float *floatOut, struct EffectContext *pContext,
               lvmConfigParams_t *plvmConfigParams) {
  const int frameLength = plvmConfigParams->frameLength;
  return
      LVM_Process(pContext->pBundledContext->hInstance, /* Instance handle */
                  floatIn,                              /* Input buffer */
                  floatOut,                             /* Output buffer */
                  (LVM_UINT16)frameLength, /* Number of samples to read */
                  0);                      /* Audio Time */
}

int lvmMainProcess(EffectContext *pContext,
                   LVM_ControlParams_t *pParams,
                   lvmConfigParams_t *plvmConfigParams,
                   FILE *finp,
                   FILE *fout) {
  int errCode = lvmControl(pContext, plvmConfigParams, pParams);
  if (errCode) {
    ALOGE("Error: lvmControl returned with %d\n", errCode);
    return errCode;
  }

  const int channelCount = plvmConfigParams->nrChannels;
  const int frameLength = plvmConfigParams->frameLength;
  const int frameSize = channelCount * sizeof(float);  // processing size
  const int ioChannelCount = plvmConfigParams->fChannels;
  const int ioFrameSize = ioChannelCount * sizeof(short); // file load size
  const int maxChannelCount = std::max(channelCount, ioChannelCount);
  /*
   * Mono input will be converted to 2 channels internally in the process call
   * by copying the same data into the second channel.
   * Hence when channelCount is 1, output buffer should be allocated for
   * 2 channels. The memAllocChCount takes care of allocation of sufficient
   * memory for the output buffer.
   */
  const int memAllocChCount = (channelCount == 1 ? 2 : channelCount);

  std::vector<short> in(frameLength * maxChannelCount);
  std::vector<short> out(frameLength * maxChannelCount);
  std::vector<float> floatIn(frameLength * channelCount);
  std::vector<float> floatOut(frameLength * memAllocChCount);

  int frameCounter = 0;
  while (fread(in.data(), ioFrameSize, frameLength, finp) == (size_t)frameLength) {
    if (ioChannelCount != channelCount) {
        adjust_channels(in.data(), ioChannelCount, in.data(), channelCount,
               sizeof(short), frameLength * ioFrameSize);
    }
    memcpy_to_float_from_i16(floatIn.data(), in.data(), frameLength * channelCount);

    // Mono mode will replicate the first channel to all other channels.
    // This ensures all audio channels are identical. This is useful for testing
    // Bass Boost, which extracts a mono signal for processing.
    if (plvmConfigParams->monoMode && channelCount > 1) {
        for (int i = 0; i < frameLength; ++i) {
            auto *fp = &floatIn[i * channelCount];
            std::fill(fp + 1, fp + channelCount, *fp); // replicate ch 0
        }
    }
#ifndef BYPASS_EXEC
    errCode = lvmExecute(floatIn.data(), floatOut.data(), pContext, plvmConfigParams);
    if (errCode) {
      printf("\nError: lvmExecute returned with %d\n", errCode);
      return errCode;
    }

    (void)frameSize; // eliminate warning
#else
    memcpy(floatOut.data(), floatIn.data(), frameLength * frameSize);
#endif
    memcpy_to_i16_from_float(out.data(), floatOut.data(), frameLength * channelCount);
    if (ioChannelCount != channelCount) {
        adjust_channels(out.data(), channelCount, out.data(), ioChannelCount,
               sizeof(short), frameLength * channelCount * sizeof(short));
    }
    (void) fwrite(out.data(), ioFrameSize, frameLength, fout);
    frameCounter += frameLength;
  }
  printf("frameCounter: [%d]\n", frameCounter);
  return 0;
}

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    printUsage();
    return -1;
  }

  lvmConfigParams_t lvmConfigParams{}; // default initialize
  const char *infile = nullptr;
  const char *outfile = nullptr;

  for (int i = 1; i < argc; i++) {
    printf("%s ", argv[i]);
    if (!strncmp(argv[i], "-i:", 3)) {
      infile = argv[i] + 3;
    } else if (!strncmp(argv[i], "-o:", 3)) {
      outfile = argv[i] + 3;
    } else if (!strncmp(argv[i], "-fs:", 4)) {
      const int samplingFreq = atoi(argv[i] + 4);
      if (samplingFreq != 8000 && samplingFreq != 11025 &&
          samplingFreq != 12000 && samplingFreq != 16000 &&
          samplingFreq != 22050 && samplingFreq != 24000 &&
          samplingFreq != 32000 && samplingFreq != 44100 &&
          samplingFreq != 48000 && samplingFreq != 88200 &&
          samplingFreq != 96000 && samplingFreq != 176400 &&
          samplingFreq != 192000) {
        printf("Error: Unsupported Sampling Frequency : %d\n", samplingFreq);
        return -1;
      }
      lvmConfigParams.samplingFreq = samplingFreq;
    } else if (!strncmp(argv[i], "-chMask:", 8)) {
      const int chMaskConfigIdx = atoi(argv[i] + 8);
      if (chMaskConfigIdx < 0 || (size_t)chMaskConfigIdx >= std::size(lvmConfigChMask)) {
        ALOGE("\nError: Unsupported Channel Mask : %d\n", chMaskConfigIdx);
        return -1;
      }
      const audio_channel_mask_t chMask = lvmConfigChMask[chMaskConfigIdx];
      lvmConfigParams.chMask = chMask;
      lvmConfigParams.nrChannels = audio_channel_count_from_out_mask(chMask);
    } else if (!strncmp(argv[i], "-vcBal:", 7)) {
      const int vcBalance = atoi(argv[i] + 7);
      if (vcBalance > 96 || vcBalance < -96) {
        ALOGE("\nError: Unsupported volume balance value: %d\n", vcBalance);
      }
      lvmConfigParams.vcBal = vcBalance;
    } else if (!strncmp(argv[i], "-fch:", 5)) {
      const int fChannels = atoi(argv[i] + 5);
      if (fChannels > 8 || fChannels < 1) {
             printf("Error: Unsupported number of file channels : %d\n", fChannels);
             return -1;
           }
           lvmConfigParams.fChannels = fChannels;
    } else if (!strcmp(argv[i],"-M")) {
          lvmConfigParams.monoMode = true;
    } else if (!strncmp(argv[i], "-basslvl:", 9)) {
      const int bassEffectLevel = atoi(argv[i] + 9);
      if (bassEffectLevel > LVM_BE_MAX_EFFECTLEVEL || bassEffectLevel < LVM_BE_MIN_EFFECTLEVEL) {
        printf("Error: Unsupported Bass Effect Level : %d\n",
               bassEffectLevel);
        printUsage();
        return -1;
      }
      lvmConfigParams.bassEffectLevel = bassEffectLevel;
    } else if (!strncmp(argv[i], "-eqPreset:", 10)) {
      const int eqPresetLevel = atoi(argv[i] + 10);
      const int numPresetLvls = std::size(gEqualizerPresets);
      if (eqPresetLevel >= numPresetLvls || eqPresetLevel < 0) {
        printf("Error: Unsupported Equalizer Preset : %d\n", eqPresetLevel);
        printUsage();
        return -1;
      }
      lvmConfigParams.eqPresetLevel = eqPresetLevel;
    } else if (!strcmp(argv[i], "-bE")) {
      lvmConfigParams.bassEnable = LVM_BE_ON;
    } else if (!strcmp(argv[i], "-eqE")) {
      lvmConfigParams.eqEnable = LVM_EQNB_ON;
    } else if (!strcmp(argv[i], "-tE")) {
      lvmConfigParams.trebleEnable = LVM_TE_ON;
    } else if (!strcmp(argv[i], "-csE")) {
      lvmConfigParams.csEnable = LVM_MODE_ON;
    } else if (!strcmp(argv[i], "-h")) {
      printUsage();
      return 0;
    }
  }

  if (infile == nullptr || outfile == nullptr) {
    printf("Error: missing input/output files\n");
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

  EffectContext context;
  LVM_ControlParams_t params;
  int errCode = lvmCreate(&context, &lvmConfigParams, &params);
  if (errCode == 0) {
    errCode = lvmMainProcess(&context, &params, &lvmConfigParams, finp, fout);
    if (errCode != 0) {
      printf("Error: lvmMainProcess returned with the error: %d",errCode);
    }
  } else {
    printf("Error: lvmCreate returned with the error: %d", errCode);
  }
  fclose(finp);
  fclose(fout);
  /* Free the allocated buffers */
  if (context.pBundledContext != nullptr) {
    if (context.pBundledContext->hInstance != nullptr) {
      LvmEffect_free(&context);
    }
    free(context.pBundledContext);
  }

  if (errCode) {
    return -1;
  }
  return 0;
}
