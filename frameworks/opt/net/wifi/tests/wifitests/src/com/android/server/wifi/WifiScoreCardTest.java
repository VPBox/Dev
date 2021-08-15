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

package com.android.server.wifi;

import static com.android.server.wifi.util.NativeUtil.hexStringFromByteArray;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.net.MacAddress;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiSsid;
import android.util.Base64;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiScoreCardProto.AccessPoint;
import com.android.server.wifi.WifiScoreCardProto.Event;
import com.android.server.wifi.WifiScoreCardProto.Network;
import com.android.server.wifi.WifiScoreCardProto.NetworkList;
import com.android.server.wifi.WifiScoreCardProto.Signal;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Unit tests for {@link com.android.server.wifi.WifiScoreCard}.
 */
@SmallTest
public class WifiScoreCardTest {

    static final WifiSsid TEST_SSID_1 = WifiSsid.createFromAsciiEncoded("Joe's Place");
    static final WifiSsid TEST_SSID_2 = WifiSsid.createFromAsciiEncoded("Poe's Raven");

    static final MacAddress TEST_BSSID_1 = MacAddress.fromString("aa:bb:cc:dd:ee:ff");
    static final MacAddress TEST_BSSID_2 = MacAddress.fromString("1:2:3:4:5:6");

    static final int TEST_NETWORK_AGENT_ID = 123;
    static final int TEST_NETWORK_CONFIG_ID = 1492;

    static final double TOL = 1e-6; // for assertEquals(double, double, tolerance)

    WifiScoreCard mWifiScoreCard;

    @Mock Clock mClock;
    @Mock WifiScoreCard.MemoryStore mMemoryStore;

    final ArrayList<String> mKeys = new ArrayList<>();
    final ArrayList<WifiScoreCard.BlobListener> mBlobListeners = new ArrayList<>();
    final ArrayList<byte[]> mBlobs = new ArrayList<>();

    long mMilliSecondsSinceBoot;
    ExtendedWifiInfo mWifiInfo;

    void millisecondsPass(long ms) {
        mMilliSecondsSinceBoot += ms;
        when(mClock.getElapsedSinceBootMillis()).thenReturn(mMilliSecondsSinceBoot);
        when(mClock.getWallClockMillis()).thenReturn(mMilliSecondsSinceBoot + 1_500_000_000_000L);
    }

    void secondsPass(long s) {
        millisecondsPass(s * 1000);
    }

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mKeys.clear();
        mBlobListeners.clear();
        mBlobs.clear();
        mMilliSecondsSinceBoot = 0;
        mWifiInfo = new ExtendedWifiInfo();
        mWifiInfo.setSSID(TEST_SSID_1);
        mWifiInfo.setBSSID(TEST_BSSID_1.toString());
        mWifiInfo.setNetworkId(TEST_NETWORK_CONFIG_ID);
        millisecondsPass(0);
        mWifiScoreCard = new WifiScoreCard(mClock, "some seed");
    }

    /**
     * Test generic update
     */
    @Test
    public void testUpdate() throws Exception {
        mWifiInfo.setSSID(TEST_SSID_1);
        mWifiInfo.setBSSID(TEST_BSSID_1.toString());

        mWifiScoreCard.noteIpConfiguration(mWifiInfo);

        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.fetchByBssid(TEST_BSSID_1);
        assertTrue(perBssid.id > 0);
        assertNotNull(perBssid.l2Key);
        assertTrue("L2Key length should be more than 16.", perBssid.l2Key.length() > 16);

        mWifiInfo.setBSSID(TEST_BSSID_2.toString());

        mWifiScoreCard.noteIpConfiguration(mWifiInfo);

        assertEquals(perBssid, mWifiScoreCard.fetchByBssid(TEST_BSSID_1));
        assertNotEquals(perBssid.id, mWifiScoreCard.fetchByBssid(TEST_BSSID_2).id);
        assertNotEquals(perBssid.l2Key, mWifiScoreCard.fetchByBssid(TEST_BSSID_2).l2Key);
    }

    /**
     * Test identifiers.
     */
    @Test
    public void testIdentifiers() throws Exception {
        mWifiInfo.setSSID(TEST_SSID_1);
        mWifiInfo.setBSSID(TEST_BSSID_1.toString());
        Pair<String, String> p1 = mWifiScoreCard.getL2KeyAndGroupHint(mWifiInfo);
        assertNotNull(p1.first);
        assertNotNull(p1.second);
        mWifiInfo.setBSSID(TEST_BSSID_2.toString());
        Pair<String, String> p2 = mWifiScoreCard.getL2KeyAndGroupHint(mWifiInfo);
        assertNotEquals(p1.first, p2.first);
        assertEquals(p1.second, p2.second);
        mWifiInfo.setBSSID(null);
        Pair<String, String> p3 = mWifiScoreCard.getL2KeyAndGroupHint(mWifiInfo);
        assertNull(p3.first);
        assertNull(p3.second);
    }

    /**
     * Test rssi poll updates
     */
    @Test
    public void testRssiPollUpdates() throws Exception {
        // Start out on one frequency
        mWifiInfo.setFrequency(5805);
        mWifiInfo.setRssi(-77);
        mWifiInfo.setLinkSpeed(12);
        mWifiScoreCard.noteSignalPoll(mWifiInfo);
        // Switch channels for a bit
        mWifiInfo.setFrequency(5290);
        mWifiInfo.setRssi(-66);
        mWifiInfo.setLinkSpeed(666);
        mWifiScoreCard.noteSignalPoll(mWifiInfo);
        // Back to the first channel
        mWifiInfo.setFrequency(5805);
        mWifiInfo.setRssi(-55);
        mWifiInfo.setLinkSpeed(86);
        mWifiScoreCard.noteSignalPoll(mWifiInfo);

        double expectSum = -77 + -55;
        double expectSumSq = 77 * 77 + 55 * 55;

        // Now verify
        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.fetchByBssid(TEST_BSSID_1);
        // Looking up the same thing twice should yield the same object.
        assertTrue(perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                == perBssid.lookupSignal(Event.SIGNAL_POLL, 5805));
        // Check the rssi statistics for the first channel
        assertEquals(2, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805).rssi.count);
        assertEquals(expectSum, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.sum, TOL);
        assertEquals(expectSumSq, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.sumOfSquares, TOL);
        assertEquals(-77.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.minValue, TOL);
        assertEquals(-55.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.maxValue, TOL);
        // Check the rssi statistics for the second channel
        assertEquals(1, perBssid.lookupSignal(Event.SIGNAL_POLL, 5290).rssi.count);
        // Check that the linkspeed was updated
        assertEquals(666.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 5290).linkspeed.sum, TOL);
    }

    /**
     * Statistics on time-to-connect, connection duration
     */
    @Test
    public void testDurationStatistics() throws Exception {
        // Start out disconnected; start connecting
        mWifiInfo.setBSSID(android.net.wifi.WifiInfo.DEFAULT_MAC_ADDRESS);
        mWifiScoreCard.noteConnectionAttempt(mWifiInfo);
        // First poll has a bad RSSI
        millisecondsPass(111);
        mWifiInfo.setBSSID(TEST_BSSID_1.toString());
        mWifiInfo.setFrequency(5805);
        mWifiInfo.setRssi(WifiInfo.INVALID_RSSI);
        // A bit later, connection is complete (up through DHCP)
        millisecondsPass(222);
        mWifiInfo.setRssi(-55);
        mWifiScoreCard.noteIpConfiguration(mWifiInfo);
        millisecondsPass(666);
        // Rssi polls for 99 seconds
        for (int i = 0; i < 99; i += 3) {
            mWifiScoreCard.noteSignalPoll(mWifiInfo);
            secondsPass(3);
        }
        // Make sure our simulated time adds up
        assertEquals(mMilliSecondsSinceBoot, 99999);
        // Validation success, rather late!
        mWifiScoreCard.noteValidationSuccess(mWifiInfo);
        // A long while later, wifi is toggled off
        secondsPass(9900);
        // Second validation success should not matter.
        mWifiScoreCard.noteValidationSuccess(mWifiInfo);
        mWifiScoreCard.noteWifiDisabled(mWifiInfo);


        // Now verify
        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.fetchByBssid(TEST_BSSID_1);
        assertEquals(1, perBssid.lookupSignal(Event.IP_CONFIGURATION_SUCCESS, 5805)
                .elapsedMs.count);
        assertEquals(333.0, perBssid.lookupSignal(Event.IP_CONFIGURATION_SUCCESS, 5805)
                .elapsedMs.sum, TOL);
        assertEquals(9999999.0, perBssid.lookupSignal(Event.WIFI_DISABLED, 5805)
                .elapsedMs.maxValue, TOL);
        assertEquals(999.0,  perBssid.lookupSignal(Event.FIRST_POLL_AFTER_CONNECTION, 5805)
                .elapsedMs.minValue, TOL);
        assertEquals(99999.0, perBssid.lookupSignal(Event.VALIDATION_SUCCESS, 5805)
                .elapsedMs.sum, TOL);
        assertNull(perBssid.lookupSignal(Event.SIGNAL_POLL, 5805).elapsedMs);
    }

    /**
     * Constructs a protobuf form of an example.
     */
    private byte[] makeSerializedAccessPointExample() {
        mWifiScoreCard.noteConnectionAttempt(mWifiInfo);
        millisecondsPass(10);
        // Association completes, a NetworkAgent is created
        mWifiScoreCard.noteNetworkAgentCreated(mWifiInfo, TEST_NETWORK_AGENT_ID);
        millisecondsPass(101);
        mWifiInfo.setRssi(-55);
        mWifiInfo.setFrequency(5805);
        mWifiInfo.setLinkSpeed(384);
        mWifiScoreCard.noteIpConfiguration(mWifiInfo);
        millisecondsPass(888);
        mWifiScoreCard.noteSignalPoll(mWifiInfo);
        millisecondsPass(1000);
        mWifiInfo.setRssi(-44);
        mWifiScoreCard.noteSignalPoll(mWifiInfo);
        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.fetchByBssid(TEST_BSSID_1);
        perBssid.lookupSignal(Event.SIGNAL_POLL, 2412).rssi.historicalMean = -42.0;
        perBssid.lookupSignal(Event.SIGNAL_POLL, 2412).rssi.historicalVariance = 4.0;
        checkSerializationExample("before serialization", perBssid);
        // Now convert to protobuf form
        byte[] serialized = perBssid.toAccessPoint().toByteArray();
        return serialized;
    }

    /**
     * Checks that the fields of the serialization example are as expected
     */
    private void checkSerializationExample(String diag, WifiScoreCard.PerBssid perBssid) {
        assertEquals(diag, 2, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805).rssi.count);
        assertEquals(diag, -55.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.minValue, TOL);
        assertEquals(diag, -44.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 5805)
                .rssi.maxValue, TOL);
        assertEquals(diag, 384.0, perBssid.lookupSignal(Event.FIRST_POLL_AFTER_CONNECTION, 5805)
                .linkspeed.sum, TOL);
        assertEquals(diag, 111.0, perBssid.lookupSignal(Event.IP_CONFIGURATION_SUCCESS, 5805)
                .elapsedMs.minValue, TOL);
        assertEquals(diag, 0, perBssid.lookupSignal(Event.SIGNAL_POLL, 2412).rssi.count);
        assertEquals(diag, -42.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 2412)
                .rssi.historicalMean, TOL);
        assertEquals(diag, 4.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 2412)
                .rssi.historicalVariance, TOL);
    }

    /**
     * AccessPoint serialization
     */
    @Test
    public void testAccessPointSerialization() throws Exception {
        byte[] serialized = makeSerializedAccessPointExample();

        // Verify by parsing it and checking that we see the expected results
        AccessPoint ap = AccessPoint.parseFrom(serialized);
        assertEquals(4, ap.getEventStatsCount());
        for (Signal signal: ap.getEventStatsList()) {
            if (signal.getFrequency() == 2412) {
                assertFalse(signal.getRssi().hasCount());
                assertEquals(-42.0, signal.getRssi().getHistoricalMean(), TOL);
                assertEquals(4.0, signal.getRssi().getHistoricalVariance(), TOL);
                continue;
            }
            assertEquals(5805, signal.getFrequency());
            switch (signal.getEvent()) {
                case IP_CONFIGURATION_SUCCESS:
                    assertEquals(384.0, signal.getLinkspeed().getMaxValue(), TOL);
                    assertEquals(111.0, signal.getElapsedMs().getMinValue(), TOL);
                    break;
                case SIGNAL_POLL:
                    assertEquals(2, signal.getRssi().getCount());
                    break;
                case FIRST_POLL_AFTER_CONNECTION:
                    assertEquals(-55.0, signal.getRssi().getSum(), TOL);
                    break;
                default:
                    fail(signal.getEvent().toString());
            }
        }
    }

    /**
     * Serialization should be reproducable
     */
    @Test
    public void testReproducableSerialization() throws Exception {
        byte[] serialized = makeSerializedAccessPointExample();
        setUp();
        assertArrayEquals(serialized, makeSerializedAccessPointExample());
    }

    /**
     * Deserialization
     */
    @Test
    public void testDeserialization() throws Exception {
        byte[] serialized = makeSerializedAccessPointExample();
        setUp(); // Get back to the initial state

        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.perBssidFromAccessPoint(
                mWifiInfo.getSSID(),
                AccessPoint.parseFrom(serialized));

        // Now verify
        String diag = hexStringFromByteArray(serialized);
        checkSerializationExample(diag, perBssid);
    }

    /**
     * Serialization of all internally represented networks
     */
    @Test
    public void testNetworksSerialization() throws Exception {
        makeSerializedAccessPointExample();

        byte[] serialized = mWifiScoreCard.getNetworkListByteArray(false);
        byte[] cleaned = mWifiScoreCard.getNetworkListByteArray(true);
        String base64Encoded = mWifiScoreCard.getNetworkListBase64(true);

        setUp(); // Get back to the initial state
        String diag = hexStringFromByteArray(serialized);
        NetworkList networkList = NetworkList.parseFrom(serialized);
        assertEquals(diag, 1, networkList.getNetworksCount());
        Network network = networkList.getNetworks(0);
        assertEquals(diag, 1, network.getAccessPointsCount());
        AccessPoint accessPoint = network.getAccessPoints(0);
        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.perBssidFromAccessPoint(network.getSsid(),
                accessPoint);
        checkSerializationExample(diag, perBssid);
        // Leaving out the bssids should make the cleaned version shorter.
        assertTrue(cleaned.length < serialized.length);
        // Check the Base64 version
        assertTrue(Arrays.equals(cleaned, Base64.decode(base64Encoded, Base64.DEFAULT)));
        // Check that the network ids were carried over
        assertEquals(TEST_NETWORK_AGENT_ID, network.getNetworkAgentId());
        assertEquals(TEST_NETWORK_CONFIG_ID, network.getNetworkConfigId());
    }

    /**
     * Installation of memory store does not crash
     */
    @Test
    public void testInstallationOfMemoryStoreDoesNotCrash() throws Exception {
        mWifiScoreCard.installMemoryStore(mMemoryStore);
        makeSerializedAccessPointExample();
        mWifiScoreCard.installMemoryStore(mMemoryStore);
    }

    /**
     * Merge of lazy reads
     */
    @Test
    public void testLazyReads() throws Exception {
        // Install our own MemoryStore object, which records read requests
        mWifiScoreCard.installMemoryStore(new WifiScoreCard.MemoryStore() {
            @Override
            public void read(String key, WifiScoreCard.BlobListener listener) {
                mKeys.add(key);
                mBlobListeners.add(listener);
            }
            @Override
            public void write(String key, byte[] value) {
                // ignore for now
            }
        });

        // Now make some changes
        byte[] serialized = makeSerializedAccessPointExample();
        assertEquals(1, mKeys.size());

        // Simulate the asynchronous completion of the read request
        millisecondsPass(33);
        mBlobListeners.get(0).onBlobRetrieved(serialized);

        // Check that the historical mean and variance were updated accordingly
        WifiScoreCard.PerBssid perBssid = mWifiScoreCard.fetchByBssid(TEST_BSSID_1);
        assertEquals(-42.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 2412)
                .rssi.historicalMean, TOL);
        assertEquals(2.0, perBssid.lookupSignal(Event.SIGNAL_POLL, 2412)
                .rssi.historicalVariance, TOL);
    }

    /**
     * Write test
     */
    @Test
    public void testWrites() throws Exception {
        // Install our own MemoryStore object, which records write requests
        mWifiScoreCard.installMemoryStore(new WifiScoreCard.MemoryStore() {
            @Override
            public void read(String key, WifiScoreCard.BlobListener listener) {
                // Just record these, never answer
                mBlobListeners.add(listener);
            }
            @Override
            public void write(String key, byte[] value) {
                mKeys.add(key);
                mBlobs.add(value);
            }
        });

        // Make some changes
        byte[] serialized = makeSerializedAccessPointExample();
        assertEquals(1, mBlobListeners.size());

        secondsPass(33);

        // There should be one changed bssid now. We may have already done some writes.
        mWifiScoreCard.doWrites();
        assertTrue(mKeys.size() > 0);

        // The written blob should not contain the BSSID, though the full serialized version does
        String writtenHex = hexStringFromByteArray(mBlobs.get(mKeys.size() - 1));
        String fullHex = hexStringFromByteArray(serialized);
        String bssidHex = hexStringFromByteArray(TEST_BSSID_1.toByteArray());
        assertFalse(writtenHex, writtenHex.contains(bssidHex));
        assertTrue(fullHex, fullHex.contains(bssidHex));

        // A second write request should not find anything to write
        final int beforeSize = mKeys.size();
        assertEquals(0, mWifiScoreCard.doWrites());
        assertEquals(beforeSize, mKeys.size());
    }

    /**
     * Calling doWrites before installing a MemoryStore should do nothing.
     */
    @Test
    public void testNoWritesUntilReady() throws Exception {
        makeSerializedAccessPointExample();
        assertEquals(0, mWifiScoreCard.doWrites());
    }

    /**
     * Installing a MemoryStore after startup should issue reads.
     */
    @Test
    public void testReadAfterDelayedMemoryStoreInstallation() throws Exception {
        makeSerializedAccessPointExample();
        mWifiScoreCard.installMemoryStore(mMemoryStore);
        verify(mMemoryStore).read(any(), any());
    }

    /**
     * Calling clear should forget the state.
     */
    @Test
    public void testClearReallyDoesClearTheState() throws Exception {
        byte[] serialized = makeSerializedAccessPointExample();
        assertNotEquals(0, serialized.length);
        mWifiScoreCard.clear();
        byte[] leftovers = mWifiScoreCard.getNetworkListByteArray(false);
        assertEquals(0, leftovers.length);
    }

}
