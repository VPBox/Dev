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


#include <gtest/gtest.h>

#include "TestInputListener.h"

namespace android {

// --- TestInputListener ---

TestInputListener::TestInputListener() { }

TestInputListener::~TestInputListener() { }

void TestInputListener::assertNotifyConfigurationChangedWasCalled(
        NotifyConfigurationChangedArgs* outEventArgs) {
    ASSERT_FALSE(mNotifyConfigurationChangedArgsQueue.empty())
            << "Expected notifyConfigurationChanged() to have been called.";
    if (outEventArgs) {
        *outEventArgs = *mNotifyConfigurationChangedArgsQueue.begin();
    }
    mNotifyConfigurationChangedArgsQueue.erase(mNotifyConfigurationChangedArgsQueue.begin());
}

void TestInputListener::assertNotifyConfigurationChangedWasNotCalled() {
    ASSERT_TRUE(mNotifyConfigurationChangedArgsQueue.empty())
            << "Expected notifyConfigurationChanged() to not have been called.";
}

void TestInputListener::assertNotifyDeviceResetWasCalled(
        NotifyDeviceResetArgs* outEventArgs) {
    ASSERT_FALSE(mNotifyDeviceResetArgsQueue.empty())
            << "Expected notifyDeviceReset() to have been called.";
    if (outEventArgs) {
        *outEventArgs = *mNotifyDeviceResetArgsQueue.begin();
    }
    mNotifyDeviceResetArgsQueue.erase(mNotifyDeviceResetArgsQueue.begin());
}

void TestInputListener::assertNotifyDeviceResetWasNotCalled() {
    ASSERT_TRUE(mNotifyDeviceResetArgsQueue.empty())
            << "Expected notifyDeviceReset() to not have been called.";
}

void TestInputListener::assertNotifyKeyWasCalled(NotifyKeyArgs* outEventArgs) {
    ASSERT_FALSE(mNotifyKeyArgsQueue.empty()) << "Expected notifyKey() to have been called.";
    if (outEventArgs) {
        *outEventArgs = *mNotifyKeyArgsQueue.begin();
    }
    mNotifyKeyArgsQueue.erase(mNotifyKeyArgsQueue.begin());
}

void TestInputListener::assertNotifyKeyWasNotCalled() {
    ASSERT_TRUE(mNotifyKeyArgsQueue.empty()) << "Expected notifyKey() to not have been called.";
}

void TestInputListener::assertNotifyMotionWasCalled(NotifyMotionArgs* outEventArgs) {
    ASSERT_FALSE(mNotifyMotionArgsQueue.empty()) << "Expected notifyMotion() to have been called.";
    if (outEventArgs) {
        *outEventArgs = *mNotifyMotionArgsQueue.begin();
    }
    mNotifyMotionArgsQueue.erase(mNotifyMotionArgsQueue.begin());
}

void TestInputListener::assertNotifyMotionWasNotCalled() {
    ASSERT_TRUE(mNotifyMotionArgsQueue.empty())
            << "Expected notifyMotion() to not have been called.";
}

void TestInputListener::assertNotifySwitchWasCalled(NotifySwitchArgs* outEventArgs) {
    ASSERT_FALSE(mNotifySwitchArgsQueue.empty())
            << "Expected notifySwitch() to have been called.";
    if (outEventArgs) {
        *outEventArgs = *mNotifySwitchArgsQueue.begin();
    }
    mNotifySwitchArgsQueue.erase(mNotifySwitchArgsQueue.begin());
}

void TestInputListener::notifyConfigurationChanged(const NotifyConfigurationChangedArgs* args) {
    mNotifyConfigurationChangedArgsQueue.push_back(*args);
}

void TestInputListener::notifyDeviceReset(const NotifyDeviceResetArgs* args) {
    mNotifyDeviceResetArgsQueue.push_back(*args);
}

void TestInputListener::notifyKey(const NotifyKeyArgs* args) {
    mNotifyKeyArgsQueue.push_back(*args);
}

void TestInputListener::notifyMotion(const NotifyMotionArgs* args) {
    mNotifyMotionArgsQueue.push_back(*args);
}

void TestInputListener::notifySwitch(const NotifySwitchArgs* args) {
        mNotifySwitchArgsQueue.push_back(*args);
    }


} // namespace android
