/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "chre/platform/shared/nanoapp_support_lib_dso.h"

#include <chre.h>

#include "chre/util/macros.h"

/**
 * @file
 * The Nanoapp Support Library (NSL) that gets built with nanoapps to act as an
 * intermediary to the reference CHRE implementation. It provides hooks so the
 * app can be registered with the system, and also provides a layer where we can
 * implement cross-version compatibility features as needed.
 */

namespace {

#ifdef CHRE_SLPI_UIMG_ENABLED
constexpr int kIsTcmNanoapp = 1;
#else
constexpr int kIsTcmNanoapp = 0;
#endif  // CHRE_SLPI_UIMG_ENABLED

#ifndef CHRE_NANOAPP_DISABLE_BACKCOMPAT
// Return a v1.3+ GnssLocationEvent for the nanoapp when running on a v1.2-
// platform.
chreGnssLocationEvent translateLegacyGnssLocation(
    const chreGnssLocationEvent& legacyEvent) {
  // Copy v1.2- fields over to a v1.3+ event.
  chreGnssLocationEvent newEvent = {};
  newEvent.timestamp = legacyEvent.timestamp;
  newEvent.latitude_deg_e7 = legacyEvent.latitude_deg_e7;
  newEvent.longitude_deg_e7 = legacyEvent.longitude_deg_e7;
  newEvent.altitude = legacyEvent.altitude;
  newEvent.speed = legacyEvent.speed;
  newEvent.bearing = legacyEvent.bearing;
  newEvent.accuracy = legacyEvent.accuracy;
  newEvent.flags = legacyEvent.flags;

  // Unset flags that are defined in v1.3+ but not in v1.2-.
  newEvent.flags &= ~(CHRE_GPS_LOCATION_HAS_ALTITUDE_ACCURACY
                      | CHRE_GPS_LOCATION_HAS_SPEED_ACCURACY
                      | CHRE_GPS_LOCATION_HAS_BEARING_ACCURACY);
  return newEvent;
}

void nanoappHandleEventCompat(uint32_t senderInstanceId, uint16_t eventType,
                              const void *eventData) {
  if (eventType == CHRE_EVENT_GNSS_LOCATION
      && chreGetApiVersion() < CHRE_API_VERSION_1_3) {
    chreGnssLocationEvent event = translateLegacyGnssLocation(
        *static_cast<const chreGnssLocationEvent *>(eventData));
    nanoappHandleEvent(senderInstanceId, eventType, &event);
  } else {
    nanoappHandleEvent(senderInstanceId, eventType, eventData);
  }
}
#endif

}  // anonymous namespace

DLL_EXPORT extern "C" const struct chreNslNanoappInfo _chreNslDsoNanoappInfo = {
  /* magic */ CHRE_NSL_NANOAPP_INFO_MAGIC,
  /* structMinorVersion */ CHRE_NSL_NANOAPP_INFO_STRUCT_MINOR_VERSION,
  /* isSystemNanoapp */ NANOAPP_IS_SYSTEM_NANOAPP,
  /* isTcmNanoapp */ kIsTcmNanoapp,
  /* reservedFlags */ 0,
  /* reserved */ 0,
  /* targetApiVersion */ CHRE_API_VERSION,

  // These values are supplied by the build environment.
  /* vendor */ NANOAPP_VENDOR_STRING,
  /* name */ NANOAPP_NAME_STRING,
  /* appId */ NANOAPP_ID,
  /* appVersion */ NANOAPP_VERSION,
  /* entryPoints */ {
    /* start */ nanoappStart,
#ifndef CHRE_NANOAPP_DISABLE_BACKCOMPAT
    /* handleEvent */ nanoappHandleEventCompat,
#else
    /* handleEvent */ nanoappHandleEvent,
#endif
    /* end */ nanoappEnd,
  },
  /* appVersionString */ NANOAPP_VERSION_STRING,
};

// The code section below provides default implementations for new symbols
// introduced in CHRE API v1.2+ to provide binary compatibility with previous
// CHRE implementations. Note that we don't presently include symbols for v1.1,
// as the current known set of CHRE platforms that use this NSL implementation
// are all v1.1+.
// If a nanoapp knows that it is only targeting the latest platform version, it
// can define the CHRE_NANOAPP_DISABLE_BACKCOMPAT flag, so this indirection will
// be avoided at the expense of a nanoapp not being able to load at all on prior
// implementations.

#ifndef CHRE_NANOAPP_DISABLE_BACKCOMPAT

#include <dlfcn.h>

/**
 * Lazily calls dlsym to find the function pointer for a given function
 * (provided without quotes) in another library (i.e. the CHRE platform DSO),
 * caching and returning the result.
 */
#define CHRE_NSL_LAZY_LOOKUP(functionName) ({         \
    static bool lookupPerformed = false;              \
    static decltype(functionName) *fptr = nullptr;    \
    if (!lookupPerformed) {                           \
      fptr = reinterpret_cast<decltype(fptr)>(        \
          dlsym(RTLD_NEXT, STRINGIFY(functionName))); \
      lookupPerformed = true;                         \
    }                                                 \
    fptr; })

WEAK_SYMBOL
bool chreAudioGetSource(uint32_t handle, struct chreAudioSource *audioSource) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreAudioGetSource);
  return (fptr != nullptr) ? fptr(handle, audioSource) : false;
}

WEAK_SYMBOL
bool chreAudioConfigureSource(uint32_t handle, bool enable,
                              uint64_t bufferDuration,
                              uint64_t deliveryInterval) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreAudioConfigureSource);
  return (fptr != nullptr) ?
      fptr(handle, enable, bufferDuration, deliveryInterval) : false;
}

WEAK_SYMBOL
bool chreAudioGetStatus(uint32_t handle, struct chreAudioSourceStatus *status) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreAudioGetStatus);
  return (fptr != nullptr) ? fptr(handle, status) : false;
}

WEAK_SYMBOL
void chreConfigureHostSleepStateEvents(bool enable) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreConfigureHostSleepStateEvents);
  if (fptr != nullptr) {
    fptr(enable);
  }
}

WEAK_SYMBOL
bool chreIsHostAwake(void) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreIsHostAwake);
  return (fptr != nullptr) ? fptr() : false;
}

WEAK_SYMBOL
bool chreGnssConfigureLocationMonitor(bool enable) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreGnssConfigureLocationMonitor);
  return (fptr != nullptr) ? fptr(enable) : false;
}

WEAK_SYMBOL
bool chreWifiRequestRangingAsync(const struct chreWifiRangingParams *params,
                                 const void *cookie) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreWifiRequestRangingAsync);
  return (fptr != nullptr) ? fptr(params, cookie) : false;
}

WEAK_SYMBOL
bool chreSensorConfigureBiasEvents(uint32_t sensorHandle, bool enable) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreSensorConfigureBiasEvents);
  return (fptr != nullptr) ? fptr(sensorHandle, enable) : false;
}

WEAK_SYMBOL
bool chreSensorGetThreeAxisBias(uint32_t sensorHandle,
                                struct chreSensorThreeAxisData *bias) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreSensorGetThreeAxisBias);
  return (fptr != nullptr) ? fptr(sensorHandle, bias) : false;
}

WEAK_SYMBOL
bool chreSensorFlushAsync(uint32_t sensorHandle, const void *cookie) {
  auto *fptr = CHRE_NSL_LAZY_LOOKUP(chreSensorFlushAsync);
  return (fptr != nullptr) ? fptr(sensorHandle, cookie) : false;
}

#endif  // CHRE_NANOAPP_DISABLE_BACKCOMPAT
