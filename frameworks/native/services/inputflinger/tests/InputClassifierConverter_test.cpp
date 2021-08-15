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

#include "../InputClassifierConverter.h"

#include <gtest/gtest.h>
#include <utils/BitSet.h>


using namespace android::hardware::input;

namespace android {

// --- InputClassifierConverterTest ---

static NotifyMotionArgs generateBasicMotionArgs() {
    // Create a basic motion event for testing
    PointerProperties properties;
    properties.id = 0;
    properties.toolType = AMOTION_EVENT_TOOL_TYPE_FINGER;

    PointerCoords coords;
    coords.clear();
    coords.setAxisValue(AMOTION_EVENT_AXIS_X, 1);
    coords.setAxisValue(AMOTION_EVENT_AXIS_Y, 2);
    coords.setAxisValue(AMOTION_EVENT_AXIS_SIZE, 0.5);
    static constexpr nsecs_t downTime = 2;
    NotifyMotionArgs motionArgs(1/*sequenceNum*/, downTime/*eventTime*/, 3/*deviceId*/,
            AINPUT_SOURCE_ANY, ADISPLAY_ID_DEFAULT, 4/*policyFlags*/, AMOTION_EVENT_ACTION_DOWN,
            0/*actionButton*/, 0/*flags*/, AMETA_NONE, 0/*buttonState*/, MotionClassification::NONE,
            AMOTION_EVENT_EDGE_FLAG_NONE, 5/*deviceTimestamp*/,
            1/*pointerCount*/, &properties, &coords, 0/*xPrecision*/, 0/*yPrecision*/,
            downTime, {}/*videoFrames*/);
    return motionArgs;
}

static float getMotionEventAxis(common::V1_0::PointerCoords coords,
        common::V1_0::Axis axis) {
    uint32_t index = BitSet64::getIndexOfBit(static_cast<uint64_t>(coords.bits),
            static_cast<uint64_t>(axis));
    return coords.values[index];
}

/**
 * Check that coordinates get converted properly from the framework's PointerCoords
 * to the hidl PointerCoords in input::common.
 */
TEST(InputClassifierConverterTest, PointerCoordsAxes) {
    const NotifyMotionArgs motionArgs = generateBasicMotionArgs();
    ASSERT_EQ(1, motionArgs.pointerCoords[0].getX());
    ASSERT_EQ(2, motionArgs.pointerCoords[0].getY());
    ASSERT_EQ(0.5, motionArgs.pointerCoords[0].getAxisValue(AMOTION_EVENT_AXIS_SIZE));
    ASSERT_EQ(3U, BitSet64::count(motionArgs.pointerCoords[0].bits));

    common::V1_0::MotionEvent motionEvent = notifyMotionArgsToHalMotionEvent(motionArgs);

    ASSERT_EQ(getMotionEventAxis(motionEvent.pointerCoords[0], common::V1_0::Axis::X),
            motionArgs.pointerCoords[0].getX());
    ASSERT_EQ(getMotionEventAxis(motionEvent.pointerCoords[0], common::V1_0::Axis::Y),
            motionArgs.pointerCoords[0].getY());
    ASSERT_EQ(getMotionEventAxis(motionEvent.pointerCoords[0], common::V1_0::Axis::SIZE),
            motionArgs.pointerCoords[0].getAxisValue(AMOTION_EVENT_AXIS_SIZE));
    ASSERT_EQ(BitSet64::count(motionArgs.pointerCoords[0].bits),
            BitSet64::count(motionEvent.pointerCoords[0].bits));
}

} // namespace android
