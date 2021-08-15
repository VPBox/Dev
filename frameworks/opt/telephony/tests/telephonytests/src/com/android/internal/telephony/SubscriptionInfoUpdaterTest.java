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
package com.android.internal.telephony;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.IPackageManager;
import android.content.pm.UserInfo;
import android.net.Uri;
import android.os.HandlerThread;
import android.os.ParcelUuid;
import android.os.PersistableBundle;
import android.service.euicc.EuiccProfileInfo;
import android.service.euicc.EuiccService;
import android.service.euicc.GetEuiccProfileInfoListResult;
import android.telephony.CarrierConfigManager;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.telephony.UiccAccessRule;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.euicc.EuiccController;
import com.android.internal.telephony.uicc.IccFileHandler;
import com.android.internal.telephony.uicc.IccRecords;
import com.android.internal.telephony.uicc.IccUtils;
import com.android.internal.telephony.uicc.UiccSlot;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

public class SubscriptionInfoUpdaterTest extends TelephonyTest {

    private static final int FAKE_SUB_ID_1 = 0;
    private static final int FAKE_SUB_ID_2 = 1;
    private static final int FAKE_CARD_ID = 0;
    private static final String FAKE_EID = "89049032000001000000031328322874";
    private static final String FAKE_MCC_MNC_1 = "123456";
    private static final String FAKE_MCC_MNC_2 = "456789";

    private SubscriptionInfoUpdaterHandlerThread mSubscriptionInfoUpdaterHandlerThread;
    private SubscriptionInfoUpdater mUpdater;
    private IccRecords mIccRecord;
    @Mock
    private UserInfo mUserInfo;
    @Mock
    private SubscriptionInfo mSubInfo;
    @Mock
    private ContentProvider mContentProvider;
    @Mock
    private HashMap<String, Object> mSubscriptionContent;
    @Mock
    private IccFileHandler mIccFileHandler;
    @Mock
    private EuiccController mEuiccController;
    @Mock
    private IntentBroadcaster mIntentBroadcaster;
    @Mock
    private IPackageManager mPackageManager;
    @Mock
    private UiccSlot mUiccSlot;

    /*Custom ContentProvider */
    private class FakeSubscriptionContentProvider extends MockContentProvider {
        @Override
        public int update(Uri uri, ContentValues values, String selection, String[] selectionArgs) {
            return mContentProvider.update(uri, values, selection, selectionArgs);
        }
    }

    private class SubscriptionInfoUpdaterHandlerThread extends HandlerThread {

        private SubscriptionInfoUpdaterHandlerThread(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mUpdater = new SubscriptionInfoUpdater(getLooper(), mContext, new Phone[]{mPhone},
                    new CommandsInterface[]{mSimulatedCommands}, mPackageManager);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());

        replaceInstance(SubscriptionInfoUpdater.class, "mIccId", null, new String[1]);
        replaceInstance(SubscriptionInfoUpdater.class, "mContext", null, null);
        replaceInstance(SubscriptionInfoUpdater.class, "PROJECT_SIM_NUM", null, 1);
        replaceInstance(SubscriptionInfoUpdater.class, "sSimCardState", null, new int[1]);
        replaceInstance(SubscriptionInfoUpdater.class, "sSimApplicationState", null, new int[1]);
        replaceInstance(SubscriptionInfoUpdater.class, "sIsSubInfoInitialized", null, false);

        replaceInstance(EuiccController.class, "sInstance", null, mEuiccController);
        replaceInstance(IntentBroadcaster.class, "sIntentBroadcaster", null, mIntentBroadcaster);

        doReturn(true).when(mUiccSlot).isActive();
        doReturn(mUiccSlot).when(mUiccController).getUiccSlotForPhone(anyInt());
        doReturn(1).when(mTelephonyManager).getSimCount();
        doReturn(1).when(mTelephonyManager).getPhoneCount();

        when(mContentProvider.update(any(), any(), any(), isNull())).thenAnswer(
                new Answer<Integer>() {
                    @Override
                    public Integer answer(InvocationOnMock invocation) throws Throwable {
                        ContentValues values = invocation.getArgument(1);
                        for (String key : values.keySet()) {
                            mSubscriptionContent.put(key, values.get(key));
                        }
                        return 1;
                    }
                });

        doReturn(mUserInfo).when(mIActivityManager).getCurrentUser();
        doReturn(new int[]{FAKE_SUB_ID_1}).when(mSubscriptionController).getSubId(0);
        doReturn(new int[]{FAKE_SUB_ID_1}).when(mSubscriptionManager).getActiveSubscriptionIdList();
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());
        doReturn(new int[]{}).when(mSubscriptionController)
                .getActiveSubIdList(/*visibleOnly*/false);
        mIccRecord = mUiccProfile.getIccRecords();

        mSubscriptionInfoUpdaterHandlerThread = new SubscriptionInfoUpdaterHandlerThread(TAG);
        mSubscriptionInfoUpdaterHandlerThread.start();
        waitUntilReady();

        assertFalse(mUpdater.isSubInfoInitialized());
    }

    @After
    public void tearDown() throws Exception {
        mSubscriptionInfoUpdaterHandlerThread.quit();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testSimAbsent() throws Exception {
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        doReturn(new int[]{FAKE_SUB_ID_1}).when(mSubscriptionController)
                .getActiveSubIdList(/*visibleOnly*/false);
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_ABSENT, null, FAKE_SUB_ID_1, false);

        waitForMs(100);
        assertTrue(mUpdater.isSubInfoInitialized());
        verify(mSubscriptionController, times(1)).clearSubInfoRecord(eq(FAKE_SUB_ID_1));

        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_ABSENT));
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testSimAbsentAndInactive() throws Exception {
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        doReturn(new int[]{FAKE_SUB_ID_1}).when(mSubscriptionController)
                .getActiveSubIdList(/*visibleOnly*/false);
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_ABSENT, null, FAKE_SUB_ID_1, true);

        waitForMs(100);
        assertTrue(mUpdater.isSubInfoInitialized());
        verify(mSubscriptionController, times(1)).clearSubInfoRecord(eq(FAKE_SUB_ID_1));

        // Verify that in the special absent and inactive case, we update subscriptions without
        // broadcasting SIM state change
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager, times(0)).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_ABSENT));
        verify(mContext, times(0)).sendBroadcast(any(), anyString());
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testSimUnknown() throws Exception {
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_UNKNOWN, null, FAKE_SUB_ID_1, false);

        waitForMs(100);
        assertFalse(mUpdater.isSubInfoInitialized());
        verify(mSubscriptionContent, times(0)).put(anyString(), any());
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_UNKNOWN));
        verify(mSubscriptionController, times(0)).clearSubInfo();
        verify(mSubscriptionController, times(0)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testSimError() throws Exception {
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_CARD_IO_ERROR, null, FAKE_SUB_ID_1, false);

        waitForMs(100);
        assertTrue(mUpdater.isSubInfoInitialized());
        verify(mSubscriptionContent, times(0)).put(anyString(), any());
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_CARD_IO_ERROR));
        verify(mSubscriptionController, times(0)).clearSubInfo();
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testWrongSimState() throws Exception {
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_IMSI, null, 2, false);

        waitForMs(100);
        assertFalse(mUpdater.isSubInfoInitialized());
        verify(mSubscriptionContent, times(0)).put(anyString(), any());
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager, times(0)).updateConfigForPhoneId(eq(2),
                eq(IccCardConstants.INTENT_VALUE_ICC_IMSI));
        verify(mSubscriptionController, times(0)).clearSubInfo();
        verify(mSubscriptionController, times(0)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testSimLoaded() throws Exception {
        doReturn(FAKE_SUB_ID_1).when(mSubInfo).getSubscriptionId();
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        doReturn("89012604200000000000").when(mIccRecord).getFullIccId();
        doReturn(FAKE_MCC_MNC_1).when(mTelephonyManager).getSimOperatorNumeric(FAKE_SUB_ID_1);

        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOADED, null, FAKE_SUB_ID_1, false);

        waitForMs(100);
        assertTrue(mUpdater.isSubInfoInitialized());

        // verify SIM_STATE_CHANGED broadcast. It should be broadcast twice, once for
        // READ_PHONE_STATE and once for READ_PRIVILEGED_PHONE_STATE
        /* todo: cannot verify as intent is sent using ActivityManagerNative.broadcastStickyIntent()
         * uncomment code below when that is fixed
         */
        /* ArgumentCaptor<Intent> intentArgumentCaptor = ArgumentCaptor.forClass(Intent.class);
        ArgumentCaptor<String> stringArgumentCaptor = ArgumentCaptor.forClass(String.class);
        verify(mContext, times(2)).sendBroadcast(intentArgumentCaptor.capture(),
                stringArgumentCaptor.capture());
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getAllValues().get(0).getAction());
        assertEquals(Manifest.permission.READ_PHONE_STATE,
                stringArgumentCaptor.getAllValues().get(0));
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getAllValues().get(1).getAction());
        assertEquals(Manifest.permission.READ_PRIVILEGED_PHONE_STATE,
                stringArgumentCaptor.getAllValues().get(1)); */

        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mTelephonyManager).getSimOperatorNumeric(FAKE_SUB_ID_1);
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(
                eq("89012604200000000000"), eq(FAKE_SUB_ID_1));
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(1)).setMccMnc(FAKE_MCC_MNC_1, FAKE_SUB_ID_1);
        verify(mSubscriptionController, times(0)).clearSubInfo();
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager, times(1)).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_LOADED));

        // ACTION_USER_UNLOCKED should trigger another SIM_STATE_CHANGED
        Intent intentSimStateChanged = new Intent(Intent.ACTION_USER_UNLOCKED);
        mContext.sendBroadcast(intentSimStateChanged);
        waitForMs(100);

        // verify SIM_STATE_CHANGED broadcast
        /* todo: cannot verify as intent is sent using ActivityManagerNative.broadcastStickyIntent()
         * uncomment code below when that is fixed
         */
        /* verify(mContext, times(4)).sendBroadcast(intentArgumentCaptor.capture(),
                stringArgumentCaptor.capture());
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getAllValues().get(2).getAction());
        assertEquals(Manifest.permission.READ_PHONE_STATE,
                stringArgumentCaptor.getAllValues().get(2));
        assertEquals(TelephonyIntents.ACTION_SIM_STATE_CHANGED,
                intentArgumentCaptor.getAllValues().get(3).getAction());
        assertEquals(Manifest.permission.READ_PRIVILEGED_PHONE_STATE,
                stringArgumentCaptor.getAllValues().get(3)); */
    }

    @Test
    @SmallTest
    public void testSimLoadedEmptyOperatorNumeric() throws Exception {
        doReturn("89012604200000000000").when(mIccRecord).getFullIccId();
        // operator numeric is empty
        doReturn("").when(mTelephonyManager).getSimOperatorNumeric(FAKE_SUB_ID_1);
        doReturn(FAKE_SUB_ID_1).when(mSubInfo).getSubscriptionId();
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOADED, null, FAKE_SUB_ID_1, false);

        waitForMs(300);
        assertTrue(mUpdater.isSubInfoInitialized());
        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mTelephonyManager).getSimOperatorNumeric(FAKE_SUB_ID_1);
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(
                eq("89012604200000000000"), eq(FAKE_SUB_ID_1));
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(0)).setMccMnc(anyString(), anyInt());
        verify(mSubscriptionController, times(0)).clearSubInfo();
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager, times(1)).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_LOADED));
    }

    @Test
    @SmallTest
    public void testSimLockedWithOutIccId() throws Exception {
        /* mock no IccId Info present and try to query IccId
         after IccId query, update subscriptionDB */
        doReturn("98106240020000000000").when(mIccRecord).getFullIccId();

        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOCKED, "TESTING", FAKE_SUB_ID_1, false);

        waitForMs(100);

        assertTrue(mUpdater.isSubInfoInitialized());
        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(
                eq("98106240020000000000"), eq(FAKE_SUB_ID_1));

        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(0)).clearSubInfo();
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        verify(mConfigManager, times(1)).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_LOCKED));
    }

    @Test
    @SmallTest
    public void testDualSimLoaded() throws Exception {
        // Mock there is two sim cards
        replaceInstance(SubscriptionInfoUpdater.class, "mIccId", null,
                new String[]{null, null});
        replaceInstance(SubscriptionInfoUpdater.class, "PROJECT_SIM_NUM", null, 2);
        replaceInstance(SubscriptionInfoUpdater.class, "mPhone", null,
                new Phone[]{mPhone, mPhone});
        replaceInstance(SubscriptionInfoUpdater.class, "sSimCardState", null,
                new int[]{0, 0});
        replaceInstance(SubscriptionInfoUpdater.class, "sSimApplicationState", null,
                new int[]{0, 0});

        doReturn(new int[]{FAKE_SUB_ID_1, FAKE_SUB_ID_2}).when(mSubscriptionManager)
                .getActiveSubscriptionIdList();
        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getPhoneId(eq(FAKE_SUB_ID_1));
        doReturn(FAKE_SUB_ID_2).when(mSubscriptionController).getPhoneId(eq(FAKE_SUB_ID_2));
        doReturn(2).when(mTelephonyManager).getSimCount();
        doReturn(FAKE_MCC_MNC_1).when(mTelephonyManager).getSimOperatorNumeric(eq(FAKE_SUB_ID_1));
        doReturn(FAKE_MCC_MNC_2).when(mTelephonyManager).getSimOperatorNumeric(eq(FAKE_SUB_ID_2));
        verify(mSubscriptionController, times(0)).clearSubInfo();
        doReturn("89012604200000000000").when(mIccRecord).getFullIccId();
        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mSubscriptionManager, times(0)).addSubscriptionInfoRecord(anyString(), anyInt());
        verify(mSubscriptionController, times(0)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(0)).setMccMnc(anyString(), anyInt());

        // Mock sending a sim loaded for SIM 1
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_1));
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOADED, null, FAKE_SUB_ID_1, false);

        waitForMs(100);

        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(anyString(), anyInt());
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(1)).setMccMnc(anyString(), anyInt());
        assertFalse(mUpdater.isSubInfoInitialized());

        // Mock sending a sim loaded for SIM 2
        doReturn(Arrays.asList(mSubInfo)).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(eq(FAKE_SUB_ID_2));
        doReturn(FAKE_SUB_ID_2).when(mSubInfo).getSubscriptionId();
        doReturn("89012604200000000001").when(mIccRecord).getFullIccId();

        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOADED, null, FAKE_SUB_ID_2, false);

        waitForMs(100);

        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(eq("89012604200000000000"),
                eq(FAKE_SUB_ID_1));
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(eq("89012604200000000001"),
                eq(FAKE_SUB_ID_2));
        verify(mSubscriptionController, times(1)).setMccMnc(eq(FAKE_MCC_MNC_1), eq(FAKE_SUB_ID_1));
        verify(mSubscriptionController, times(1)).setMccMnc(eq(FAKE_MCC_MNC_2), eq(FAKE_SUB_ID_2));
        verify(mSubscriptionController, times(2)).notifySubscriptionInfoChanged();
        assertTrue(mUpdater.isSubInfoInitialized());
    }

    @Test
    @SmallTest
    public void testSimLockWithIccId() throws Exception {
        /* no need for IccId query */

        replaceInstance(SubscriptionInfoUpdater.class, "mIccId", null,
                new String[]{"89012604200000000000"});

        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOCKED, "TESTING", FAKE_SUB_ID_1, false);

        waitForMs(100);

        assertTrue(mUpdater.isSubInfoInitialized());
        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(
                anyString(), eq(FAKE_SUB_ID_1));
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionController, times(0)).clearSubInfo();
        CarrierConfigManager mConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        /* broadcast is done */
        verify(mConfigManager, times(1)).updateConfigForPhoneId(eq(FAKE_SUB_ID_1),
                eq(IccCardConstants.INTENT_VALUE_ICC_LOCKED));
    }

    @Test
    @SmallTest
    public void testUpdateEmbeddedSubscriptions_listSuccess() throws Exception {
        when(mEuiccManager.isEnabled()).thenReturn(true);
        when(mEuiccManager.createForCardId(anyInt())).thenReturn(mEuiccManager);
        when(mEuiccManager.getEid()).thenReturn(FAKE_EID);

        EuiccProfileInfo[] euiccProfiles = new EuiccProfileInfo[] {
                new EuiccProfileInfo("1", null /* accessRules */, null /* nickname */),
                new EuiccProfileInfo("3", null /* accessRules */, null /* nickname */),
        };
        when(mEuiccController.blockingGetEuiccProfileInfoList(FAKE_CARD_ID)).thenReturn(
                new GetEuiccProfileInfoListResult(
                        EuiccService.RESULT_OK, euiccProfiles, false /* removable */));

        List<SubscriptionInfo> subInfoList = new ArrayList<>();
        // 1: not embedded, but has matching iccid with an embedded subscription.
        subInfoList.add(new SubscriptionInfo(
                        0, "1", 0, "", "", 0, 0, "", 0, null, "0", "0", "", false /* isEmbedded */,
                        null /* accessRules */, null));
        // 2: embedded but no longer present.
        subInfoList.add(new SubscriptionInfo(
                0, "2", 0, "", "", 0, 0, "", 0, null, "0", "0", "", true /* isEmbedded */,
                null /* accessRules */, null));

        when(mSubscriptionController.getSubscriptionInfoListForEmbeddedSubscriptionUpdate(
                new String[] { "1", "3"}, false /* removable */)).thenReturn(subInfoList);

        List<Integer> cardIds = new ArrayList<>();
        cardIds.add(FAKE_CARD_ID);
        mUpdater.updateEmbeddedSubscriptions(cardIds, null /* callback */);

        // Wait for some time until the callback is triggered.
        waitForMs(100);

        // 3 is new and so a new entry should have been created.
        verify(mSubscriptionController).insertEmptySubInfoRecord(
                "3", SubscriptionManager.SIM_NOT_INSERTED);
        // 1 already existed, so no new entries should be created for it.
        verify(mSubscriptionController, times(0)).clearSubInfo();
        verify(mSubscriptionController, never()).insertEmptySubInfoRecord(eq("1"), anyInt());

        // Info for 1 and 3 should be updated as active embedded subscriptions.
        ArgumentCaptor<ContentValues> iccid1Values = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider).update(eq(SubscriptionManager.CONTENT_URI), iccid1Values.capture(),
                eq(SubscriptionManager.ICC_ID + "=\"1\""), isNull());
        assertEquals(1,
                iccid1Values.getValue().getAsInteger(SubscriptionManager.IS_EMBEDDED).intValue());
        ArgumentCaptor<ContentValues> iccid3Values = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider).update(eq(SubscriptionManager.CONTENT_URI), iccid3Values.capture(),
                eq(SubscriptionManager.ICC_ID + "=\"3\""), isNull());
        assertEquals(1,
                iccid3Values.getValue().getAsInteger(SubscriptionManager.IS_EMBEDDED).intValue());

        // 2 should have been removed since it was returned from the cache but was not present
        // in the list provided by the LPA.
        ArgumentCaptor<ContentValues> iccid2Values = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider).update(eq(SubscriptionManager.CONTENT_URI), iccid2Values.capture(),
                eq(SubscriptionManager.ICC_ID + " IN (\"2\")"), isNull());
        assertEquals(0,
                iccid2Values.getValue().getAsInteger(SubscriptionManager.IS_EMBEDDED).intValue());
    }

    @Test
    @SmallTest
    public void testUpdateEmbeddedSubscriptions_listFailure() throws Exception {
        when(mEuiccManager.isEnabled()).thenReturn(true);
        when(mEuiccController.blockingGetEuiccProfileInfoList(FAKE_CARD_ID))
                .thenReturn(new GetEuiccProfileInfoListResult(
                        42, null /* subscriptions */, false /* removable */));

        List<SubscriptionInfo> subInfoList = new ArrayList<>();
        // 1: not embedded, but has matching iccid with an embedded subscription.
        subInfoList.add(new SubscriptionInfo(
                0, "1", 0, "", "", 0, 0, "", 0, null, "0", "0", "", false /* isEmbedded */,
                null /* accessRules */, null));
        // 2: embedded.
        subInfoList.add(new SubscriptionInfo(
                0, "2", 0, "", "", 0, 0, "", 0, null, "0", "0", "", true /* isEmbedded */,
                null /* accessRules */, null));

        when(mSubscriptionController.getSubscriptionInfoListForEmbeddedSubscriptionUpdate(
                new String[0], false /* removable */)).thenReturn(subInfoList);

        ArrayList<Integer> cardIds = new ArrayList<>(1);
        cardIds.add(FAKE_CARD_ID);
        mUpdater.updateEmbeddedSubscriptions(cardIds, null /* callback */);

        // No new entries should be created.
        verify(mSubscriptionController, times(0)).clearSubInfo();
        verify(mSubscriptionController, never()).insertEmptySubInfoRecord(anyString(), anyInt());

        // No existing entries should have been updated.
        verify(mContentProvider, never()).update(eq(SubscriptionManager.CONTENT_URI), any(),
                any(), isNull());
    }

    @Test
    @SmallTest
    public void testUpdateEmbeddedSubscriptions_emptyToEmpty() throws Exception {
        when(mEuiccManager.isEnabled()).thenReturn(true);
        when(mEuiccController.blockingGetEuiccProfileInfoList(FAKE_CARD_ID))
                .thenReturn(new GetEuiccProfileInfoListResult(
                        42, null /* subscriptions */, true /* removable */));

        List<SubscriptionInfo> subInfoList = new ArrayList<>();
        // 1: not embedded.
        subInfoList.add(new SubscriptionInfo(
                0, "1", 0, "", "", 0, 0, "", 0, null, "0", "0", "", false /* isEmbedded */,
                null /* accessRules */, null));

        when(mSubscriptionController.getSubscriptionInfoListForEmbeddedSubscriptionUpdate(
                new String[0], false /* removable */)).thenReturn(subInfoList);

        ArrayList<Integer> cardIds = new ArrayList<>(1);
        cardIds.add(FAKE_CARD_ID);
        mUpdater.updateEmbeddedSubscriptions(cardIds, null /* callback */);

        // No new entries should be created.
        verify(mSubscriptionController, never()).insertEmptySubInfoRecord(anyString(), anyInt());

        // No existing entries should have been updated.
        verify(mContentProvider, never()).update(eq(SubscriptionManager.CONTENT_URI), any(),
                any(), isNull());
    }

    @Test
    @SmallTest
    public void testHexIccIdSuffix() throws Exception {
        doReturn(null).when(mSubscriptionController)
                .getSubInfoUsingSlotIndexPrivileged(anyInt());
        verify(mSubscriptionController, times(0)).clearSubInfo();
        doReturn("890126042000000000Ff").when(mIccRecord).getFullIccId();

        // Mock sending a sim loaded for SIM 1
        mUpdater.updateInternalIccState(
                IccCardConstants.INTENT_VALUE_ICC_LOADED, "TESTING", FAKE_SUB_ID_1, false);

        waitForMs(100);

        SubscriptionManager mSubscriptionManager = SubscriptionManager.from(mContext);
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
        verify(mSubscriptionManager, times(1)).addSubscriptionInfoRecord(eq("890126042000000000"),
                eq(FAKE_SUB_ID_1));
        verify(mSubscriptionController, times(0)).clearSubInfo();
    }

    PersistableBundle getCarrierConfigForSubInfoUpdate(
            boolean isOpportunistic, String groupUuid) {
        PersistableBundle p = new PersistableBundle();
        p.putBoolean(CarrierConfigManager.KEY_IS_OPPORTUNISTIC_SUBSCRIPTION_BOOL, isOpportunistic);
        p.putString(CarrierConfigManager.KEY_SUBSCRIPTION_GROUP_UUID_STRING, groupUuid);
        return p;
    }

    @Test
    @SmallTest
    public void testUpdateFromCarrierConfigOpportunisticUnchanged() throws Exception {
        final int phoneId = mPhone.getPhoneId();
        String carrierPackageName = "FakeCarrierPackageName";

        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getSubIdUsingPhoneId(phoneId);
        doReturn(mSubInfo).when(mSubscriptionController).getSubscriptionInfo(eq(FAKE_SUB_ID_1));
        doReturn(Collections.singletonList(carrierPackageName)).when(mTelephonyManager)
                .getCarrierPackageNamesForIntentAndPhone(any(), eq(phoneId));
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());

        mUpdater.updateSubscriptionByCarrierConfig(mPhone.getPhoneId(),
                carrierPackageName, new PersistableBundle());

        verify(mContentProvider, never()).update(any(), any(), any(), any());
        verify(mSubscriptionController, never()).refreshCachedActiveSubscriptionInfoList();
        verify(mSubscriptionController, never()).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testUpdateFromCarrierConfigOpportunisticSetOpportunistic() throws Exception {
        final int phoneId = mPhone.getPhoneId();
        PersistableBundle carrierConfig = getCarrierConfigForSubInfoUpdate(
                true, "");
        String carrierPackageName = "FakeCarrierPackageName";

        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getSubIdUsingPhoneId(phoneId);
        doReturn(mSubInfo).when(mSubscriptionController).getSubscriptionInfo(eq(FAKE_SUB_ID_1));
        doReturn(false).when(mSubInfo).isOpportunistic();
        doReturn(Collections.singletonList(carrierPackageName)).when(mTelephonyManager)
                .getCarrierPackageNamesForIntentAndPhone(any(), eq(phoneId));
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());

        mUpdater.updateSubscriptionByCarrierConfig(mPhone.getPhoneId(),
                carrierPackageName, carrierConfig);

        ArgumentCaptor<ContentValues> cvCaptor = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider, times(1)).update(
                eq(SubscriptionManager.getUriForSubscriptionId(FAKE_SUB_ID_1)),
                cvCaptor.capture(), eq(null), eq(null));
        assertEquals(1, cvCaptor.getValue().getAsInteger(
                SubscriptionManager.IS_OPPORTUNISTIC).intValue());
        assertEquals(1, cvCaptor.getValue().size());
        verify(mSubscriptionController, times(1)).refreshCachedActiveSubscriptionInfoList();
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
    }

    @Test
    @SmallTest
    public void testUpdateFromCarrierConfigOpportunisticAddToGroup() throws Exception {
        final int phoneId = mPhone.getPhoneId();
        PersistableBundle carrierConfig = getCarrierConfigForSubInfoUpdate(
                true, "11111111-2222-3333-4444-555555555555");
        String carrierPackageName = "FakeCarrierPackageName";

        doReturn(true).when(mSubscriptionController).canPackageManageGroup(
                ParcelUuid.fromString("11111111-2222-3333-4444-555555555555"), carrierPackageName);
        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getSubIdUsingPhoneId(phoneId);
        doReturn(mSubInfo).when(mSubscriptionController).getSubscriptionInfo(eq(FAKE_SUB_ID_1));
        doReturn(Collections.singletonList(carrierPackageName)).when(mTelephonyManager)
                .getCarrierPackageNamesForIntentAndPhone(any(), eq(phoneId));
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());

        mUpdater.updateSubscriptionByCarrierConfig(mPhone.getPhoneId(),
                carrierPackageName, carrierConfig);

        ArgumentCaptor<ContentValues> cvCaptor = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider, times(1)).update(
                eq(SubscriptionManager.getUriForSubscriptionId(FAKE_SUB_ID_1)),
                cvCaptor.capture(), eq(null), eq(null));
        assertEquals(1, cvCaptor.getValue().getAsInteger(
                SubscriptionManager.IS_OPPORTUNISTIC).intValue());
        assertEquals("11111111-2222-3333-4444-555555555555",
                cvCaptor.getValue().getAsString(SubscriptionManager.GROUP_UUID));
        assertEquals(carrierPackageName,
                cvCaptor.getValue().getAsString(SubscriptionManager.GROUP_OWNER));
        assertEquals(3, cvCaptor.getValue().size());
    }

    @Test
    @SmallTest
    public void testUpdateFromCarrierConfigOpportunisticRemoveFromGroup() throws Exception {
        final int phoneId = mPhone.getPhoneId();
        PersistableBundle carrierConfig = getCarrierConfigForSubInfoUpdate(
                true, "00000000-0000-0000-0000-000000000000");
        String carrierPackageName = "FakeCarrierPackageName";

        doReturn(true).when(mSubscriptionController).canPackageManageGroup(
                ParcelUuid.fromString("11111111-2222-3333-4444-555555555555"), carrierPackageName);
        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getSubIdUsingPhoneId(phoneId);
        doReturn(mSubInfo).when(mSubscriptionController).getSubscriptionInfo(eq(FAKE_SUB_ID_1));
        doReturn(ParcelUuid.fromString("11111111-2222-3333-4444-555555555555"))
            .when(mSubInfo).getGroupUuid();
        doReturn(Collections.singletonList(carrierPackageName)).when(mTelephonyManager)
                .getCarrierPackageNamesForIntentAndPhone(any(), eq(phoneId));
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());

        mUpdater.updateSubscriptionByCarrierConfig(mPhone.getPhoneId(),
                carrierPackageName, carrierConfig);

        ArgumentCaptor<ContentValues> cvCaptor = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider, times(1)).update(
                eq(SubscriptionManager.getUriForSubscriptionId(FAKE_SUB_ID_1)),
                cvCaptor.capture(), eq(null), eq(null));
        assertEquals(1, cvCaptor.getValue().getAsInteger(
                SubscriptionManager.IS_OPPORTUNISTIC).intValue());
        assertNull(cvCaptor.getValue().getAsString(SubscriptionManager.GROUP_UUID));
        assertEquals(2, cvCaptor.getValue().size());
    }

    @Test
    @SmallTest
    public void testUpdateFromCarrierConfigCarrierCertificates() {
        String[] certs = new String[2];
        certs[0] = "d1f1";
        certs[1] = "b5d6";

        UiccAccessRule[] carrierConfigAccessRules = new UiccAccessRule[certs.length];
        for (int i = 0; i < certs.length; i++) {
            carrierConfigAccessRules[i] = new UiccAccessRule(
                IccUtils.hexStringToBytes(certs[i]), null, 0);
        }

        final int phoneId = mPhone.getPhoneId();
        PersistableBundle carrierConfig = new PersistableBundle();
        carrierConfig.putStringArray(
                CarrierConfigManager.KEY_CARRIER_CERTIFICATE_STRING_ARRAY, certs);

        String carrierPackageName = "FakeCarrierPackageName";

        doReturn(FAKE_SUB_ID_1).when(mSubscriptionController).getSubIdUsingPhoneId(phoneId);
        doReturn(mSubInfo).when(mSubscriptionController).getSubscriptionInfo(eq(FAKE_SUB_ID_1));
        doReturn(false).when(mSubInfo).isOpportunistic();
        doReturn(Collections.singletonList(carrierPackageName)).when(mTelephonyManager)
                .getCarrierPackageNamesForIntentAndPhone(any(), eq(phoneId));
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                SubscriptionManager.CONTENT_URI.getAuthority(),
                new FakeSubscriptionContentProvider());

        mUpdater.updateSubscriptionByCarrierConfig(mPhone.getPhoneId(),
                carrierPackageName, carrierConfig);

        ArgumentCaptor<ContentValues> cvCaptor = ArgumentCaptor.forClass(ContentValues.class);
        verify(mContentProvider, times(1)).update(
                eq(SubscriptionManager.getUriForSubscriptionId(FAKE_SUB_ID_1)),
                cvCaptor.capture(), eq(null), eq(null));
        assertEquals(carrierConfigAccessRules, UiccAccessRule.decodeRules(cvCaptor.getValue()
                .getAsByteArray(SubscriptionManager.ACCESS_RULES_FROM_CARRIER_CONFIGS)));
        assertEquals(1, cvCaptor.getValue().size());
        verify(mSubscriptionController, times(1)).refreshCachedActiveSubscriptionInfoList();
        verify(mSubscriptionController, times(1)).notifySubscriptionInfoChanged();
    }
}
