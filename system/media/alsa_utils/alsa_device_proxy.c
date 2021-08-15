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

#define LOG_TAG "alsa_device_proxy"
/*#define LOG_NDEBUG 0*/
/*#define LOG_PCM_PARAMS 0*/

#include <log/log.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <audio_utils/clock.h>

#include "include/alsa_device_proxy.h"

#include "include/alsa_logging.h"

#define DEFAULT_PERIOD_SIZE     1024
#define DEFAULT_PERIOD_COUNT    2

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const unsigned format_byte_size_map[] = {
    2, /* PCM_FORMAT_S16_LE */
    4, /* PCM_FORMAT_S32_LE */
    1, /* PCM_FORMAT_S8 */
    4, /* PCM_FORMAT_S24_LE */
    3, /* PCM_FORMAT_S24_3LE */
};

int proxy_prepare(alsa_device_proxy * proxy, const alsa_device_profile* profile,
                   struct pcm_config * config)
{
    int ret = 0;

    ALOGV("proxy_prepare(c:%d, d:%d)", profile->card, profile->device);

    proxy->profile = profile;

#ifdef LOG_PCM_PARAMS
    log_pcm_config(config, "proxy_setup()");
#endif

    if (config->format != PCM_FORMAT_INVALID && profile_is_format_valid(profile, config->format)) {
        proxy->alsa_config.format = config->format;
    } else {
        proxy->alsa_config.format = profile->default_config.format;
        ALOGW("Invalid format %d - using default %d.",
              config->format, profile->default_config.format);
        // Indicate override when default format was not requested
        if (config->format != PCM_FORMAT_INVALID) {
            ret = -EINVAL;
        }
    }

    if (config->rate != 0 && profile_is_sample_rate_valid(profile, config->rate)) {
        proxy->alsa_config.rate = config->rate;
    } else {
        proxy->alsa_config.rate = profile->default_config.rate;
        ALOGW("Invalid sample rate %u - using default %u.",
              config->rate, profile->default_config.rate);
        // Indicate override when default rate was not requested
        if (config->rate != 0) {
            ret = -EINVAL;
        }
    }

    if (config->channels != 0 && profile_is_channel_count_valid(profile, config->channels)) {
        proxy->alsa_config.channels = config->channels;
    } else {
        proxy->alsa_config.channels = profile_get_closest_channel_count(profile, config->channels);
        ALOGW("Invalid channel count %u - using closest %u.",
              config->channels, proxy->alsa_config.channels);
        // Indicate override when default channel count was not requested
        if (config->channels != 0) {
            ret = -EINVAL;
        }
    }

    proxy->alsa_config.period_count = profile->default_config.period_count;
    proxy->alsa_config.period_size =
            profile_get_period_size(proxy->profile, proxy->alsa_config.rate);

    // Hack for USB accessory audio.
    // Here we set the correct value for period_count if tinyalsa fails to get it from the
    // f_audio_source driver.
    if (proxy->alsa_config.period_count == 0) {
        proxy->alsa_config.period_count = 4;
    }

    proxy->pcm = NULL;
    // config format should be checked earlier against profile.
    if (config->format >= 0 && (size_t)config->format < ARRAY_SIZE(format_byte_size_map)) {
        proxy->frame_size = format_byte_size_map[config->format] * proxy->alsa_config.channels;
    } else {
        proxy->frame_size = 1;
    }

    // let's check to make sure we can ACTUALLY use the maximum rate (with the channel count)
    // Note that profile->sample_rates is sorted highest to lowest, so the scan will get
    // us the highest working rate
    int max_rate_index = proxy_scan_rates(proxy, profile->sample_rates);
    if (max_rate_index >= 0) {
        if (proxy->alsa_config.rate > profile->sample_rates[max_rate_index]) {
            ALOGW("Limiting samplnig rate from %u to %u.",
                  proxy->alsa_config.rate, profile->sample_rates[max_rate_index]);
            proxy->alsa_config.rate = profile->sample_rates[max_rate_index];
            ret = -EINVAL;
        }
    }
    return ret;
}

int proxy_open(alsa_device_proxy * proxy)
{
    const alsa_device_profile* profile = proxy->profile;
    ALOGV("proxy_open(card:%d device:%d %s)", profile->card, profile->device,
          profile->direction == PCM_OUT ? "PCM_OUT" : "PCM_IN");

    if (profile->card < 0 || profile->device < 0) {
        return -EINVAL;
    }

    proxy->pcm = pcm_open(profile->card, profile->device,
            profile->direction | PCM_MONOTONIC, &proxy->alsa_config);
    if (proxy->pcm == NULL) {
        return -ENOMEM;
    }

    if (!pcm_is_ready(proxy->pcm)) {
        ALOGE("  proxy_open() pcm_is_ready() failed: %s", pcm_get_error(proxy->pcm));
#if defined(LOG_PCM_PARAMS)
        log_pcm_config(&proxy->alsa_config, "config");
#endif
        pcm_close(proxy->pcm);
        proxy->pcm = NULL;
        return -ENOMEM;
    }

    return 0;
}

void proxy_close(alsa_device_proxy * proxy)
{
    ALOGV("proxy_close() [pcm:%p]", proxy->pcm);

    if (proxy->pcm != NULL) {
        pcm_close(proxy->pcm);
        proxy->pcm = NULL;
    }
}

/*
 * Sample Rate
 */
unsigned proxy_get_sample_rate(const alsa_device_proxy * proxy)
{
    return proxy->alsa_config.rate;
}

/*
 * Format
 */
enum pcm_format proxy_get_format(const alsa_device_proxy * proxy)
{
    return proxy->alsa_config.format;
}

/*
 * Channel Count
 */
unsigned proxy_get_channel_count(const alsa_device_proxy * proxy)
{
    return proxy->alsa_config.channels;
}

/*
 * Other
 */
unsigned int proxy_get_period_size(const alsa_device_proxy * proxy)
{
    return proxy->alsa_config.period_size;
}

unsigned int proxy_get_period_count(const alsa_device_proxy * proxy)
{
    return proxy->alsa_config.period_count;
}

unsigned proxy_get_latency(const alsa_device_proxy * proxy)
{
    return (proxy_get_period_size(proxy) * proxy_get_period_count(proxy) * 1000)
               / proxy_get_sample_rate(proxy);
}

int proxy_get_presentation_position(const alsa_device_proxy * proxy,
        uint64_t *frames, struct timespec *timestamp)
{
    int ret = -EPERM; // -1
    unsigned int avail;
    if (proxy->pcm != NULL
            && pcm_get_htimestamp(proxy->pcm, &avail, timestamp) == 0) {
        const size_t kernel_buffer_size =
                proxy->alsa_config.period_size * proxy->alsa_config.period_count;
        if (avail > kernel_buffer_size) {
            ALOGE("available frames(%u) > buffer size(%zu)", avail, kernel_buffer_size);
        } else {
            int64_t signed_frames = proxy->transferred - kernel_buffer_size + avail;
            // It is possible to compensate for additional driver and device delay
            // by changing signed_frames.  Example:
            // signed_frames -= 20 /* ms */ * proxy->alsa_config.rate / 1000;
            if (signed_frames >= 0) {
                *frames = signed_frames;
                ret = 0;
            }
        }
    }
    return ret;
}

int proxy_get_capture_position(const alsa_device_proxy * proxy,
        int64_t *frames, int64_t *time)
{
    int ret = -ENOSYS;
    unsigned int avail;
    struct timespec timestamp;
    // TODO: add logging for tinyalsa errors.
    if (proxy->pcm != NULL
            && pcm_get_htimestamp(proxy->pcm, &avail, &timestamp) == 0) {
        const size_t kernel_buffer_size =
                proxy->alsa_config.period_size * proxy->alsa_config.period_count;
        if (avail > kernel_buffer_size) {
            ALOGE("available frames(%u) > buffer size(%zu)", avail, kernel_buffer_size);
        } else {
            *frames = proxy->transferred + avail;
            *time = audio_utils_ns_from_timespec(&timestamp);
            ret = 0;
        }
    }
    return ret;
}

/*
 * I/O
 */
int proxy_write(alsa_device_proxy * proxy, const void *data, unsigned int count)
{
    int ret = pcm_write(proxy->pcm, data, count);
    if (ret == 0) {
        proxy->transferred += count / proxy->frame_size;
    }
    return ret;
}

int proxy_read(alsa_device_proxy * proxy, void *data, unsigned int count)
{
    int ret = pcm_read(proxy->pcm, data, count);
    if (ret == 0) {
        proxy->transferred += count / proxy->frame_size;
    }
    return ret;
}

/*
 * Debugging
 */
void proxy_dump(const alsa_device_proxy* proxy, int fd)
{
    if (proxy != NULL) {
        dprintf(fd, "  channels: %d\n", proxy->alsa_config.channels);
        dprintf(fd, "  rate: %d\n", proxy->alsa_config.rate);
        dprintf(fd, "  period_size: %d\n", proxy->alsa_config.period_size);
        dprintf(fd, "  period_count: %d\n", proxy->alsa_config.period_count);
        dprintf(fd, "  format: %d\n", proxy->alsa_config.format);
    }
}

int proxy_scan_rates(alsa_device_proxy * proxy, const unsigned sample_rates[]) {
    const alsa_device_profile* profile = proxy->profile;
    if (profile->card < 0 || profile->device < 0) {
        return -EINVAL;
    }

    struct pcm_config alsa_config;
    memcpy(&alsa_config, &proxy->alsa_config, sizeof(alsa_config));

    struct pcm * alsa_pcm;
    int rate_index = 0;
    while (sample_rates[rate_index] != 0) {
        alsa_config.rate = sample_rates[rate_index];
        alsa_pcm = pcm_open(profile->card, profile->device,
                profile->direction | PCM_MONOTONIC, &alsa_config);
        if (alsa_pcm != NULL) {
            if (pcm_is_ready(alsa_pcm)) {
                pcm_close(alsa_pcm);
                return rate_index;
            }

            pcm_close(alsa_pcm);
        }

        rate_index++;
    }

    return -EINVAL;
}
