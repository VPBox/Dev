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

#ifndef _CHRE_EVENT_H_
#define _CHRE_EVENT_H_

/**
 * Context Hub Runtime Environment API dealing with events and messages.
 */


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The CHRE implementation is required to provide the following
 * preprocessor defines via the build system.
 *
 * CHRE_MESSAGE_TO_HOST_MAX_SIZE: The maximum size, in bytes, allowed for
 *     a message sent to chreSendMessageToHost().  This must be at least
 *     CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE.
 */

#ifndef CHRE_MESSAGE_TO_HOST_MAX_SIZE
#error CHRE_MESSAGE_TO_HOST_MAX_SIZE must be defined by the Context Hub Runtime Environment implementation
#endif

/**
 * The minimum size, in bytes, any CHRE implementation will
 * use for CHRE_MESSAGE_TO_HOST_MAX_SIZE.
 */
#define CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE 128

#if CHRE_MESSAGE_TO_HOST_MAX_SIZE < CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE
#error CHRE_MESSAGE_TO_HOST_MAX_SIZE is too small.
#endif

/**
 * The lowest numerical value legal for a user-defined event.
 *
 * The system reserves all event values from 0 to 0x7FFF, inclusive.
 * User events may use any value in the range 0x8000 to 0xFFFF, inclusive.
 *
 * Note that the same event values might be used by different nanoapps
 * for different meanings.  This is not a concern, as these values only
 * have meaning when paired with the originating nanoapp.
 */
#define CHRE_EVENT_FIRST_USER_VALUE  UINT16_C(0x8000)

/**
 * nanoappHandleEvent argument: struct chreMessageFromHostData
 *
 * The format of the 'message' part of this structure is left undefined,
 * and it's up to the nanoapp and host to have an established protocol
 * beforehand.
 */
#define CHRE_EVENT_MESSAGE_FROM_HOST  UINT16_C(0x0001)

/**
 * nanoappHandleEvent argument: 'cookie' given to chreTimerSet() method.
 *
 * Indicates that a timer has elapsed, in accordance with how chreTimerSet() was
 * invoked.
 */
#define CHRE_EVENT_TIMER  UINT16_C(0x0002)

/**
 * First possible value for CHRE_EVENT_SENSOR events.
 *
 * This allows us to separately define our CHRE_EVENT_SENSOR_* events in
 * chre_sensor.h, without fear of collision with other event values.
 */
#define CHRE_EVENT_SENSOR_FIRST_EVENT  UINT16_C(0x0100)

/**
 * Last possible value for CHRE_EVENT_SENSOR events.
 *
 * This allows us to separately define our CHRE_EVENT_SENSOR_* events in
 * chre_sensor.h, without fear of collision with other event values.
 */
#define CHRE_EVENT_SENSOR_LAST_EVENT  UINT16_C(0x02FF)

/**
 * First in a range of values dedicated for internal CHRE implementation usage.
 *
 * If a CHRE wishes to use events internally, any values within this range
 * are assured not to be taken by future CHRE API additions.
 */
#define CHRE_EVENT_INTERNAL_FIRST_EVENT  UINT16_C(0x7E00)

/**
 * Last in a range of values dedicated for internal CHRE implementation usage.
 *
 * If a CHRE wishes to use events internally, any values within this range
 * are assured not to be taken by future CHRE API additions.
 */
#define CHRE_EVENT_INTERNAL_LAST_EVENT  UINT16_C(0x7FFF)


/**
 * CHRE_EVENT_MESSAGE_FROM_HOST
 */
struct chreMessageFromHostData {
    /**
     * Message type (NOTE: not implemented correctly in the Android N release).
     *
     * In future releases, this will be a message type provided by the host.
     */
    uint32_t reservedMessageType;

    /**
     * The size, in bytes of the following 'message'.
     *
     * This can be 0.
     */
    uint32_t messageSize;

    /**
     * The message from the host.
     *
     * These contents are of a format that the host and nanoapp must have
     * established beforehand.
     *
     * This data is 'messageSize' bytes in length.  Note that if 'messageSize'
     * is 0, this might be NULL.
     */
    const void *message;
};

/**
 * Callback which frees data associated with an event.
 *
 * This callback is (optionally) provided to the chreSendEvent() method as
 * a means for freeing the event data and performing any other cleanup
 * necessary when the event is completed.  When this callback is invoked,
 * 'eventData' is no longer needed and can be released.
 *
 * @param eventType  The 'eventType' argument from chreSendEvent().
 * @param eventData  The 'eventData' argument from chreSendEvent().
 *
 * @see chreSendEvent
 */
typedef void (chreEventCompleteFunction)(uint16_t eventType, void *eventData);

/**
 * Callback which frees a message.
 *
 * This callback is (optionally) provided to the chreSendMessageToHost() method
 * as a means for freeing the message.  When this callback is invoked,
 * 'message' is no longer needed and can be released.  Note that this in
 * no way assures that said message did or did not make it to the host, simply
 * that this memory is no longer needed.
 *
 * @param message  The 'message' argument from chreSendMessageToHost().
 * @param messageSize  The 'messageSize' argument from chreSendMessageToHost().
 *
 * @see chreSendMessageToHost
 */
typedef void (chreMessageFreeFunction)(void *message, size_t messageSize);



/**
 * Enqueue an event to be sent to another nanoapp.
 *
 * Note: This version of the API does not give an easy means to discover
 * another nanoapp's instance ID.  For now, events will need to be sent to/from
 * the host to initially discover these IDs.
 *
 * @param eventType  This is a user-defined event type, of at least the
 *     value CHRE_EVENT_FIRST_USER_VALUE.  It is illegal to attempt to use any
 *     of the CHRE_EVENT_* values reserved for the CHRE.
 * @param eventData  A pointer value that will be understood by the receiving
 *     app.  Note that NULL is perfectly acceptable.  It also is not required
 *     that this be a valid pointer, although if this nanoapp is intended to
 *     work on arbitrary CHRE implementations, then the size of a
 *     pointer cannot be assumed to be a certain size.  Note that the caller
 *     no longer owns this memory after the call.
 * @param freeCallback  A pointer to a callback function.  After the lifetime
 *     of 'eventData' is over (either through successful delivery or the event
 *     being dropped), this callback will be invoked.  This argument is allowed
 *     to be NULL, in which case no callback will be invoked.
 * @param targetInstanceId  The ID of the instance we're delivering this event
 *     to.  Note that this is allowed to be our own instance.
 * @returns true if the event was enqueued, false otherwise.  Note that even
 *     if this method returns 'false', the 'freeCallback' will be invoked,
 *     if non-NULL.  Note in the 'false' case, the 'freeCallback' may be
 *     invoked directly from within chreSendEvent(), so it's necessary
 *     for nanoapp authors to avoid possible recursion with this.
 *
 * @see chreEventDataFreeFunction
 */
bool chreSendEvent(uint16_t eventType, void *eventData,
                   chreEventCompleteFunction *freeCallback,
                   uint32_t targetInstanceId);

/**
 * Send a message to the host.
 *
 * This message is by definition arbitrarily defined.  Since we're not
 * just a passing a pointer to memory around the system, but need to copy
 * this into various buffers to send it to the host, the CHRE
 * implementation cannot be asked to support an arbitrarily large message
 * size.  As a result, we have the CHRE implementation define
 * CHRE_MESSAGE_TO_HOST_MAX_SIZE.
 *
 * CHRE_MESSAGE_TO_HOST_MAX_SIZE is not given a value by the Platform API.  The
 * Platform API does define CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE, and requires
 * that CHRE_MESSAGE_TO_HOST_MAX_SIZE is at least that value.
 *
 * As a result, if your message sizes are all less than
 * CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE, then you have no concerns on any
 * CHRE implementation.  If your message sizes are larger, you'll need to
 * come up with a strategy for splitting your message across several calls
 * to this method.  As long as that strategy works for
 * CHRE_MESSAGE_TO_HOST_MINIMUM_MAX_SIZE, it will work across all CHRE
 * implementations (although on some implementations less calls to this
 * method may be necessary).
 *
 * @param message  Pointer to a block of memory to send to the host.
 *     NULL is acceptable only if messageSize is 0.  If non-NULL, this
 *     must be a legitimate pointer (that is, unlike chreSendEvent(), a small
 *     integral value cannot be cast to a pointer for this).  Note that the
 *     caller no longer owns this memory after the call.
 * @param messageSize  The size, in bytes, of the given message.
 *     This cannot exceed CHRE_MESSAGE_TO_HOST_MAX_SIZE.
 * @param reservedMessageType  Message type sent to the app on the host.
 *     NOTE: In the N release, there is a bug in some HAL implementations
 *     where this data does not make it to the app running on the host.
 *     Nanoapps cannot trust this across all platforms for N, but that
 *     will be fixed in O.
 * @param freeCallback  A pointer to a callback function.  After the lifetime
 *     of 'message' is over (which does not assure that 'message' made it to
 *     the host, just that the transport layer no longer needs this memory),
 *     this callback will be invoked.  This argument is allowed
 *     to be NULL, in which case no callback will be invoked.
 * @returns true if the message was accepted for transmission, false otherwise.
 *     Note that even if this method returns 'false', the 'freeCallback' will
 *     be invoked, if non-NULL.  In either case, the 'freeCallback' may be
 *     invoked directly from within chreSendMessageToHost(), so it's necessary
 *     for nanoapp authors to avoid possible recursion with this.
 *
 * @see chreMessageFreeFunction
 */
bool chreSendMessageToHost(void *message, uint32_t messageSize,
                           uint32_t reservedMessageType,
                           chreMessageFreeFunction *freeCallback);


#ifdef __cplusplus
}
#endif

#endif  /* _CHRE_EVENT_H_ */

