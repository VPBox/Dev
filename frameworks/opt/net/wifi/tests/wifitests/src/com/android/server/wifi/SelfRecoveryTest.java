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

package com.android.server.wifi;

import static org.mockito.Mockito.*;
import static org.mockito.MockitoAnnotations.*;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

/**
 * Unit tests for {@link com.android.server.wifi.SelfRecovery}.
 */
@SmallTest
public class SelfRecoveryTest {
    SelfRecovery mSelfRecovery;
    @Mock WifiController mWifiController;
    @Mock Clock mClock;

    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mSelfRecovery = new SelfRecovery(mWifiController, mClock);
    }

    /**
     * Verifies that invocations of {@link SelfRecovery#trigger(int)} with valid reasons will send
     * the restart message to {@link WifiController}.
     */
    @Test
    public void testValidTriggerReasonsSendMessageToWifiController() {
        mSelfRecovery.trigger(SelfRecovery.REASON_LAST_RESORT_WATCHDOG);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI), anyInt());
        reset(mWifiController);

        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(SelfRecovery.MAX_RESTARTS_TIME_WINDOW_MILLIS + 1);
        mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI), anyInt());
        reset(mWifiController);
    }

    /**
     * Verifies that invocations of {@link SelfRecovery#trigger(int)} with invalid reasons will not
     * send the restart message to {@link WifiController}.
     */
    @Test
    public void testInvalidTriggerReasonsDoesNotSendMessageToWifiController() {
        mSelfRecovery.trigger(-1);
        verify(mWifiController, never()).sendMessage(anyInt(), anyString());

        mSelfRecovery.trigger(8);
        verify(mWifiController, never()).sendMessage(anyInt(), anyString());
    }

    /**
     * Verifies that a STA interface down event will trigger WifiController to disable wifi.
     */
    @Test
    public void testStaIfaceDownDisablesWifi() {
        mSelfRecovery.trigger(SelfRecovery.REASON_STA_IFACE_DOWN);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_DISABLE_WIFI));
    }

    /**
     * Verifies that invocations of {@link SelfRecovery#trigger(int)} for REASON_HAL_CRASH &
     * REASON_WIFICOND_CRASH are limited to {@link SelfRecovery#MAX_RESTARTS_IN_TIME_WINDOW} in a
     * {@link SelfRecovery#MAX_RESTARTS_TIME_WINDOW_MILLIS} millisecond time window.
     */
    @Test
    public void testTimeWindowLimiting_typicalUse() {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(0L);
        // Fill up the SelfRecovery's restart time window buffer, ensure all the restart triggers
        // aren't ignored
        for (int i = 0; i < SelfRecovery.MAX_RESTARTS_IN_TIME_WINDOW / 2; i++) {
            mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
            verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                                                anyInt());
            reset(mWifiController);

            mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
            verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                                                anyInt());
            reset(mWifiController);
        }
        if ((SelfRecovery.MAX_RESTARTS_IN_TIME_WINDOW % 2) == 1) {
            mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
            verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                                                anyInt());
            reset(mWifiController);
        }

        // Verify that further attempts to trigger restarts disable wifi
        mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
        verify(mWifiController, never()).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                anyString());
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_DISABLE_WIFI));
        reset(mWifiController);

        mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
        verify(mWifiController, never()).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                anyString());
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_DISABLE_WIFI));
        reset(mWifiController);

        // Verify L.R.Watchdog can still restart things (It has its own complex limiter)
        mSelfRecovery.trigger(SelfRecovery.REASON_LAST_RESORT_WATCHDOG);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                                            anyInt());
        reset(mWifiController);

        // Verify Sta Interface Down will still disable wifi
        mSelfRecovery.trigger(SelfRecovery.REASON_STA_IFACE_DOWN);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_DISABLE_WIFI));
        reset(mWifiController);

        // now TRAVEL FORWARDS IN TIME and ensure that more restarts can occur
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(SelfRecovery.MAX_RESTARTS_TIME_WINDOW_MILLIS + 1);
        mSelfRecovery.trigger(SelfRecovery.REASON_LAST_RESORT_WATCHDOG);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI), anyInt());
        reset(mWifiController);

        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(SelfRecovery.MAX_RESTARTS_TIME_WINDOW_MILLIS + 1);
        mSelfRecovery.trigger(SelfRecovery.REASON_WIFINATIVE_FAILURE);
        verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI), anyInt());
        reset(mWifiController);
    }

    /**
     * Verifies that invocations of {@link SelfRecovery#trigger(int)} for
     * REASON_LAST_RESORT_WATCHDOG are NOT limited to
     * {@link SelfRecovery#MAX_RESTARTS_IN_TIME_WINDOW} in a
     * {@link SelfRecovery#MAX_RESTARTS_TIME_WINDOW_MILLIS} millisecond time window.
     */
    @Test
    public void testTimeWindowLimiting_lastResortWatchdog_noEffect() {
        for (int i = 0; i < SelfRecovery.MAX_RESTARTS_IN_TIME_WINDOW * 2; i++) {
            // Verify L.R.Watchdog can still restart things (It has it's own complex limiter)
            mSelfRecovery.trigger(SelfRecovery.REASON_LAST_RESORT_WATCHDOG);
            verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI),
                                                anyInt());
            reset(mWifiController);
        }
    }

    /**
     * Verifies that invocations of {@link SelfRecovery#trigger(int)} for
     * REASON_STA_IFACE_DOWN are NOT limited to
     * {@link SelfRecovery#MAX_RESTARTS_IN_TIME_WINDOW} in a
     * {@link SelfRecovery#MAX_RESTARTS_TIME_WINDOW_MILLIS} millisecond time window.
     */
    @Test
    public void testTimeWindowLimiting_staIfaceDown_noEffect() {
        for (int i = 0; i < SelfRecovery.MAX_RESTARTS_IN_TIME_WINDOW * 2; i++) {
            mSelfRecovery.trigger(SelfRecovery.REASON_STA_IFACE_DOWN);
            verify(mWifiController).sendMessage(eq(WifiController.CMD_RECOVERY_DISABLE_WIFI));
            verify(mWifiController, never())
                    .sendMessage(eq(WifiController.CMD_RECOVERY_RESTART_WIFI), anyInt());
            reset(mWifiController);
        }
    }
}
