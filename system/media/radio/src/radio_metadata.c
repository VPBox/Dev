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

#define LOG_TAG "radio_metadata"
/*#define LOG_NDEBUG 0*/

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <log/log.h>

#include <system/radio.h>
#include <system/radio_metadata.h>
#include "radio_metadata_hidden.h"

const radio_metadata_type_t metadata_key_type_table[] =
{
    RADIO_METADATA_TYPE_INT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_INT,
    RADIO_METADATA_TYPE_INT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_TEXT,
    RADIO_METADATA_TYPE_RAW,
    RADIO_METADATA_TYPE_RAW,
    RADIO_METADATA_TYPE_CLOCK,
};

/**
 * private functions
 */

bool is_valid_metadata_key(const radio_metadata_key_t key)
{
    if (key < RADIO_METADATA_KEY_MIN || key > RADIO_METADATA_KEY_MAX) {
        return false;
    }
    return true;
}

int check_size(radio_metadata_buffer_t **metadata_ptr, const uint32_t size_int)
{
    radio_metadata_buffer_t *metadata = *metadata_ptr;
    uint32_t index_offset = metadata->size_int - metadata->count - 1;
    uint32_t data_offset = *((uint32_t *)metadata + index_offset);
    uint32_t req_size_int;
    uint32_t new_size_int;

    LOG_ALWAYS_FATAL_IF(metadata->size_int < (metadata->count + 1),
                        "%s: invalid size %u", __func__, metadata->size_int);
    if (size_int == 0) {
        return 0;
    }

    req_size_int = data_offset + metadata->count + 1 + 1 + size_int;
    /* do not grow buffer if it can accommodate the new entry plus an additional index entry */

    if (req_size_int <= metadata->size_int) {
        return 0;
    }

    if (req_size_int > RADIO_METADATA_MAX_SIZE || metadata->size_int >= RADIO_METADATA_MAX_SIZE) {
        return -ENOMEM;
    }
    /* grow meta data buffer by a factor of 2 until new data fits */
    new_size_int = metadata->size_int;
    while (new_size_int < req_size_int)
        new_size_int *= 2;

    ALOGV("%s growing from %u to %u", __func__, metadata->size_int, new_size_int);
    metadata = realloc(metadata, new_size_int * sizeof(uint32_t));
    if (metadata == NULL) {
        return -ENOMEM;
    }
    /* move index table */
    memmove((uint32_t *)metadata + new_size_int - (metadata->count + 1),
            (uint32_t *)metadata + metadata->size_int - (metadata->count + 1),
            (metadata->count + 1) * sizeof(uint32_t));
    metadata->size_int = new_size_int;

    *metadata_ptr = metadata;
    return 0;
}

/* checks on size and key validity are done before calling this function */
int add_metadata(radio_metadata_buffer_t **metadata_ptr,
                 const radio_metadata_key_t key,
                 const radio_metadata_type_t type,
                 const void *value,
                 const size_t size)
{
    uint32_t entry_size_int;
    int ret;
    radio_metadata_entry_t *entry;
    uint32_t index_offset;
    uint32_t data_offset;
    radio_metadata_buffer_t *metadata = *metadata_ptr;

    entry_size_int = (uint32_t)(size + sizeof(radio_metadata_entry_t));
    entry_size_int = (entry_size_int + sizeof(uint32_t) - 1) / sizeof(uint32_t);

    ret = check_size(metadata_ptr, entry_size_int);
    if (ret < 0) {
        return ret;
    }
    metadata = *metadata_ptr;
    index_offset = metadata->size_int - metadata->count - 1;
    data_offset = *((uint32_t *)metadata + index_offset);

    entry = (radio_metadata_entry_t *)((uint32_t *)metadata + data_offset);
    entry->key = key;
    entry->type = type;
    entry->size = (uint32_t)size;
    memcpy(entry->data, value, size);

    data_offset += entry_size_int;
    *((uint32_t *)metadata + index_offset -1) = data_offset;
    metadata->count++;

    return 0;
}

radio_metadata_entry_t *get_entry_at_index(
                                    const radio_metadata_buffer_t *metadata,
                                    const unsigned index,
                                    bool check)
{
    uint32_t index_offset = metadata->size_int - index - 1;
    uint32_t data_offset = *((uint32_t *)metadata + index_offset);

    LOG_ALWAYS_FATAL_IF(metadata->size_int < (index + 1),
                        "%s: invalid size %u", __func__, metadata->size_int);
    if (check) {
        if (index >= metadata->count) {
            return NULL;
        }
        uint32_t min_offset;
        uint32_t max_offset;
        uint32_t min_entry_size_int;
        min_offset = (sizeof(radio_metadata_buffer_t) + sizeof(uint32_t) - 1) /
                        sizeof(uint32_t);
        if (data_offset < min_offset) {
            return NULL;
        }
        min_entry_size_int = 1 + sizeof(radio_metadata_entry_t);
        min_entry_size_int = (min_entry_size_int + sizeof(uint32_t) - 1) / sizeof(uint32_t);

        LOG_ALWAYS_FATAL_IF(metadata->size_int < (metadata->count + 1),
                            "%s: invalid size %u vs count %u", __func__,
                            metadata->size_int, metadata->count);

        max_offset = metadata->size_int - metadata->count - 1 - min_entry_size_int;
        if (data_offset > max_offset) {
            return NULL;
        }
    }
    return (radio_metadata_entry_t *)((uint32_t *)metadata + data_offset);
}

/**
 * metadata API functions
 */

radio_metadata_type_t radio_metadata_type_of_key(const radio_metadata_key_t key)
{
    if (!is_valid_metadata_key(key)) {
        return RADIO_METADATA_TYPE_INVALID;
    }
    return metadata_key_type_table[key - RADIO_METADATA_KEY_MIN];
}

int radio_metadata_allocate(radio_metadata_t **metadata,
                            const uint32_t channel,
                            const uint32_t sub_channel)
{
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)calloc(RADIO_METADATA_DEFAULT_SIZE, sizeof(uint32_t));
    if (metadata_buf == NULL) {
        return -ENOMEM;
    }

    metadata_buf->channel = channel;
    metadata_buf->sub_channel = sub_channel;
    metadata_buf->size_int = RADIO_METADATA_DEFAULT_SIZE;
    *((uint32_t *)metadata_buf + RADIO_METADATA_DEFAULT_SIZE - 1) =
            (sizeof(radio_metadata_buffer_t) + sizeof(uint32_t) - 1) /
                sizeof(uint32_t);
    *metadata = (radio_metadata_t *)metadata_buf;
    return 0;
}

void radio_metadata_deallocate(radio_metadata_t *metadata)
{
    free(metadata);
}

int radio_metadata_add_int(radio_metadata_t **metadata,
                           const radio_metadata_key_t key,
                           const int32_t value)
{
    radio_metadata_type_t type = radio_metadata_type_of_key(key);
    if (metadata == NULL || *metadata == NULL || type != RADIO_METADATA_TYPE_INT) {
        return -EINVAL;
    }
    return add_metadata((radio_metadata_buffer_t **)metadata,
                        key, type, &value, sizeof(int32_t));
}

int radio_metadata_add_text(radio_metadata_t **metadata,
                            const radio_metadata_key_t key,
                            const char *value)
{
    radio_metadata_type_t type = radio_metadata_type_of_key(key);
    if (metadata == NULL || *metadata == NULL || type != RADIO_METADATA_TYPE_TEXT ||
            value == NULL || strlen(value) >= RADIO_METADATA_TEXT_LEN_MAX) {
        return -EINVAL;
    }
    return add_metadata((radio_metadata_buffer_t **)metadata, key, type, value, strlen(value) + 1);
}

int radio_metadata_add_raw(radio_metadata_t **metadata,
                           const radio_metadata_key_t key,
                           const unsigned char *value,
                           const size_t size)
{
    radio_metadata_type_t type = radio_metadata_type_of_key(key);
    if (metadata == NULL || *metadata == NULL || type != RADIO_METADATA_TYPE_RAW || value == NULL) {
        return -EINVAL;
    }
    return add_metadata((radio_metadata_buffer_t **)metadata, key, type, value, size);
}

int radio_metadata_add_clock(radio_metadata_t **metadata,
                             const radio_metadata_key_t key,
                             const radio_metadata_clock_t *clock) {
    radio_metadata_type_t type = radio_metadata_type_of_key(key);
    if (metadata == NULL || *metadata == NULL || type != RADIO_METADATA_TYPE_CLOCK ||
        clock == NULL || clock->timezone_offset_in_minutes < (-12 * 60) ||
        clock->timezone_offset_in_minutes > (14 * 60)) {
        return -EINVAL;
    }
    return add_metadata(
        (radio_metadata_buffer_t **)metadata, key, type, clock, sizeof(radio_metadata_clock_t));
}

int radio_metadata_add_metadata(radio_metadata_t **dst_metadata,
                           radio_metadata_t *src_metadata)
{
    radio_metadata_buffer_t *src_metadata_buf = (radio_metadata_buffer_t *)src_metadata;
    radio_metadata_buffer_t *dst_metadata_buf;
    int status;
    uint32_t index;

    if (dst_metadata == NULL || src_metadata == NULL) {
        return -EINVAL;
    }
    if (*dst_metadata == NULL) {
        status = radio_metadata_allocate(dst_metadata, src_metadata_buf->channel,
                                src_metadata_buf->sub_channel);
        if (status != 0) {
            return status;
        }
    }

    dst_metadata_buf = (radio_metadata_buffer_t *)*dst_metadata;
    dst_metadata_buf->channel = src_metadata_buf->channel;
    dst_metadata_buf->sub_channel = src_metadata_buf->sub_channel;

    for (index = 0; index < src_metadata_buf->count; index++) {
        radio_metadata_key_t key;
        radio_metadata_type_t type;
        void *value;
        size_t size;
        status = radio_metadata_get_at_index(src_metadata, index, &key, &type, &value, &size);
        if (status != 0)
            continue;
        status = add_metadata((radio_metadata_buffer_t **)dst_metadata, key, type, value, size);
        if (status != 0)
            break;
    }
    return status;
}

int radio_metadata_check(const radio_metadata_t *metadata)
{
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;
    uint32_t count;
    uint32_t min_entry_size_int;

    if (metadata_buf == NULL) {
        return -EINVAL;
    }

    if (metadata_buf->size_int > RADIO_METADATA_MAX_SIZE) {
        return -EINVAL;
    }

    /* sanity check on entry count versus buffer size */
    min_entry_size_int = 1 + sizeof(radio_metadata_entry_t);
    min_entry_size_int = (min_entry_size_int + sizeof(uint32_t) - 1) /
                                sizeof(uint32_t);
    if ((metadata_buf->count * min_entry_size_int + metadata_buf->count + 1 +
            (sizeof(radio_metadata_buffer_t) + sizeof(uint32_t) - 1) / sizeof(uint32_t)) >
                    metadata_buf->size_int) {
        return -EINVAL;
    }

    /* sanity check on each entry */
    for (count = 0; count < metadata_buf->count; count++) {
        radio_metadata_entry_t *entry = get_entry_at_index(metadata_buf, count, true);
        radio_metadata_entry_t *next_entry;
        if (entry == NULL) {
            return -EINVAL;
        }
        if (!is_valid_metadata_key(entry->key)) {
            return -EINVAL;
        }
        if (entry->type != radio_metadata_type_of_key(entry->key)) {
            return -EINVAL;
        }

        /* do not request check because next entry can be the free slot */
        next_entry = get_entry_at_index(metadata_buf, count + 1, false);
        if ((char *)entry->data + entry->size > (char *)next_entry) {
            return -EINVAL;
        }
    }

    return 0;
}

size_t radio_metadata_get_size(const radio_metadata_t *metadata)
{
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;

    if (metadata_buf == NULL) {
        return 0;
    }
    return metadata_buf->size_int * sizeof(uint32_t);
}

int radio_metadata_get_count(const radio_metadata_t *metadata)
{
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;

    if (metadata_buf == NULL) {
        return -EINVAL;
    }
    return (int)metadata_buf->count;
}

int radio_metadata_get_at_index(const radio_metadata_t *metadata,
                                const uint32_t index,
                                radio_metadata_key_t *key,
                                radio_metadata_type_t *type,
                                void **value,
                                size_t *size)
{
    radio_metadata_entry_t *entry;
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;

    if (metadata_buf == NULL || key == NULL || type == NULL ||
            value == NULL || size == NULL) {
        return -EINVAL;
    }
    if (index >= metadata_buf->count) {
        return -EINVAL;
    }

    entry = get_entry_at_index(metadata_buf, index, false);
    *key = entry->key;
    *type = entry->type;
    *value = (void *)entry->data;
    *size = (size_t)entry->size;

    return 0;
}

int radio_metadata_get_from_key(const radio_metadata_t *metadata,
                                const radio_metadata_key_t key,
                                radio_metadata_type_t *type,
                                void **value,
                                size_t *size)
{
    uint32_t count;
    radio_metadata_entry_t *entry = NULL;
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;

    if (metadata_buf == NULL || type == NULL || value == NULL || size == NULL) {
        return -EINVAL;
    }
    if (!is_valid_metadata_key(key)) {
        return -EINVAL;
    }

    for (count = 0; count < metadata_buf->count; entry = NULL, count++) {
        entry = get_entry_at_index(metadata_buf, count, false);
        if (entry->key == key) {
            break;
        }
    }
    if (entry == NULL) {
        return -ENOENT;
    }
    *type = entry->type;
    *value = (void *)entry->data;
    *size = (size_t)entry->size;
    return 0;
}

int radio_metadata_get_channel(radio_metadata_t *metadata,
                               uint32_t *channel,
                               uint32_t *sub_channel)
{
    radio_metadata_buffer_t *metadata_buf =
            (radio_metadata_buffer_t *)metadata;

    if (metadata_buf == NULL || channel == NULL || sub_channel == NULL) {
        return -EINVAL;
    }
    *channel = metadata_buf->channel;
    *sub_channel = metadata_buf->sub_channel;
    return 0;
}
