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
 * This header file includes all the headers which combine to fully defined
 * the interface for the Context Hub Runtime Environment (CHRE).  This is the
 * environment in which a nanoapp runs.
 *
 * This interface is of interest to both implementors of CHREs and
 * authors of nanoapps.  The API documentation attempts to address concerns
 * of both.
 *
 * See individual header files for API specific, and general comments below
 * for overall platform information.
 */

#include <chre/event.h>
#include <chre/nanoapp.h>
#include <chre/re.h>
#include <chre/sensor.h>
#include <chre/version.h>


/**
 * Entry points.
 *
 * The following entry points are required to be handled by the CHRE
 * implementation, and the functions must all be implemented by nanoapps.
 * o nanoappStart function (see chre_nanoapp.h)
 * o nanoappHandleEvent function (see chre_nanoapp.h)
 * o nanoappEnd function (see chre_nanoapp.h)
 * o bss section zeroed out (prior to nanoappStart)
 * o static variables initialized (prior to nanoappStart)
 * o global C++ constructors called (prior to nanoappStart)
 * o global C++ destructors called (after nanoappEnd)
 */


/**
 * Threading model.
 *
 * A CHRE implementation is free to chose among many different
 * threading models, including a single threaded system or a multi-threaded
 * system with preemption.  The current platform definition is agnostic to this
 * underlying choice [1].
 *
 * However, the Platform does require that all nanoapps are treated as
 * non-reentrant.  That is, any of the functions of the nanoapp, including
 * the entry points defined above and the memory freeing callbacks defined
 * below, cannot be invoked by the CHRE if a previous invocation
 * hasn't completed.  Note this means no nanoapp function can be invoked
 * from an interrupt context.
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
 *
 * [1] Note to CHRE implementors: A future version of the CHRE platform may
 * require multi-threading with preemption.  This is mentioned as a heads up,
 * and to allow implementors deciding between implementation approaches to
 * make the most informed choice.
 */

/**
 * Notes on timing.
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
 * diversity of Context Hub hardware.  For Android N, there is no firm
 * definition of "quickly", but expect this term to gain definition in
 * future releases as we get feedback from partners.
 *
 * In order to write a nanoapp that will be able to adopt to future
 * stricter notions of "quickly", all nanoapp methods should be written so
 * they execute in a small amount of time.  Some nanoapps may have the need
 * to occasionally perform a large block of calculations, which may seem
 * to violate this.  The recommended approach in this case is to
 * split up the large block of calculations into smaller batches.  In one
 * call into the nanoapp, the nanoapp can perform the first batch, and then
 * send an event (chreSendEvent()) to itself indicating which batch should be
 * done next.  This will allow the nanoapp to perform the entire calculation
 * over time, without monopolizing system resources.
 */

/**
 * Floating point support.
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
 */

/**
 * CHRE and Nanoapp compatibility.
 *
 * The Android N release introduces the first version of this API.
 * It is anticipated that there will be a lot of feedback from
 * Android partners on this initial API.  To allow more flexibility
 * in addressing that feedback, there is no plan to assure
 * binary compatibility between the Android N and Android O CHRE
 * implementations and nanoapps.
 *
 * That is, a nanoapp built with the Android O version of this
 * API should not expect to run on a CHRE built with
 * the Android N API.  Similarly, a nanoapp build with the
 * Android N API should not expect to run on a CHRE
 * build with the Android O API.  Such a nanoapp will need to
 * recompiled with the appropriate API in order to work.
 */

#endif  /* _CHRE_H_ */

