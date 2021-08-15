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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.KeyMgmt;
import android.os.Build;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.internal.messages.nano.SystemMessageProto.SystemMessage;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

/**
 * Unit tests for {@link com.android.server.wifi.WifiApConfigStore}.
 */
@SmallTest
public class WifiApConfigStoreTest {

    private static final String TAG = "WifiApConfigStoreTest";

    private static final String TEST_AP_CONFIG_FILE_PREFIX = "APConfig_";
    private static final String TEST_DEFAULT_2G_CHANNEL_LIST = "1,2,3,4,5,6";
    private static final String TEST_DEFAULT_AP_SSID = "TestAP";
    private static final String TEST_CONFIGURED_AP_SSID = "ConfiguredAP";
    private static final String TEST_DEFAULT_HOTSPOT_SSID = "TestShare";
    private static final String TEST_DEFAULT_HOTSPOT_PSK = "TestPassword";
    private static final String TEST_APCONFIG_CHANGE_NOTIFICATION_TITLE = "Notification title";
    private static final String TEST_APCONFIG_CHANGE_NOTIFICATION_SUMMARY = "Notification summary";
    private static final String TEST_APCONFIG_CHANGE_NOTIFICATION_DETAILED =
            "Notification detailed";
    private static final int RAND_SSID_INT_MIN = 1000;
    private static final int RAND_SSID_INT_MAX = 9999;
    private static final String TEST_CHAR_SET_AS_STRING = "abcdefghijklmnopqrstuvwxyz0123456789";
    private static final String TEST_STRING_UTF8_WITH_30_BYTES = "智者務其實愚者爭虛名";
    private static final String TEST_STRING_UTF8_WITH_32_BYTES = "ΣωκράτηςΣωκράτης";
    private static final String TEST_STRING_UTF8_WITH_33_BYTES = "一片汪洋大海中的一條魚";
    private static final String TEST_STRING_UTF8_WITH_34_BYTES = "Ευπροσηγοροςγινου";

    @Mock private Context mContext;
    private TestLooper mLooper;
    @Mock private BackupManagerProxy mBackupManagerProxy;
    @Mock private FrameworkFacade mFrameworkFacade;
    private File mApConfigFile;
    private Random mRandom;
    private MockResources mResources;
    @Mock private ApplicationInfo mMockApplInfo;
    private BroadcastReceiver mBroadcastReceiver;
    @Mock private NotificationManager mNotificationManager;
    private ArrayList<Integer> mKnownGood2GChannelList;

    @Before
    public void setUp() throws Exception {
        mLooper = new TestLooper();
        MockitoAnnotations.initMocks(this);
        when(mContext.getSystemService(Context.NOTIFICATION_SERVICE))
                .thenReturn(mNotificationManager);
        mMockApplInfo.targetSdkVersion = Build.VERSION_CODES.P;
        when(mContext.getApplicationInfo()).thenReturn(mMockApplInfo);

        /* Create a temporary file for AP config file storage. */
        mApConfigFile = File.createTempFile(TEST_AP_CONFIG_FILE_PREFIX, "");

        /* Setup expectations for Resources to return some default settings. */
        mResources = new MockResources();
        mResources.setString(R.string.config_wifi_framework_sap_2G_channel_list,
                             TEST_DEFAULT_2G_CHANNEL_LIST);
        mResources.setString(R.string.wifi_tether_configure_ssid_default,
                             TEST_DEFAULT_AP_SSID);
        mResources.setString(R.string.wifi_localhotspot_configure_ssid_default,
                             TEST_DEFAULT_HOTSPOT_SSID);
        /* Default to device that does not require ap band conversion */
        mResources.setBoolean(R.bool.config_wifi_convert_apband_5ghz_to_any, false);
        mResources.setText(R.string.wifi_softap_config_change,
                           TEST_APCONFIG_CHANGE_NOTIFICATION_TITLE);
        mResources.setText(R.string.wifi_softap_config_change_summary,
                           TEST_APCONFIG_CHANGE_NOTIFICATION_SUMMARY);
        mResources.setText(R.string.wifi_softap_config_change_detailed,
                           TEST_APCONFIG_CHANGE_NOTIFICATION_DETAILED);
        when(mContext.getResources()).thenReturn(mResources);

        // build the known good 2G channel list: TEST_DEFAULT_2G_CHANNEL_LIST
        mKnownGood2GChannelList = new ArrayList(Arrays.asList(1, 2, 3, 4, 5, 6));

        mRandom = new Random();
    }

    @After
    public void cleanUp() {
        /* Remove the temporary AP config file. */
        mApConfigFile.delete();
    }

    /**
     * Helper method to create and verify actions for the ApConfigStore used in the following tests.
     */
    private WifiApConfigStore createWifiApConfigStore() {
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());

        ArgumentCaptor<BroadcastReceiver> broadcastReceiverCaptor =
                ArgumentCaptor.forClass(BroadcastReceiver.class);
        verify(mContext).registerReceiver(broadcastReceiverCaptor.capture(), any(), any(), any());
        mBroadcastReceiver = broadcastReceiverCaptor.getValue();

        return store;
    }

    /**
     * Generate a WifiConfiguration based on the specified parameters.
     */
    private WifiConfiguration setupApConfig(
            String ssid, String preSharedKey, int keyManagement, int band, int channel,
            boolean hiddenSSID) {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = ssid;
        config.preSharedKey = preSharedKey;
        config.allowedKeyManagement.set(keyManagement);
        config.apBand = band;
        config.apChannel = channel;
        config.hiddenSSID = hiddenSSID;
        return config;
    }

    private void writeApConfigFile(WifiConfiguration config) throws Exception {
        Method m = WifiApConfigStore.class.getDeclaredMethod(
                "writeApConfiguration", String.class, WifiConfiguration.class);
        m.setAccessible(true);
        m.invoke(null, mApConfigFile.getPath(), config);
    }

    private void verifyApConfig(WifiConfiguration config1, WifiConfiguration config2) {
        assertEquals(config1.SSID, config2.SSID);
        assertEquals(config1.preSharedKey, config2.preSharedKey);
        assertEquals(config1.getAuthType(), config2.getAuthType());
        assertEquals(config1.apBand, config2.apBand);
        assertEquals(config1.apChannel, config2.apChannel);
        assertEquals(config1.hiddenSSID, config2.hiddenSSID);
    }

    private void verifyDefaultApConfig(WifiConfiguration config, String expectedSsid) {
        String[] splitSsid = config.SSID.split("_");
        assertEquals(2, splitSsid.length);
        assertEquals(expectedSsid, splitSsid[0]);
        assertEquals(WifiConfiguration.AP_BAND_2GHZ, config.apBand);
        assertFalse(config.hiddenSSID);
        int randomPortion = Integer.parseInt(splitSsid[1]);
        assertTrue(randomPortion >= RAND_SSID_INT_MIN && randomPortion <= RAND_SSID_INT_MAX);
        assertTrue(config.allowedKeyManagement.get(KeyMgmt.WPA2_PSK));
    }

    private void verifyDefaultLocalOnlyApConfig(WifiConfiguration config, String expectedSsid,
            int expectedApBand) {
        String[] splitSsid = config.SSID.split("_");
        assertEquals(2, splitSsid.length);
        assertEquals(expectedSsid, splitSsid[0]);
        assertEquals(expectedApBand, config.apBand);
        int randomPortion = Integer.parseInt(splitSsid[1]);
        assertTrue(randomPortion >= RAND_SSID_INT_MIN && randomPortion <= RAND_SSID_INT_MAX);
        assertTrue(config.allowedKeyManagement.get(KeyMgmt.WPA2_PSK));
    }


    /**
     * AP Configuration is not specified in the config file,
     * WifiApConfigStore should fallback to use the default configuration.
     */
    @Test
    public void initWithDefaultConfiguration() throws Exception {
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);
    }

    /**
     * Verify WifiApConfigStore can correctly load the existing configuration
     * from the config file.
     */
    @Test
    public void initWithExistingConfiguration() throws Exception {
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",    /* SSID */
                "randomKey",       /* preshared key */
                KeyMgmt.WPA_EAP,   /* key management */
                1,                 /* AP band (5GHz) */
                40,                /* AP channel */
                true               /* Hidden SSID */);
        writeApConfigFile(expectedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(expectedConfig, store.getApConfiguration());
    }

    /**
     * Verify the handling of setting a null ap configuration.
     * WifiApConfigStore should fallback to the default configuration when
     * null ap configuration is provided.
     */
    @Test
    public void setNullApConfiguration() throws Exception {
        /* Initialize WifiApConfigStore with existing configuration. */
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",    /* SSID */
                "randomKey",       /* preshared key */
                KeyMgmt.WPA_EAP,   /* key management */
                1,                 /* AP band (5GHz) */
                40,                /* AP channel */
                true               /* Hidden SSID */);
        writeApConfigFile(expectedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(expectedConfig, store.getApConfiguration());

        store.setApConfiguration(null);
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Verify AP configuration is correctly updated via setApConfiguration call.
     */
    @Test
    public void updateApConfiguration() throws Exception {
        /* Initialize WifiApConfigStore with default configuration. */
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);

        /* Update with a valid configuration. */
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                   /* SSID */
                "randomKey",                      /* preshared key */
                KeyMgmt.WPA_EAP,                  /* key management */
                WifiConfiguration.AP_BAND_2GHZ,   /* AP band */
                40,                               /* AP channel */
                true                              /* Hidden SSID */);
        store.setApConfiguration(expectedConfig);
        verifyApConfig(expectedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a single mode device will have apBand =
     * ANY converted to 5GHZ.
     */
    @Test
    public void convertSingleModeDeviceAnyTo5Ghz() throws Exception {
        /* Initialize WifiApConfigStore with default configuration. */
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);

        /* Update with a valid configuration. */
        WifiConfiguration providedConfig = setupApConfig(
                "ConfiguredAP",                /* SSID */
                "randomKey",                   /* preshared key */
                KeyMgmt.WPA_EAP,               /* key management */
                WifiConfiguration.AP_BAND_ANY, /* AP band (ANY) */
                40,                            /* AP channel */
                false                          /* Hidden SSID */);

        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                       /* SSID */
                "randomKey",                          /* preshared key */
                KeyMgmt.WPA_EAP,                      /* key management */
                WifiConfiguration.AP_BAND_5GHZ,       /* AP band (5GHz) */
                WifiApConfigStore.AP_CHANNEL_DEFAULT, /* AP channel */
                false                                 /* Hidden SSID */);
        store.setApConfiguration(providedConfig);
        verifyApConfig(expectedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a single mode device does not convert
     * apBand to ANY.
     */
    @Test
    public void singleModeDevice5GhzNotConverted() throws Exception {
        /* Initialize WifiApConfigStore with default configuration. */
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);

        /* Update with a valid configuration. */
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                 /* SSID */
                "randomKey",                    /* preshared key */
                KeyMgmt.WPA_EAP,                /* key management */
                WifiConfiguration.AP_BAND_5GHZ, /* AP band */
                40,                             /* AP channel */
                false                           /* Hidden SSID */);
        store.setApConfiguration(expectedConfig);
        verifyApConfig(expectedConfig, store.getApConfiguration());
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a dual mode device will have apBand =
     * 5GHz converted to ANY.
     */
    @Test
    public void convertDualModeDevice5GhzToAny() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_convert_apband_5ghz_to_any, true);

        /* Initialize WifiApConfigStore with default configuration. */
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);

        /* Update with a valid configuration. */
        WifiConfiguration providedConfig = setupApConfig(
                "ConfiguredAP",                 /* SSID */
                "randomKey",                    /* preshared key */
                KeyMgmt.WPA_EAP,                /* key management */
                WifiConfiguration.AP_BAND_5GHZ, /* AP band */
                40,                             /* AP channel */
                false                           /* Hidden SSID */);

        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                       /* SSID */
                "randomKey",                          /* preshared key */
                KeyMgmt.WPA_EAP,                      /* key management */
                WifiConfiguration.AP_BAND_ANY,        /* AP band */
                WifiApConfigStore.AP_CHANNEL_DEFAULT, /* AP channel */
                false                                 /* Hidden SSID */);
        store.setApConfiguration(providedConfig);
        verifyApConfig(expectedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a dual mode device does not convert
     * apBand to 5Ghz.
     */
    @Test
    public void dualModeDeviceAnyNotConverted() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_convert_apband_5ghz_to_any, true);

        /* Initialize WifiApConfigStore with default configuration. */
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyDefaultApConfig(store.getApConfiguration(), TEST_DEFAULT_AP_SSID);

        /* Update with a valid configuration. */
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                 /* SSID */
                "randomKey",                    /* preshared key */
                KeyMgmt.WPA_EAP,                /* key management */
                WifiConfiguration.AP_BAND_ANY,  /* AP band */
                40,                             /* AP channel */
                false                           /* Hidden SSID */);
        store.setApConfiguration(expectedConfig);
        verify(mBackupManagerProxy).notifyDataChanged();
        verifyApConfig(expectedConfig, store.getApConfiguration());
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a single mode device converts a persisted
     * ap config with ANY set for the apBand to 5GHz.
     */
    @Test
    public void singleModeDeviceAnyConvertedTo5GhzAtRetrieval() throws Exception {

        WifiConfiguration persistedConfig = setupApConfig(
                "ConfiguredAP",                 /* SSID */
                "randomKey",                    /* preshared key */
                KeyMgmt.WPA_EAP,                /* key management */
                WifiConfiguration.AP_BAND_ANY,  /* AP band */
                40,                             /* AP channel */
                false                           /* Hidden SSID */);
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                        /* SSID */
                "randomKey",                           /* preshared key */
                KeyMgmt.WPA_EAP,                       /* key management */
                WifiConfiguration.AP_BAND_5GHZ,        /* AP band */
                WifiApConfigStore.AP_CHANNEL_DEFAULT,  /* AP channel */
                false                                  /* Hidden SSID */);
        writeApConfigFile(persistedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(expectedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a single mode device does not convert
     * a persisted ap config with 5GHz set for the apBand.
     */
    @Test
    public void singleModeDeviceNotConvertedAtRetrieval() throws Exception {
        WifiConfiguration persistedConfig = setupApConfig(
                "ConfiguredAP",                  /* SSID */
                "randomKey",                     /* preshared key */
                KeyMgmt.WPA_EAP,                 /* key management */
                WifiConfiguration.AP_BAND_5GHZ,  /* AP band */
                40,                              /* AP channel */
                false                            /* Hidden SSID */);

        writeApConfigFile(persistedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(persistedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy, never()).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a dual mode device converts a persisted ap
     * config with 5GHz only set for the apBand to ANY.
     */
    @Test
    public void dualModeDevice5GhzConvertedToAnyAtRetrieval() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_convert_apband_5ghz_to_any, true);

        WifiConfiguration persistedConfig = setupApConfig(
                "ConfiguredAP",                  /* SSID */
                "randomKey",                     /* preshared key */
                KeyMgmt.WPA_EAP,                 /* key management */
                WifiConfiguration.AP_BAND_5GHZ,  /* AP band */
                40,                              /* AP channel */
                false                            /* Hidden SSID */);
        WifiConfiguration expectedConfig = setupApConfig(
                "ConfiguredAP",                       /* SSID */
                "randomKey",                          /* preshared key */
                KeyMgmt.WPA_EAP,                      /* key management */
                WifiConfiguration.AP_BAND_ANY,        /* AP band */
                WifiApConfigStore.AP_CHANNEL_DEFAULT, /* AP channel */
                false                                 /* Hidden SSID */);

        writeApConfigFile(persistedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(expectedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy).notifyDataChanged();
    }

    /**
     * Due to different device hw capabilities, some bands are not available if a device is
     * dual/single mode capable.  This test verifies that a dual mode device does not convert
     * a persisted ap config with ANY set for the apBand.
     */
    @Test
    public void dualModeDeviceNotConvertedAtRetrieval() throws Exception {
        mResources.setBoolean(R.bool.config_wifi_convert_apband_5ghz_to_any, true);

        WifiConfiguration persistedConfig = setupApConfig(
                "ConfiguredAP",                 /* SSID */
                "randomKey",                    /* preshared key */
                KeyMgmt.WPA_EAP,                /* key management */
                WifiConfiguration.AP_BAND_ANY,  /* AP band */
                40,                             /* AP channel */
                false                           /* Hidden SSID */);

        writeApConfigFile(persistedConfig);
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        verifyApConfig(persistedConfig, store.getApConfiguration());
        verify(mBackupManagerProxy, never()).notifyDataChanged();
    }

    /**
     * Verify a proper WifiConfiguration is generate by getDefaultApConfiguration().
     */
    @Test
    public void getDefaultApConfigurationIsValid() {
        WifiApConfigStore store = new WifiApConfigStore(
                mContext, mLooper.getLooper(), mBackupManagerProxy, mFrameworkFacade,
                mApConfigFile.getPath());
        WifiConfiguration config = store.getApConfiguration();
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify a proper local only hotspot config is generated when called properly with the valid
     * context.
     */
    @Test
    public void generateLocalOnlyHotspotConfigIsValid() {
        WifiConfiguration config = WifiApConfigStore
                .generateLocalOnlyHotspotConfig(mContext, WifiConfiguration.AP_BAND_2GHZ);
        verifyDefaultLocalOnlyApConfig(config, TEST_DEFAULT_HOTSPOT_SSID,
                WifiConfiguration.AP_BAND_2GHZ);
        // The LOHS config should also have a specific network id set - check that as well.
        assertEquals(WifiConfiguration.LOCAL_ONLY_NETWORK_ID, config.networkId);

        // verify that the config passes the validateApWifiConfiguration check
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify a proper local only hotspot config is generated for 5Ghz band.
     */
    @Test
    public void generateLocalOnlyHotspotConfigIsValid5G() {
        WifiConfiguration config = WifiApConfigStore
                .generateLocalOnlyHotspotConfig(mContext, WifiConfiguration.AP_BAND_5GHZ);
        verifyDefaultLocalOnlyApConfig(config, TEST_DEFAULT_HOTSPOT_SSID,
                WifiConfiguration.AP_BAND_5GHZ);
        // The LOHS config should also have a specific network id set - check that as well.
        assertEquals(WifiConfiguration.LOCAL_ONLY_NETWORK_ID, config.networkId);

        // verify that the config passes the validateApWifiConfiguration check
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Helper method to generate random SSIDs.
     *
     * Note: this method has limited use as a random SSID generator.  The characters used in this
     * method do no not cover all valid inputs.
     * @param length number of characters to generate for the name
     * @return String generated string of random characters
     */
    private String generateRandomString(int length) {

        StringBuilder stringBuilder = new StringBuilder(length);
        int index = -1;
        while (stringBuilder.length() < length) {
            index = mRandom.nextInt(TEST_CHAR_SET_AS_STRING.length());
            stringBuilder.append(TEST_CHAR_SET_AS_STRING.charAt(index));
        }
        return stringBuilder.toString();
    }

    /**
     * Verify the SSID checks in validateApWifiConfiguration.
     *
     * Cases to check and verify they trigger failed verification:
     * null WifiConfiguration.SSID
     * empty WifiConfiguration.SSID
     * invalid WifiConfiguaration.SSID length
     *
     * Additionally check a valid SSID with a random (within valid ranges) length.
     */
    @Test
    public void testSsidVerificationInValidateApWifiConfigurationCheck() {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = null;
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
        config.SSID = "";
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
        // check a string if it's larger than 32 bytes with UTF-8 encode
        // Case 1 : one byte per character (use english words and Arabic numerals)
        config.SSID = generateRandomString(WifiApConfigStore.SSID_MAX_LEN + 1);
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
        // Case 2 : two bytes per character
        config.SSID = TEST_STRING_UTF8_WITH_34_BYTES;
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
        // Case 3 : three bytes per character
        config.SSID = TEST_STRING_UTF8_WITH_33_BYTES;
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));

        // now check a valid SSID within 32 bytes
        // Case 1 :  one byte per character with random length
        int validLength = WifiApConfigStore.SSID_MAX_LEN - WifiApConfigStore.SSID_MIN_LEN;
        config.SSID = generateRandomString(
                mRandom.nextInt(validLength) + WifiApConfigStore.SSID_MIN_LEN);
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
        // Case 2 : two bytes per character
        config.SSID = TEST_STRING_UTF8_WITH_32_BYTES;
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
        // Case 3 : three bytes per character
        config.SSID = TEST_STRING_UTF8_WITH_30_BYTES;
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify the Open network checks in validateApWifiConfiguration.
     *
     * If the configured network is open, it should not have a password set.
     *
     * Additionally verify a valid open network passes verification.
     */
    @Test
    public void testOpenNetworkConfigInValidateApWifiConfigurationCheck() {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = TEST_DEFAULT_HOTSPOT_SSID;

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        config.preSharedKey = TEST_DEFAULT_HOTSPOT_PSK;
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));

        // open networks should not have a password set
        config.preSharedKey = null;
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
        config.preSharedKey = "";
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify the WPA2_PSK network checks in validateApWifiConfiguration.
     *
     * If the configured network is configured with a preSharedKey, verify that the passwork is set
     * and it meets length requirements.
     */
    @Test
    public void testWpa2PskNetworkConfigInValidateApWifiConfigurationCheck() {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = TEST_DEFAULT_HOTSPOT_SSID;

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA2_PSK);
        config.preSharedKey = null;
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
        config.preSharedKey = "";
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));

        // test too short
        config.preSharedKey =
                generateRandomString(WifiApConfigStore.PSK_MIN_LEN - 1);
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));

        // test too long
        config.preSharedKey =
                generateRandomString(WifiApConfigStore.PSK_MAX_LEN + 1);
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));

        // explicitly test min length
        config.preSharedKey =
            generateRandomString(WifiApConfigStore.PSK_MIN_LEN);
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));

        // explicitly test max length
        config.preSharedKey =
                generateRandomString(WifiApConfigStore.PSK_MAX_LEN);
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));

        // test random (valid length)
        int maxLen = WifiApConfigStore.PSK_MAX_LEN;
        int minLen = WifiApConfigStore.PSK_MIN_LEN;
        config.preSharedKey =
                generateRandomString(mRandom.nextInt(maxLen - minLen) + minLen);
        assertTrue(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify an invalid AuthType setting (that would trigger an IllegalStateException)
     * returns false when triggered in the validateApWifiConfiguration.
     */
    @Test
    public void testInvalidAuthTypeInValidateApWifiConfigurationCheck() {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = TEST_DEFAULT_HOTSPOT_SSID;

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA2_PSK);
        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify an unsupported authType returns false for validateApWifiConfigurationCheck.
     */
    @Test
    public void testUnsupportedAuthTypeInValidateApWifiConfigurationCheck() {
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = TEST_DEFAULT_HOTSPOT_SSID;

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        assertFalse(WifiApConfigStore.validateApWifiConfiguration(config));
    }

    /**
     * Verify the default 2GHz channel list is properly returned.
     */
    @Test
    public void testDefault2GHzChannelListReturned() {
        // first build known good list
        WifiApConfigStore store = createWifiApConfigStore();
        ArrayList<Integer> channels = store.getAllowed2GChannel();

        assertEquals(mKnownGood2GChannelList.size(), channels.size());
        for (int channel : channels) {
            assertTrue(mKnownGood2GChannelList.contains(channel));
        }
    }

    /**
     * Verify a notification is posted when triggered when the ap config was converted.
     */
    @Test
    public void testNotifyUserOfApBandConversion() throws Exception {
        WifiApConfigStore store = createWifiApConfigStore();
        store.notifyUserOfApBandConversion(TAG);
        // verify the notification is posted
        ArgumentCaptor<Notification> notificationCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(mNotificationManager).notify(eq(SystemMessage.NOTE_SOFTAP_CONFIG_CHANGED),
                                            notificationCaptor.capture());
        Notification notification = notificationCaptor.getValue();
        assertEquals(TEST_APCONFIG_CHANGE_NOTIFICATION_TITLE,
                     notification.extras.getCharSequence(Notification.EXTRA_TITLE));
        assertEquals(TEST_APCONFIG_CHANGE_NOTIFICATION_DETAILED,
                     notification.extras.getCharSequence(Notification.EXTRA_BIG_TEXT));
        assertEquals(TEST_APCONFIG_CHANGE_NOTIFICATION_SUMMARY,
                     notification.extras.getCharSequence(Notification.EXTRA_SUMMARY_TEXT));
    }

    /**
     * Verify the posted notification is cleared when the user interacts with it.
     */
    @Test
    public void testNotificationClearedWhenContentIsTapped() throws Exception {
        WifiApConfigStore store = createWifiApConfigStore();
        Intent intent = new Intent(WifiApConfigStore.ACTION_HOTSPOT_CONFIG_USER_TAPPED_CONTENT);
        mBroadcastReceiver.onReceive(mContext, intent);
        verify(mNotificationManager).cancel(eq(SystemMessage.NOTE_SOFTAP_CONFIG_CHANGED));
    }
}
