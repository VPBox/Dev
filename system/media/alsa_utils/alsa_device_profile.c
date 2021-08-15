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

#define LOG_TAG "alsa_device_profile"
/*#define LOG_NDEBUG 0*/
/*#define LOG_PCM_PARAMS 0*/

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <cutils/properties.h>

#include <log/log.h>

#include "include/alsa_device_profile.h"
#include "include/alsa_format.h"
#include "include/alsa_logging.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PERIOD_DURATION_US (5 * 1000)

#define DEFAULT_PERIOD_SIZE 1024

static const char * const format_string_map[] = {
    "AUDIO_FORMAT_PCM_16_BIT",      /* "PCM_FORMAT_S16_LE", */
    "AUDIO_FORMAT_PCM_32_BIT",      /* "PCM_FORMAT_S32_LE", */
    "AUDIO_FORMAT_PCM_8_BIT",       /* "PCM_FORMAT_S8", */
    "AUDIO_FORMAT_PCM_8_24_BIT",    /* "PCM_FORMAT_S24_LE", */
    "AUDIO_FORMAT_PCM_24_BIT_PACKED"/* "PCM_FORMAT_S24_3LE" */
};

extern int8_t const pcm_format_value_map[50];

/* Sort these in terms of preference (best first).
   192 kHz is not first because it requires significant resources for possibly worse
   quality and driver instability (depends on device).
   The order here determines the default sample rate for the device.
   AudioPolicyManager may not respect this ordering when picking sample rates.
   Update MAX_PROFILE_SAMPLE_RATES after changing the array size.

   TODO: remove 32000, 22050, 12000, 11025?  Each sample rate check
   requires opening the device which may cause pops. */
static const unsigned std_sample_rates[] =
    {96000, 88200, 192000, 176400, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000};

static void profile_reset(alsa_device_profile* profile)
{
    profile->card = profile->device = -1;

    /* terminate the attribute arrays with invalid values */
    profile->formats[0] = PCM_FORMAT_INVALID;
    profile->sample_rates[0] = 0;
    profile->channel_counts[0] = 0;

    profile->min_period_size = profile->max_period_size = 0;
    profile->min_channel_count = profile->max_channel_count = DEFAULT_CHANNEL_COUNT;

    profile->is_valid = false;
}

void profile_init(alsa_device_profile* profile, int direction)
{
    profile->direction = direction;
    profile_reset(profile);
}

bool profile_is_initialized(const alsa_device_profile* profile)
{
    return profile->card >= 0 && profile->device >= 0;
}

bool profile_is_valid(const alsa_device_profile* profile) {
    return profile->is_valid;
}

bool profile_is_cached_for(const alsa_device_profile* profile, int card, int device) {
    return card == profile->card && device == profile->device;
}

void profile_decache(alsa_device_profile* profile) {
    profile_reset(profile);
}

/*
 * Returns the supplied value rounded up to the next even multiple of 16
 */
static unsigned int round_to_16_mult(unsigned int size)
{
    return (size + 15) & ~15;   /* 0xFFFFFFF0; */
}

/*
 * Returns the system defined minimum period size based on the supplied sample rate.
 */
unsigned profile_calc_min_period_size(const alsa_device_profile* profile, unsigned sample_rate)
{
    ALOGV("profile_calc_min_period_size(%p, rate:%d)", profile, sample_rate);
    if (profile == NULL) {
        return DEFAULT_PERIOD_SIZE;
    } else {
        unsigned period_us = property_get_int32("ro.audio.usb.period_us", PERIOD_DURATION_US);
        unsigned num_sample_frames = ((uint64_t)sample_rate * period_us) / 1000000;

        if (num_sample_frames < profile->min_period_size) {
            num_sample_frames = profile->min_period_size;
        }
        return round_to_16_mult(num_sample_frames);
    }
}

unsigned int profile_get_period_size(const alsa_device_profile* profile, unsigned sample_rate)
{
    unsigned int period_size = profile_calc_min_period_size(profile, sample_rate);
    ALOGV("profile_get_period_size(rate:%d) = %d", sample_rate, period_size);
    return period_size;
}

/*
 * Sample Rate
 */
unsigned profile_get_default_sample_rate(const alsa_device_profile* profile)
{
    /*
     * This is probably a poor algorithm. The default sample rate should be the highest (within
     * limits) rate that is available for both input and output. HOWEVER, the profile has only
     * one or the other, so that will need to be done at a higher level, like in the HAL.
     */
    /*
     * TODO this won't be right in general. we should store a preferred rate as we are scanning.
     * But right now it will return the highest rate, which may be correct.
     */
    return profile_is_valid(profile) ? profile->sample_rates[0] : DEFAULT_SAMPLE_RATE;
}

unsigned profile_get_highest_sample_rate(const alsa_device_profile* profile) {
    /* The hightest sample rate is always stored in the first element of sample_rates.
     * Note that profile_reset() initiaizes the first element of samples_rates to 0
     * Which is what we want to return if the profile had not been read anyway.
     */
    return profile->sample_rates[0];
}

bool profile_is_sample_rate_valid(const alsa_device_profile* profile, unsigned rate)
{
    if (profile_is_valid(profile)) {
        size_t index;
        for (index = 0; profile->sample_rates[index] != 0; index++) {
            if (profile->sample_rates[index] == rate) {
                return true;
            }
        }

        return false;
    } else {
        ALOGW("**** PROFILE NOT VALID!");
        return rate == DEFAULT_SAMPLE_RATE;
    }
}

/*
 * Format
 */
enum pcm_format profile_get_default_format(const alsa_device_profile* profile)
{
    /*
     * TODO this won't be right in general. we should store a preferred format as we are scanning.
     */
    return profile_is_valid(profile) ? profile->formats[0] : DEFAULT_SAMPLE_FORMAT;
}

bool profile_is_format_valid(const alsa_device_profile* profile, enum pcm_format fmt) {
    if (profile_is_valid(profile)) {
        size_t index;
        for (index = 0; profile->formats[index] != PCM_FORMAT_INVALID; index++) {
            if (profile->formats[index] == fmt) {
                return true;
            }
        }

        return false;
    } else {
        return fmt == DEFAULT_SAMPLE_FORMAT;
    }
}

/*
 * Channels
 */
unsigned profile_get_default_channel_count(const alsa_device_profile* profile)
{
    return profile_is_valid(profile) ? profile->channel_counts[0] : DEFAULT_CHANNEL_COUNT;
}

unsigned profile_get_closest_channel_count(const alsa_device_profile* profile, unsigned count)
{
    if (profile_is_valid(profile)) {
        if (count < profile->min_channel_count) {
            return profile->min_channel_count;
        } else if (count > profile->max_channel_count) {
            return profile->max_channel_count;
        } else {
            return count;
        }
    } else {
        return 0;
    }
}

bool profile_is_channel_count_valid(const alsa_device_profile* profile, unsigned count)
{
    if (profile_is_initialized(profile)) {
        return count >= profile->min_channel_count && count <= profile->max_channel_count;
    } else {
        return count == DEFAULT_CHANNEL_COUNT;
    }
}

static bool profile_test_sample_rate(const alsa_device_profile* profile, unsigned rate)
{
    struct pcm_config config = profile->default_config;
    config.rate = rate;

    bool works = false; /* let's be pessimistic */
    struct pcm * pcm = pcm_open(profile->card, profile->device,
                                profile->direction, &config);

    if (pcm != NULL) {
        works = pcm_is_ready(pcm);
        pcm_close(pcm);
    }

    return works;
}

static unsigned profile_enum_sample_rates(alsa_device_profile* profile, unsigned min, unsigned max)
{
    unsigned num_entries = 0;
    unsigned index;

    for (index = 0; index < ARRAY_SIZE(std_sample_rates) &&
                    num_entries < ARRAY_SIZE(profile->sample_rates) - 1;
         index++) {
        if (std_sample_rates[index] >= min && std_sample_rates[index] <= max
                && profile_test_sample_rate(profile, std_sample_rates[index])) {
            profile->sample_rates[num_entries++] = std_sample_rates[index];
        }
    }
    profile->sample_rates[num_entries] = 0; /* terminate */
    return num_entries; /* return # of supported rates */
}

static unsigned profile_enum_sample_formats(alsa_device_profile* profile, struct pcm_mask * mask)
{
    const int num_slots = ARRAY_SIZE(mask->bits);
    const int bits_per_slot = sizeof(mask->bits[0]) * 8;

    const int table_size = ARRAY_SIZE(pcm_format_value_map);

    int slot_index, bit_index, table_index;
    table_index = 0;
    int num_written = 0;
    for (slot_index = 0; slot_index < num_slots && table_index < table_size;
            slot_index++) {
        unsigned bit_mask = 1;
        for (bit_index = 0;
                bit_index < bits_per_slot && table_index < table_size;
                bit_index++) {
            if ((mask->bits[slot_index] & bit_mask) != 0) {
                enum pcm_format format = pcm_format_value_map[table_index];
                /* Never return invalid (unrecognized) or 8-bit */
                if (format != PCM_FORMAT_INVALID && format != PCM_FORMAT_S8) {
                    profile->formats[num_written++] = format;
                    if (num_written == ARRAY_SIZE(profile->formats) - 1) {
                        /* leave at least one PCM_FORMAT_INVALID at the end */
                        goto end;
                    }
                }
            }
            bit_mask <<= 1;
            table_index++;
        }
    }
end:
    profile->formats[num_written] = PCM_FORMAT_INVALID;
    return num_written;
}

static unsigned profile_enum_channel_counts(alsa_device_profile* profile, unsigned min,
        unsigned max)
{
    /* modify alsa_device_profile.h if you change the std_channel_counts[] array. */
    static const unsigned std_channel_counts[] = {8, 7, 6, 5, 4, 3, 2, 1};

    unsigned num_counts = 0;
    unsigned index;
    /* TODO write a profile_test_channel_count() */
    /* Ensure there is at least one invalid channel count to terminate the channel counts array */
    for (index = 0; index < ARRAY_SIZE(std_channel_counts) &&
                    num_counts < ARRAY_SIZE(profile->channel_counts) - 1;
         index++) {
        /* TODO Do we want a channel counts test? */
        if (std_channel_counts[index] >= min && std_channel_counts[index] <= max /* &&
            profile_test_channel_count(profile, channel_counts[index])*/) {
            profile->channel_counts[num_counts++] = std_channel_counts[index];
        }
    }
    // if we have no match with the standard counts, we use the largest (preferred) std count.
    if (num_counts == 0) {
        ALOGW("usb device does not match std channel counts, setting to %d",
                std_channel_counts[0]);
        profile->channel_counts[num_counts++] = std_channel_counts[0];
    }
    profile->channel_counts[num_counts] = 0;
    return num_counts; /* return # of supported counts */
}

/*
 * Reads and decodes configuration info from the specified ALSA card/device.
 */
static int read_alsa_device_config(alsa_device_profile * profile, struct pcm_config * config)
{
    ALOGV("usb:audio_hw - read_alsa_device_config(c:%d d:%d t:0x%X)",
          profile->card, profile->device, profile->direction);

    if (profile->card < 0 || profile->device < 0) {
        return -EINVAL;
    }

    struct pcm_params * alsa_hw_params =
        pcm_params_get(profile->card, profile->device, profile->direction);
    if (alsa_hw_params == NULL) {
        return -EINVAL;
    }

    profile->min_period_size = pcm_params_get_min(alsa_hw_params, PCM_PARAM_PERIOD_SIZE);
    profile->max_period_size = pcm_params_get_max(alsa_hw_params, PCM_PARAM_PERIOD_SIZE);

    profile->min_channel_count = pcm_params_get_min(alsa_hw_params, PCM_PARAM_CHANNELS);
    profile->max_channel_count = pcm_params_get_max(alsa_hw_params, PCM_PARAM_CHANNELS);

    int ret = 0;

    /*
     * This Logging will be useful when testing new USB devices.
     */
#ifdef LOG_PCM_PARAMS
    log_pcm_params(alsa_hw_params);
#endif

    config->channels = pcm_params_get_min(alsa_hw_params, PCM_PARAM_CHANNELS);
    // For output devices, let's make sure we choose at least stereo
    // (assuming the device supports it).
    if (profile->direction == PCM_OUT &&
        config->channels < 2 && pcm_params_get_max(alsa_hw_params, PCM_PARAM_CHANNELS) >= 2) {
        config->channels = 2;
    }
    config->rate = pcm_params_get_min(alsa_hw_params, PCM_PARAM_RATE);
    // Prefer 48K or 44.1K
    if (config->rate < 48000 &&
        pcm_params_get_max(alsa_hw_params, PCM_PARAM_RATE) >= 48000) {
        config->rate = 48000;
    } else if (config->rate < 44100 &&
               pcm_params_get_max(alsa_hw_params, PCM_PARAM_RATE) >= 44100) {
        config->rate = 44100;
    }
    config->period_size = profile_calc_min_period_size(profile, config->rate);
    config->period_count = pcm_params_get_min(alsa_hw_params, PCM_PARAM_PERIODS);
    config->format = get_pcm_format_for_mask(pcm_params_get_mask(alsa_hw_params, PCM_PARAM_FORMAT));
#ifdef LOG_PCM_PARAMS
    log_pcm_config(config, "read_alsa_device_config");
#endif
    if (config->format == PCM_FORMAT_INVALID) {
        ret = -EINVAL;
    }

    pcm_params_free(alsa_hw_params);

    return ret;
}

bool profile_read_device_info(alsa_device_profile* profile)
{
    if (!profile_is_initialized(profile)) {
        return false;
    }

    /* let's get some defaults */
    read_alsa_device_config(profile, &profile->default_config);
    ALOGV("default_config chans:%d rate:%d format:%d count:%d size:%d",
          profile->default_config.channels, profile->default_config.rate,
          profile->default_config.format, profile->default_config.period_count,
          profile->default_config.period_size);

    struct pcm_params * alsa_hw_params = pcm_params_get(profile->card,
                                                        profile->device,
                                                        profile->direction);
    if (alsa_hw_params == NULL) {
        return false;
    }

    /* Formats */
    struct pcm_mask * format_mask = pcm_params_get_mask(alsa_hw_params, PCM_PARAM_FORMAT);
    profile_enum_sample_formats(profile, format_mask);

    /* Channels */
    profile_enum_channel_counts(
            profile, pcm_params_get_min(alsa_hw_params, PCM_PARAM_CHANNELS),
            pcm_params_get_max(alsa_hw_params, PCM_PARAM_CHANNELS));

    /* Sample Rates */
    profile_enum_sample_rates(
            profile, pcm_params_get_min(alsa_hw_params, PCM_PARAM_RATE),
            pcm_params_get_max(alsa_hw_params, PCM_PARAM_RATE));

    profile->is_valid = true;

    pcm_params_free(alsa_hw_params);
    return true;
}

char * profile_get_sample_rate_strs(const alsa_device_profile* profile)
{
    /* if we assume that rate strings are about 5 characters (48000 is 5), plus ~1 for a
     * delimiter "|" this buffer has room for about 22 rate strings which seems like
     * way too much, but it's a stack variable so only temporary.
     */
    char buffer[128];
    buffer[0] = '\0';
    size_t buffSize = ARRAY_SIZE(buffer);
    size_t curStrLen = 0;

    char numBuffer[32];

    size_t numEntries = 0;
    size_t index;
    for (index = 0; profile->sample_rates[index] != 0; index++) {
        snprintf(numBuffer, sizeof(numBuffer), "%u", profile->sample_rates[index]);
        // account for both the null, and potentially the bar.
        if (buffSize - curStrLen < strlen(numBuffer) + (numEntries != 0 ? 2 : 1)) {
            /* we don't have room for another, so bail at this point rather than
             * return a malformed rate string
             */
            break;
        }
        if (numEntries++ != 0) {
            strlcat(buffer, "|", buffSize);
        }
        curStrLen = strlcat(buffer, numBuffer, buffSize);
    }

    return strdup(buffer);
}

char * profile_get_format_strs(const alsa_device_profile* profile)
{
    /* if we assume that format strings are about 24 characters (AUDIO_FORMAT_PCM_16_BIT is 23),
     * plus ~1 for a delimiter "|" this buffer has room for about 10 format strings which seems
     *  like way too much, but it's a stack variable so only temporary.
     */
    char buffer[256];
    buffer[0] = '\0';
    size_t buffSize = ARRAY_SIZE(buffer);
    size_t curStrLen = 0;

    size_t numEntries = 0;
    size_t index = 0;
    for (index = 0; profile->formats[index] != PCM_FORMAT_INVALID; index++) {
        // account for both the null, and potentially the bar.
        if (buffSize - curStrLen < strlen(format_string_map[profile->formats[index]])
                                   + (numEntries != 0 ? 2 : 1)) {
            /* we don't have room for another, so bail at this point rather than
             * return a malformed rate string
             */
            break;
        }
        if (numEntries++ != 0) {
            strlcat(buffer, "|", buffSize);
        }
        curStrLen = strlcat(buffer, format_string_map[profile->formats[index]], buffSize);
    }

    return strdup(buffer);
}

char * profile_get_channel_count_strs(const alsa_device_profile* profile)
{
    // FIXME implicit fixed channel count assumption here (FCC_8).
    // we use only the canonical even number channel position masks.
    static const char * const out_chans_strs[] = {
        /* 0 */"AUDIO_CHANNEL_NONE", /* will never be taken as this is a terminator */
        /* 1 */"AUDIO_CHANNEL_OUT_MONO",
        /* 2 */"AUDIO_CHANNEL_OUT_STEREO",
        /* 3 */ /* "AUDIO_CHANNEL_OUT_STEREO|AUDIO_CHANNEL_OUT_FRONT_CENTER" */ NULL,
        /* 4 */"AUDIO_CHANNEL_OUT_QUAD",
        /* 5 */ /* "AUDIO_CHANNEL_OUT_QUAD|AUDIO_CHANNEL_OUT_FRONT_CENTER" */ NULL,
        /* 6 */"AUDIO_CHANNEL_OUT_5POINT1",
        /* 7 */ /* "AUDIO_CHANNEL_OUT_5POINT1|AUDIO_CHANNEL_OUT_BACK_CENTER" */ NULL,
        /* 8 */"AUDIO_CHANNEL_OUT_7POINT1",
        /* channel counts greater than this not considered */
    };

    static const char * const in_chans_strs[] = {
        /* 0 */"AUDIO_CHANNEL_NONE", /* will never be taken as this is a terminator */
        /* 1 */"AUDIO_CHANNEL_IN_MONO",
        /* 2 */"AUDIO_CHANNEL_IN_STEREO",
        /* channel counts greater than this not considered */
    };

    static const char * const index_chans_strs[] = {
        /* 0 */"AUDIO_CHANNEL_NONE", /* will never be taken as this is a terminator */
        /* 1 */"AUDIO_CHANNEL_INDEX_MASK_1",
        /* 2 */"AUDIO_CHANNEL_INDEX_MASK_2",
        /* 3 */"AUDIO_CHANNEL_INDEX_MASK_3",
        /* 4 */"AUDIO_CHANNEL_INDEX_MASK_4",
        /* 5 */"AUDIO_CHANNEL_INDEX_MASK_5",
        /* 6 */"AUDIO_CHANNEL_INDEX_MASK_6",
        /* 7 */"AUDIO_CHANNEL_INDEX_MASK_7",
        /* 8 */"AUDIO_CHANNEL_INDEX_MASK_8",
    };

    const bool isOutProfile = profile->direction == PCM_OUT;

    const char * const * const chans_strs = isOutProfile ? out_chans_strs : in_chans_strs;
    const size_t chans_strs_size =
            isOutProfile ? ARRAY_SIZE(out_chans_strs) : ARRAY_SIZE(in_chans_strs);

    /*
     * If we assume each channel string is 26 chars ("AUDIO_CHANNEL_INDEX_MASK_8" is 26) + 1 for,
     * the "|" delimiter, then we allocate room for 16 strings.
     */
    char buffer[27 * 16 + 1]; /* caution, may need to be expanded */
    buffer[0] = '\0';
    size_t buffSize = ARRAY_SIZE(buffer);
    size_t curStrLen = 0;

    /* We currently support MONO and STEREO, and always report STEREO but some (many)
     * USB Audio Devices may only announce support for MONO (a headset mic for example), or
     * The total number of output channels. SO, if the device itself doesn't explicitly
     * support STEREO, append to the channel config strings we are generating.
     *
     * The MONO and STEREO positional channel masks are provided for legacy compatibility.
     * For multichannel (n > 2) we only expose channel index masks.
     */
    // Always support stereo
    curStrLen = strlcat(buffer, chans_strs[2], buffSize);

    size_t index;
    unsigned channel_count;
    for (index = 0;
         (channel_count = profile->channel_counts[index]) != 0;
         index++) {

        /* we only show positional information for mono (stereo handled already) */
        if (channel_count < chans_strs_size
                && chans_strs[channel_count] != NULL
                && channel_count < 2 /* positional only for fewer than 2 channels */) {
            // account for the '|' and the '\0'
            if (buffSize - curStrLen < strlen(chans_strs[channel_count]) + 2) {
                /* we don't have room for another, so bail at this point rather than
                 * return a malformed rate string
                 */
                break;
            }

            strlcat(buffer, "|", buffSize);
            curStrLen = strlcat(buffer, chans_strs[channel_count], buffSize);
        }

        // handle channel index masks for both input and output
        // +2 to account for the '|' and the '\0'
         if (buffSize - curStrLen < strlen(index_chans_strs[channel_count]) + 2) {
             /* we don't have room for another, so bail at this point rather than
              * return a malformed rate string
              */
             break;
         }

         strlcat(buffer, "|", buffSize);
         curStrLen = strlcat(buffer, index_chans_strs[channel_count], buffSize);
    }

    return strdup(buffer);
}

void profile_dump(const alsa_device_profile* profile, int fd)
{
    if (profile == NULL) {
        dprintf(fd, "  %s\n", "No USB Profile");
        return; /* bail early */
    }

    if (!profile->is_valid) {
        dprintf(fd, "  Profile is INVALID");
    }

    /* card/device/direction */
    dprintf(fd, "  card:%d, device:%d - %s\n",
                profile->card, profile->device, profile->direction == PCM_OUT ? "OUT" : "IN");

    /* formats */
    dprintf(fd, "  Formats: ");
    for (int fmtIndex = 0;
          profile->formats[fmtIndex] != PCM_FORMAT_INVALID && fmtIndex < MAX_PROFILE_FORMATS;
          fmtIndex++) {
        dprintf(fd, "%d ", profile->formats[fmtIndex]);
    }
    dprintf(fd, "\n");

    /* sample rates */
    dprintf(fd, "  Rates: ");
    for (int rateIndex = 0;
          profile->sample_rates[rateIndex] != 0 && rateIndex < MAX_PROFILE_SAMPLE_RATES;
          rateIndex++) {
        dprintf(fd, "%u ", profile->sample_rates[rateIndex]);
    }
    dprintf(fd, "\n");

    // channel counts
    dprintf(fd, "  Channel Counts: ");
    for (int cntIndex = 0;
          profile->channel_counts[cntIndex] != 0 && cntIndex < MAX_PROFILE_CHANNEL_COUNTS;
          cntIndex++) {
        dprintf(fd, "%u ", profile->channel_counts[cntIndex]);
    }
    dprintf(fd, "\n");

    dprintf(fd, "  min/max period size [%u : %u]\n",
            profile->min_period_size,profile-> max_period_size);
    dprintf(fd, "  min/max channel count [%u : %u]\n",
            profile->min_channel_count, profile->max_channel_count);

    // struct pcm_config default_config;
    dprintf(fd, "  Default Config:\n");
    dprintf(fd, "    channels: %d\n", profile->default_config.channels);
    dprintf(fd, "    rate: %d\n", profile->default_config.rate);
    dprintf(fd, "    period_size: %d\n", profile->default_config.period_size);
    dprintf(fd, "    period_count: %d\n", profile->default_config.period_count);
    dprintf(fd, "    format: %d\n", profile->default_config.format);
}
