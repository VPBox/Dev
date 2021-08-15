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

import static android.app.AppOpsManager.MODE_ALLOWED;
import static android.app.AppOpsManager.MODE_IGNORED;
import static android.app.AppOpsManager.OPSTR_CHANGE_WIFI_STATE;
import static android.app.AppOpsManager.OP_CHANGE_WIFI_STATE;
import static android.app.Notification.EXTRA_BIG_TEXT;

import static com.android.server.wifi.WifiNetworkSuggestionsManager.NOTIFICATION_USER_ALLOWED_APP_INTENT_ACTION;
import static com.android.server.wifi.WifiNetworkSuggestionsManager.NOTIFICATION_USER_DISALLOWED_APP_INTENT_ACTION;
import static com.android.server.wifi.WifiNetworkSuggestionsManager.NOTIFICATION_USER_DISMISSED_INTENT_ACTION;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.*;

import android.app.AppOpsManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.net.MacAddress;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiNetworkSuggestion;
import android.net.wifi.WifiScanner;
import android.os.Handler;
import android.os.UserHandle;
import android.os.test.TestLooper;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.R;
import com.android.internal.messages.nano.SystemMessageProto.SystemMessage;
import com.android.server.wifi.WifiNetworkSuggestionsManager.ExtendedWifiNetworkSuggestion;
import com.android.server.wifi.WifiNetworkSuggestionsManager.PerAppInfo;
import com.android.server.wifi.util.WifiPermissionsUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * Unit tests for {@link com.android.server.wifi.WifiNetworkSuggestionsManager}.
 */
@SmallTest
public class WifiNetworkSuggestionsManagerTest {
    private static final String TEST_PACKAGE_1 = "com.test12345";
    private static final String TEST_PACKAGE_2 = "com.test54321";
    private static final String TEST_APP_NAME_1 = "test12345";
    private static final String TEST_APP_NAME_2 = "test54321";
    private static final String TEST_BSSID = "00:11:22:33:44:55";
    private static final int TEST_UID_1 = 5667;
    private static final int TEST_UID_2 = 4537;

    private @Mock Context mContext;
    private @Mock Resources mResources;
    private @Mock AppOpsManager mAppOpsManager;
    private @Mock NotificationManager mNotificationManger;
    private @Mock PackageManager mPackageManager;
    private @Mock WifiPermissionsUtil mWifiPermissionsUtil;
    private @Mock WifiInjector mWifiInjector;
    private @Mock FrameworkFacade mFrameworkFacade;
    private @Mock WifiConfigStore mWifiConfigStore;
    private @Mock WifiConfigManager mWifiConfigManager;
    private @Mock NetworkSuggestionStoreData mNetworkSuggestionStoreData;
    private @Mock ClientModeImpl mClientModeImpl;
    private @Mock WifiMetrics mWifiMetrics;
    private TestLooper mLooper;
    private ArgumentCaptor<AppOpsManager.OnOpChangedListener> mAppOpChangedListenerCaptor =
            ArgumentCaptor.forClass(AppOpsManager.OnOpChangedListener.class);
    private ArgumentCaptor<BroadcastReceiver> mBroadcastReceiverCaptor =
            ArgumentCaptor.forClass(BroadcastReceiver.class);

    private InOrder mInorder;

    private WifiNetworkSuggestionsManager mWifiNetworkSuggestionsManager;
    private NetworkSuggestionStoreData.DataSource mDataSource;

    /**
     * Setup the mocks.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();

        mInorder = inOrder(mContext, mWifiPermissionsUtil);

        when(mWifiInjector.makeNetworkSuggestionStoreData(any()))
                .thenReturn(mNetworkSuggestionStoreData);
        when(mWifiInjector.getFrameworkFacade()).thenReturn(mFrameworkFacade);
        when(mWifiInjector.getClientModeImpl()).thenReturn(mClientModeImpl);
        when(mFrameworkFacade.getBroadcast(any(), anyInt(), any(), anyInt()))
                .thenReturn(mock(PendingIntent.class));
        when(mContext.getResources()).thenReturn(mResources);
        when(mContext.getSystemService(Context.APP_OPS_SERVICE)).thenReturn(mAppOpsManager);
        when(mContext.getSystemService(Context.NOTIFICATION_SERVICE))
                .thenReturn(mNotificationManger);
        when(mContext.getPackageManager()).thenReturn(mPackageManager);

        // setup resource strings for notification.
        when(mResources.getString(eq(R.string.wifi_suggestion_title), anyString()))
                .thenAnswer(s -> "blah" + s.getArguments()[1]);
        when(mResources.getString(eq(R.string.wifi_suggestion_content), anyString()))
                .thenAnswer(s -> "blah" + s.getArguments()[1]);
        when(mResources.getText(eq(R.string.wifi_suggestion_action_allow_app)))
                .thenReturn("blah");
        when(mResources.getText(eq(R.string.wifi_suggestion_action_disallow_app)))
                .thenReturn("blah");

        // Our app Info. Needed for notification builder.
        ApplicationInfo ourAppInfo = new ApplicationInfo();
        when(mContext.getApplicationInfo()).thenReturn(ourAppInfo);
        // test app info
        ApplicationInfo appInfO1 = new ApplicationInfo();
        when(mPackageManager.getApplicationInfoAsUser(eq(TEST_PACKAGE_1), eq(0), anyInt()))
            .thenReturn(appInfO1);
        when(mPackageManager.getApplicationLabel(appInfO1)).thenReturn(TEST_APP_NAME_1);
        ApplicationInfo appInfO2 = new ApplicationInfo();
        when(mPackageManager.getApplicationInfoAsUser(eq(TEST_PACKAGE_2), eq(0), anyInt()))
            .thenReturn(appInfO2);
        when(mPackageManager.getApplicationLabel(appInfO2)).thenReturn(TEST_APP_NAME_2);

        mWifiNetworkSuggestionsManager =
                new WifiNetworkSuggestionsManager(mContext, new Handler(mLooper.getLooper()),
                        mWifiInjector, mWifiPermissionsUtil, mWifiConfigManager, mWifiConfigStore,
                        mWifiMetrics);
        verify(mContext).getResources();
        verify(mContext).getSystemService(Context.APP_OPS_SERVICE);
        verify(mContext).getSystemService(Context.NOTIFICATION_SERVICE);
        verify(mContext).getPackageManager();
        verify(mContext).registerReceiver(mBroadcastReceiverCaptor.capture(), any());

        ArgumentCaptor<NetworkSuggestionStoreData.DataSource> dataSourceArgumentCaptor =
                ArgumentCaptor.forClass(NetworkSuggestionStoreData.DataSource.class);

        verify(mWifiInjector).makeNetworkSuggestionStoreData(dataSourceArgumentCaptor.capture());
        mDataSource = dataSourceArgumentCaptor.getValue();
        assertNotNull(mDataSource);

        mWifiNetworkSuggestionsManager.enableVerboseLogging(1);
    }

    /**
     * Verify successful addition of network suggestions.
     */
    @Test
    public void testAddNetworkSuggestionsSuccess() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion1);
            }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion2);
            }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                    add(networkSuggestion2);
            }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        verify(mWifiMetrics, times(2)).incrementNetworkSuggestionApiNumModification();
        ArgumentCaptor<List<Integer>> maxSizesCaptor = ArgumentCaptor.forClass(List.class);
        verify(mWifiMetrics, times(2)).noteNetworkSuggestionApiListSizeHistogram(
                maxSizesCaptor.capture());
        assertNotNull(maxSizesCaptor.getValue());
        assertEquals(maxSizesCaptor.getValue(), new ArrayList<Integer>() {{ add(1); add(1); }});
    }

    /**
     * Verify successful removal of network suggestions.
     */
    @Test
    public void testRemoveNetworkSuggestionsSuccess() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion1);
            }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion2);
            }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_1,
                        TEST_PACKAGE_2));

        // Now remove all of them.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList1,
                        TEST_UID_1, TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList2,
                        TEST_UID_1, TEST_PACKAGE_2));

        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());

        verify(mWifiMetrics, times(4)).incrementNetworkSuggestionApiNumModification();
        ArgumentCaptor<List<Integer>> maxSizesCaptor = ArgumentCaptor.forClass(List.class);
        verify(mWifiMetrics, times(4)).noteNetworkSuggestionApiListSizeHistogram(
                maxSizesCaptor.capture());
        assertNotNull(maxSizesCaptor.getValue());
        assertEquals(maxSizesCaptor.getValue(), new ArrayList<Integer>() {{ add(1); add(1); }});
    }

    /**
     * Verify successful removal of all network suggestions.
     */
    @Test
    public void testRemoveAllNetworkSuggestionsSuccess() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        // Now remove all of them by sending an empty list.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(new ArrayList<>(), TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(new ArrayList<>(), TEST_UID_2,
                        TEST_PACKAGE_2));

        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());
    }

    /**
     * Verify successful replace (add,remove, add) of network suggestions.
     */
    @Test
    public void testReplaceNetworkSuggestionsSuccess() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion);
            }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                add(networkSuggestion);
            }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);
    }

    /**
     * Verify that an attempt to modify networks that are already active is rejected.
     */
    @Test
    public void testAddNetworkSuggestionsFailureOnInPlaceModification() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion);
            }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Modify the original suggestion.
        networkSuggestion.wifiConfiguration.meteredOverride =
                WifiConfiguration.METERED_OVERRIDE_METERED;

        // Replace attempt should fail.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_ERROR_ADD_DUPLICATE,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
    }

    /**
     * Verify that an attempt to add networks beyond the max per app is rejected.
     */
    @Test
    public void testAddNetworkSuggestionsFailureOnExceedsMaxPerApp() {
        // Add the max per app first.
        List<WifiNetworkSuggestion> networkSuggestionList = new ArrayList<>();
        for (int i = 0; i < WifiManager.NETWORK_SUGGESTIONS_MAX_PER_APP; i++) {
            networkSuggestionList.add(new WifiNetworkSuggestion(
                    WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                    TEST_PACKAGE_1));
        }
        // The first add should succeed.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        List<WifiNetworkSuggestion> originalNetworkSuggestionsList = networkSuggestionList;

        // Now add 3 more.
        networkSuggestionList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            networkSuggestionList.add(new WifiNetworkSuggestion(
                    WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                    TEST_PACKAGE_1));
        }
        // The second add should fail.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_ERROR_ADD_EXCEEDS_MAX_PER_APP,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Now remove 3 of the initially added ones.
        networkSuggestionList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            networkSuggestionList.add(originalNetworkSuggestionsList.get(i));
        }
        // The remove should succeed.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Now add 2 more.
        networkSuggestionList = new ArrayList<>();
        for (int i = 0; i < 2; i++) {
            networkSuggestionList.add(new WifiNetworkSuggestion(
                    WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                    TEST_PACKAGE_1));
        }
        // This add should now succeed.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
    }

    /**
     * Verify that an attempt to remove an invalid set of network suggestions is rejected.
     */
    @Test
    public void testRemoveNetworkSuggestionsFailureOnInvalid() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion1);
            }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion2);
            }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        // Remove should fail because the network list is different.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_ERROR_REMOVE_INVALID,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList2, TEST_UID_1,
                        TEST_PACKAGE_1));
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the provided scan detail.
     */
    @Test
    public void
            testGetNetworkSuggestionsForScanDetailSuccessWithOneMatchForCarrierProvisioningApp() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        // This app should be pre-approved. No need to explicitly call
        // |setHasUserApprovedForApp(true, TEST_PACKAGE_1)|
        when(mWifiPermissionsUtil.checkNetworkCarrierProvisioningPermission(TEST_UID_1))
                .thenReturn(true);
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));

        ScanDetail scanDetail = createScanDetailForNetwork(networkSuggestion.wifiConfiguration);

        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailSuccessWithOneMatch() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        ScanDetail scanDetail = createScanDetailForNetwork(networkSuggestion.wifiConfiguration);

        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of multiple network suggestions matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailSuccessWithMultipleMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        // Reuse the same network credentials to ensure they both match.
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion1);
            }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion2);
            }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);

        ScanDetail scanDetail = createScanDetailForNetwork(networkSuggestion1.wifiConfiguration);

        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                add(networkSuggestion1);
                add(networkSuggestion2);
            }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailSuccessWithBssidOneMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration);
        wifiConfiguration.BSSID = scanDetail.getBSSIDString();

        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of multiple network suggestions matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailSuccessWithBssidMultipleMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration);
        wifiConfiguration.BSSID = scanDetail.getBSSIDString();

        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        // Reuse the same network credentials to ensure they both match.
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);


        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                    add(networkSuggestion2);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of multiple network suggestions matching the provided scan detail.
     */
    @Test
    public void
            testGetNetworkSuggestionsForScanDetailSuccessWithBssidMultipleMatchFromSamePackage() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration);
        wifiConfiguration.BSSID = scanDetail.getBSSIDString();

        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        // Reuse the same network credentials to ensure they both match.
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                TEST_PACKAGE_1);

        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                    add(networkSuggestion2);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify a successful lookup of multiple network suggestions matching the provided scan detail.
     */
    @Test
    public void
            testGetNetworkSuggestionsForScanDetailSuccessWithBssidAndWithoutBssidMultipleMatch() {
        WifiConfiguration wifiConfiguration1 = WifiConfigurationTestUtil.createOpenNetwork();
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration1);
        WifiConfiguration wifiConfiguration2 = new WifiConfiguration(wifiConfiguration1);
        wifiConfiguration2.BSSID = scanDetail.getBSSIDString();

        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration1, false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        // Reuse the same network credentials to ensure they both match.
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration2, false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);


        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                    add(networkSuggestion2);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);

        // Now change the bssid of the scan result to a different value, now only the general
        // (without bssid) suggestion.
        scanDetail.getScanResult().BSSID = MacAddress.createRandomUnicastAddress().toString();
        matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify failure to lookup any network suggestion matching the provided scan detail when the
     * app providing the suggestion has not been approved.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailFailureOnAppNotApproved() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertFalse(mWifiNetworkSuggestionsManager.hasUserApprovedForApp(TEST_PACKAGE_1));

        ScanDetail scanDetail = createScanDetailForNetwork(networkSuggestion.wifiConfiguration);

        assertNull(mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail));
    }

    /**
     * Verify failure to lookup any network suggestion matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailFailureOnSuggestionRemoval() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                wifiConfiguration, false, false, TEST_UID_1,
                        TEST_PACKAGE_1);
        ScanDetail scanDetail = createScanDetailForNetwork(wifiConfiguration);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                add(networkSuggestion);
            }};

        // add the suggestion & ensure lookup works.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        assertNotNull(mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                scanDetail));

        // remove the suggestion & ensure lookup fails.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(Collections.EMPTY_LIST, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertNull(mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail));
    }

    /**
     * Verify failure to lookup any network suggestion matching the provided scan detail.
     */
    @Test
    public void testGetNetworkSuggestionsForScanDetailFailureOnWrongNetwork() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Create a scan result corresponding to a different network.
        ScanDetail scanDetail = createScanDetailForNetwork(
                WifiConfigurationTestUtil.createPskNetwork());

        assertNull(mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail));
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * This should trigger a broadcast to the app.
     */
    @Test
    public void testOnNetworkConnectionSuccessWithOneMatch() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        verify(mWifiMetrics).incrementNetworkSuggestionApiNumConnectSuccess();

        // Verify that the correct broadcast was sent out.
        mInorder.verify(mWifiPermissionsUtil)
                .enforceCanAccessScanResults(TEST_PACKAGE_1, TEST_UID_1);
        validatePostConnectionBroadcastSent(TEST_PACKAGE_1, networkSuggestion);

        // Verify no more broadcast were sent out.
        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the current network
     * connection failure.
     */
    @Test
    public void testOnNetworkConnectionFailureWithOneMatch() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_DHCP, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        verify(mWifiMetrics).incrementNetworkSuggestionApiNumConnectFailure();

        // Verify no more broadcast were sent out.
        mInorder.verify(mWifiPermissionsUtil, never()).enforceCanAccessScanResults(
                anyString(), anyInt());
        mInorder.verify(mContext,  never()).sendBroadcastAsUser(
                any(), any());

    }

    /**
     * Verify a successful lookup of multiple network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * This should trigger a broadcast to all the apps.
     */
    @Test
    public void testOnNetworkConnectionSuccessWithMultipleMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_1,
                        TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_2,
                        TEST_PACKAGE_2);
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, wifiConfiguration, TEST_BSSID);

        verify(mWifiMetrics).incrementNetworkSuggestionApiNumConnectSuccess();

        // Verify that the correct broadcasts were sent out.
        for (int i = 0; i < 2; i++) {
            ArgumentCaptor<String> packageNameCaptor = ArgumentCaptor.forClass(String.class);
            ArgumentCaptor<Integer> uidCaptor = ArgumentCaptor.forClass(Integer.class);
            mInorder.verify(mWifiPermissionsUtil)
                    .enforceCanAccessScanResults(packageNameCaptor.capture(), uidCaptor.capture());
            if (packageNameCaptor.getValue().equals(TEST_PACKAGE_1)) {
                assertEquals(Integer.valueOf(TEST_UID_1), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_1, networkSuggestion1);
            } else if (packageNameCaptor.getValue().equals(TEST_PACKAGE_2)) {
                assertEquals(Integer.valueOf(TEST_UID_2), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_2, networkSuggestion2);
            } else {
                fail();
            }
        }

        // Verify no more broadcast were sent out.
        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify a successful lookup of multiple network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * This should trigger a broadcast to all the apps.
     */
    @Test
    public void testOnNetworkConnectionSuccessWithBssidMultipleMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        wifiConfiguration.BSSID = TEST_BSSID;
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_1,
                        TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_2,
                        TEST_PACKAGE_2);
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, wifiConfiguration, TEST_BSSID);

        verify(mWifiMetrics).incrementNetworkSuggestionApiNumConnectSuccess();

        // Verify that the correct broadcasts were sent out.
        for (int i = 0; i < 2; i++) {
            ArgumentCaptor<String> packageNameCaptor = ArgumentCaptor.forClass(String.class);
            ArgumentCaptor<Integer> uidCaptor = ArgumentCaptor.forClass(Integer.class);
            mInorder.verify(mWifiPermissionsUtil)
                    .enforceCanAccessScanResults(packageNameCaptor.capture(), uidCaptor.capture());
            if (packageNameCaptor.getValue().equals(TEST_PACKAGE_1)) {
                assertEquals(Integer.valueOf(TEST_UID_1), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_1, networkSuggestion1);
            } else if (packageNameCaptor.getValue().equals(TEST_PACKAGE_2)) {
                assertEquals(Integer.valueOf(TEST_UID_2), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_2, networkSuggestion2);
            } else {
                fail();
            }
        }

        // Verify no more broadcast were sent out.
        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify a successful lookup of multiple network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * This should trigger a broadcast to all the apps.
     */
    @Test
    public void testOnNetworkConnectionSuccessWithBssidAndWithoutBssidMultipleMatch() {
        WifiConfiguration wifiConfiguration1 = WifiConfigurationTestUtil.createOpenNetwork();
        WifiConfiguration wifiConfiguration2 = new WifiConfiguration(wifiConfiguration1);
        wifiConfiguration2.BSSID = TEST_BSSID;
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration1, true, false, TEST_UID_1,
                        TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration2, true, false, TEST_UID_2,
                        TEST_PACKAGE_2);
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, wifiConfiguration1, TEST_BSSID);

        verify(mWifiMetrics).incrementNetworkSuggestionApiNumConnectSuccess();

        // Verify that the correct broadcasts were sent out.
        for (int i = 0; i < 2; i++) {
            ArgumentCaptor<String> packageNameCaptor = ArgumentCaptor.forClass(String.class);
            ArgumentCaptor<Integer> uidCaptor = ArgumentCaptor.forClass(Integer.class);
            mInorder.verify(mWifiPermissionsUtil)
                    .enforceCanAccessScanResults(packageNameCaptor.capture(), uidCaptor.capture());
            if (packageNameCaptor.getValue().equals(TEST_PACKAGE_1)) {
                assertEquals(Integer.valueOf(TEST_UID_1), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_1, networkSuggestion1);
            } else if (packageNameCaptor.getValue().equals(TEST_PACKAGE_2)) {
                assertEquals(Integer.valueOf(TEST_UID_2), uidCaptor.getValue());
                validatePostConnectionBroadcastSent(TEST_PACKAGE_2, networkSuggestion2);
            } else {
                fail();
            }
        }

        // Verify no more broadcast were sent out.
        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * c) App has not been approved by the user.
     * This should trigger a broadcast to the app.
     */
    @Test
    public void testOnNetworkConnectionWhenAppNotApproved() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mWifiPermissionsUtil).checkNetworkCarrierProvisioningPermission(TEST_UID_1);
        assertFalse(mWifiNetworkSuggestionsManager.hasUserApprovedForApp(TEST_PACKAGE_1));

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Verify no broadcast was sent out.
        mInorder.verify(mWifiPermissionsUtil, never()).enforceCanAccessScanResults(
                anyString(), anyInt());
        mInorder.verify(mContext,  never()).sendBroadcastAsUser(
                any(), any());
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the connected network.
     * a) The corresponding network suggestion does not have the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app holds location permission.
     * This should not trigger a broadcast to the app.
     */
    @Test
    public void testOnNetworkConnectionWhenIsAppInteractionRequiredNotSet() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mWifiPermissionsUtil).checkNetworkCarrierProvisioningPermission(TEST_UID_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Verify no broadcast was sent out.
        mInorder.verify(mWifiPermissionsUtil, never()).enforceCanAccessScanResults(
                anyString(), anyInt());
        mInorder.verify(mContext,  never()).sendBroadcastAsUser(
                any(), any());
    }

    /**
     * Verify a successful lookup of a single network suggestion matching the connected network.
     * a) The corresponding network suggestion has the
     * {@link WifiNetworkSuggestion#isAppInteractionRequired} flag set.
     * b) The app does not hold location permission.
     * This should not trigger a broadcast to the app.
     */
    @Test
    public void testOnNetworkConnectionWhenAppDoesNotHoldLocationPermission() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mWifiPermissionsUtil).checkNetworkCarrierProvisioningPermission(TEST_UID_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        doThrow(new SecurityException())
                .when(mWifiPermissionsUtil).enforceCanAccessScanResults(TEST_PACKAGE_1, TEST_UID_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        mInorder.verify(mWifiPermissionsUtil)
                .enforceCanAccessScanResults(TEST_PACKAGE_1, TEST_UID_1);

        // Verify no broadcast was sent out.
        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify triggering of config store write after successful addition of network suggestions.
     */
    @Test
    public void testAddNetworkSuggestionsConfigStoreWrite() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);

        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Verify config store interactions.
        verify(mWifiConfigManager).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        Map<String, PerAppInfo> networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertEquals(1, networkSuggestionsMapToWrite.size());
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_1));
        assertFalse(networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).hasUserApproved);
        Set<ExtendedWifiNetworkSuggestion> extNetworkSuggestionsToWrite =
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).extNetworkSuggestions;
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedAllNetworkSuggestions,
                extNetworkSuggestionsToWrite
                        .stream()
                        .collect(Collectors.mapping(
                                n -> n.wns,
                                Collectors.toSet())));

        // Ensure that the new data flag has been reset after read.
        assertFalse(mDataSource.hasNewDataToSerialize());
    }

    /**
     * Verify triggering of config store write after successful removal of network suggestions.
     */
    @Test
    public void testRemoveNetworkSuggestionsConfigStoreWrite() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);

        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Verify config store interactions.
        verify(mWifiConfigManager, times(2)).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        // Expect a single app entry with no active suggestions.
        Map<String, PerAppInfo> networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertEquals(1, networkSuggestionsMapToWrite.size());
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_1));
        assertFalse(networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).hasUserApproved);
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).extNetworkSuggestions.isEmpty());

        // Ensure that the new data flag has been reset after read.
        assertFalse(mDataSource.hasNewDataToSerialize());
    }

    /**
     * Verify handling of initial config store read.
     */
    @Test
    public void testNetworkSuggestionsConfigStoreLoad() {
        PerAppInfo appInfo = new PerAppInfo(TEST_PACKAGE_1);
        appInfo.hasUserApproved = true;
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        appInfo.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion, appInfo));
        mDataSource.fromDeserialized(new HashMap<String, PerAppInfo>() {{
                        put(TEST_PACKAGE_1, appInfo);
                }});

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        // Ensure we can lookup the network.
        ScanDetail scanDetail = createScanDetailForNetwork(networkSuggestion.wifiConfiguration);
        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);
    }

    /**
     * Verify handling of config store read after user switch.
     */
    @Test
    public void testNetworkSuggestionsConfigStoreLoadAfterUserSwitch() {
        // Read the store initially.
        PerAppInfo appInfo1 = new PerAppInfo(TEST_PACKAGE_1);
        appInfo1.hasUserApproved = true;
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        appInfo1.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion1, appInfo1));
        mDataSource.fromDeserialized(new HashMap<String, PerAppInfo>() {{
                    put(TEST_PACKAGE_1, appInfo1);
                }});


        // Now simulate user switch.
        mDataSource.reset();
        PerAppInfo appInfo2 = new PerAppInfo(TEST_PACKAGE_2);
        appInfo2.hasUserApproved = true;
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        appInfo2.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion2, appInfo2));
        mDataSource.fromDeserialized(new HashMap<String, PerAppInfo>() {{
                    put(TEST_PACKAGE_2, appInfo2);
                }});

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        // Ensure we can lookup the new network.
        ScanDetail scanDetail2 = createScanDetailForNetwork(networkSuggestion2.wifiConfiguration);
        Set<WifiNetworkSuggestion> matchingNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail2);
        Set<WifiNetworkSuggestion> expectedMatchingNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};
        assertEquals(expectedMatchingNetworkSuggestions, matchingNetworkSuggestions);

        // Ensure that the previous network can no longer be looked up.
        ScanDetail scanDetail1 = createScanDetailForNetwork(networkSuggestion1.wifiConfiguration);
        assertNull(mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(scanDetail1));
    }

    /**
     * Verify that we don't disconnect from the network if the only network suggestion matching the
     * connected network is removed when App doesn't have NetworkCarrierProvisioningPermission.
     */
    @Test
    public void
            testRemoveNetworkSuggestionsMatchingConnectionSuccessWithOneMatchNoCarrierProvision() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        when(mWifiPermissionsUtil.checkNetworkCarrierProvisioningPermission(TEST_UID_1))
                .thenReturn(false);
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Now remove the network suggestion and ensure we did not trigger a disconnect.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mClientModeImpl, never()).disconnectCommand();
    }

    /**
     * Verify that we will disconnect from the network if the only network suggestion matching the
     * connected network is removed when App has NetworkCarrierProvisioningPermission.
     */
    @Test
    public void
            testRemoveNetworkSuggestionsMatchingConnectionSuccessWithOneMatchCarrierProvision() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        when(mWifiPermissionsUtil.checkNetworkCarrierProvisioningPermission(TEST_UID_1))
                .thenReturn(true);
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Now remove the network suggestion and ensure we did trigger a disconnect.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mClientModeImpl).disconnectCommand();
    }

    /**
     * Verify that we will disconnect from network when App has NetworkCarrierProvisioningPermission
     * and removed all its suggestions by remove empty list.
     */
    @Test
    public void
            testRemoveAllNetworkSuggestionsMatchingConnectionSuccessWithOneMatchCarrierProvision() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        when(mWifiPermissionsUtil.checkNetworkCarrierProvisioningPermission(TEST_UID_1))
                .thenReturn(true);
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Now remove all network suggestion and ensure we did trigger a disconnect.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(new ArrayList<>(), TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mClientModeImpl).disconnectCommand();
    }


    /**
     * Verify that we do not disconnect from the network if there are network suggestion from
     * multiple apps matching the connected network when one of the apps is removed.
     */
    @Test
    public void testRemoveAppMatchingConnectionSuccessWithMultipleMatch() {
        WifiConfiguration wifiConfiguration = WifiConfigurationTestUtil.createOpenNetwork();
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_1,
                        TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                wifiConfiguration, true, false, TEST_UID_2,
                        TEST_PACKAGE_2);
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_2);

        // Simulate connecting to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE, wifiConfiguration, TEST_BSSID);

        // Now remove one of the apps and ensure we did not trigger a disconnect.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_1);
        verify(mClientModeImpl, never()).disconnectCommand();

        // Now remove the other app and ensure we trigger a disconnect.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_2);
        verify(mClientModeImpl).disconnectCommand();
    }

    /**
     * Verify that we do not disconnect from the network if there are no network suggestion matching
     * the connected network when one of the app is removed.
     */
    @Test
    public void testRemoveAppNotMatchingConnectionSuccess() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate connecting to some other network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiConfigurationTestUtil.createEapNetwork(), TEST_BSSID);

        // Now remove the app and ensure we did not trigger a disconnect.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_1);
        verify(mClientModeImpl, never()).disconnectCommand();
    }

    /**
     * Verify that we do not disconnect from the network if there are no network suggestion matching
     * the connected network when one of them is removed.
     */
    @Test
    public void testRemoveNetworkSuggestionsNotMatchingConnectionSuccessAfterConnectionFailure() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);

        // Simulate failing connection to the network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_DHCP, networkSuggestion.wifiConfiguration,
                TEST_BSSID);

        // Simulate connecting to some other network.
        mWifiNetworkSuggestionsManager.handleConnectionAttemptEnded(
                WifiMetrics.ConnectionEvent.FAILURE_NONE,
                WifiConfigurationTestUtil.createEapNetwork(), TEST_BSSID);

        // Now remove the app and ensure we did not trigger a disconnect.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_1);
        verify(mClientModeImpl, never()).disconnectCommand();
    }

    /**
     * Verify that we start tracking app-ops on first network suggestion add & stop tracking on the
     * last network suggestion remove.
     */
    @Test
    public void testAddRemoveNetworkSuggestionsStartStopAppOpsWatch() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        mInorder = inOrder(mAppOpsManager);

        // Watch app-ops changes on first add.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        mInorder.verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE),
                eq(TEST_PACKAGE_1), mAppOpChangedListenerCaptor.capture());

        // Nothing happens on second add.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Now remove first add, nothing happens.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        // Stop watching app-ops changes on last remove.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList2, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());
        mInorder.verify(mAppOpsManager).stopWatchingMode(mAppOpChangedListenerCaptor.getValue());

        mInorder.verifyNoMoreInteractions();
    }

    /**
     * Verify app-ops disable/enable after suggestions add.
     */
    @Test
    public void testAppOpsChangeAfterSuggestionsAdd() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE), eq(TEST_PACKAGE_1),
                mAppOpChangedListenerCaptor.capture());
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);

        // allow change wifi state.
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_ALLOWED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        allNetworkSuggestions = mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        // disallow change wifi state & ensure we remove the app from database.
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        verify(mAppOpsManager).stopWatchingMode(mAppOpChangedListenerCaptor.getValue());
        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());
    }

    /**
     * Verify app-ops disable/enable after config store load.
     */
    @Test
    public void testAppOpsChangeAfterConfigStoreLoad() {
        PerAppInfo appInfo = new PerAppInfo(TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        appInfo.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion, appInfo));
        mDataSource.fromDeserialized(new HashMap<String, PerAppInfo>() {{
                    put(TEST_PACKAGE_1, appInfo);
                }});

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE), eq(TEST_PACKAGE_1),
                mAppOpChangedListenerCaptor.capture());
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);

        // allow change wifi state.
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_ALLOWED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        allNetworkSuggestions = mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        // disallow change wifi state & ensure we remove all the suggestions for that app.
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());
    }

    /**
     * Verify app-ops disable with wrong uid to package mapping.
     */
    @Test
    public void testAppOpsChangeWrongUid() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        Set<WifiNetworkSuggestion> allNetworkSuggestions =
                mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        Set<WifiNetworkSuggestion> expectedAllNetworkSuggestions =
                new HashSet<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);

        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE), eq(TEST_PACKAGE_1),
                mAppOpChangedListenerCaptor.capture());
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);

        // disallow change wifi state & ensure we don't remove all the suggestions for that app.
        doThrow(new SecurityException()).when(mAppOpsManager).checkPackage(
                eq(TEST_UID_1), eq(TEST_PACKAGE_1));
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        allNetworkSuggestions = mWifiNetworkSuggestionsManager.getAllNetworkSuggestions();
        assertEquals(expectedAllNetworkSuggestions, allNetworkSuggestions);
    }

    /**
     * Verify that we stop tracking the package on its removal.
     */
    @Test
    public void testRemoveApp() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        // Remove all suggestions from TEST_PACKAGE_1 & TEST_PACKAGE_2, we should continue to track.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        assertTrue(mDataSource.hasNewDataToSerialize());
        Map<String, PerAppInfo> networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertEquals(2, networkSuggestionsMapToWrite.size());
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_1));
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_2));
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).extNetworkSuggestions.isEmpty());
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_2).extNetworkSuggestions.isEmpty());

        // Now remove TEST_PACKAGE_1, continue to track TEST_PACKAGE_2.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_1);
        assertTrue(mDataSource.hasNewDataToSerialize());
        networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertEquals(1, networkSuggestionsMapToWrite.size());
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_2));
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_2).extNetworkSuggestions.isEmpty());

        // Now remove TEST_PACKAGE_2.
        mWifiNetworkSuggestionsManager.removeApp(TEST_PACKAGE_2);
        assertTrue(mDataSource.hasNewDataToSerialize());
        networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertTrue(networkSuggestionsMapToWrite.isEmpty());

        // Verify that we stopped watching these apps for app-ops changes.
        verify(mAppOpsManager, times(2)).stopWatchingMode(any());
    }


    /**
     * Verify that we stop tracking all packages & it's suggestions on network settings reset.
     */
    @Test
    public void testClear() {
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_2,
                TEST_PACKAGE_2);

        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion2);
                }};

        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        // Remove all suggestions from TEST_PACKAGE_1 & TEST_PACKAGE_2, we should continue to track.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.remove(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));

        assertTrue(mDataSource.hasNewDataToSerialize());
        Map<String, PerAppInfo> networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertEquals(2, networkSuggestionsMapToWrite.size());
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_1));
        assertTrue(networkSuggestionsMapToWrite.keySet().contains(TEST_PACKAGE_2));
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_1).extNetworkSuggestions.isEmpty());
        assertTrue(
                networkSuggestionsMapToWrite.get(TEST_PACKAGE_2).extNetworkSuggestions.isEmpty());

        // Now clear everything.
        mWifiNetworkSuggestionsManager.clear();
        assertTrue(mDataSource.hasNewDataToSerialize());
        networkSuggestionsMapToWrite = mDataSource.toSerialize();
        assertTrue(networkSuggestionsMapToWrite.isEmpty());

        // Verify that we stopped watching these apps for app-ops changes.
        verify(mAppOpsManager, times(2)).stopWatchingMode(any());
    }

    /**
     * Verify user dismissal notification when first time add suggestions and dismissal the user
     * approval notification when framework gets scan results.
     */
    @Test
    public void testUserApprovalNotificationDismissalWhenGetScanResult() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        validateUserApprovalNotification(TEST_APP_NAME_1);
        // Simulate user dismissal notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISMISSED_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        reset(mNotificationManger);

        // Simulate finding the network in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user dismissal notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISMISSED_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);

        reset(mNotificationManger);
        // We should resend the notification next time the network is found in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);
        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Verify user dismissal notification when first time add suggestions and click on allow on
     * the user approval notification when framework gets scan results.
     */
    @Test
    public void testUserApprovalNotificationClickOnAllowWhenGetScanResult() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user dismissal notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISMISSED_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        reset(mNotificationManger);

        // Simulate finding the network in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user clicking on allow in the notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_ALLOWED_APP_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        // Cancel the notification.
        verify(mNotificationManger).cancel(SystemMessage.NOTE_NETWORK_SUGGESTION_AVAILABLE);

        // Verify config store interactions.
        verify(mWifiConfigManager, times(2)).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        reset(mNotificationManger);
        // We should not resend the notification next time the network is found in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));
        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Verify user dismissal notification when first time add suggestions and click on disallow on
     * the user approval notification when framework gets scan results.
     */
    @Test
    public void testUserApprovalNotificationClickOnDisallowWhenGetScanResult() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE),
                eq(TEST_PACKAGE_1), mAppOpChangedListenerCaptor.capture());
        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user dismissal notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISMISSED_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        reset(mNotificationManger);

        // Simulate finding the network in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user clicking on disallow in the notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISALLOWED_APP_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        // Ensure we turn off CHANGE_WIFI_STATE app-ops.
        verify(mAppOpsManager).setMode(
                OP_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1, MODE_IGNORED);
        // Cancel the notification.
        verify(mNotificationManger).cancel(SystemMessage.NOTE_NETWORK_SUGGESTION_AVAILABLE);

        // Verify config store interactions.
        verify(mWifiConfigManager, times(2)).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        reset(mNotificationManger);

        // Now trigger the app-ops callback to ensure we remove all of their suggestions.
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                        TEST_PACKAGE_1))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());

        // Assuming the user re-enabled the app again & added the same suggestions back.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // We should resend the notification when the network is again found in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);
        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Verify that we don't send a new notification when a pending notification is active.
     */
    @Test
    public void testUserApprovalNotificationWhilePreviousNotificationActive() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));

        // Simulate finding the network in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        validateUserApprovalNotification(TEST_APP_NAME_1);

        reset(mNotificationManger);
        // We should not resend the notification next time the network is found in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));

        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Verify get hidden networks from All user approve network suggestions
     */
    @Test
    public void testGetHiddenNetworks() {

        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion hiddenNetworkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createPskHiddenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        WifiNetworkSuggestion hiddenNetworkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createPskHiddenNetwork(), true, false, TEST_UID_2,
                TEST_PACKAGE_2);
        List<WifiNetworkSuggestion> networkSuggestionList1 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                    add(hiddenNetworkSuggestion1);
                }};
        List<WifiNetworkSuggestion> networkSuggestionList2 =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(hiddenNetworkSuggestion2);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList1, TEST_UID_1,
                        TEST_PACKAGE_1));
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList2, TEST_UID_2,
                        TEST_PACKAGE_2));
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(true, TEST_PACKAGE_1);
        mWifiNetworkSuggestionsManager.setHasUserApprovedForApp(false, TEST_PACKAGE_2);
        List<WifiScanner.ScanSettings.HiddenNetwork> hiddenNetworks =
                mWifiNetworkSuggestionsManager.retrieveHiddenNetworkList();
        assertEquals(1, hiddenNetworks.size());
        assertEquals(hiddenNetworkSuggestion1.wifiConfiguration.SSID, hiddenNetworks.get(0).ssid);
    }

    /**
     * Verify handling of user clicking allow on the user approval notification when first time
     * add suggestions.
     */
    @Test
    public void testUserApprovalNotificationClickOnAllowDuringAddingSuggestions() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user clicking on allow in the notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_ALLOWED_APP_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        // Cancel the notification.
        verify(mNotificationManger).cancel(SystemMessage.NOTE_NETWORK_SUGGESTION_AVAILABLE);

        // Verify config store interactions.
        verify(mWifiConfigManager, times(2)).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        reset(mNotificationManger);
        // We should not resend the notification next time the network is found in scan results.
        mWifiNetworkSuggestionsManager.getNetworkSuggestionsForScanDetail(
                createScanDetailForNetwork(networkSuggestion.wifiConfiguration));
        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Verify handling of user clicking Disallow on the user approval notification when first time
     * add suggestions.
     */
    @Test
    public void testUserApprovalNotificationClickOnDisallowWhenAddSuggestions() {
        WifiNetworkSuggestion networkSuggestion = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_1);
        List<WifiNetworkSuggestion> networkSuggestionList =
                new ArrayList<WifiNetworkSuggestion>() {{
                    add(networkSuggestion);
                }};
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        verify(mAppOpsManager).startWatchingMode(eq(OPSTR_CHANGE_WIFI_STATE),
                eq(TEST_PACKAGE_1), mAppOpChangedListenerCaptor.capture());
        validateUserApprovalNotification(TEST_APP_NAME_1);

        // Simulate user clicking on disallow in the notification.
        sendBroadcastForUserAction(
                NOTIFICATION_USER_DISALLOWED_APP_INTENT_ACTION, TEST_PACKAGE_1, TEST_UID_1);
        // Ensure we turn off CHANGE_WIFI_STATE app-ops.
        verify(mAppOpsManager).setMode(
                OP_CHANGE_WIFI_STATE, TEST_UID_1,
                TEST_PACKAGE_1, MODE_IGNORED);
        // Cancel the notification.
        verify(mNotificationManger).cancel(SystemMessage.NOTE_NETWORK_SUGGESTION_AVAILABLE);

        // Verify config store interactions.
        verify(mWifiConfigManager, times(2)).saveToStore(true);
        assertTrue(mDataSource.hasNewDataToSerialize());

        reset(mNotificationManger);

        // Now trigger the app-ops callback to ensure we remove all of their suggestions.
        AppOpsManager.OnOpChangedListener listener = mAppOpChangedListenerCaptor.getValue();
        assertNotNull(listener);
        when(mAppOpsManager.unsafeCheckOpNoThrow(
                OPSTR_CHANGE_WIFI_STATE, TEST_UID_1,
                TEST_PACKAGE_1))
                .thenReturn(MODE_IGNORED);
        listener.onOpChanged(OPSTR_CHANGE_WIFI_STATE, TEST_PACKAGE_1);
        mLooper.dispatchAll();
        assertTrue(mWifiNetworkSuggestionsManager.getAllNetworkSuggestions().isEmpty());

        // Assuming the user re-enabled the app again & added the same suggestions back.
        assertEquals(WifiManager.STATUS_NETWORK_SUGGESTIONS_SUCCESS,
                mWifiNetworkSuggestionsManager.add(networkSuggestionList, TEST_UID_1,
                        TEST_PACKAGE_1));
        validateUserApprovalNotification(TEST_APP_NAME_1);
        verifyNoMoreInteractions(mNotificationManger);
    }

    /**
     * Creates a scan detail corresponding to the provided network values.
     */
    private ScanDetail createScanDetailForNetwork(WifiConfiguration configuration) {
        return WifiConfigurationTestUtil.createScanDetailForNetwork(configuration,
                MacAddress.createRandomUnicastAddress().toString(), -45, 0, 0, 0);
    }

    private void validatePostConnectionBroadcastSent(
            String expectedPackageName, WifiNetworkSuggestion expectedNetworkSuggestion) {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        ArgumentCaptor<UserHandle> userHandleCaptor = ArgumentCaptor.forClass(UserHandle.class);
        mInorder.verify(mContext,  calls(1)).sendBroadcastAsUser(
                intentCaptor.capture(), userHandleCaptor.capture());

        assertEquals(userHandleCaptor.getValue(), UserHandle.SYSTEM);

        Intent intent = intentCaptor.getValue();
        assertEquals(WifiManager.ACTION_WIFI_NETWORK_SUGGESTION_POST_CONNECTION,
                intent.getAction());
        String packageName = intent.getPackage();
        WifiNetworkSuggestion networkSuggestion =
                intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_SUGGESTION);
        assertEquals(expectedPackageName, packageName);
        assertEquals(expectedNetworkSuggestion, networkSuggestion);
    }

    private boolean checkUserApprovalNotificationParams(
            Notification notification, String expectedAppName) {
        if (!notification.extras.getString(EXTRA_BIG_TEXT).contains(expectedAppName)) return false;
        return true;
    }

    private void validateUserApprovalNotification(String... anyOfExpectedAppNames) {
        ArgumentCaptor<Notification> notificationArgumentCaptor =
                ArgumentCaptor.forClass(Notification.class);
        verify(mNotificationManger).notify(eq(SystemMessage.NOTE_NETWORK_SUGGESTION_AVAILABLE),
                notificationArgumentCaptor.capture());
        Notification notification = notificationArgumentCaptor.getValue();
        assertNotNull(notification);

        boolean foundMatch = false;
        for (int i = 0; i < anyOfExpectedAppNames.length; i++) {
            foundMatch = checkUserApprovalNotificationParams(
                    notification, anyOfExpectedAppNames[i]);
            if (foundMatch) break;
        }
        assertTrue(foundMatch);
    }

    private void sendBroadcastForUserAction(String action, String packageName, int uid) {
        Intent intent = new Intent()
                .setAction(action)
                .putExtra(WifiNetworkSuggestionsManager.EXTRA_PACKAGE_NAME, packageName)
                .putExtra(WifiNetworkSuggestionsManager.EXTRA_UID, uid);
        assertNotNull(mBroadcastReceiverCaptor.getValue());
        mBroadcastReceiverCaptor.getValue().onReceive(mContext, intent);
    }
}
