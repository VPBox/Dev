/*
 * Copyright (C) 2017 The Android Open Source Project
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

/*******************************************************************************
 *                          ~ ~ ~ W A R N I N G ~ ~ ~
 *
 * The following code is used to load a nanoapp into a Qualcomm implementation
 * of the CHRE API that is based on Nanohub. This is not intended as a reference
 * for future platforms and is provided for backwards compatibility with this
 * implementation.
 *
 * You may find more suitable examples of app support libraries under the
 * build/app_support directory for other variants. These files are typically
 * designed to perform early initialization of the CHRE nanoapp and may be
 * required by some platforms but not all.
 *
 ******************************************************************************/

#include <chre.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LEGACY_APP_HDR_MAGIC_ARRAY \
    {'G', 'o', 'o', 'g', 'l', 'e', 'N', 'a', 'n', 'o', 'A', 'p', 'p'}

#define APP_HDR_VER_CUR            0
#define APP_HDR_MARKER_INTERNAL    0xFF01

#define EVT_APP_FROM_HOST                0x000000F8
#define EVT_APP_TIMER                    0x000000DF

struct AppFuncs {
    bool (*init)(uint32_t yourTid);
    void (*end)(void);
    void (*handle)(uint32_t evtType, const void *evtData);
};

// This was the old "struct AppHdr" before the binary format was refactored as
// part of b/28265099. It's what Qualcomm's implementation currently expects as
// input when registering an app.
struct LegacyAppHdr {
    char magic[13];
    uint8_t fmtVer;  //app header format version
    uint16_t marker;

    uint64_t appId;

    uint32_t data_start;
    uint32_t data_end;
    uint32_t data_data;

    uint32_t bss_start;
    uint32_t bss_end;

    uint32_t got_start;
    uint32_t got_end;
    uint32_t rel_start;
    uint32_t rel_end;

    uint32_t appVer;
    uint32_t rfu;

    struct AppFuncs funcs;
};

struct TimerEvent {
    uint32_t timerId;
    void *data;
};

// These two functions are specific to Qualcomm's Nanohub platform
// implementation
extern void platSlpiAddInternalApp(const struct LegacyAppHdr *, bool);
extern void platSlpiRemoveInternalApp(const struct LegacyAppHdr *);

static void __appInit(void) __attribute__((constructor));
static void __appEnd(void) __attribute__((destructor));

static bool chreappStart(uint32_t tid);
static void chreappHandle(uint32_t eventTypeAndTid, const void *eventData);

#if !defined(NANOAPP_ID) || !defined(NANOAPP_VERSION)
#error NANOAPP_ID and NANOAPP_VERSION must be defined in the build environment
#endif

static const struct LegacyAppHdr mAppHdr = {
   .magic        = LEGACY_APP_HDR_MAGIC_ARRAY,
   .fmtVer       = APP_HDR_VER_CUR,
   .marker       = APP_HDR_MARKER_INTERNAL,
   .appId        = NANOAPP_ID,
   .appVer       = NANOAPP_VERSION,
   .funcs.init   = chreappStart,
   .funcs.end    = nanoappEnd,
   .funcs.handle = chreappHandle,
};

// Note: this runs when CHRE first loads the Nanoapp. We use it to register the
// app's entry points with the runtime environment.
static void __appInit(void)
{
   platSlpiAddInternalApp(&mAppHdr, false);
}

static void __appEnd(void)
{
   platSlpiRemoveInternalApp(&mAppHdr);
}

static bool chreappStart(uint32_t tid)
{
    return nanoappStart();
}

static void chreappHandle(uint32_t eventTypeAndTid, const void *eventData)
{
    uint16_t evt = eventTypeAndTid;
    uint16_t srcTid = eventTypeAndTid >> 16;
    const void *data = eventData;

    union EventLocalData {
        struct chreMessageFromHostData msg;
    } u;

    switch(evt) {
    case EVT_APP_TIMER:
        evt = CHRE_EVENT_TIMER;
        data = ((struct TimerEvent *)eventData)->data;
        break;
    case EVT_APP_FROM_HOST:
        evt = CHRE_EVENT_MESSAGE_FROM_HOST;
        data = &u.msg;
        u.msg.message = (uint8_t*)eventData + 1;
        // TODO: fill messageType with the correct value once available.
        u.msg.messageType = 0;
        u.msg.messageSize = *(uint8_t*)eventData;
        break;
    }
    nanoappHandleEvent(srcTid, evt, data);
}

#ifdef __cplusplus
}
#endif
