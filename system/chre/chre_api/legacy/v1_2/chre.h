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

#ifndef _CHRE_H_
#define _CHRE_H_

/**
 * @file
 * This header file includes all the headers which combine to fully define the
 * interface for the Context Hub Runtime Environment (CHRE).  This interface is
 * of interest to both implementers of CHREs and authors of nanoapps.  The API
 * documentation attempts to address concerns of both.
 *
 * See individual header files for API details, and general comments below
 * for overall platform information.
 */

#include <chre/audio.h>
#include <chre/common.h>
#include <chre/event.h>
#include <chre/gnss.h>
#include <chre/nanoapp.h>
#include <chre/re.h>
#include <chre/sensor.h>
#include <chre/version.h>
#include <chre/wifi.h>
#include <chre/wwan.h>

/**
 * @mainpage
 * CHRE is the Context Hub Runtime Environment.  CHRE is used in Android to run
 * contextual applications, called nanoapps, in a low-power processing domain
 * other than the applications processor that runs Android itself.  The CHRE
 * API, documented herein, is the common interface exposed to nanoapps for any
 * compatible CHRE implementation.  The CHRE API provides the ability for
 * creating nanoapps that are code-compatible across different CHRE
 * implementations and underlying platforms. Refer to the following sections for
 * a discussion on some important details of CHRE that aren't explicitly exposed
 * in the API itself.
 *
 * @section entry_points Entry points
 *
 * The following entry points are used to bind a nanoapp to the CHRE system, and
 * all three must be implemented by any nanoapp (see chre/nanoapp.h):
 * - nanoappStart: initialization
 * - nanoappHandleEvent: hook for event-driven processing
 * - nanoappEnd: graceful teardown
 *
 * The CHRE implementation must also ensure that it performs these functions
 * prior to invoking nanoappStart, or after nanoappEnd returns:
 * - bss section zeroed out (prior to nanoappStart)
 * - static variables initialized (prior to nanoappStart)
 * - global C++ constructors called (prior to nanoappStart)
 * - global C++ destructors called (after nanoappEnd)
 *
 * @section threading Threading model
 *
 * A CHRE implementation is free to choose among many different
 * threading models, including a single-threaded system or a multi-threaded
 * system with preemption.  The current platform definition is agnostic to this
 * underlying choice.  However, the CHRE implementation must ensure that time
 * spent executing within a nanoapp does not significantly degrade or otherwise
 * interfere with other functions of the system in which CHRE is implemented,
 * especially latency-sensitive tasks such as sensor event delivery to the AP.
 * In other words, it must ensure that these functions can either occur in
 * parallel or preempt a nanoapp's execution.  The current version of the API
 * does not specify whether the implementation allows for CPU sharing between
 * nanoapps on a more granular level than the handling of individual events [1].
 * In any case, event ordering from the perspective of an individual nanoapp
 * must be FIFO, but the CHRE implementation may choose to violate total
 * ordering of events across all nanoapps to achieve more fair resource sharing,
 * but this is not required.
 *
 * This version of the CHRE API does require that all nanoapps are treated as
 * non-reentrant, meaning that only one instance of program flow can be inside
 * an individual nanoapp at any given time.  That is, any of the functions of
 * the nanoapp, including the entry points and all other callbacks, cannot be
 * invoked if a previous invocation to the same or any other function in the
 * nanoapp has not completed yet.
 *
 * For example, if a nanoapp is currently in nanoappHandleEvent(), the CHRE is
 * not allowed to call nanoappHandleEvent() again, or to call a memory freeing
 * callback.  Similarly, if a nanoapp is currently in a memory freeing
 * callback, the CHRE is not allowed to call nanoappHandleEvent(), or invoke
 * another memory freeing callback.
 *
 * There are two exceptions to this rule: If an invocation of chreSendEvent()
 * fails (returns 'false'), it is allowed to immediately invoke the memory
 * freeing callback passed into that function.  This is a rare case, and one
 * where otherwise a CHRE implementation is likely to leak memory. Similarly,
 * chreSendMessageToHost() is allowed to invoke the memory freeing callback
 * directly, whether it returns 'true' or 'false'.  This is because the CHRE
 * implementation may copy the message data to its own buffer, and therefore
 * wouldn't need the nanoapp-supplied buffer after chreSendMessageToHost()
 * returns.
 *
 * For a nanoapp author, this means no thought needs to be given to
 * synchronization issues with global objects, as they will, by definition,
 * only be accessed by a single thread at once.
 *
 * [1]: Note to CHRE implementers: A future version of the CHRE platform may
 * require multi-threading with preemption.  This is mentioned as a heads up,
 * and to allow implementors deciding between implementation approaches to
 * make the most informed choice.
 *
 * @section timing Timing
 *
 * Nanoapps should expect to be running on a highly constrained system, with
 * little memory and little CPU.  Any single nanoapp should expect to
 * be one of several nanoapps on the system, which also share the CPU with the
 * CHRE and possibly other services as well.
 *
 * Thus, a nanoapp needs to be efficient in its memory and CPU usage.
 * Also, as noted in the Threading Model section, a CHRE implementation may
 * be single threaded.  As a result, all methods invoked in a nanoapp
 * (like nanoappStart, nanoappHandleEvent, memory free callbacks, etc.)
 * must run "quickly".  "Quickly" is difficult to define, as there is a
 * diversity of Context Hub hardware.  Nanoapp authors are strongly recommended
 * to limit their application to consuming no more than 1 second of CPU time
 * prior to returning control to the CHRE implementation.  A CHRE implementation
 * may consider a nanoapp as unresponsive if it spends more time than this to
 * process a single event, and take corrective action.
 *
 * A nanoapp may have the need to occasionally perform a large block of
 * calculations that exceeds the 1 second guidance.  The recommended approach in
 * this case is to split up the large block of calculations into smaller
 * batches.  In one call into the nanoapp, the nanoapp can perform the first
 * batch, and then set a timer or send an event (chreSendEvent()) to itself
 * indicating which batch should be done next. This will allow the nanoapp to
 * perform the entire calculation over time, without monopolizing system
 * resources.
 *
 * @section floats Floating point support
 *
 * The C type 'float' is used in this API, and thus a CHRE implementation
 * is required to support 'float's.
 *
 * Support of the C types 'double' and 'long double' is optional for a
 * CHRE implementation.  Note that if a CHRE decides to support them, unlike
 * 'float' support, there is no requirement that this support is particularly
 * efficient.  So nanoapp authors should be aware this may be inefficient.
 *
 * If a CHRE implementation choses not to support 'double' or
 * 'long double', then the build toolchain setup provided needs to set
 * the preprocessor define CHRE_NO_DOUBLE_SUPPORT.
 *
 * @section compat CHRE and Nanoapp compatibility
 *
 * CHRE implementations must make affordances to maintain binary compatibility
 * across minor revisions of the API version (e.g. v1.1 to v1.2).  This applies
 * to both running a nanoapp compiled for a newer version of the API on a CHRE
 * implementation built against an older version (backwards compatibility), and
 * vice versa (forwards compatibility).  API changes that are acceptable in
 * minor version changes that may require special measures to ensure binary
 * compatibility include: addition of new functions; addition of arguments to
 * existing functions when the default value used for nanoapps compiled against
 * the old version is well-defined and does not affect existing functionality;
 * and addition of fields to existing structures, even when this induces a
 * binary layout change (this should be made rare via judicious use of reserved
 * fields).  API changes that must only occur alongside a major version change
 * and are therefore not compatible include: removal of any function, argument,
 * field in a data structure, or mandatory functional behavior that a nanoapp
 * may depend on; any change in the interpretation of an existing data structure
 * field that alters the way it was defined previously (changing the units of a
 * field would fall under this, but appropriating a previously reserved field
 * for some new functionality would not); and any change in functionality or
 * expected behavior that conflicts with the previous definition.
 *
 * Note that the CHRE API only specifies the software interface between a
 * nanoapp and the CHRE system - the binary interface (ABI) between nanoapp and
 * CHRE is necessarily implementation-dependent.  Therefore, the recommended
 * approach to accomplish binary compatibility is to build a Nanoapp Support
 * Library (NSL) that is specific to the CHRE implementation into the nanoapp
 * binary, and use it to handle ABI details in a way that ensures compatibility.
 * In addition, to accomplish forwards compatibility, the CHRE implementation is
 * expected to recognize the CHRE API version that a nanoapp is targeting and
 * engage compatibility behaviors where necessary.
 *
 * By definition, major API version changes (e.g. v1.1 to v2.0) break
 * compatibility.  Therefore, a CHRE implementation must not attempt to load a
 * nanoapp that is targeting a newer major API version.
 */

#endif  /* _CHRE_H_ */

