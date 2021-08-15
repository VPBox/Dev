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

#include <type_traits>

extern "C" {

#include "HAP_farf.h"
#include "timer.h"
#include "qurt.h"

}  // extern "C"

#include "chre/core/event_loop.h"
#include "chre/core/event_loop_manager.h"
#include "chre/core/init.h"
#include "chre/core/static_nanoapps.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/memory.h"
#include "chre/platform/mutex.h"
#include "chre/platform/slpi/debug_dump.h"
#include "chre/platform/slpi/fastrpc.h"
#include "chre/platform/slpi/uimg_util.h"
#include "chre/util/lock_guard.h"

#ifdef CHRE_SLPI_SEE
#include "chre/platform/slpi/see/island_vote_client.h"
#endif

using chre::EventLoop;
using chre::EventLoopManagerSingleton;
using chre::LockGuard;
using chre::Mutex;
using chre::UniquePtr;

extern "C" int chre_slpi_stop_thread(void);

// Qualcomm-defined function needed to indicate that the CHRE thread may call
// dlopen() (without it, the thread will deadlock when calling dlopen()). Not in
// any header file in the SLPI tree or Hexagon SDK (3.0), so declaring here.
// Returns 0 to indicate success.
extern "C" int HAP_thread_migrate(qurt_thread_t thread);

namespace {

//! Size of the stack for the CHRE thread, in bytes.
constexpr size_t kStackSize = (8 * 1024);

//! Memory partition where the thread control block (TCB) should be stored,
//! which controls micro-image support.
//! @see qurt_thread_attr_set_tcb_partition
constexpr unsigned char kTcbPartition = chre::isSlpiUimgSupported() ?
    QURT_THREAD_ATTR_TCB_PARTITION_TCM : QURT_THREAD_ATTR_TCB_PARTITION_RAM;

//! The priority to set for the CHRE thread (value between 1-255, with 1 being
//! the highest).
//! @see qurt_thread_attr_set_priority
constexpr unsigned short kThreadPriority = 192;

//! How long we wait (in microseconds) between checks on whether the CHRE thread
//! has exited after we invoked stop().
constexpr time_timetick_type kThreadStatusPollingIntervalUsec = 5000;  // 5ms

//! Buffer to use for the CHRE thread's stack.
typename std::aligned_storage<kStackSize>::type gStack;

//! QuRT OS handle for the CHRE thread.
qurt_thread_t gThreadHandle;

//! Protects access to thread metadata, like gThreadRunning, during critical
//! sections (starting/stopping the CHRE thread).
Mutex gThreadMutex;

//! Set to true when the CHRE thread starts, and false when it exits normally.
bool gThreadRunning;

//! A thread-local storage key, which is currently only used to add a thread
//! destructor callback for the host FastRPC thread.
int gTlsKey;
bool gTlsKeyValid;

void performDebugDumpCallback(uint16_t /*eventType*/, void *data) {
  auto *handle = static_cast<const uint32_t *>(data);
  UniquePtr<char> dump = chre::EventLoopManagerSingleton::get()->debugDump();
  chre::commitDebugDump(*handle, dump.get(), true /*done*/);
}

void onDebugDumpRequested(void * /*cookie*/, uint32_t handle) {
  static uint32_t debugDumpHandle;

  debugDumpHandle = handle;
  chre::EventLoopManagerSingleton::get()->deferCallback(
      chre::SystemCallbackType::PerformDebugDump, &debugDumpHandle,
      performDebugDumpCallback);
}

/**
 * Entry point for the QuRT thread that runs CHRE.
 *
 * @param data Argument passed to qurt_thread_create()
 */
void chreThreadEntry(void * /*data*/) {
  EventLoopManagerSingleton::get()->lateInit();
  chre::loadStaticNanoapps();
  chre::registerDebugDumpCallback("CHRE", onDebugDumpRequested, nullptr);
  EventLoopManagerSingleton::get()->getEventLoop().run();

  chre::unregisterDebugDumpCallback(onDebugDumpRequested);
  chre::deinit();
#if defined(CHRE_SLPI_SEE) && !defined(IMPORT_CHRE_UTILS)
  chre::IslandVoteClientSingleton::deinit();
#endif
  // Perform this as late as possible - if we are shutting down because we
  // detected exit of the host process, FastRPC will unload us once all our
  // FastRPC calls have returned. Doing this late helps ensure that the call
  // to chre_slpi_get_message_to_host() stays open until we're done with
  // cleanup.
  chre::HostLinkBase::shutdown();
  gThreadRunning = false;
}

void onHostProcessTerminated(void * /*data*/) {
  LOGW("Host process died, exiting CHRE (running %d)", gThreadRunning);
  if (gThreadRunning) {
    EventLoopManagerSingleton::get()->getEventLoop().stop();
  }
}

}  // anonymous namespace

namespace chre {

bool inEventLoopThread() {
  return (qurt_thread_get_id() == gThreadHandle);
}

}  // namespace chre

/**
 * Invoked over FastRPC to initialize and start the CHRE thread.
 *
 * @return 0 on success, nonzero on failure (per FastRPC requirements)
 */
extern "C" int chre_slpi_start_thread(void) {
  // This lock ensures that we only start the thread once
  LockGuard<Mutex> lock(gThreadMutex);
  int fastRpcResult = CHRE_FASTRPC_ERROR;

  if (gThreadRunning) {
    LOGE("CHRE thread already running");
  } else {
#if defined(CHRE_SLPI_SEE) && !defined(IMPORT_CHRE_UTILS)
    chre::IslandVoteClientSingleton::init("CHRE" /* clientName */);
#endif

    // This must complete before we can receive messages that might result in
    // posting an event
    chre::init();

    // Human-readable name for the CHRE thread (not const in QuRT API, but they
    // make a copy)
    char threadName[] = "CHRE";
    qurt_thread_attr_t attributes;

    qurt_thread_attr_init(&attributes);
    qurt_thread_attr_set_name(&attributes, threadName);
    qurt_thread_attr_set_priority(&attributes, kThreadPriority);
    qurt_thread_attr_set_stack_addr(&attributes, &gStack);
    qurt_thread_attr_set_stack_size(&attributes, kStackSize);
    qurt_thread_attr_set_tcb_partition(&attributes, kTcbPartition);

    gThreadRunning = true;
    LOGI("Starting CHRE thread");
    int result = qurt_thread_create(&gThreadHandle, &attributes,
                                    chreThreadEntry, nullptr);
    if (result != QURT_EOK) {
      LOGE("Couldn't create CHRE thread: %d", result);
      gThreadRunning = false;
    } else if (HAP_thread_migrate(gThreadHandle) != 0) {
      FATAL_ERROR("Couldn't migrate thread");
    } else {
      LOGD("Started CHRE thread");
      fastRpcResult = CHRE_FASTRPC_SUCCESS;
    }
  }

  return fastRpcResult;
}

/**
 * Blocks until the CHRE thread exits. Called over FastRPC to monitor for
 * abnormal termination of the CHRE thread and/or SLPI as a whole.
 *
 * @return Always returns 0, indicating success (per FastRPC requirements)
 */
extern "C" int chre_slpi_wait_on_thread_exit(void) {
  if (!gThreadRunning) {
    LOGE("Tried monitoring for CHRE thread exit, but thread not running!");
  } else {
    int status;
    int result = qurt_thread_join(gThreadHandle, &status);
    if (result != QURT_EOK) {
      LOGE("qurt_thread_join failed with result %d", result);
    }
    LOGI("Detected CHRE thread exit");
  }

  return CHRE_FASTRPC_SUCCESS;
}

/**
 * If the CHRE thread is running, requests it to perform graceful shutdown,
 * waits for it to exit, then completes teardown.
 *
 * @return Always returns 0, indicating success (per FastRPC requirements)
 */
extern "C" int chre_slpi_stop_thread(void) {
  // This lock ensures that we will complete shutdown before the thread can be
  // started again
  LockGuard<Mutex> lock(gThreadMutex);

  if (!gThreadRunning) {
    LOGD("Tried to stop CHRE thread, but not running");
  } else {
    EventLoopManagerSingleton::get()->getEventLoop().stop();
    if (gTlsKeyValid) {
      int ret = qurt_tls_delete_key(gTlsKey);
      if (ret != QURT_EOK) {
        // Note: LOGE is not necessarily safe to use after stopping CHRE
        FARF(ERROR, "Deleting TLS key failed: %d", ret);
      }
      gTlsKeyValid = false;
    }

    // Poll until the thread has stopped; note that we can't use
    // qurt_thread_join() here because chreMonitorThread() will already be
    // blocking in it, and attempting to join the same target from two threads
    // is invalid. Technically, we could use a condition variable, but this is
    // simpler and we don't care too much about being notified right away.
    while (gThreadRunning) {
      timer_sleep(kThreadStatusPollingIntervalUsec, T_USEC,
                  true /* non_deferrable */);
    }
    gThreadHandle = 0;
  }

  return CHRE_FASTRPC_SUCCESS;
}

/**
 * Creates a thread-local storage (TLS) key in QuRT, which we use to inject a
 * destructor that is called when the current FastRPC thread terminates. This is
 * used to get a notification when the original FastRPC thread dies for any
 * reason, so we can stop the CHRE thread.
 *
 * Note that this needs to be invoked from a separate thread on the host process
 * side. It doesn't work if called from a thread that will be blocking inside a
 * FastRPC call, such as the monitor thread.
 *
 * @return 0 on success, nonzero on failure (per FastRPC requirements)
 */
extern "C" int chre_slpi_initialize_reverse_monitor(void) {
  LockGuard<Mutex> lock(gThreadMutex);

  if (!gTlsKeyValid) {
    int result = qurt_tls_create_key(&gTlsKey, onHostProcessTerminated);
    if (result != QURT_EOK) {
      LOGE("Couldn't create TLS key: %d", result);
    } else {
      // We need to set the value to something for the destructor to be invoked
      result = qurt_tls_set_specific(gTlsKey, &gTlsKey);
      if (result != QURT_EOK) {
        LOGE("Couldn't set TLS data: %d", result);
        qurt_tls_delete_key(gTlsKey);
      } else {
        gTlsKeyValid = true;
      }
    }
  }

  return (gTlsKeyValid) ? CHRE_FASTRPC_SUCCESS : CHRE_FASTRPC_ERROR;
}
