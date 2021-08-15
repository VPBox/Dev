/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef ANDROID_RADIO_METADATA_H
#define ANDROID_RADIO_METADATA_H

#include <stdbool.h>
#include <cutils/compiler.h>
#include <system/radio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* maximum length for text metadata including NUL terminator */
#define RADIO_METADATA_TEXT_LEN_MAX 1024

/* radio meta data key values */
enum {
    RADIO_METADATA_KEY_INVALID      = -1,
    RADIO_METADATA_KEY_RDS_PI       = 0,      /* RDS PI                 - int  */
    RADIO_METADATA_KEY_RDS_PS       = 1,      /* RDS PS                 - text */
    RADIO_METADATA_KEY_RDS_PTY      = 2,      /* RDS PTY                - int  */
    RADIO_METADATA_KEY_RBDS_PTY     = 3,      /* RBDS PTY               - int  */
    RADIO_METADATA_KEY_RDS_RT       = 4,      /* RDS RT                 - text  */
    RADIO_METADATA_KEY_TITLE        = 5,      /* Song title             - text  */
    RADIO_METADATA_KEY_ARTIST       = 6,      /* Artist name            - text  */
    RADIO_METADATA_KEY_ALBUM        = 7,      /* Album name             - text  */
    RADIO_METADATA_KEY_GENRE        = 8,      /* Musical genre          - text  */
    RADIO_METADATA_KEY_ICON         = 9,      /* Station icon           - raw  */
    RADIO_METADATA_KEY_ART          = 10,     /* Album art              - raw  */
    RADIO_METADATA_KEY_CLOCK        = 11,     /* Clock                  - radio_metadata_clock_t */
    RADIO_METADATA_KEY_MIN          = RADIO_METADATA_KEY_RDS_PI,
    RADIO_METADATA_KEY_MAX          = RADIO_METADATA_KEY_CLOCK,
};
typedef int32_t radio_metadata_key_t;

enum {
    RADIO_METADATA_TYPE_INVALID    = -1,
    RADIO_METADATA_TYPE_INT        = 0,      /* signed 32 bit integer  */
    RADIO_METADATA_TYPE_TEXT       = 1,      /* text in UTF-8 format, NUL terminated.
                                                RADIO_METADATA_TEXT_LEN_MAX length including NUL. */
    RADIO_METADATA_TYPE_RAW        = 2,      /* raw binary data (icon or art) */
    RADIO_METADATA_TYPE_CLOCK      = 3,      /* clock data, see radio_metadata_clock_t */
};
typedef int32_t radio_metadata_type_t;

typedef struct radio_metadata_clock {
    uint64_t utc_seconds_since_epoch;            /* Seconds since epoch at GMT + 0. */
    int32_t timezone_offset_in_minutes;       /* Minutes offset from the GMT. */
} radio_metadata_clock_t;

/*
 * Return the type of the meta data corresponding to the key specified
 *
 * arguments:
 * - key: the meta data key.
 *
 * returns:
 *  the meta data type corresponding to the key or RADIO_METADATA_TYPE_INVALID
 */
ANDROID_API
radio_metadata_type_t radio_metadata_type_of_key(const radio_metadata_key_t key);

/*
 * Allocate a meta data buffer for use by radio HAL callback for RADIO_EVENT_TUNED and
 * RADIO_EVENT_METADATA events.
 *
 * arguments:
 * - metadata: the address where the allocate meta data buffer should be returned.
 * - channel: channel (frequency) this meta data is associated with.
 * - sub_channel: sub channel this meta data is associated with.
 *
 * returns:
 *  0 if successfully allocated
 *  -ENOMEM if meta data buffer cannot be allocated
 */
ANDROID_API
int radio_metadata_allocate(radio_metadata_t **metadata,
                            const uint32_t channel,
                            const uint32_t sub_channel);

/*
 * De-allocate a meta data buffer.
 *
 * arguments:
 * - metadata: the meta data buffer to be de-allocated.
 */
ANDROID_API
void radio_metadata_deallocate(radio_metadata_t *metadata);

/*
 * Add an integer meta data to the buffer.
 *
 * arguments:
 * - metadata: the address of the meta data buffer. I/O. the meta data can be modified if the
 * buffer is re-allocated
 * - key: the meta data key.
 * - value: the meta data value.
 *
 * returns:
 *  0 if successfully added
 *  -EINVAL if the buffer passed is invalid or the key does not match an integer type
 *  -ENOMEM if meta data buffer cannot be re-allocated
 */
ANDROID_API
int radio_metadata_add_int(radio_metadata_t **metadata,
                           const radio_metadata_key_t key,
                           const int32_t value);

/*
 * Add an text meta data to the buffer.
 *
 * arguments:
 * - metadata: the address of the meta data buffer. I/O. the meta data can be modified if the
 * buffer is re-allocated
 * - key: the meta data key.
 * - value: the meta data value.
 *
 * returns:
 *  0 if successfully added
 *  -EINVAL if the buffer passed is invalid or the key does not match a text type or text
 *  is too long
 *  -ENOMEM if meta data buffer cannot be re-allocated
 */
ANDROID_API
int radio_metadata_add_text(radio_metadata_t **metadata,
                            const radio_metadata_key_t key,
                            const char *value);

/*
 * Add an raw meta data to the buffer.
 *
 * arguments:
 * - metadata: the address of the meta data buffer. I/O. the meta data can be modified if the
 * buffer is re-allocated
 * - key: the meta data key.
 * - value: the meta data value.
 *
 * returns:
 *  0 if successfully added
 *  -EINVAL if the buffer passed is invalid or the key does not match a raw type
 *  -ENOMEM if meta data buffer cannot be re-allocated
 */
ANDROID_API
int radio_metadata_add_raw(radio_metadata_t **metadata,
                           const radio_metadata_key_t key,
                           const unsigned char *value,
                           const size_t size);

/*
 * Add a clock meta data to the buffer.
 *
 * arguments:
 * - metadata: the address of the meta data buffer. I/O. the meta data can be modified if the
 * buffer is re-allocated.
 * - key: the meta data key.
 * - value: the meta data value.
 *
 * returns:
 * 0 if successfully added
 * - EINVAL if the buffer passed is invalid or the key does not match a raw type
 * - ENOMEM if meta data buffer cannot be re-allocated
 */
ANDROID_API
int radio_metadata_add_clock(radio_metadata_t **metadata,
                             const radio_metadata_key_t key,
                             const radio_metadata_clock_t *clock);

/*
 * add all meta data in source buffer to destinaiton buffer.
 *
 * arguments:
 * - dst_metadata: the address of the destination meta data buffer. if *dst_metadata is NULL,
 * a new buffer is created.
 * - src_metadata: the source meta data buffer.
 *
 * returns:
 *  0 if successfully added
 *  -ENOMEM if meta data buffer cannot be re-allocated
 */
ANDROID_API
int radio_metadata_add_metadata(radio_metadata_t **dst_metadata,
                           radio_metadata_t *src_metadata);

/*
 * Perform sanity check on a meta data buffer.
 *
 * arguments:
 * - metadata: the meta data buffer.
 *
 * returns:
 *  0 if no error found
 *  -EINVAL if a consistency problem is found in the meta data buffer
 */
ANDROID_API
int radio_metadata_check(const radio_metadata_t *metadata);

/*
 * Return the total size used by the meta data buffer.
 * No sanity check is performed on the meta data buffer.
 *
 * arguments:
 * - metadata: the meta data buffer.
 *
 * returns:
 *  0 if an invalid meta data buffer is passed
 *  the size in bytes otherwise
 */
ANDROID_API
size_t radio_metadata_get_size(const radio_metadata_t *metadata);

/*
 * Return the number of meta data entries in the buffer.
 * No sanity check is performed on the meta data buffer.
 *
 * arguments:
 * - metadata: the meta data buffer.
 *
 * returns:
 *  -EINVAL if an invalid meta data buffer is passed
 *  the number of entries otherwise
 */
ANDROID_API
int radio_metadata_get_count(const radio_metadata_t *metadata);

/*
 * Get a meta data at a specified index. Used to parse a meta data buffer.
 * No sanity check is performed on the meta data buffer.
 *
 * arguments:
 * - metadata: the meta data buffer.
 * - index: the index to read from
 * - key: where the meta data key should be returned
 * - type: where the meta data type should be returned
 * - value: where the address of the meta data value should be returned
 * - size: where the size of the meta data value should be returned
 *
 * returns:
 *  -EINVAL if an invalid argument is passed
 *  0 otherwise
 */
ANDROID_API
int radio_metadata_get_at_index(const radio_metadata_t *metadata,
                                const uint32_t index,
                                radio_metadata_key_t *key,
                                radio_metadata_type_t *type,
                                void **value,
                                size_t *size);

/*
 * Get a meta data with the specified key.
 * No sanity check is performed on the meta data buffer.
 * This will return the first meta data found with the matching key.
 *
 * arguments:
 * - metadata: the meta data buffer.
 * - index: the index to read from
 * - key: the meta data key to look for
 * - type: where the meta data type should be returned
 * - value: where the address of the meta data value should be returned
 * - size: where the size of the meta data value should be returned
 *
 * returns:
 *  -EINVAL if an invalid argument is passed
 *  -ENOENT if no entry with the specified key is found
 *  0 otherwise
 */
ANDROID_API
int radio_metadata_get_from_key(const radio_metadata_t *metadata,
                                const radio_metadata_key_t key,
                                radio_metadata_type_t *type,
                                void **value,
                                size_t *size);

/*
 * Get channel and sub channel associated with metadata.
 *
 * arguments:
 * - metadata: the meta data buffer
 * - channel: address where to return the channel.
 * - sub_channel: address where to return the sub channel.
 *
 * returns:
 *  0 if successfully added
 *  -EINVAL if the buffer passed is invalid
 */
ANDROID_API
int radio_metadata_get_channel(radio_metadata_t *metadata,
                               uint32_t *channel,
                               uint32_t *sub_channel);

#ifdef __cplusplus
}
#endif

#endif  // ANDROID_RADIO_METADATA_H
