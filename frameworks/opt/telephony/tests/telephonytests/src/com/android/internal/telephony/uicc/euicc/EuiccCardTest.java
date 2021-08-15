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

package com.android.internal.telephony.uicc.euicc;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.res.Resources;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.service.carrier.CarrierIdentifier;
import android.service.euicc.EuiccProfileInfo;
import android.telephony.UiccAccessRule;
import android.telephony.euicc.EuiccCardManager;
import android.telephony.euicc.EuiccNotification;
import android.telephony.euicc.EuiccRulesAuthTable;
import android.util.ExceptionUtils;
import android.util.Log;

import com.android.internal.telephony.CommandsInterface;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.uicc.IccCardApplicationStatus;
import com.android.internal.telephony.uicc.IccCardStatus;
import com.android.internal.telephony.uicc.IccUtils;
import com.android.internal.telephony.uicc.asn1.Asn1Node;
import com.android.internal.telephony.uicc.asn1.InvalidAsn1DataException;
import com.android.internal.telephony.uicc.asn1.TagNotFoundException;
import com.android.internal.telephony.uicc.euicc.apdu.ApduException;
import com.android.internal.telephony.uicc.euicc.apdu.LogicalChannelMocker;
import com.android.internal.telephony.uicc.euicc.async.AsyncResultCallback;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class EuiccCardTest extends TelephonyTest {
    private static final long WAIT_TIMEOUT_MLLIS = 5000;

    private static class ResultCaptor<T> extends AsyncResultCallback<T> {
        public T result;
        public Throwable exception;

        private CountDownLatch mLatch;

        private ResultCaptor() {
            mLatch = new CountDownLatch(1);
        }

        public void await() {
            try {
                mLatch.await(WAIT_TIMEOUT_MLLIS, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                fail("Execution is interrupted: " + e);
            }
        }

        @Override
        public void onResult(T r) {
            result = r;
            mLatch.countDown();
        }

        @Override
        public void onException(Throwable e) {
            exception = e;
            mLatch.countDown();
        }
    }

    private class UiccCardHandlerThread extends HandlerThread {
        private UiccCardHandlerThread(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mEuiccCard =
                    new EuiccCard(mContextFixture.getTestDouble(), mMockCi, mMockIccCardStatus,
                            0 /* phoneId */, new Object()) {
                        @Override
                        protected byte[] getDeviceId() {
                            return IccUtils.bcdToBytes("987654321012345");
                        }

                        @Override
                        protected void loadEidAndNotifyRegistrants() {}

                        @Override
                        protected Resources getResources() {
                            return mMockResources;
                        }

                    };
            mHandler = new Handler(mTestHandlerThread.getLooper());
            setReady(true);
        }
    }

    @Mock
    private CommandsInterface mMockCi;
    @Mock
    private IccCardStatus mMockIccCardStatus;

    private UiccCardHandlerThread mTestHandlerThread;
    private Handler mHandler;

    private EuiccCard mEuiccCard;

    @Mock
    private Resources mMockResources;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());

        mMockIccCardStatus.mApplications = new IccCardApplicationStatus[]{};
        mMockIccCardStatus.mCdmaSubscriptionAppIndex =
                mMockIccCardStatus.mImsSubscriptionAppIndex =
                        mMockIccCardStatus.mGsmUmtsSubscriptionAppIndex = -1;
        mMockIccCardStatus.mCardState = IccCardStatus.CardState.CARDSTATE_PRESENT;

        mTestHandlerThread = new UiccCardHandlerThread(getClass().getSimpleName());
        mTestHandlerThread.start();

        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mTestHandlerThread.quit();
        super.tearDown();
    }

    private void assertUnexpectedException(Throwable e) {
        if (e != null) {
            fail("Unexpected exception: " + ExceptionUtils.getCompleteMessage(e) + "\n-----\n"
                    + Log.getStackTraceString(e.getCause()) + "-----");
        }
    }

    @Test
    public void testPassEidInContructor() throws InterruptedException {
        mMockIccCardStatus.eid = "1A2B3C4D";
        mEuiccCard = new EuiccCard(mContextFixture.getTestDouble(), mMockCi,
                mMockIccCardStatus, 0 /* phoneId */, new Object());

        final int eventEidReady = 0;
        final CountDownLatch latch = new CountDownLatch(1);
        Handler handler = new Handler(mTestHandlerThread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == eventEidReady) {
                    assertEquals("1A2B3C4D", mEuiccCard.getEid());
                    latch.countDown();
                }
            }
        };
        // This will instantly return, since EID is already set
        mEuiccCard.registerForEidReady(handler, eventEidReady, null /* obj */);
        assertTrue(latch.await(WAIT_TIMEOUT_MLLIS, TimeUnit.MILLISECONDS));
    }

    @Test
    public void testLoadEidAndNotifyRegistrants() throws InterruptedException {
        int channel = mockLogicalChannelResponses("BF3E065A041A2B3C4D9000");

        {
            final CountDownLatch latch = new CountDownLatch(1);
            mHandler.post(() -> {
                mEuiccCard = new EuiccCard(mContextFixture.getTestDouble(), mMockCi,
                        mMockIccCardStatus, 0 /* phoneId */, new Object());
                latch.countDown();
            });
            assertTrue(latch.await(WAIT_TIMEOUT_MLLIS, TimeUnit.MILLISECONDS));
        }

        final int eventEidReady = 0;
        final CountDownLatch latch = new CountDownLatch(1);
        Handler handler = new Handler(mTestHandlerThread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == eventEidReady) {
                    assertEquals("1A2B3C4D", mEuiccCard.getEid());
                    latch.countDown();
                }
            }
        };

        mEuiccCard.registerForEidReady(handler, eventEidReady, null /* obj */);
        assertTrue(latch.await(WAIT_TIMEOUT_MLLIS, TimeUnit.MILLISECONDS));

        verifyStoreData(channel, "BF3E035C015A");
    }

    @Test
    public void testGetAllProfiles() {
        int channel = mockLogicalChannelResponses(
                "BF2D14A012E3105A0A896700000000004523019F7001019000");

        ResultCaptor<EuiccProfileInfo[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getAllProfiles(resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        EuiccProfileInfo[] profiles = resultCaptor.result;
        assertEquals(1, profiles.length);
        assertEquals("98760000000000543210", profiles[0].getIccid());
        assertEquals(EuiccProfileInfo.PROFILE_STATE_ENABLED, profiles[0].getState());
        verifyStoreData(channel, "BF2D0D5C0B5A909192B79F709599BF76");
    }

    @Test
    public void testFSuffix() {
        // iccID is 987600000000005432FF.
        int channel = mockLogicalChannelResponses(
                "BF2D14A012E3105A0A896700000000004523FF9F7001019000");

        ResultCaptor<EuiccProfileInfo[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getAllProfiles(resultCaptor, mHandler);
        resultCaptor.await();

        EuiccProfileInfo[] profiles = resultCaptor.result;
        assertEquals(1, profiles.length);
        assertEquals("987600000000005432", profiles[0].getIccid());
        assertEquals(EuiccProfileInfo.PROFILE_STATE_ENABLED, profiles[0].getState());
        verifyStoreData(channel, "BF2D0D5C0B5A909192B79F709599BF76");
    }

    @Test
    public void testGetProfile() {
        int channel = mockLogicalChannelResponses("BF2D8184A08181E37F"
                + "5A0A89670000000000452301" // ICCID
                + "90046E69636B" // Nickname
                + "9103746D6F" // Service provider name
                + "92027031" // Profile name
                + "B70F800312F34581030102038203040506" // Operator id
                + "9F700101" // Profile state
                + "950101" // Profile class
                + "990206C0" // Policy rules
                + "BF7645E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4"
                + "CA1D636F6D2E676F6F676C652E616E64726F69642E617070732E6D79617070"
                + "E30ADB080000000000000001" // Carrier privilege rules
                + "9000");

        ResultCaptor<EuiccProfileInfo> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getProfile("98760000000000543210", resultCaptor, mHandler);
        resultCaptor.await();

        EuiccProfileInfo profile = resultCaptor.result;
        assertEquals("98760000000000543210", profile.getIccid());
        assertEquals("nick", profile.getNickname());
        assertEquals("tmo", profile.getServiceProviderName());
        assertEquals("p1", profile.getProfileName());
        assertEquals("213", profile.getCarrierIdentifier().getMcc());
        assertEquals("54", profile.getCarrierIdentifier().getMnc());
        assertEquals("010203", profile.getCarrierIdentifier().getGid1());
        assertEquals("040506", profile.getCarrierIdentifier().getGid2());
        assertEquals(EuiccProfileInfo.PROFILE_STATE_ENABLED, profile.getState());
        assertEquals(EuiccProfileInfo.PROFILE_CLASS_PROVISIONING, profile.getProfileClass());
        assertEquals(
                EuiccProfileInfo.POLICY_RULE_DO_NOT_DELETE
                        | EuiccProfileInfo.POLICY_RULE_DO_NOT_DISABLE,
                profile.getPolicyRules());
        assertArrayEquals(
                new UiccAccessRule[] {
                        new UiccAccessRule(
                                IccUtils.hexStringToBytes(
                                        "ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4"),
                                "com.google.android.apps.myapp", 1)
                },
                profile.getUiccAccessRules().toArray());
        verifyStoreData(channel, "BF2D1BA00C5A0A896700000000004523015C0B5A909192B79F709599BF76");
    }

    @Test
    public void testDisableProfile() {
        int channel = mockLogicalChannelResponses("BF32038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.disableProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3211A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testDisableProfile_SimRefresh() {
        int channel = mockLogicalChannelResponses("6106", "6f00");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.disableProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3211A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testDisableProfile_Error() {
        int channel = mockLogicalChannelResponses("BF32038001039000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.disableProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF3211A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testSwitchToProfile() {
        int channel = mockLogicalChannelResponses("BF31038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.switchToProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3111A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testSwitchToProfile_SimRefresh() {
        int channel = mockLogicalChannelResponses("6106", "6f00");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.switchToProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3111A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testSwitchToProfile_Error() {
        int channel = mockLogicalChannelResponses("BF31038001039000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.switchToProfile("98760000000000543210", true, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF3111A00C5A0A896700000000004523018101FF");
    }

    @Test
    public void testGetEid() {
        int channel = mockLogicalChannelResponses("BF3E065A041A2B3C4D9000");

        ResultCaptor<String> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getEid(resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("1A2B3C4D", resultCaptor.result);
        verifyStoreData(channel, "BF3E035C015A");
    }

    @Test
    public void testSetNickname() {
        int channel = mockLogicalChannelResponses("BF29038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.setNickname("98760000000000543210", "new nickname", resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF291A5A0A89670000000000452301900C6E6577206E69636B6E616D65");
    }

    @Test
    public void testDeleteProfile() {
        int channel = mockLogicalChannelResponses("BF33038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.deleteProfile("98760000000000543210", resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF330C5A0A89670000000000452301");
    }

    @Test
    public void testDeleteProfile_Error() {
        int channel = mockLogicalChannelResponses("BF33038001039000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.deleteProfile("98760000000000543210", resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF330C5A0A89670000000000452301");
    }

    @Test
    public void testGetDefaultSmdpAddress() {
        int channel = mockLogicalChannelResponses(
                "BF3C148008534D44502E434F4D8108736D64732E636F6D9000");

        ResultCaptor<String> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getDefaultSmdpAddress(resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("SMDP.COM", resultCaptor.result);
        verifyStoreData(channel, "BF3C00");
    }

    @Test
    public void testGetSmdsAddress() {
        int channel = mockLogicalChannelResponses(
                "BF3C148008534D44502E434F4D8108736D64732E636F6D9000");

        ResultCaptor<String> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getSmdsAddress(resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("smds.com", resultCaptor.result);
        verifyStoreData(channel, "BF3C00");
    }

    @Test
    public void testSetDefaultSmdpAddress() {
        int channel = mockLogicalChannelResponses("BF3F038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.setDefaultSmdpAddress("smdp.gsma.com", resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3F0F800D736D64702E67736D612E636F6D");
    }

    @Test
    public void testGetRulesAuthTable() {
        int channel = mockLogicalChannelResponses("BF434B"
                + "A0233021" // Rule #1
                + "800206C0" // Policy rules: DO_NOT_DELETE | DO_NOT_DISABLE
                + "A118" // Operator IDs
                + "B70A800312F3458103010203" // ID #1: 213, 54, [1,2,3], null
                + "B70A800312F3458203040506" // ID #2: 213, 54, null, [4,5,6]
                + "820108" // Flag (no user consent)
                + "A0243022" // Rule #2
                + "80020780" // Policy rules: DO_NOT_DISABLE
                + "A118" // Operator IDs
                + "B70A800312E3458103010203" // ID #1: 213, 54E, [1,2,3], null
                + "B70A8003EEEE458203040506" // ID #2: EEE, 54E, null, [4,5,6]
                + "82020780" // Flag (user consent)
                + "9000");

        ResultCaptor<EuiccRulesAuthTable> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getRulesAuthTable(resultCaptor, mHandler);
        resultCaptor.await();

        EuiccRulesAuthTable rat = resultCaptor.result;
        assertEquals(-1,
                rat.findIndex(EuiccProfileInfo.POLICY_RULE_DO_NOT_DELETE,
                        new CarrierIdentifier(new byte[] {0x12, (byte) 0xF3, 0x45}, null, null)));
        assertEquals(1,
                rat.findIndex(EuiccProfileInfo.POLICY_RULE_DO_NOT_DISABLE,
                        new CarrierIdentifier(new byte[] {0x23, 0x67, 0x45}, null, "040506")));
        assertFalse(rat.hasPolicyRuleFlag(0,
                EuiccRulesAuthTable.POLICY_RULE_FLAG_CONSENT_REQUIRED));
        assertTrue(rat.hasPolicyRuleFlag(1,
                EuiccRulesAuthTable.POLICY_RULE_FLAG_CONSENT_REQUIRED));
        verifyStoreData(channel, "BF4300");
    }

    @Test
    public void testResetMemory() {
        int channel = mockLogicalChannelResponses("BF34038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.resetMemory(EuiccCardManager.RESET_OPTION_DELETE_FIELD_LOADED_TEST_PROFILES,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF340482020640");
    }

    @Test
    public void testResetMemory_SimRefresh() {
        int channel = mockLogicalChannelResponses("6106", "6f00");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.resetMemory(EuiccCardManager.RESET_OPTION_DELETE_FIELD_LOADED_TEST_PROFILES,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF340482020640");
    }

    @Test
    public void testGetEuiccChallenge() {
        int channel = mockLogicalChannelResponses("BF2E0580030102039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getEuiccChallenge(resultCaptor, mHandler);
        resultCaptor.await();

        assertArrayEquals(new byte[] {1, 2, 3}, resultCaptor.result);
        verifyStoreData(channel, "BF2E00");
    }

    @Test
    public void testGetEuiccInfo1() {
        int channel = mockLogicalChannelResponses("BF20030102039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getEuiccInfo1(resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("BF2003010203", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel, "BF2000");
    }

    @Test
    public void testGetEuiccInfo2() {
        int channel = mockLogicalChannelResponses("BF22030102039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.getEuiccInfo2(resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("BF2203010203", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel, "BF2200");
    }

    @Test
    public void testAuthenticateServer() {
        when(mMockResources.getStringArray(
                com.android.internal.R.array.config_telephonyEuiccDeviceCapabilities))
                .thenReturn(new String[] {});

        int channel = mockLogicalChannelResponses("BF3802A0009000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.authenticateServer("A1B2C3-X4Y5Z6", // Matching id
                Asn1Node.newBuilder(0xA0).build().toBytes(),
                Asn1Node.newBuilder(0xA1).build().toBytes(),
                Asn1Node.newBuilder(0xA2).build().toBytes(),
                Asn1Node.newBuilder(0xA3).build().toBytes(), resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        assertEquals("BF3802A000", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel,
                "BF382D" + "A000" + "A100" + "A200" + "A300" + "A023"
                        + "800D4131423243332D583459355A36" // Matching id
                        + "A112800489674523" // TAC
                        + "A100" // Device capabilities
                        + "82088967452301214305"); // IMEI
    }

    @Test
    public void testAuthenticateServer_Error() {
        when(mMockResources.getStringArray(
                com.android.internal.R.array.config_telephonyEuiccDeviceCapabilities))
                .thenReturn(new String[] {});

        int channel = mockLogicalChannelResponses("BF3805A1030201039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.authenticateServer("A1B2C3-X4Y5Z6", // Matching id
                Asn1Node.newBuilder(0xA0).build().toBytes(),
                Asn1Node.newBuilder(0xA1).build().toBytes(),
                Asn1Node.newBuilder(0xA2).build().toBytes(),
                Asn1Node.newBuilder(0xA3).build().toBytes(), resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel,
                "BF382D" + "A000" + "A100" + "A200" + "A300" + "A023"
                        + "800D4131423243332D583459355A36" // Matching id
                        + "A112800489674523" // TAC
                        + "A100" // Device capabilities
                        + "82088967452301214305"); // IMEI
    }

    @Test
    public void testAuthenticateService_devCap() {
        when(mMockResources.getStringArray(
                com.android.internal.R.array.config_telephonyEuiccDeviceCapabilities))
                .thenReturn(new String[] {
                        "gsm,11",
                        "utran,11",
                        "cdma1x,1",
                        "hrpd,3",
                        "ehrpd,12",
                        "eutran,11"});

        int channel = mockLogicalChannelResponses("BF3802A0009000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.authenticateServer("A1B2C3-X4Y5Z6", // Matching id
                Asn1Node.newBuilder(0xA0).build().toBytes(),
                Asn1Node.newBuilder(0xA1).build().toBytes(),
                Asn1Node.newBuilder(0xA2).build().toBytes(),
                Asn1Node.newBuilder(0xA3).build().toBytes(), resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        assertEquals("BF3802A000", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel,
                "BF384B" + "A000" + "A100" + "A200" + "A300" + "A041"
                        + "800D4131423243332D583459355A36" // Matching id
                        + "A130800489674523" // TAC
                        // Device capabilities
                        + "A11E80030B000081030B00008203010000830303000084030C000085030B0000"
                        + "82088967452301214305"); // IMEI
    }

    @Test
    public void testPrepareDownload() {
        int channel = mockLogicalChannelResponses("BF2102A0009000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.prepareDownload(
                IccUtils.hexStringToBytes("4131423243332D583459355A36"), // hashCc
                Asn1Node.newBuilder(0xA0).build().toBytes(),
                Asn1Node.newBuilder(0xA1).build().toBytes(),
                Asn1Node.newBuilder(0xA2).build().toBytes(), resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("BF2102A000", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel,
                "BF2115" + "A000" + "A100"
                        + "040D4131423243332D583459355A36" // hashCc
                        + "A200");
    }

    @Test
    public void testPrepareDownload_Error() {
        int channel = mockLogicalChannelResponses("BF2105A1030201039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.prepareDownload(
                IccUtils.hexStringToBytes("4131423243332D583459355A36"), // hashCc
                Asn1Node.newBuilder(0xA0).build().toBytes(),
                Asn1Node.newBuilder(0xA1).build().toBytes(),
                Asn1Node.newBuilder(0xA2).build().toBytes(), resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel,
                "BF2115" + "A000" + "A100"
                        + "040D4131423243332D583459355A36" // hashCc
                        + "A200");
    }

    @Test
    public void testLoadBoundProfilePackage() {
        int channel = mockLogicalChannelResponses(
                // For boundProfilePackage head + initialiseSecureChannelRequest
                // (ES8+.InitialiseSecureChannel)
                "9000",
                // For firstSequenceOf87 (ES8+.ConfigureISDP)
                "9000",
                // For head of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                // For body (element 1) of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                "9000",
                // For head of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // For body (element 1) of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // Profile installation result (element 2 of sequenceOf86)
                "BF37009000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        .addChild(Asn1Node.newBuilder(0xA3)
                                .addChildAsBytes(0x86, new byte[] {7, 8, 9})
                                .addChildAsBytes(0x86, new byte[] {0xA, 0xB, 0xC}))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("BF3700", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel, "BF361FBF2300"); // ES8+.InitialiseSecureChannel
        verifyStoreData(channel, "A0058703010203"); // ES8+.ConfigureISDP
        verifyStoreData(channel, "A105"); // ES8+.StoreMetadata
        verifyStoreData(channel, "8803040506"); // ES8+.StoreMetadata
        verifyStoreData(channel, "A200");
        verifyStoreData(channel, "A30A"); // ES8+.LoadProfileElements
        verifyStoreData(channel, "8603070809"); // ES8+.LoadProfileElements
        verifyStoreData(channel, "86030A0B0C"); // ES8+.LoadProfileElements
    }

    @Test
    public void testLoadBoundProfilePackage_ErrorAtEnd() {
        int channel = mockLogicalChannelResponses(
                // For boundProfilePackage head + initialiseSecureChannelRequest
                // (ES8+.InitialiseSecureChannel)
                "9000",
                // For firstSequenceOf87 (ES8+.ConfigureISDP)
                "9000",
                // For head of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                // For body (element 1) of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                "9000",
                // For head of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // For body (element 1) of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // Profile installation result (element 2 of sequenceOf86)
                "BF370ABF2707A205A1038101039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        .addChild(Asn1Node.newBuilder(0xA3)
                                .addChildAsBytes(0x86, new byte[] {7, 8, 9})
                                .addChildAsBytes(0x86, new byte[] {0xA, 0xB, 0xC}))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF361FBF2300"); // ES8+.InitialiseSecureChannel
        verifyStoreData(channel, "A0058703010203"); // ES8+.ConfigureISDP
        verifyStoreData(channel, "A105"); // ES8+.StoreMetadata
        verifyStoreData(channel, "8803040506"); // ES8+.StoreMetadata
        verifyStoreData(channel, "A200");
        verifyStoreData(channel, "A30A"); // ES8+.LoadProfileElements
        verifyStoreData(channel, "8603070809"); // ES8+.LoadProfileElements
        verifyStoreData(channel, "86030A0B0C"); // ES8+.LoadProfileElements
    }

    @Test
    public void testLoadBoundProfilePackage_ErrorInMiddle() {
        int channel = mockLogicalChannelResponses(
                // For boundProfilePackage head + initialiseSecureChannelRequest
                // (ES8+.InitialiseSecureChannel)
                "9000",
                // For firstSequenceOf87 (ES8+.ConfigureISDP)
                "9000",
                // For head of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                // For body (element 1) of sequenceOf88 (ES8+.StoreMetadata)
                "BF370ABF2707A205A1038101039000",
                "9000",
                // For head of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // For body (element 1) of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // Profile installation result (element 2 of sequenceOf86)
                "9000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        .addChild(Asn1Node.newBuilder(0xA3)
                                .addChildAsBytes(0x86, new byte[] {7, 8, 9})
                                .addChildAsBytes(0x86, new byte[] {0xA, 0xB, 0xC}))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF361FBF2300"); // ES8+.InitialiseSecureChannel
        verifyStoreData(channel, "A0058703010203"); // ES8+.ConfigureISDP
        verifyStoreData(channel, "A105"); // ES8+.StoreMetadata
        verifyStoreData(channel, "8803040506"); // ES8+.StoreMetadata
    }

    @Test
    public void testLoadBoundProfilePackage_ErrorStatus() {
        int channel = mockLogicalChannelResponses(
                // For boundProfilePackage head + initialiseSecureChannelRequest
                // (ES8+.InitialiseSecureChannel)
                "9000",
                // For firstSequenceOf87 (ES8+.ConfigureISDP)
                "9000",
                // For head of sequenceOf88 (ES8+.StoreMetadata)
                "9000",
                // For body (element 1) of sequenceOf88 (ES8+.StoreMetadata)
                "6985",
                "9000",
                // For head of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // For body (element 1) of sequenceOf86 (ES8+.LoadProfileElements)
                "9000",
                // Profile installation result (element 2 of sequenceOf86)
                "9000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        .addChild(Asn1Node.newBuilder(0xA3)
                                .addChildAsBytes(0x86, new byte[] {7, 8, 9})
                                .addChildAsBytes(0x86, new byte[] {0xA, 0xB, 0xC}))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        EuiccCardException e = (EuiccCardException) resultCaptor.exception;
        assertEquals(0x6985, ((ApduException) e.getCause()).getApduStatus());
        verifyStoreData(channel, "BF361FBF2300"); // ES8+.InitialiseSecureChannel
        verifyStoreData(channel, "A0058703010203"); // ES8+.ConfigureISDP
        verifyStoreData(channel, "A105"); // ES8+.StoreMetadata
        verifyStoreData(channel, "8803040506"); // ES8+.StoreMetadata
    }

    @Test
    public void testLoadBoundProfilePackage_NoProfileElements() {
        int channel = mockLogicalChannelResponses_sgp22v210();

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        // No children
                        .addChild(Asn1Node.newBuilder(0xA3))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        EuiccCardException e = (EuiccCardException) resultCaptor.exception;
        assertEquals("No profile elements in BPP", e.getCause().getMessage());
        verify(mMockCi, never())
                .iccTransmitApduLogicalChannel(
                        eq(channel), anyInt(), anyInt(), anyInt(), anyInt(), anyInt(), any(),
                        any());
    }

    @Test
    public void testLoadBoundProfilePackage_UnrecognizedTag() {
        int channel = mockLogicalChannelResponses_sgp22v210();

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.loadBoundProfilePackage(
                Asn1Node.newBuilder(0xBF36)
                        .addChild(Asn1Node.newBuilder(0xBF23))
                        .addChild(Asn1Node.newBuilder(0xA0)
                                .addChildAsBytes(0x87, new byte[] {1, 2, 3}))
                        .addChild(Asn1Node.newBuilder(0xA1)
                                .addChildAsBytes(0x88, new byte[] {4, 5, 6}))
                        .addChild(Asn1Node.newBuilder(0xA2))
                        .addChild(Asn1Node.newBuilder(0xA3)
                                .addChildAsBytes(0x86, new byte[] {7, 8, 9})
                                .addChildAsBytes(0x86, new byte[] {0xA, 0xB, 0xC}))
                        // Unrecognized tag
                        .addChild(Asn1Node.newBuilder(0xA4))
                        .build().toBytes(),
                resultCaptor, mHandler);
        resultCaptor.await();

        EuiccCardException e = (EuiccCardException) resultCaptor.exception;
        assertEquals(
                "Actual BPP length (33) does not match segmented length (31), this must be due to a"
                        + " malformed BPP",
                e.getCause().getMessage());
        verify(mMockCi, never())
                .iccTransmitApduLogicalChannel(
                        eq(channel), anyInt(), anyInt(), anyInt(), anyInt(), anyInt(), any(),
                        any());
    }

    @Test
    public void testCancelSession() {
        int channel = mockLogicalChannelResponses("BF41009000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.cancelSession(IccUtils.hexStringToBytes("A1B2C3"),
                EuiccCardManager.CANCEL_REASON_POSTPONED, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals("BF4100", IccUtils.bytesToHexString(resultCaptor.result));
        verifyStoreData(channel, "BF41088003A1B2C3810101");
    }

    @Test
    public void testCancelSession_Error() {
        int channel = mockLogicalChannelResponses("BF41038101039000");

        ResultCaptor<byte[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.cancelSession(IccUtils.hexStringToBytes("A1B2C3"),
                EuiccCardManager.CANCEL_REASON_POSTPONED, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF41088003A1B2C3810101");
    }

    @Test
    public void testListNotifications() {
        int channel = mockLogicalChannelResponses("BF282BA029"
                + "BF2F118001010C08736D64702E636F6D81020410" // Notification #1
                + "BF2F128001020C09736D6470322E636F6D81020420" // Notification #2
                + "9000");

        ResultCaptor<EuiccNotification[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.listNotifications(
                EuiccNotification.EVENT_DELETE | EuiccNotification.EVENT_DISABLE,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertArrayEquals(
                new EuiccNotification[] {
                        new EuiccNotification(1, "smdp.com", EuiccNotification.EVENT_DELETE, null),
                        new EuiccNotification(2, "smdp2.com", EuiccNotification.EVENT_DISABLE, null)
                },
                resultCaptor.result);
        verifyStoreData(channel, "BF280481020430");
    }

    @Test
    public void testListNotifications_Error() {
        int channel = mockLogicalChannelResponses("BF28038101039000");

        ResultCaptor<EuiccNotification[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.listNotifications(
                EuiccNotification.EVENT_DELETE | EuiccNotification.EVENT_DISABLE,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF280481020430");
    }

    @Test
    public void testRetrieveNotificationList() {
        int channel = mockLogicalChannelResponses("BF2B2FA02D"
                // Notification #1
                + "3014BF2F118001010C08736D64702E636F6D81020410"
                // Notification #2
                + "3015BF2F128001020C09736D6470322E636F6D81020420"
                + "9000");

        ResultCaptor<EuiccNotification[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.retrieveNotificationList(
                EuiccNotification.EVENT_DELETE | EuiccNotification.EVENT_DISABLE,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertArrayEquals(
                new EuiccNotification[] {
                        new EuiccNotification(1, "smdp.com", EuiccNotification.EVENT_DELETE,
                                IccUtils.hexStringToBytes(
                                        "3014BF2F118001010C08736D64702E636F6D81020410")),
                        new EuiccNotification(2, "smdp2.com", EuiccNotification.EVENT_DISABLE,
                                IccUtils.hexStringToBytes(
                                        "3015BF2F128001020C09736D6470322E636F6D81020420"))
                },
                resultCaptor.result);
        verifyStoreData(channel, "BF2B06A00481020430");
    }

    @Test
    public void testRetrieveNotificationList_Empty() {
        int channel = mockLogicalChannelResponses("BF2B038101019000");

        ResultCaptor<EuiccNotification[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.retrieveNotificationList(
                EuiccNotification.EVENT_DELETE | EuiccNotification.EVENT_DISABLE,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertArrayEquals(new EuiccNotification[0], resultCaptor.result);
        verifyStoreData(channel, "BF2B06A00481020430");
    }

    @Test
    public void testRetrieveNotificationList_Error() {
        int channel = mockLogicalChannelResponses("BF2B038101039000");

        ResultCaptor<EuiccNotification[]> resultCaptor = new ResultCaptor<>();
        mEuiccCard.retrieveNotificationList(
                EuiccNotification.EVENT_DELETE | EuiccNotification.EVENT_DISABLE,
                resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(3, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF2B06A00481020430");
    }

    @Test
    public void testRetrieveNotification() {
        int channel = mockLogicalChannelResponses("BF2B18A016"
                + "3014BF2F118001010C08736D64702E636F6D81020410" // Notification
                + "9000");

        ResultCaptor<EuiccNotification> resultCaptor = new ResultCaptor<>();
        mEuiccCard.retrieveNotification(5, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(
                new EuiccNotification(1, "smdp.com", EuiccNotification.EVENT_DELETE,
                        IccUtils.hexStringToBytes("3014BF2F118001010C08736D64702E636F6D81020410")),
                resultCaptor.result);
        verifyStoreData(channel, "BF2B05A003800105");
    }

    @Test
    public void testRetrieveNotification_Error() {
        int channel = mockLogicalChannelResponses("BF2B038101019000");

        ResultCaptor<EuiccNotification> resultCaptor = new ResultCaptor<>();
        mEuiccCard.retrieveNotification(5, resultCaptor, mHandler);
        resultCaptor.await();

        assertEquals(1, ((EuiccCardErrorException) resultCaptor.exception).getErrorCode());
        verifyStoreData(channel, "BF2B05A003800105");
    }

    @Test
    public void testRemoveNotificationFromList() {
        int channel = mockLogicalChannelResponses("BF30038001009000");

        ResultCaptor<Void> resultCaptor = new ResultCaptor<>();
        mEuiccCard.removeNotificationFromList(5, resultCaptor, mHandler);
        resultCaptor.await();

        assertUnexpectedException(resultCaptor.exception);
        verifyStoreData(channel, "BF3003800105");
    }

    @Test
    public void testAddDeviceCapability() throws InvalidAsn1DataException, TagNotFoundException {
        Asn1Node.Builder devCapsBuilder = Asn1Node.newBuilder(Tags.TAG_CTX_COMP_1);

        String devCapItem = "gsm,11";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        Asn1Node node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_0));
        Asn1Node child = node.getChild(Tags.TAG_CTX_0);
        assertTrue(Arrays.equals(new byte[] {11, 0 , 0}, child.asBytes()));

        devCapItem = "utran,11";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_1));
        child = node.getChild(Tags.TAG_CTX_1);
        assertTrue(Arrays.equals(new byte[] {11, 0 , 0}, child.asBytes()));

        devCapItem = "cdma1x,1";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_2));
        child = node.getChild(Tags.TAG_CTX_2);
        assertTrue(Arrays.equals(new byte[] {1, 0 , 0}, child.asBytes()));

        devCapItem = "hrpd,1";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_3));
        child = node.getChild(Tags.TAG_CTX_3);
        assertTrue(Arrays.equals(new byte[] {1, 0 , 0}, child.asBytes()));

        devCapItem = "ehrpd,12";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_4));
        child = node.getChild(Tags.TAG_CTX_4);
        assertTrue(Arrays.equals(new byte[] {12, 0 , 0}, child.asBytes()));

        devCapItem = "eutran,11";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_5));
        child = node.getChild(Tags.TAG_CTX_5);
        assertTrue(Arrays.equals(new byte[] {11, 0 , 0}, child.asBytes()));

        devCapItem = "nfc,0";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_6));
        child = node.getChild(Tags.TAG_CTX_6);
        assertTrue(Arrays.equals(new byte[] {0, 0 , 0}, child.asBytes()));

        devCapItem = "crl,0";
        mEuiccCard.addDeviceCapability(devCapsBuilder, devCapItem);
        node = devCapsBuilder.build();

        assertTrue(node.hasChild(Tags.TAG_CTX_7));
        child = node.getChild(Tags.TAG_CTX_7);
        assertTrue(Arrays.equals(new byte[] {0, 0 , 0}, child.asBytes()));

        // Array length should not be 3.
        Asn1Node.Builder devCapsBuilder2 = Asn1Node.newBuilder(Tags.TAG_CTX_COMP_1);
        devCapItem = "gsm,1,1";
        mEuiccCard.addDeviceCapability(devCapsBuilder2, devCapItem);
        node = devCapsBuilder2.build();

        assertFalse(node.hasChild(Tags.TAG_CTX_0));
    }

    @Test
    public void testGetDeviceId() {
        // Unclear v2.0 definition
        assertArrayEquals(
                new byte[] {0x21, 0x43, 0x65, (byte) 0x87, 0x09, 0x21, 0x43, 0x05},
                EuiccCard.getDeviceId("123456789012345", new EuiccSpecVersion(2, 0, 0)));
        // Clarified v2.1+ definition
        assertArrayEquals(
                new byte[] {0x21, 0x43, 0x65, (byte) 0x87, 0x09, 0x21, 0x43, 0x5F},
                EuiccCard.getDeviceId("123456789012345", new EuiccSpecVersion(2, 1, 0)));
        // Same definition on v2.2
        assertArrayEquals(
                new byte[] {0x21, 0x43, 0x65, (byte) 0x87, 0x09, 0x21, 0x43, 0x5F},
                EuiccCard.getDeviceId("123456789012345", new EuiccSpecVersion(2, 2, 0)));
    }

    private void verifyStoreData(int channel, String command) {
        verify(mMockCi, times(1))
                .iccTransmitApduLogicalChannel(eq(channel), eq(0x80 | channel), eq(0xE2), eq(0x91),
                        eq(0), eq(command.length() / 2), eq(command), any());
    }

    private int mockLogicalChannelResponses(Object... responses) {
        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi,
                "E00582030200009000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, responses);
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);
        return channel;
    }

    private int mockLogicalChannelResponses_sgp22v210(Object... responses) {
        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi,
                "E00582030201009000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, responses);
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);
        return channel;
    }
}
