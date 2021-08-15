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

#ifndef ANDROID_RADIO_METADATA_HIDDEN_H
#define ANDROID_RADIO_METADATA_HIDDEN_H

#include <stdbool.h>
#include <system/radio.h>
#include <system/radio_metadata.h>

/* default size allocated for a metadata buffer in 32 bits units */
#define RADIO_METADATA_DEFAULT_SIZE 64
/* maximum size allocated for a metadata buffer in 32 bits units */
#define RADIO_METADATA_MAX_SIZE (RADIO_METADATA_DEFAULT_SIZE << 12)

/* meta data entry in a meta data buffer */
typedef struct radio_metadata_entry {
    radio_metadata_key_t    key;
    radio_metadata_type_t   type;
    uint32_t                size;
    uint8_t                 data[];
} radio_metadata_entry_t;


/**
* meta data buffer layout:
*
*   |    <---  32 bit   --->    |
*   |---------------------------|
*   | channel                   |
*   |---------------------------|
*   | sub_channel               |
*   |---------------------------|
*   | size_int                  | total size in 32 bit units including header and index
*   |---------------------------|
*   | count                     | number of entries
*   |---------------------------|<--+
*   | first entry               |   |
*   |                           |   |
*   |---------------------------|<+ |
*   | second entry              | | |
*   |                           | | |
*   |                           | | |
*   |---------------------------| | |
*   |     :                     | | |
*   |---------------------------| | |       \
*   | offset of next free space | | |       |
*   |---------------------------| | |       |
*   |     :                     | | |       |
*   |---------------------------| | |       >  index
*   | offset of second entry    |-+ |       |
*   |---------------------------|   |       |
*   | offset of first entry     |---+       |
*   |---------------------------|           /
*
*   A radio meta data buffer is allocated with radio_metadata_allocate() and released with
*   radio_metadata_deallocate().
*   Meta data entries are added with radio_metadata_add_xxx() where xxx is int, text or raw.
*   The buffer is allocated with a default size (RADIO_METADATA_DEFAULT_SIZE entries)
*   by radio_metadata_allocate() and reallocated if needed by radio_metadata_add_xxx()
*/

/* Radio meta data buffer header */
typedef struct radio_metadata_buffer {
    uint32_t channel;       /* channel (frequency) this meta data is associated with */
    uint32_t sub_channel;   /* sub channel this meta data is associated with */
    uint32_t size_int;      /* Total size in 32 bit word units */
    uint32_t count;         /* number of meta data entries */
} radio_metadata_buffer_t;



#endif  // ANDROID_RADIO_METADATA_HIDDEN_H
