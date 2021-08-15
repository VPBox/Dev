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
package com.android.server.wifi.p2p;

import static org.junit.Assert.assertEquals;

import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pGroupList;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.nano.WifiMetricsProto.GroupEvent;
import com.android.server.wifi.nano.WifiMetricsProto.P2pConnectionEvent;
import com.android.server.wifi.nano.WifiMetricsProto.WifiP2pStats;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiP2pMetrics}.
 */
@SmallTest
public class WifiP2pMetricsTest {
    @Mock Clock mClock;
    WifiP2pMetrics mWifiP2pMetrics;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mWifiP2pMetrics = new WifiP2pMetrics(mClock);
    }

    /**
     * Test that startConnectionEvent and endConnectionEvent can be called repeatedly and out of
     * order. Only tests no exception occurs and the count is correct.
     * @throws Exception
     */
    @Test
    public void startAndEndConnectionEventSucceeds() throws Exception {
        WifiP2pConfig config = new WifiP2pConfig();
        WifiP2pStats stats;

        // Start and end Connection event.
        mWifiP2pMetrics.startConnectionEvent(P2pConnectionEvent.CONNECTION_FRESH, null);
        mWifiP2pMetrics.endConnectionEvent(P2pConnectionEvent.CLF_NONE);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.connectionEvent.length);

        // Start and end Connection event.
        mWifiP2pMetrics.startConnectionEvent(P2pConnectionEvent.CONNECTION_FRESH, config);
        mWifiP2pMetrics.endConnectionEvent(P2pConnectionEvent.CLF_NONE);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(2, stats.connectionEvent.length);

        // End Connection event without starting one.
        // this would create a new connection event immediately.
        mWifiP2pMetrics.endConnectionEvent(P2pConnectionEvent.CLF_TIMEOUT);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(3, stats.connectionEvent.length);

        // Start two ConnectionEvents in a row.
        // The current active un-ended connection event is excluded.
        mWifiP2pMetrics.startConnectionEvent(P2pConnectionEvent.CONNECTION_REINVOKE, config);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(3, stats.connectionEvent.length);

        // The last un-ended connection is ended.
        // The current active un-ended connection event is excluded.
        mWifiP2pMetrics.startConnectionEvent(P2pConnectionEvent.CONNECTION_REINVOKE, config);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(4, stats.connectionEvent.length);
    }

    /**
     * Test that startGroupEvent and endGroupEvent can be called repeastedly and out of order.
     * Only tests no exception occurs and the count is correct.
     * @throws Exception
     */
    @Test
    public void startAndEndGroupEventSucceeds() throws Exception {
        WifiP2pGroup group = new WifiP2pGroup();
        WifiP2pStats stats;

        // Start and end Group event with null group.
        // this won't generate valid group event.
        mWifiP2pMetrics.startGroupEvent(null);
        mWifiP2pMetrics.endGroupEvent();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(0, stats.groupEvent.length);

        // Start and end Group event.
        mWifiP2pMetrics.startGroupEvent(group);
        mWifiP2pMetrics.endGroupEvent();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.groupEvent.length);

        // End Group event without starting one.
        // just ignore this event.
        mWifiP2pMetrics.endGroupEvent();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.groupEvent.length);

        // Start two GroupEvents in a row.
        // The current active un-ended group event is excluded.
        mWifiP2pMetrics.startGroupEvent(group);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.groupEvent.length);

        // The last un-ended group is ended.
        // The current active un-ended group event is excluded.
        mWifiP2pMetrics.startGroupEvent(group);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(2, stats.groupEvent.length);
    }

    /**
     * Test that peer scan counter works normally.
     * @throws Exception
     */
    @Test
    public void increasePeerScans() throws Exception {
        WifiP2pStats stats;
        mWifiP2pMetrics.incrementPeerScans();
        mWifiP2pMetrics.incrementPeerScans();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(2, stats.numTotalPeerScans);

        mWifiP2pMetrics.clear();

        mWifiP2pMetrics.incrementPeerScans();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.numTotalPeerScans);
    }

    /**
     * Test that service scan counter works normally.
     * @throws Exception
     */
    @Test
    public void increaseServiceScans() throws Exception {
        WifiP2pStats stats;

        mWifiP2pMetrics.incrementServiceScans();
        mWifiP2pMetrics.incrementServiceScans();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(2, stats.numTotalServiceScans);

        mWifiP2pMetrics.clear();

        mWifiP2pMetrics.incrementServiceScans();
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.numTotalServiceScans);
    }

    /**
     * Test that updating persistent group number works normally.
     * @throws Exception
     */
    @Test
    public void updatePersistentGroup() throws Exception {
        WifiP2pStats stats;
        WifiP2pGroupList mGroups = new WifiP2pGroupList();

        mWifiP2pMetrics.updatePersistentGroup(mGroups);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(0, stats.numPersistentGroup);

        WifiP2pGroup group1 = new WifiP2pGroup();
        group1.setNetworkId(1);
        WifiP2pGroup group2 = new WifiP2pGroup();
        group2.setNetworkId(2);
        WifiP2pGroup group3 = new WifiP2pGroup();
        group3.setNetworkId(4);
        mGroups.add(group1);
        mGroups.add(group2);
        mGroups.add(group3);
        mWifiP2pMetrics.updatePersistentGroup(mGroups);
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(3, mGroups.getGroupList().size());
        assertEquals(3, stats.numPersistentGroup);
    }

    /**
     * Test that updateGroupEvent work normally.
     * @throws Exception
     */
    @Test
    public void updateGroupEvent() throws Exception {
        WifiP2pStats stats;
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(1);

        mWifiP2pMetrics.startGroupEvent(group);

        // total 4 new clients, and only one stays.
        group.addClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:04");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(group);

        mWifiP2pMetrics.endGroupEvent();

        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.groupEvent.length);
        GroupEvent event = stats.groupEvent[0];
        assertEquals(1, event.numConnectedClients);
        assertEquals(4, event.numCumulativeClients);
    }

    /**
     * Test that updateGroupEvent is called before calling startGroupEvent.
     * @throws Exception
     */
    @Test
    public void updateGroupEventWithNoCurrentGroup() throws Exception {
        WifiP2pStats stats;
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(1);

        // total 4 new clients, and only one stays.
        group.addClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.addClient("11:22:33:44:55:04");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(group);
        group.removeClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(group);

        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(0, stats.groupEvent.length);
    }

    /**
     * Test that updateGroupEvent is called with different group from current group.
     * @throws Exception
     */
    @Test
    public void updateGroupEventWithDifferentGroup() throws Exception {
        WifiP2pStats stats;
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(1);
        WifiP2pGroup differentGroup = new WifiP2pGroup();
        group.setNetworkId(2);

        mWifiP2pMetrics.startGroupEvent(group);

        // total 4 new clients, and only one stays.
        differentGroup.addClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.addClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.removeClient("11:22:33:44:55:01");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.addClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.addClient("11:22:33:44:55:04");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.removeClient("11:22:33:44:55:03");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);
        differentGroup.removeClient("11:22:33:44:55:02");
        mWifiP2pMetrics.updateGroupEvent(differentGroup);

        mWifiP2pMetrics.endGroupEvent();

        // since it is different group, the is still 0 client in current group.
        stats = mWifiP2pMetrics.consolidateProto();
        assertEquals(1, stats.groupEvent.length);
        GroupEvent event = stats.groupEvent[0];
        assertEquals(0, event.numConnectedClients);
        assertEquals(0, event.numCumulativeClients);
    }
}
