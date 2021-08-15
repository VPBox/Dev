/*
 * Copyright 2018 The Android Open Source Project
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

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertNotNull;
import static junit.framework.Assert.assertNull;
import static junit.framework.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto.WifiWakeStats;

import org.junit.Before;
import org.junit.Test;

@SmallTest
public class WifiWakeMetricsTest {

    private WifiWakeMetrics mWifiWakeMetrics;

    @Before
    public void setUp() {
        mWifiWakeMetrics = new WifiWakeMetrics();
    }

    @Test
    public void buildsEmptyProto() {
        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 0);
        assertEquals(wifiWakeStats.numWakeups, 0);
        assertEquals(wifiWakeStats.numIgnoredStarts, 0);
        assertEquals(wifiWakeStats.sessions.length, 0);
    }

    @Test
    public void buildsMultiSessionProto_fewerThanMax() {
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(1 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(2 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 3);
        assertEquals(wifiWakeStats.numWakeups, 0);
        assertEquals(wifiWakeStats.sessions.length, 3);
    }

    @Test
    public void buildsMultiSessionProto_greaterThanMax() {
        int numSessions = WifiWakeMetrics.MAX_RECORDED_SESSIONS + 1;
        for (int i = 0; i < numSessions; i++) {
            mWifiWakeMetrics.recordStartEvent(i);
            mWifiWakeMetrics.recordInitializeEvent(i, i);
            mWifiWakeMetrics.recordUnlockEvent(i);
            mWifiWakeMetrics.recordWakeupEvent(i);
            mWifiWakeMetrics.recordResetEvent(i);
        }

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, numSessions);
        assertEquals(wifiWakeStats.numWakeups, numSessions);
        assertEquals(wifiWakeStats.sessions.length, WifiWakeMetrics.MAX_RECORDED_SESSIONS);

        // ensure that the first (not last) MAX_RECORDED_SESSIONS are recorded
        for (int i = 0; i < WifiWakeMetrics.MAX_RECORDED_SESSIONS; i++) {
            WifiWakeStats.Session session = wifiWakeStats.sessions[i];
            assertNotNull(session);
            assertEquals(session.lockedNetworksAtStart, i);
            assertEquals(session.lockedNetworksAtInitialize, i);
        }
    }

    @Test
    public void buildProtoCountsWakes() {
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordWakeupEvent(3 /* numScans */);
        mWifiWakeMetrics.recordResetEvent(3 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(1 /* numNetworks */);
        mWifiWakeMetrics.recordWakeupEvent(3 /* numScans */);
        mWifiWakeMetrics.recordResetEvent(3 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(2 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 3);
        assertEquals(wifiWakeStats.numWakeups, 2);
        assertEquals(wifiWakeStats.sessions.length, 3);
    }

    @Test
    public void buildProtoDoesNotCountWakeInCurrentSession() {
        mWifiWakeMetrics.recordStartEvent(1 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(2 /* numNetworks */);
        mWifiWakeMetrics.recordWakeupEvent(3 /* numScans */);

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 1);
        assertEquals(wifiWakeStats.numWakeups, 0);
        assertEquals(wifiWakeStats.sessions.length, 1);
    }

    @Test
    public void buildProtoCountsIgnoredStarts() {
        mWifiWakeMetrics.recordIgnoredStart();
        mWifiWakeMetrics.recordIgnoredStart();
        mWifiWakeMetrics.recordIgnoredStart();

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numIgnoredStarts, 3);
    }

    @Test
    public void buildProtoDoesNotIncludeCurrentSession() {
        mWifiWakeMetrics.recordStartEvent(1 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(2 /* numNetworks */);

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 1);
        assertEquals(wifiWakeStats.sessions.length, 1);
        assertEquals(wifiWakeStats.sessions[0].lockedNetworksAtStart, 1);
    }

    @Test
    public void ignoresEventsIfStartNotCalled() {
        mWifiWakeMetrics.recordUnlockEvent(1 /* numScans */);
        mWifiWakeMetrics.recordWakeupEvent(1 /* numScans */);
        mWifiWakeMetrics.recordResetEvent(1 /* numScans */);

        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 0);
        assertEquals(wifiWakeStats.sessions.length, 0);
    }

    @Test
    public void ignoresEventsAfterResetAndBeforeStartCalled() {
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordWakeupEvent(1 /* numScans */);
        mWifiWakeMetrics.recordResetEvent(1 /* numScans */);

        mWifiWakeMetrics.recordWakeupEvent(10 /* numScans */);

        // verify only 1 session
        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 1);
        assertEquals(wifiWakeStats.sessions.length, 1);

        // verify it didn't overwrite session
        WifiWakeStats.Session session = wifiWakeStats.sessions[0];
        assertNotNull(session.wakeupEvent);
        assertEquals(session.wakeupEvent.elapsedScans, 1);
    }

    @Test
    public void clearRemovesSessions() {
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordWakeupEvent(3 /* numScans */);
        mWifiWakeMetrics.recordResetEvent(3 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordIgnoredStart();
        mWifiWakeMetrics.recordIgnoredStart();
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        // verify sessions
        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 3);
        assertEquals(wifiWakeStats.numWakeups, 1);
        assertEquals(wifiWakeStats.numIgnoredStarts, 2);
        assertEquals(wifiWakeStats.sessions.length, 3);

        mWifiWakeMetrics.clear();
        wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 0);
        assertEquals(wifiWakeStats.numWakeups, 0);
        assertEquals(wifiWakeStats.numIgnoredStarts, 0);
        assertEquals(wifiWakeStats.sessions.length, 0);
    }

    @Test
    public void clearDoesNotInterruptCurrentSession() {
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);
        mWifiWakeMetrics.recordStartEvent(0 /* numNetworks */);
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        mWifiWakeMetrics.recordStartEvent(1 /* numNetworks */);
        mWifiWakeMetrics.clear();
        mWifiWakeMetrics.recordResetEvent(0 /* numScans */);

        // keeps the current session
        WifiWakeStats wifiWakeStats = mWifiWakeMetrics.buildProto();
        assertNotNull(wifiWakeStats);
        assertEquals(wifiWakeStats.numSessions, 1);
        assertEquals(wifiWakeStats.sessions.length, 1);
    }

    @Test
    public void session_buildsEmptyProto() {
        WifiWakeMetrics.Session session =
                new WifiWakeMetrics.Session(1 /* numNetworks */, 1000 /* timestamp */);

        WifiWakeStats.Session sessionProto = session.buildProto();
        assertNotNull(sessionProto);
        assertEquals(sessionProto.lockedNetworksAtStart, 1);
        assertEquals(sessionProto.startTimeMillis, 1000);
        assertNull(sessionProto.unlockEvent);
        assertNull(sessionProto.wakeupEvent);
        assertNull(sessionProto.resetEvent);
    }

    @Test
    public void session_recordsEvents() {
        WifiWakeMetrics.Session session =
                new WifiWakeMetrics.Session(1 /* numNetworks */, 1000 /* timestamp */);

        session.recordUnlockEvent(1 /* numScans */, 1100 /* timestamp */);
        assertNotNull(session.mUnlockEvent);
        assertEquals(session.mUnlockEvent.mNumScans, 1);
        assertEquals(session.mUnlockEvent.mElapsedTime, 100);

        session.recordWakeupEvent(2 /* numScans */, 1200 /* timestamp */);
        assertNotNull(session.mWakeupEvent);
        assertEquals(session.mWakeupEvent.mNumScans, 2);
        assertEquals(session.mWakeupEvent.mElapsedTime, 200);

        session.recordResetEvent(3 /* numScans */, 1300 /* timestamp */);
        assertNotNull(session.mResetEvent);
        assertEquals(session.mResetEvent.mNumScans, 3);
        assertEquals(session.mResetEvent.mElapsedTime, 300);
    }

    @Test
    public void session_buildProto() {
        WifiWakeMetrics.Session session =
                new WifiWakeMetrics.Session(1 /* numNetworks */, 1000 /* timestamp */);

        session.recordInitializeEvent(1 /* numScans */, 2 /* numNetworks */, 1100 /* timestamp */);
        session.recordUnlockEvent(2 /* numScans */, 1200 /* timestamp */);
        session.recordWakeupEvent(3 /* numScans */, 1300 /* timestamp */);
        session.recordResetEvent(4 /* numScans */, 1400 /* timestamp */);

        WifiWakeStats.Session sessionProto = session.buildProto();
        assertNotNull(sessionProto);
        assertEquals(sessionProto.lockedNetworksAtStart, 1);
        assertEquals(sessionProto.lockedNetworksAtInitialize, 2);
        assertEquals(sessionProto.startTimeMillis, 1000);

        verifyEventProto(sessionProto.initializeEvent, 1, 100);
        verifyEventProto(sessionProto.unlockEvent, 2, 200);
        verifyEventProto(sessionProto.wakeupEvent, 3, 300);
        verifyEventProto(sessionProto.resetEvent, 4, 400);
    }

    @Test
    public void session_ignoresRepeatedEvents() {
        WifiWakeMetrics.Session session =
                new WifiWakeMetrics.Session(1 /* numNetworks */, 1000 /* timestamp */);

        session.recordResetEvent(1 /* numScans */, 1100 /* timestamp */);
        assertNotNull(session.mResetEvent);
        assertEquals(session.mResetEvent.mNumScans, 1);
        assertEquals(session.mResetEvent.mElapsedTime, 100);

        session.recordResetEvent(2 /* numScans */, 1200 /* timestamp */);
        assertEquals(session.mResetEvent.mNumScans, 1);
        assertEquals(session.mResetEvent.mElapsedTime, 100);
    }

    @Test
    public void session_hasWakeupTriggered() {
        WifiWakeMetrics.Session session =
                new WifiWakeMetrics.Session(0 /* numNetworks */, 1000 /* timestamp */);
        assertFalse(session.hasWakeupTriggered());

        session.recordInitializeEvent(3 /* numScans */, 0 /* numNetworks */, 1100 /* timestamp */);
        assertFalse(session.hasWakeupTriggered());

        session.recordWakeupEvent(3 /* numScans */, 1100 /* timestamp */);
        assertTrue(session.hasWakeupTriggered());

        session.recordResetEvent(3 /* numScans */, 1100 /* timestamp */);
        assertTrue(session.hasWakeupTriggered());
    }

    @Test
    public void event_buildsProto() {
        WifiWakeMetrics.Event event =
                new WifiWakeMetrics.Event(1 /* numScans */, 1000 /* elapsedTime */);

        verifyEventProto(event.buildProto(), 1, 1000);
    }

    private void verifyEventProto(WifiWakeStats.Session.Event event, int scans, int elapsedTime) {
        assertNotNull(event);
        assertEquals(event.elapsedScans, scans);
        assertEquals(event.elapsedTimeMillis, elapsedTime);
    }
}
