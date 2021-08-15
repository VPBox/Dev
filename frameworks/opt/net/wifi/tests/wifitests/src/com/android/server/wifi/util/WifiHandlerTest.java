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
package com.android.server.wifi.util;

import static org.mockito.Matchers.contains;
import static org.mockito.Mockito.verify;

import android.os.Looper;
import android.os.Message;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.FakeWifiLog;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

/** Unit tests for {@link WifiHandler}. */
@RunWith(JUnit4.class)
@SmallTest
public class WifiHandlerTest {
    private static final String TAG = "WifiHandlerTest";
    private WifiHandler mCodeUnderTest;
    @Spy FakeWifiLog mWifiLog;
    TestLooper mLooper;

    private class WifiHandlerTestClass extends WifiHandler {
        WifiHandlerTestClass(String tag, Looper looper) {
            super(tag, looper);
            super.setWifiLog(mWifiLog);
        }
    }

    @Before public void setUp() {
        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();
        mCodeUnderTest = new WifiHandlerTestClass(TAG, mLooper.getLooper());
    }

    @Test public void testHandleMessage() {
        Message msg = Message.obtain();
        msg.what = 0;
        msg.sendingUid = 0;
        mCodeUnderTest.handleMessage(msg);
        verify(mWifiLog).trace(contains("message"));
    }
}
