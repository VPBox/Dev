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

/**
 * @file
 * The daemon that hosts CHRE on a hexagon DSP via FastRPC. This is typically
 * the SLPI but could be the ADSP or another DSP that supports FastRPC.
 *
 * Several threads are required for this functionality:
 *   - Main thread: blocked waiting on SIGINT/SIGTERM, and requests graceful
 *     shutdown of CHRE when caught
 *   - Monitor thread: persistently blocked in a FastRPC call to the DSP that
 *     only returns when CHRE exits or the DSP crashes
 *     - TODO: see whether we can merge this with the RX thread
 *   - Reverse monitor thread: after initializing the DSP-side monitor for this
 *     process, blocks on a condition variable. If this thread exits, CHRE on
 *     the DSP side will be notified and shut down (this is only possible if
 *     this thread is not blocked in a FastRPC call).
 *     - TODO: confirm this and see whether we can merge this responsibility
 *       into the TX thread
 *   - Message to host (RX) thread: blocks in FastRPC call, waiting on incoming
 *     message from CHRE
 *   - Message to CHRE (TX) thread: blocks waiting on outbound queue, delivers
 *     messages to CHRE over FastRPC
 *
 * TODO: This file originated from an implementation for another device, and was
 * written in C, but then it was converted to C++ when adding socket support. It
 * should be fully converted to C++.
 */

// Disable verbose logging
// TODO: use property_get_bool to make verbose logging runtime configurable
// #define LOG_NDEBUG 0

#include <ctype.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <string>
#include <queue>

#include "chre/platform/slpi/fastrpc.h"
#include "chre_host/log.h"
#include "chre_host/host_protocol_host.h"
#include "chre_host/socket_server.h"
#include "generated/chre_slpi.h"

#include <json/json.h>
#include <utils/SystemClock.h>

#ifdef ADSPRPC
#include "remote.h"

#define ITRANSPORT_PREFIX "'\":;./\\"
#endif  // ADSPRPC

//! The format string to use for logs from the CHRE implementation.
#define HUB_LOG_FORMAT_STR "Hub (t=%.6f): %s"

#ifdef CHRE_DAEMON_LPMA_ENABLED
#include <android/hardware/soundtrigger/2.0/ISoundTriggerHw.h>
#include <hardware_legacy/power.h>

using android::sp;
using android::wp;
using android::hardware::Return;
using android::hardware::soundtrigger::V2_0::ISoundTriggerHw;
using android::hardware::soundtrigger::V2_0::SoundModelHandle;
using android::hardware::soundtrigger::V2_0::SoundModelType;
#endif  // CHRE_DAEMON_LPMA_ENABLED

using android::chre::HostProtocolHost;
using android::chre::FragmentedLoadTransaction;
using android::elapsedRealtimeNano;

// Aliased for consistency with the way these symbols are referenced in
// CHRE-side code
namespace fbs = ::chre::fbs;

typedef void *(thread_entry_point_f)(void *);

struct reverse_monitor_thread_data {
  pthread_t       thread;
  pthread_mutex_t mutex;
  pthread_cond_t  cond;
};

static void *chre_message_to_host_thread(void *arg);
static void *chre_monitor_thread(void *arg);
static void *chre_reverse_monitor_thread(void *arg);
static bool init_reverse_monitor(struct reverse_monitor_thread_data *data);
static bool start_thread(pthread_t *thread_handle,
                         thread_entry_point_f *thread_entry,
                         void *arg);

#ifdef CHRE_DAEMON_LPMA_ENABLED
//! The name of the wakelock to use for the CHRE daemon.
static const char kWakeLockName[] = "chre_daemon";

//! Forward declarations
static void onStHalServiceDeath();

//! Class to handle when a connected ST HAL service dies.
class StHalDeathRecipient : public android::hardware::hidl_death_recipient {
  virtual void serviceDied(
      uint64_t /* cookie */,
      const wp<::android::hidl::base::V1_0::IBase>& /* who */) override {
    LOGE("ST HAL service died.");
    onStHalServiceDeath();
  }
};

struct LpmaEnableThreadData {
  pthread_t thread;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  bool currentLpmaEnabled;
  bool targetLpmaEnabled;
  bool connectedToService;
  sp<StHalDeathRecipient> deathRecipient = new StHalDeathRecipient();
  sp<ISoundTriggerHw> stHalService;
};

static LpmaEnableThreadData lpmaEnableThread;

#endif  // CHRE_DAEMON_LPMA_ENABLED

//! The host ID to use when preloading nanoapps. This is used before the server
//! is started and is sufficiently high enough so as to not collide with any
//! clients after the server starts.
static const uint16_t kHostClientIdDaemon = UINT16_MAX;

//! Contains a set of transaction IDs used to load the preloaded nanoapps.
//! The IDs are stored in the order they are sent.
static std::queue<uint32_t> gPreloadedNanoappPendingTransactionIds;

//! Set to true when we request a graceful shutdown of CHRE
static volatile bool chre_shutdown_requested = false;

#if !defined(LOG_NDEBUG) || LOG_NDEBUG != 0
static void log_buffer(const uint8_t * /*buffer*/, size_t /*size*/) {}
#else
static void log_buffer(const uint8_t *buffer, size_t size) {
  char line[32];
  int offset = 0;
  char line_chars[32];
  int offset_chars = 0;

  size_t orig_size = size;
  if (size > 128) {
    size = 128;
    LOGV("Dumping first 128 bytes of buffer of size %zu", orig_size);
  } else {
    LOGV("Dumping buffer of size %zu bytes", size);
  }
  for (size_t i = 1; i <= size; ++i) {
    offset += snprintf(&line[offset], sizeof(line) - offset, "%02x ",
                       buffer[i - 1]);
    offset_chars += snprintf(
        &line_chars[offset_chars], sizeof(line_chars) - offset_chars,
        "%c", (isprint(buffer[i - 1])) ? buffer[i - 1] : '.');
    if ((i % 8) == 0) {
      LOGV("  %s\t%s", line, line_chars);
      offset = 0;
      offset_chars = 0;
    } else if ((i % 4) == 0) {
      offset += snprintf(&line[offset], sizeof(line) - offset, " ");
    }
  }

  if (offset > 0) {
    char tabs[8];
    char *pos = tabs;
    while (offset < 28) {
      *pos++ = '\t';
      offset += 8;
    }
    *pos = '\0';
    LOGV("  %s%s%s", line, tabs, line_chars);
  }
}
#endif

static void parseAndEmitLogMessages(unsigned char *message) {
  const fbs::MessageContainer *container = fbs::GetMessageContainer(message);
  const auto *logMessage = static_cast<const fbs::LogMessage *>(
      container->message());

  constexpr size_t kLogMessageHeaderSize = 2 + sizeof(uint64_t);
  const flatbuffers::Vector<int8_t>& logData = *logMessage->buffer();
  for (size_t i = 0; i <= (logData.size() - kLogMessageHeaderSize);) {
    // Parse out the log level.
    const char *log = reinterpret_cast<const char *>(&logData.data()[i]);
    char logLevel = *log;
    log++;

    // Parse out the timestampNanos.
    uint64_t timestampNanos;
    memcpy(&timestampNanos, log, sizeof(uint64_t));
    timestampNanos = le64toh(timestampNanos);
    log += sizeof(uint64_t);

    float timestampSeconds = timestampNanos / 1e9;

    // Log the message.
    switch (logLevel) {
      case 1:
        LOGE(HUB_LOG_FORMAT_STR, timestampSeconds, log);
        break;
      case 2:
        LOGW(HUB_LOG_FORMAT_STR, timestampSeconds, log);
        break;
      case 3:
        LOGI(HUB_LOG_FORMAT_STR, timestampSeconds, log);
        break;
      case 4:
        LOGD(HUB_LOG_FORMAT_STR, timestampSeconds, log);
        break;
      default:
        LOGE("Invalid CHRE hub log level, omitting log");
    }

    // Advance the log pointer.
    size_t strLen = strlen(log);
    i += kLogMessageHeaderSize + strLen;
  }
}

static int64_t getTimeOffset(bool *success) {
  int64_t timeOffset = 0;

#if defined(__aarch64__)
  // Reads the system time counter (CNTVCT) and its frequency (CNTFRQ)
  // CNTVCT is used in the sensors HAL for time synchronization.
  // More information can be found in the ARM reference manual
  // (http://infocenter.arm.com/help/index.jsp?topic=
  // /com.arm.doc.100048_0002_05_en/jfa1406793266982.html)
  // Use uint64_t to store since the MRS instruction uses 64 bit (X) registers
  // (http://infocenter.arm.com/help/topic/
  // com.arm.doc.den0024a/ch06s05s02.html)
  uint64_t qTimerCount = 0, qTimerFreq = 0;
  uint64_t hostTimeNano = elapsedRealtimeNano();
  asm volatile("mrs %0, cntvct_el0" : "=r"(qTimerCount));
  asm volatile("mrs %0, cntfrq_el0" : "=r"(qTimerFreq));

  constexpr uint64_t kOneSecondInNanoseconds = 1000000000;
  if (qTimerFreq != 0) {
    // Get the seconds part first, then convert the remainder to prevent
    // overflow
    uint64_t qTimerNanos = (qTimerCount / qTimerFreq);
    if (qTimerNanos > UINT64_MAX / kOneSecondInNanoseconds) {
      LOGE("CNTVCT_EL0 conversion to nanoseconds overflowed during time sync."
           " Aborting time sync.");
      *success = false;
    } else {
      qTimerNanos *= kOneSecondInNanoseconds;

      // Round the remainder portion to the nearest nanosecond
      uint64_t remainder = (qTimerCount % qTimerFreq);
      qTimerNanos +=
          (remainder * kOneSecondInNanoseconds + qTimerFreq / 2) / qTimerFreq;

      timeOffset = hostTimeNano - qTimerNanos;
      *success = true;
    }
  } else {
    LOGE("CNTFRQ_EL0 had 0 value. Aborting time sync.");
    *success = false;
  }
#else
#error "Unsupported CPU architecture type"
#endif

  return timeOffset;
}


/**
 * @param logOnError If true, logs an error message on failure.
 *
 * @return true if the time sync message was successfully sent to CHRE.
 */
static bool sendTimeSyncMessage(bool logOnError) {
  bool timeSyncSuccess = true;
  int64_t timeOffset = getTimeOffset(&timeSyncSuccess);

  if (timeSyncSuccess) {
    flatbuffers::FlatBufferBuilder builder(64);
    HostProtocolHost::encodeTimeSyncMessage(builder, timeOffset);
    int success = chre_slpi_deliver_message_from_host(
        static_cast<const unsigned char *>(builder.GetBufferPointer()),
        static_cast<int>(builder.GetSize()));

    if (success != 0) {
      if (logOnError) {
        LOGE("Failed to deliver time sync message from host to CHRE: %d",
             success);
      }
      timeSyncSuccess = false;
    }
  }

  return timeSyncSuccess;
}

/**
 * Sends a time sync message to CHRE, retrying a specified time until success.
 *
 * @param maxNumRetries The number of times to retry sending the message
 *
 * @return true if the time sync message was successfully sent to CHRE.
 */
static bool sendTimeSyncMessageRetry(size_t maxNumRetries) {
  size_t numRetries = 0;
  useconds_t retryDelayUs = 50000; // 50 ms initially
  bool success = sendTimeSyncMessage(numRetries == maxNumRetries);
  while (!success && numRetries < maxNumRetries) {
    usleep(retryDelayUs);
    numRetries++;
    retryDelayUs *= 2;
    success = sendTimeSyncMessage(numRetries == maxNumRetries);
  }

  return success;
}

#ifdef CHRE_DAEMON_LPMA_ENABLED

static void acquireWakeLock() {
  if (acquire_wake_lock(PARTIAL_WAKE_LOCK, kWakeLockName) != 0) {
    LOGE("Failed to acquire wakelock");
  }
}

static void releaseWakeLock() {
  static bool initialRelease = true;

  // It's expected to get an error when we first try to release the wakelock
  // as it won't exist unless it was leaked previously - don't output a
  // false warning for this case
  if (release_wake_lock(kWakeLockName) != 0 && !initialRelease) {
    LOGE("Failed to release wakelock");
  }

  initialRelease = false;
}

/**
 * Sets the target state for LPMA to be enabled. This triggers another thread to
 * perform the async operation of enabling or disabling the LPMA use case.
 *
 * @param enabled Whether LPMA is to be enabled or disabled.
 */
static void setLpmaState(bool enabled) {
  pthread_mutex_lock(&lpmaEnableThread.mutex);
  lpmaEnableThread.targetLpmaEnabled = enabled;
  pthread_mutex_unlock(&lpmaEnableThread.mutex);
  pthread_cond_signal(&lpmaEnableThread.cond);
}

static void onStHalServiceDeath() {
  pthread_mutex_lock(&lpmaEnableThread.mutex);
  lpmaEnableThread.connectedToService = false;
  if (lpmaEnableThread.targetLpmaEnabled) {
    // ST HAL has died, so assume that the sound model is no longer active,
    // and trigger a reload of the sound model.
    lpmaEnableThread.currentLpmaEnabled = false;
    pthread_cond_signal(&lpmaEnableThread.cond);
  }
  pthread_mutex_unlock(&lpmaEnableThread.mutex);
}

/**
 * Connects to the ST HAL service, if not already. This method should only
 * be invoked after acquiring the lpmaEnableThread.mutex lock.
 *
 * @return true if successfully connected to the HAL.
 */
static bool connectToStHalServiceLocked() {
  if (!lpmaEnableThread.connectedToService) {
    lpmaEnableThread.stHalService = ISoundTriggerHw::getService();
    if (lpmaEnableThread.stHalService != nullptr) {
      LOGI("Connected to ST HAL service");
      lpmaEnableThread.connectedToService = true;
      lpmaEnableThread.stHalService->linkToDeath(
          lpmaEnableThread.deathRecipient, 0 /* flags */);
    }
  }

  return lpmaEnableThread.connectedToService;
}

/**
 * Loads the LPMA use case via the SoundTrigger HAL HIDL service.
 *
 * @param lpmaHandle The handle that was generated as a result of enabling
 *        the LPMA use case successfully.
 * @return true if LPMA was enabled successfully, false otherwise.
 */
static bool loadLpma(SoundModelHandle *lpmaHandle) {
  LOGD("Loading LPMA");

  ISoundTriggerHw::SoundModel soundModel;
  soundModel.type = SoundModelType::GENERIC;
  soundModel.vendorUuid.timeLow = 0x57CADDB1;
  soundModel.vendorUuid.timeMid = 0xACDB;
  soundModel.vendorUuid.versionAndTimeHigh = 0x4DCE;
  soundModel.vendorUuid.variantAndClockSeqHigh = 0x8CB0;

  const uint8_t uuidNode[6] = { 0x2E, 0x95, 0xA2, 0x31, 0x3A, 0xEE };
  memcpy(&soundModel.vendorUuid.node[0], uuidNode, sizeof(uuidNode));
  soundModel.data.resize(1);  // Insert a dummy byte to bypass HAL NULL checks.

  bool loaded = false;
  if (!connectToStHalServiceLocked()) {
    LOGE("Failed to get ST HAL service for LPMA load");
  } else {
    int32_t loadResult;
    Return<void> hidlResult = lpmaEnableThread.stHalService->loadSoundModel(
        soundModel, NULL /* callback */, 0 /* cookie */,
        [&](int32_t retval, SoundModelHandle handle) {
            loadResult = retval;
            *lpmaHandle = handle;
        });

    if (hidlResult.isOk()) {
      if (loadResult == 0) {
        LOGI("Loaded LPMA");
        loaded = true;
      } else {
        LOGE("Failed to load LPMA with %" PRId32, loadResult);
      }
    } else {
      LOGE("Failed to load LPMA due to hidl error %s",
           hidlResult.description().c_str());
    }
  }

  return loaded;
}

/**
 * Unloads the LPMA use case via the SoundTrigger HAL HIDL service. This
 * function does not indicate success/failure as it is expected that even in the
 * event of a failure to unload, the use case will be unloaded. As long as the
 * sound trigger HAL received the request we can be assured that the use case
 * will be unloaded (even if it means reseting the codec or otherwise).
 *
 * @param lpmaHandle A handle that was previously produced by the setLpmaEnabled
 *        function. This is the handle that is unloaded from the ST HAL to
 *        disable LPMA.
 */
static void unloadLpma(SoundModelHandle lpmaHandle) {
  LOGD("Unloading LPMA");

  if (!connectToStHalServiceLocked()) {
    LOGE("Failed to get ST HAL service for LPMA unload");
  } else {
    Return<int32_t> hidlResult =
        lpmaEnableThread.stHalService->unloadSoundModel(lpmaHandle);

    if (hidlResult.isOk()) {
      if (hidlResult == 0) {
        LOGI("Unloaded LPMA");
      } else {
        LOGE("Failed to unload LPMA with %" PRId32, int32_t(hidlResult));
      }
    } else {
      LOGE("Failed to unload LPMA due to hidl error %s",
           hidlResult.description().c_str());
    }
  }
}

static void *chreLpmaEnableThread(void *arg) {
  auto *state = static_cast<LpmaEnableThreadData *>(arg);

  const useconds_t kInitialRetryDelayUs = 500000;
  const int kRetryGrowthFactor = 2;
  const int kRetryGrowthLimit = 5;  // Terminates at 8s retry interval.
  const int kRetryWakeLockLimit = 10;  // Retry with a wakelock 10 times.

  int retryCount = 0;
  useconds_t retryDelay = 0;
  SoundModelHandle lpmaHandle;

  while (true) {
    pthread_mutex_lock(&state->mutex);
    if (state->currentLpmaEnabled == state->targetLpmaEnabled) {
      retryCount = 0;
      retryDelay = 0;
      releaseWakeLock();  // Allow the system to suspend while waiting.
      pthread_cond_wait(&state->cond, &state->mutex);
      acquireWakeLock();  // Ensure the system stays up while retrying.
    } else if (state->targetLpmaEnabled && loadLpma(&lpmaHandle)) {
      state->currentLpmaEnabled = state->targetLpmaEnabled;
    } else if (!state->targetLpmaEnabled) {
      // Regardless of whether the use case fails to unload, set the
      // currentLpmaEnabled to the targetLpmaEnabled. This will allow the next
      // enable request to proceed. After a failure to unload occurs, the
      // supplied handle is invalid and should not be unloaded again.
      unloadLpma(lpmaHandle);
      state->currentLpmaEnabled = state->targetLpmaEnabled;
    } else {
      // Unlock while delaying to avoid blocking the client thread. No shared
      // state is modified here.
      pthread_mutex_unlock(&state->mutex);

      if (retryDelay == 0) {
        retryDelay = kInitialRetryDelayUs;
      } else if (retryCount < kRetryGrowthLimit) {
        retryDelay *= kRetryGrowthFactor;
      }

      LOGD("Delaying retry %d for %uus", retryCount, retryDelay);
      usleep(retryDelay);

      retryCount++;
      if (retryCount > kRetryWakeLockLimit) {
        releaseWakeLock();
      }

      pthread_mutex_lock(&state->mutex);
    }

    pthread_mutex_unlock(&state->mutex);
  }

  LOGV("LPMA enable thread exited");
  return NULL;
}

/**
 * Initializes the data shared with the LPMA enable thread and starts the
 * thread.
 *
 * @param data Pointer to structure containing the (uninitialized) condition
 *        variable and associated data passed to the LPMA enable thread.
 * @return true on success, false otherwise.
 */
static bool initLpmaEnableThread(LpmaEnableThreadData *data) {
  bool success = false;
  int ret;

  if ((ret = pthread_mutex_init(&data->mutex, NULL)) != 0) {
    LOG_ERROR("Failed to initialize lpma enable mutex", ret);
  } else if ((ret = pthread_cond_init(&data->cond, NULL)) != 0) {
    LOG_ERROR("Failed to initialize lpma enable condition variable", ret);
  } else if (!start_thread(&data->thread, chreLpmaEnableThread, data)) {
    LOGE("Couldn't start lpma enable thread");
  } else {
    data->currentLpmaEnabled = false;
    data->targetLpmaEnabled = false;
    success = true;
  }

  return success;
}

#endif  // CHRE_DAEMON_LPMA_ENABLED

/**
 * Sends a message to CHRE.
 *
 * @param clientId The client ID that this message originates from.
 * @param data The data to pass down.
 * @param length The size of the data to send.
 * @return true if successful, false otherwise.
 */
static bool sendMessageToChre(uint16_t clientId, void *data, size_t length) {
  constexpr size_t kMaxPayloadSize = 1024 * 1024;  // 1 MiB

  // This limitation is due to FastRPC, but there's no case where we should come
  // close to this limit...
  static_assert(kMaxPayloadSize <= INT32_MAX,
                "DSP uses 32-bit signed integers to represent message size");

  bool success = false;
  if (length > kMaxPayloadSize) {
    LOGE("Message too large (got %zu, max %zu bytes)", length, kMaxPayloadSize);
  } else if (!HostProtocolHost::mutateHostClientId(data, length, clientId)) {
    LOGE("Couldn't set host client ID in message container!");
  } else {
    LOGV("Delivering message from host (size %zu)", length);
    log_buffer(static_cast<const uint8_t *>(data), length);
    int ret = chre_slpi_deliver_message_from_host(
        static_cast<const unsigned char *>(data), static_cast<int>(length));
    if (ret != 0) {
      LOGE("Failed to deliver message from host to CHRE: %d", ret);
    } else {
      success = true;
    }
  }

  return success;
}

/**
 * Loads a nanoapp by sending the nanoapp filename to the CHRE framework. This
 * method will return after sending the request so no guarantee is made that
 * the nanoapp is loaded until after the response is received.
 *
 * @param appId The ID of the nanoapp to load.
 * @param appVersion The version of the nanoapp to load.
 * @param appTargetApiVersion The version of the CHRE API that the app targets.
 * @param appBinaryName The name of the binary as stored in the filesystem. This
 *     will be used to load the nanoapp into CHRE.
 * @param transactionId The transaction ID to use when loading.
 * @return true if a request was successfully sent, false otherwise.
 */
static bool sendNanoappLoad(
    uint64_t appId, uint32_t appVersion, uint32_t appTargetApiVersion,
    const std::string& appBinaryName, uint32_t transactionId) {
  flatbuffers::FlatBufferBuilder builder;
  HostProtocolHost::encodeLoadNanoappRequestForFile(
      builder, transactionId, appId, appVersion, appTargetApiVersion,
      appBinaryName.c_str());

  bool success = sendMessageToChre(
      kHostClientIdDaemon, builder.GetBufferPointer(), builder.GetSize());

  if (!success) {
    LOGE("Failed to send nanoapp filename.");
  } else {
    gPreloadedNanoappPendingTransactionIds.push(transactionId);
  }

  return success;
}

/**
 * Sends a preloaded nanoapp filename / metadata to CHRE.
 *
 * @param header The nanoapp header binary blob.
 * @param nanoappName The filename of the nanoapp to be loaded.
 * @param transactionId The transaction ID to use when loading the app.
 * @return true if successful, false otherwise.
 */
static bool loadNanoapp(const std::vector<uint8_t>& header,
                        const std::string& nanoappName,
                        uint32_t transactionId) {
  // This struct comes from build/build_template.mk and must not be modified.
  // Refer to that file for more details.
  struct NanoAppBinaryHeader {
    uint32_t headerVersion;
    uint32_t magic;
    uint64_t appId;
    uint32_t appVersion;
    uint32_t flags;
    uint64_t hwHubType;
    uint8_t targetChreApiMajorVersion;
    uint8_t targetChreApiMinorVersion;
    uint8_t reserved[6];
  } __attribute__((packed));

  bool success = false;
  if (header.size() != sizeof(NanoAppBinaryHeader)) {
    LOGE("Header size mismatch");
  } else {
    // The header blob contains the struct above.
    const auto *appHeader = reinterpret_cast<
        const NanoAppBinaryHeader *>(header.data());

    // Build the target API version from major and minor.
    uint32_t targetApiVersion = (appHeader->targetChreApiMajorVersion << 24)
        | (appHeader->targetChreApiMinorVersion << 16);

    success = sendNanoappLoad(appHeader->appId, appHeader->appVersion,
                              targetApiVersion, nanoappName, transactionId);
  }

  return success;
}

/**
 * Loads the supplied file into the provided buffer.
 *
 * @param filename The name of the file to load.
 * @param buffer The buffer to load into.
 * @return true if successful, false otherwise.
 */
static bool readFileContents(const char *filename,
                             std::vector<uint8_t> *buffer) {
  bool success = false;
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    LOGE("Couldn't open file '%s': %d (%s)", filename, errno, strerror(errno));
  } else {
    ssize_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer->resize(size);
    if (!file.read(reinterpret_cast<char *>(buffer->data()), size)) {
      LOGE("Couldn't read from file '%s': %d (%s)",
           filename, errno, strerror(errno));
    } else {
      success = true;
    }
  }

  return success;
}

/**
 * Loads a preloaded nanoapp given a filename to load from. Allows the
 * transaction to complete before the nanoapp starts so the server can start
 * serving requests as soon as possible.
 *
 * @param name The filepath to load the nanoapp from.
 * @param transactionId The transaction ID to use when loading the app.
 */
static void loadPreloadedNanoapp(const std::string& name,
                                 uint32_t transactionId) {
  std::vector<uint8_t> headerBuffer;

  std::string headerFilename = std::string(name) + ".napp_header";
  std::string nanoappFilename = std::string(name) + ".so";

  // Only send the filename itself e.g activity.so since CHRE will load from
  // the same directory its own binary resides in.
  nanoappFilename = nanoappFilename.substr(
      nanoappFilename.find_last_of("/\\") + 1);
  if (nanoappFilename.empty()) {
    LOGE("Failed to get the name of the nanoapp %s", name.c_str());
  } else if (readFileContents(headerFilename.c_str(), &headerBuffer)
      && !loadNanoapp(headerBuffer, nanoappFilename, transactionId)) {
    LOGE("Failed to load nanoapp: '%s'", name.c_str());
  }
}

/**
 * Attempts to load all preloaded nanoapps from a config file. The config file
 * is expected to be valid JSON with the following structure:
 *
 * { "nanoapps": [
 *     "/path/to/nanoapp_1",
 *     "/path/to/nanoapp_2"
 * ]}
 *
 * The napp_header and so files will both be loaded. All errors are logged.
 */
static void loadPreloadedNanoapps() {
  constexpr char kPreloadedNanoappsConfigPath[] =
      "/vendor/etc/chre/preloaded_nanoapps.json";
  std::ifstream configFileStream(kPreloadedNanoappsConfigPath);

  Json::Reader reader;
  Json::Value config;
  if (!configFileStream) {
    LOGE("Failed to open config file '%s': %d (%s)",
         kPreloadedNanoappsConfigPath, errno, strerror(errno));
  } else if (!reader.parse(configFileStream, config)) {
    LOGE("Failed to parse nanoapp config file");
  } else if (!config.isMember("nanoapps")) {
    LOGE("Malformed preloaded nanoapps config");
  } else {
    for (Json::ArrayIndex i = 0; i < config["nanoapps"].size(); i++) {
      const Json::Value& nanoapp = config["nanoapps"][i];
      loadPreloadedNanoapp(nanoapp.asString(), static_cast<uint32_t>(i));
    }
  }
}

/**
 * Handles a message that is directed towards the daemon.
 *
 * @param message The message sent to the daemon.
 */
static void handleDaemonMessage(const uint8_t *message) {
  std::unique_ptr<fbs::MessageContainerT> container =
      fbs::UnPackMessageContainer(message);
  if (container->message.type
          != fbs::ChreMessage::LoadNanoappResponse) {
    LOGE("Invalid message from CHRE directed to daemon");
  } else {
    const auto *response = container->message.AsLoadNanoappResponse();
    if (gPreloadedNanoappPendingTransactionIds.empty()) {
      LOGE("Received nanoapp load response with no pending load");
    } else if (gPreloadedNanoappPendingTransactionIds.front()
                   != response->transaction_id) {
      LOGE("Received nanoapp load response with ID %" PRIu32
           " expected transaction id %" PRIu32, response->transaction_id,
           gPreloadedNanoappPendingTransactionIds.front());
    } else {
      if (!response->success) {
        LOGE("Received unsuccessful nanoapp load response with ID %" PRIu32,
             gPreloadedNanoappPendingTransactionIds.front());
      }
      gPreloadedNanoappPendingTransactionIds.pop();
    }
  }
}

/**
 * Entry point for the thread that receives messages sent by CHRE.
 *
 * @return always returns NULL
 */
static void *chre_message_to_host_thread(void *arg) {
  unsigned char messageBuffer[4096];
  unsigned int messageLen;
  int result = 0;
  auto *server = static_cast<::android::chre::SocketServer *>(arg);

  while (true) {
    messageLen = 0;
    LOGV("Calling into chre_slpi_get_message_to_host");
    result = chre_slpi_get_message_to_host(
        messageBuffer, sizeof(messageBuffer), &messageLen);
    LOGV("Got message from CHRE with size %u (result %d)", messageLen, result);

    if (result == CHRE_FASTRPC_ERROR_SHUTTING_DOWN) {
      LOGD("CHRE shutting down, exiting CHRE->Host message thread");
      break;
    } else if (result == CHRE_FASTRPC_SUCCESS && messageLen > 0) {
      log_buffer(messageBuffer, messageLen);
      uint16_t hostClientId;
      fbs::ChreMessage messageType;
      if (!HostProtocolHost::extractHostClientIdAndType(
          messageBuffer, messageLen, &hostClientId, &messageType)) {
        LOGW("Failed to extract host client ID from message - sending "
             "broadcast");
        hostClientId = chre::kHostClientIdUnspecified;
      }

      if (messageType == fbs::ChreMessage::LogMessage) {
        parseAndEmitLogMessages(messageBuffer);
      } else if (messageType == fbs::ChreMessage::TimeSyncRequest) {
        sendTimeSyncMessage(true /* logOnError */);
#ifdef CHRE_DAEMON_LPMA_ENABLED
      } else if (messageType == fbs::ChreMessage::LowPowerMicAccessRequest) {
        setLpmaState(true);
      } else if (messageType == fbs::ChreMessage::LowPowerMicAccessRelease) {
        setLpmaState(false);
#endif  // CHRE_DAEMON_LPMA_ENABLED
      } else if (hostClientId == kHostClientIdDaemon) {
        handleDaemonMessage(messageBuffer);
      } else if (hostClientId == chre::kHostClientIdUnspecified) {
        server->sendToAllClients(messageBuffer,
                                 static_cast<size_t>(messageLen));
      } else {
        server->sendToClientById(messageBuffer,
                                 static_cast<size_t>(messageLen), hostClientId);
      }
    } else if (!chre_shutdown_requested) {
      LOGE("Received an unknown result and no shutdown was requested. Quitting");
      exit(-1);
    } else {
      // Received an unknown result but a shutdown was requested. Break from the
      // loop to allow the daemon to cleanup.
      break;
    }
  }

  LOGV("Message to host thread exited");
  return NULL;
}

/**
 * Entry point for the thread that blocks in a FastRPC call to monitor for
 * abnormal exit of CHRE or reboot of the DSP.
 *
 * @return always returns NULL
 */
static void *chre_monitor_thread(void *arg) {
  (void) arg;
  int ret = chre_slpi_wait_on_thread_exit();
  if (!chre_shutdown_requested) {
    LOGE("Detected unexpected CHRE thread exit (%d)\n", ret);
    exit(EXIT_FAILURE);
  }

  LOGV("Monitor thread exited");
  return NULL;
}

/**
 * Entry point for the "reverse" monitor thread, which invokes a FastRPC method
 * to register a thread destructor, and blocks waiting on a condition variable.
 * This allows for the code running in the DSP to detect abnormal shutdown of
 * the host-side binary and perform graceful cleanup.
 *
 * @return always returns NULL
 */
static void *chre_reverse_monitor_thread(void *arg) {
  struct reverse_monitor_thread_data *thread_data =
      (struct reverse_monitor_thread_data *) arg;

  int ret = chre_slpi_initialize_reverse_monitor();
  if (ret != CHRE_FASTRPC_SUCCESS) {
    LOGE("Failed to initialize reverse monitor: %d", ret);
  } else {
    // Block here on the condition variable until the main thread notifies
    // us to exit
    pthread_mutex_lock(&thread_data->mutex);
    pthread_cond_wait(&thread_data->cond, &thread_data->mutex);
    pthread_mutex_unlock(&thread_data->mutex);
  }

  LOGV("Reverse monitor thread exited");
  return NULL;
}

/**
 * Initializes the data shared with the reverse monitor thread, and starts the
 * thread.
 *
 * @param data Pointer to structure containing the (uninitialized) condition
 *        variable and associated data passed to the reverse monitor thread
 *
 * @return true on success
 */
static bool init_reverse_monitor(struct reverse_monitor_thread_data *data) {
  bool success = false;
  int ret;

  if ((ret = pthread_mutex_init(&data->mutex, NULL)) != 0) {
    LOG_ERROR("Failed to initialize mutex", ret);
  } else if ((ret = pthread_cond_init(&data->cond, NULL)) != 0) {
    LOG_ERROR("Failed to initialize condition variable", ret);
  } else if (!start_thread(&data->thread, chre_reverse_monitor_thread, data)) {
    LOGE("Couldn't start reverse monitor thread");
  } else {
    success = true;
  }

  return success;
}

/**
 * Start a thread with default attributes, or log an error on failure
 *
 * @return bool true if the thread was successfully started
 */
static bool start_thread(pthread_t *thread_handle,
                         thread_entry_point_f *thread_entry,
                         void *arg) {
  int ret = pthread_create(thread_handle, NULL, thread_entry, arg);
  if (ret != 0) {
    LOG_ERROR("pthread_create failed", ret);
  }
  return (ret == 0);
}

namespace {

void onMessageReceivedFromClient(uint16_t clientId, void *data, size_t length) {
  sendMessageToChre(clientId, data, length);
}

}  // anonymous namespace

int main() {
  int ret = -1;
  pthread_t monitor_thread;
  pthread_t msg_to_host_thread;

  struct reverse_monitor_thread_data reverse_monitor;
  ::android::chre::SocketServer server;

#ifdef ADSPRPC
  remote_handle remote_handle_fd = 0xFFFFFFFF;
  LOGD("Attaching to ADSP sensors PD");
  if (remote_handle_open(ITRANSPORT_PREFIX "createstaticpd:sensorspd",
                         &remote_handle_fd)) {
    LOGE("Failed to open remote handle for sensorspd");
  } else {
    LOGV("Successfully opened remote handle for sensorspd");
  }
#endif  // ADSPRPC

  // Send time sync message before nanoapps start, retrying a few times
  // in case the SLPI is not ready yet. This retry logic must be placed before
  // any of the other FastRPC method invocations.
  constexpr size_t kMaxNumRetries = 5;
  if (!sendTimeSyncMessageRetry(kMaxNumRetries)) {
    LOGE("Failed to send initial time sync message");
  } else if (!init_reverse_monitor(&reverse_monitor)) {
    LOGE("Couldn't initialize reverse monitor");
#ifdef CHRE_DAEMON_LPMA_ENABLED
  } else if (!initLpmaEnableThread(&lpmaEnableThread)) {
    LOGE("Couldn't initialize LPMA enable thread");
#endif  // CHRE_DAEMON_LPMA_ENABLED
  } else {
    if ((ret = chre_slpi_start_thread()) != CHRE_FASTRPC_SUCCESS) {
      LOGE("Failed to start CHRE: %d", ret);
    } else {
      if (!start_thread(&monitor_thread, chre_monitor_thread, NULL)) {
        LOGE("Couldn't start monitor thread");
      } else if (!start_thread(&msg_to_host_thread, chre_message_to_host_thread,
                               &server)) {
        LOGE("Couldn't start CHRE->Host message thread");
      } else {
        LOGI("CHRE started");
        loadPreloadedNanoapps();

        // TODO: take 2nd argument as command-line parameter
        server.run("chre", true, onMessageReceivedFromClient);
      }

      chre_shutdown_requested = true;
      ret = chre_slpi_stop_thread();
      if (ret != CHRE_FASTRPC_SUCCESS) {
        LOGE("Failed to stop CHRE: %d", ret);
      } else {
        // TODO: don't call pthread_join if the thread failed to start
        LOGV("Joining monitor thread");
        ret = pthread_join(monitor_thread, NULL);
        if (ret != 0) {
          LOG_ERROR("Join on monitor thread failed", ret);
        }

        LOGV("Joining reverse monitor thread");
        pthread_cond_signal(&reverse_monitor.cond);
        ret = pthread_join(reverse_monitor.thread, NULL);
        if (ret != 0) {
          LOG_ERROR("Join on reverse monitor thread failed", ret);
        }

        LOGV("Joining message to host thread");
        ret = pthread_join(msg_to_host_thread, NULL);
        if (ret != 0) {
          LOG_ERROR("Join on monitor thread failed", ret);
        }

        LOGI("Shutdown complete");
      }
    }
  }

  return ret;
}

