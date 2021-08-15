/*
 * Copyright (C) 2018 The Android Open Source Project
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

package com.google.android.startop.iorap;

import com.google.android.startop.iorap.ITaskListener;

import com.google.android.startop.iorap.PackageEvent;
import com.google.android.startop.iorap.AppLaunchEvent;
import com.google.android.startop.iorap.AppIntentEvent;
import com.google.android.startop.iorap.SystemServiceEvent;
import com.google.android.startop.iorap.SystemServiceUserEvent;
import com.google.android.startop.iorap.RequestId;

/**
* IIOrap is a client interface to the input/output readahead and pin daemon (iorapd).
*
* The aim is to speed-up the cold start-up time of certain use-cases like application startup
* by utilizing trace-based pinning or readahead.
*
* Programatically, the behavior of iorapd should be treated like a black box. There is no
* "correctness", but only performance. By sending the right events at the appropriate time,
* we can squeeze more performance out of the system.
*
* If some events are not appropriately wired up, system performance may (temporarily) degrade.
*
* {@hide} */
oneway interface IIorap {
   /**
    * Set an ITaskListener which will be used to deliver notifications of in-progress/completition
    * for the onXEvent method calls below this.<br /><br />
    *
    * iorapd does all the work asynchronously and may deliver one or more onProgress events after
    * the event begins to be processed. It will always send back one onComplete that is considered
    * terminal.<br /><br />
    *
    * onProgress/onComplete are matched to the original event by the requestId. Once an onComplete
    * occurs for any given requestId, no further callbacks with the same requestId will occur.
    * It is illegal for the caller to reuse the same requestId on different invocations of IIorap.
    * <br /><br />
    *
    * onXEvent(id1) must be well-ordered w.r.t. onXEvent(id2), the assumption is that later
    * calls happen-after earlier calls and that id2 > id1. Decreasing request IDs will
    * immediately get rejected.
    * <br /><br />
    *
    * Sequence diagram of stereotypical successful event delivery and response notification:
    *
    * <pre>
    *
    *           ┌─────────────┐                ┌──────┐
    *           │system_server│                │iorapd│
    *           └──────┬──────┘                └──┬───┘
    *                  Request [01]: onSomeEvent ┌┴┐
    *                  │────────────────────────>│ │
    *                  │                         │ │
    *                  │                         │ │  ╔════════════════════════╗
    *                  │                         │ │  ║start processing event ░║
    *                  │                         │ │  ╚════════════════════════╝
    *                  │                         │ │
    * ╔═══════╤════════╪═════════════════════════╪═╪══════════════════════════════╗
    * ║ LOOP  │  1 or more times                 │ │                              ║
    * ╟───────┘        │                         │ │                              ║
    * ║                │Request [01]: onProgress │ │                              ║
    * ║                │<────────────────────────│ │                              ║
    * ║                │                         │ │                              ║
    * ║                │                         │ │────┐                         ║
    * ║                │                         │ │    │ workload in progress    ║
    * ║                │                         │ │<───┘                         ║
    * ╚════════════════╪═════════════════════════╪═╪══════════════════════════════╝
    *                  │                         └┬┘
    *                  .                          .
    *                  .                          .
    *                  .                          .
    *                  .                          .
    *                  .                          .
    *                  │                         ┌┴┐  ╔═════════════════════════╗
    *                  │                         │ │  ║finish processing event ░║
    *                  │                         │ │  ╚═════════════════════════╝
    *                  │Request [01]: onComplete │ │
    *                  │<────────────────────────│ │
    *           ┌──────┴──────┐                ┌─└┬┘──┐
    *           │system_server│                │iorapd│
    *           └─────────────┘                └──────┘
    *
    * </pre> <!-- system/iorap/docs/binder/IIorap_setTaskListener.plantuml -->
    */
    void setTaskListener(ITaskListener listener);

    // All callbacks will be done via the ITaskListener.
    // The RequestId passed in is the same RequestId sent back via the ITaskListener.
    // See above for more details.

    // Note: For each ${Type}Event, see the ${Type}Event.java for more documentation
    // in frameworks/base/startop/src/com/google/android/startop/iorap/${Type}Event.java

    // void onActivityHintEvent(in RequestId request, in ActivityHintEvent event);
    void onAppLaunchEvent(in RequestId request, in AppLaunchEvent event);
    void onPackageEvent(in RequestId request, in PackageEvent event);
    void onAppIntentEvent(in RequestId request, in AppIntentEvent event);
    void onSystemServiceEvent(in RequestId request, in SystemServiceEvent event);
    void onSystemServiceUserEvent(in RequestId request, in SystemServiceUserEvent event);
}
