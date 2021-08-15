/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_DEBUG_DUMP_H_
#define CHRE_PLATFORM_SLPI_DEBUG_DUMP_H_

#include <cstddef>
#include <cstdint>

namespace chre {

//! Size of the buffer used to store the debug dump string, including NULL
//! termination. Sets an upper bound on the largest string that can be provided
//! in a single call to ashCommitDebugDump(). Currently driven by the maximum
//! CHRE transfer size (4KB), minus some space for overhead.
extern size_t debugDumpStrMaxSize;

//! How long we wait for all clients to commit their debug dump string
extern uint16_t debugDumpTimeoutMs;

/**
 * Type signature for a callback that can be registered in
 * registerDebugDumpCallback(). A call to this function should result in
 * commitDebugDump() getting called with the given handle and done=true
 * within debugDumpStrMaxSize.
 *
 * This callback will be invoked from an arbitrary thread.
 *
 * @param cookie The cookie given to registerDebugDumpCallback() when this
 *        callback was registered
 * @param handle A unique handle for this callback, which must be used when
 *        calling commitDebugDump()
 *
 * @see registerDebugDumpCallback()
 */
typedef void (debugDumpCbFunc)(void *cookie, uint32_t handle);

/**
 * Register a function to be invoked when a debug dump has been requested. Upon
 * receiving this callback, start the process of gathering debugging information
 * and pass it to commitDebugDump() when ready. This can be done synchronously
 * and/or asynchronously.
 *
 * It is invalid to attempt to register a new callback from the context of a
 * callback - this will result in a deadlock. Otherwise safe to call from any
 * thread, including during early initialization.
 *
 * A given callback function may only be registered once.
 *
 * @param name Human-friendly name for the module associated with this callback
 * @param callback Function to call when debug dump is requested
 * @param cookie Opaque data to pass to the callback
 *
 * @return true if the callback was successfully registered
 */
bool registerDebugDumpCallback(const char *name, debugDumpCbFunc *callback,
	                             void *cookie);

/**
 * Unregisters a debug dump callback. After this function returns, the given
 * callback is guaranteed to not be invoked again (until it is re-registered),
 * but note that it may be called from another thread up to and during the
 * execution of this function.
 *
 * It is invalid to attempt to unregister a callback from the context of a
 * callback - this will result in a deadlock. Otherwise safe to call from any
 * thread.
 *
 * @param callback Callback function previously given to
 *        registerDebugDumpCallback()
 */
void unregisterDebugDumpCallback(debugDumpCbFunc *callback);

/**
 * Add an ASCII string to appear in the debug dump, which appears in Android bug
 * reports.
 *
 * Strings longer than ASH_DEBUG_DUMP_STR_MAX_SIZE will be truncated.
 *
 * While the "done" parameter allows for this function to be called multiple
 * times for a single handle, no guarantee is made that all strings for a handle
 * appear contiguously. In other words, if more than one call is made to this
 * function, another module's debug data could appear in the middle. So modules
 * are encouraged to make as few calls to this function as are necessary, and
 * always terminate their strings with a newline.
 *
 * Safe to call from within the debug dump callback, or from any thread.
 *
 * @param handle The handle supplied in the call to the debug dump callback
 * @param debugStr A null-terminated string containing debugging information
 *        from this module (should be prefixed with some human-readable
 *        identifier). Must be a valid pointer, but can be an empty string.
 * @param done true if no more data is expected for this handle
 *
 * @return true if the string was accepted without truncation
 */
bool commitDebugDump(uint32_t handle, const char *debugStr, bool done);

/**
 * Function signature for the callback given to triggerDebugDump() and used to
 * pass populated strings back to the entity triggering the dump.
 *
 * This callback will be invoked from an arbitrary thread, but is guaranteed to
 * not be called concurrently from multiple threads.
 *
 * @param cookie Opaque data given to triggerDebugDump()
 * @param debugStr A non-null, null-terminated string containing debugging
 *        information
 * @param debugStrSize Size of debugStr (including null termination)
 * @param complete true if this is the final call to this callback for the
 *        current dump session
 */
typedef void (debugDumpReadyCbFunc)(void *cookie, const char *debugStr,
                                    size_t debugStrSize, bool complete);

/**
 * Kick off the debug dump data collection procedure. Synchronously invokes all
 * callbacks registered via registerDebugDumpCallback() to notify them that the
 * dump collection process is starting. Once all registered callbacks have
 * provided their output via commitDebugDump() or a timeout occurs, the supplied
 * data ready callback will be invoked with complete=true to finalize the dump
 * process. If the local buffer fills up before the dump is complete, the ready
 * callback will be invoked with complete=false.
 *
 * Only one debug dump can be in progress at a given time.
 *
 * Note that ideally this should be handled internally to the ASH
 * implementation, but we are currently leveraging an outside entity (CHRE) to
 * handle the debug dump process due to AP-side considerations.
 *
 * @param readyCb Callback used to pass debug string data back to the caller.
 *        May be invoked synchronously from the context of this function, and/or
 *        asynchronously later on from the context of an arbitrary thread.
 * @param cookie Opaque data to pass to readyCb
 *
 * @return true if the dump process was successfully started
 */
bool triggerDebugDump(debugDumpReadyCbFunc *readyCb, void *cookie);

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_DEBUG_DUMP_H_
