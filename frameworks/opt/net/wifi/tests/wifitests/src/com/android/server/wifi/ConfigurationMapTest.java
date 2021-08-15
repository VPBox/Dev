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
 * limitations under the License
 */

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.content.pm.UserInfo;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.os.UserHandle;
import android.os.UserManager;
import android.util.SparseArray;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.ConfigurationMapTest}.
 */
@SmallTest
public class ConfigurationMapTest {
    private static final List<WifiConfiguration> CONFIGS = Arrays.asList(
            WifiConfigurationTestUtil.generateWifiConfig(
                    0, 1000000, "\"red\"", true, true, null, null),
            WifiConfigurationTestUtil.generateWifiConfig(
                    1, 1000001, "\"green\"", true, false, "example.com", "Green"),
            WifiConfigurationTestUtil.generateWifiConfig(
                    2, 1200000, "\"blue\"", false, true, null, null),
            WifiConfigurationTestUtil.generateWifiConfig(
                    3, 1100000, "\"cyan\"", true, true, null, null),
            WifiConfigurationTestUtil.generateWifiConfig(
                    4, 1100001, "\"yellow\"", true, true, "example.org", "Yellow"),
            WifiConfigurationTestUtil.generateWifiConfig(
                    5, 1100002, "\"magenta\"", false, false, null, null));

    private static final SparseArray<List<UserInfo>> USER_PROFILES = new SparseArray<>();
    static {
        USER_PROFILES.put(UserHandle.USER_SYSTEM, Arrays.asList(
                new UserInfo(UserHandle.USER_SYSTEM, "Owner", 0),
                new UserInfo(12, "Managed Profile", 0)));
        USER_PROFILES.put(10, Arrays.asList(new UserInfo(10, "Alice", 0)));
        USER_PROFILES.put(11, Arrays.asList(new UserInfo(11, "Bob", 0)));
    }

    @Mock UserManager mUserManager;

    private int mCurrentUserId = UserHandle.USER_SYSTEM;
    private ConfigurationMap mConfigs;

    /**
     * Sets up the test harness before running a test.
     */
    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);

        when(mUserManager.getProfiles(anyInt()))
                .then(new AnswerWithArguments() {
                    public List<UserInfo> answer(int userId) {
                        return USER_PROFILES.get(userId);
                    }
                });
        mConfigs = new ConfigurationMap(mUserManager);
    }

    private void switchUser(int newUserId) {
        mCurrentUserId = newUserId;
        mConfigs.setNewUser(newUserId);
        mConfigs.clear();
    }

    private Collection<WifiConfiguration> getEnabledNetworksForCurrentUser() {
        List<WifiConfiguration> list = new ArrayList<>();
        for (WifiConfiguration config : mConfigs.valuesForCurrentUser()) {
            if (config.status != WifiConfiguration.Status.DISABLED) {
                list.add(config);
            }
        }
        return list;
    }

    private WifiConfiguration getEphemeralForCurrentUser(String ssid) {
        for (WifiConfiguration config : mConfigs.valuesForCurrentUser()) {
            if (ssid.equals(config.SSID) && config.ephemeral) {
                return config;
            }
        }
        return null;
    }

    private void addNetworks(List<WifiConfiguration> configs) {
        for (WifiConfiguration config : configs) {
            assertNull(mConfigs.put(config));
        }
    }

    private void verifyGetters(List<WifiConfiguration> configs) {
        final Set<WifiConfiguration> configsForCurrentUser = new HashSet<>();
        final Set<WifiConfiguration> enabledConfigsForCurrentUser = new HashSet<>();
        final List<WifiConfiguration> configsNotForCurrentUser = new ArrayList<>();

        // Find out which network configurations should be / should not be visible to the current
        // user. Also, check that *ForAllUsers() methods can be used to access all network
        // configurations, irrespective of their visibility to the current user.
        for (WifiConfiguration config : configs) {
            if (WifiConfigurationUtil.isVisibleToAnyProfile(config,
                    USER_PROFILES.get(mCurrentUserId))) {
                configsForCurrentUser.add(config);
                if (config.status != WifiConfiguration.Status.DISABLED) {
                    enabledConfigsForCurrentUser.add(config);
                }
            } else {
                configsNotForCurrentUser.add(config);
            }

            assertEquals(config, mConfigs.getForAllUsers(config.networkId));
        }

        // Verify that *ForCurrentUser() methods can be used to access network configurations
        // visible to the current user.
        for (WifiConfiguration config : configsForCurrentUser) {
            assertEquals(config, mConfigs.getForCurrentUser(config.networkId));
            assertEquals(config, mConfigs.getByConfigKeyForCurrentUser(config.configKey()));
            final boolean wasEphemeral = config.ephemeral;
            config.ephemeral = false;
            assertNull(getEphemeralForCurrentUser(config.SSID));
            config.ephemeral = true;
            assertEquals(config, getEphemeralForCurrentUser(config.SSID));
            config.ephemeral = wasEphemeral;
        }

        // Verify that *ForCurrentUser() methods cannot be used to access network configurations not
        // visible to the current user.
        for (WifiConfiguration config : configsNotForCurrentUser) {
            assertNull(mConfigs.getForCurrentUser(config.networkId));
            assertNull(mConfigs.getByConfigKeyForCurrentUser(config.configKey()));
            final boolean wasEphemeral = config.ephemeral;
            config.ephemeral = false;
            assertNull(getEphemeralForCurrentUser(config.SSID));
            config.ephemeral = true;
            assertNull(getEphemeralForCurrentUser(config.SSID));
            config.ephemeral = wasEphemeral;
        }

        // Verify that the methods which refer to more than one network configuration return the
        // correct sets of networks.
        assertEquals(configs.size(), mConfigs.sizeForAllUsers());
        assertEquals(configsForCurrentUser.size(), mConfigs.sizeForCurrentUser());
        assertEquals(enabledConfigsForCurrentUser,
                new HashSet<WifiConfiguration>(getEnabledNetworksForCurrentUser()));
        assertEquals(new HashSet<>(configs),
                new HashSet<WifiConfiguration>(mConfigs.valuesForAllUsers()));
    }

    private ScanResult createScanResultForNetwork(WifiConfiguration config) {
        return WifiConfigurationTestUtil.createScanDetailForNetwork(config, "", 0, 0, 0, 0)
                .getScanResult();
    }

    /**
     * Helper function to create a scan result matching the network and ensuring that
     * {@link ConfigurationMap#getByScanResultForCurrentUser(ScanResult)} can match that network.
     */
    private void verifyScanResultMatchWithNetwork(WifiConfiguration config) {
        mConfigs.put(config);
        ScanResult scanResult = createScanResultForNetwork(config);
        WifiConfiguration retrievedConfig =
                mConfigs.getByScanResultForCurrentUser(scanResult);
        assertNotNull(retrievedConfig);
        assertEquals(config.configKey(), retrievedConfig.configKey());
    }

    /**
     * Verifies that all getters return the correct network configurations, taking into account the
     * current user. Also verifies that handleUserSwitch() returns the list of network
     * configurations that are no longer visible.
     */
    @Test
    public void testGettersAndHandleUserSwitch() {
        addNetworks(CONFIGS);
        verifyGetters(CONFIGS);

        switchUser(10);
        addNetworks(CONFIGS);
        verifyGetters(CONFIGS);

        switchUser(11);
        addNetworks(CONFIGS);
        verifyGetters(CONFIGS);
    }

    /**
     * Verifies put(), remove() and clear().
     */
    @Test
    public void testPutRemoveClear() {
        final List<WifiConfiguration> configs = new ArrayList<>();
        final WifiConfiguration config1 = CONFIGS.get(0);

        // Verify that there are no network configurations to start with.
        switchUser(UserHandle.getUserId(config1.creatorUid));
        verifyGetters(configs);

        // Add |config1|.
        assertNull(mConfigs.put(config1));
        // Verify that the getters return |config1|.
        configs.add(config1);
        verifyGetters(configs);

        // Overwrite |config1| with |config2|.
        final WifiConfiguration config2 = CONFIGS.get(1);
        config2.networkId = config1.networkId;
        assertEquals(config1, mConfigs.put(config2));
        // Verify that the getters return |config2| only.
        configs.clear();
        configs.add(config2);
        verifyGetters(configs);

        // Add |config3|, which belongs to a managed profile of the current user.
        final WifiConfiguration config3 = CONFIGS.get(2);
        assertNull(mConfigs.put(config3));
        // Verify that the getters return |config2| and |config3|.
        configs.add(config3);
        verifyGetters(configs);

        // Remove |config2|.
        assertEquals(config2, mConfigs.remove(config2.networkId));
        // Verify that the getters return |config3| only.
        configs.remove(config2);
        verifyGetters(configs);

        // Clear all network configurations.
        mConfigs.clear();
        // Verify that the getters do not return any network configurations.
        configs.clear();
        verifyGetters(configs);
    }

    /**
     * Verifies that {@link ConfigurationMap#getByScanResultForCurrentUser(ScanResult)} can
     * positively match the corresponding networks.
     */
    @Test
    public void testScanResultDoesMatchCorrespondingNetworks() {
        verifyScanResultMatchWithNetwork(WifiConfigurationTestUtil.createOpenNetwork());
        verifyScanResultMatchWithNetwork(WifiConfigurationTestUtil.createPskNetwork());
        verifyScanResultMatchWithNetwork(WifiConfigurationTestUtil.createWepNetwork());
        verifyScanResultMatchWithNetwork(WifiConfigurationTestUtil.createEapNetwork());
    }

    /**
     * Verifies that {@link ConfigurationMap#getByScanResultForCurrentUser(ScanResult)} does not
     * match other networks.
     */
    @Test
    public void testScanResultDoesNotMatchWithOtherNetworks() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        ScanResult scanResult = createScanResultForNetwork(config);
        // Change the network security type and the old scan result should not match now.
        config.allowedKeyManagement.clear();
        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        mConfigs.put(config);
        assertNull(mConfigs.getByScanResultForCurrentUser(scanResult));
    }

    /**
     * Verifies that {@link ConfigurationMap#getByScanResultForCurrentUser(ScanResult)} does not
     * match networks which have been removed.
     */
    @Test
    public void testScanResultDoesNotMatchAfterNetworkRemove() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        ScanResult scanResult = createScanResultForNetwork(config);
        config.networkId = 5;
        mConfigs.put(config);
        // Create another network in the map.
        mConfigs.put(WifiConfigurationTestUtil.createPskNetwork());
        assertNotNull(mConfigs.getByScanResultForCurrentUser(scanResult));

        mConfigs.remove(config.networkId);
        assertNull(mConfigs.getByScanResultForCurrentUser(scanResult));
    }

    /**
     * Verifies that {@link ConfigurationMap#getByScanResultForCurrentUser(ScanResult)} does not
     * match networks after clear.
     */
    @Test
    public void testScanResultDoesNotMatchAfterClear() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        ScanResult scanResult = createScanResultForNetwork(config);
        config.networkId = 5;
        mConfigs.put(config);
        // Create another network in the map.
        mConfigs.put(WifiConfigurationTestUtil.createPskNetwork());
        assertNotNull(mConfigs.getByScanResultForCurrentUser(scanResult));

        mConfigs.clear();
        assertNull(mConfigs.getByScanResultForCurrentUser(scanResult));
    }
}
