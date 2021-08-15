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
package com.android.internal.telephony.uicc;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.doAnswer;

import android.content.pm.Signature;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.CommandException;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

public class UiccCarrierPrivilegeRulesTest extends TelephonyTest {
    private UiccCarrierPrivilegeRules mUiccCarrierPrivilegeRules;
    public UiccCarrierPrivilegeRulesTest() {
        super();
    }
    private UiccCarrierPrivilegeRulesHandlerThread mTestHandlerThread;
    private Handler mHandler;

    private static final int EVENT_OPEN_LOGICAL_CHANNEL_DONE = 1;
    private static final int EVENT_TEST_DONE = 2;

    @Mock
    private UiccProfile mUiccProfile;

    private class UiccCarrierPrivilegeRulesHandlerThread extends HandlerThread {

        private UiccCarrierPrivilegeRulesHandlerThread(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            /* create a custom handler for the Handler Thread */
            mHandler = new Handler(mTestHandlerThread.getLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case EVENT_OPEN_LOGICAL_CHANNEL_DONE:
                            /* Upon handling this event, new CarrierPrivilegeRule
                            will be created with the looper of HandlerThread */
                            mUiccCarrierPrivilegeRules = new UiccCarrierPrivilegeRules(
                                    mUiccProfile, mHandler.obtainMessage(EVENT_TEST_DONE));
                            break;
                        case EVENT_TEST_DONE:
                            setReady(true);
                            break;
                        default:
                            logd("Unknown Event " + msg.what);
                    }
                }
            };
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mTestHandlerThread = new UiccCarrierPrivilegeRulesHandlerThread(TAG);
        mTestHandlerThread.start();

        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mTestHandlerThread.quit();
        super.tearDown();
        mUiccCarrierPrivilegeRules = null;
    }

    private void testHelper(String hexString) {
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[2];
                AsyncResult ar = new AsyncResult(null, new int[]{0}, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes(hexString));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                anyInt(), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));

        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();
    }

    @Test
    @SmallTest
    public void testParseRule_Normal() {
        /**
         * FF40 45
         *   E2 43
         *      E1 35
         *         C1 14 ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4
         *         CA 1D 636F6D2E676F6F676C652E616E64726F69642E617070732E6D79617070
         *      E3 0A
         *         DB 08 0000000000000001
         */
        final String hexString =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30ADB080000000000000001";

        testHelper(hexString);

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(2, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        Signature signature = new Signature("abcd92cbb156b280fa4e1429a6eceeb6e5c1bfe4");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
    }

    @Test
    @SmallTest
    public void testParseRule_With4FD0D1() {
        /**
         * FF40 34
         *   E2 32
         *      E1 1E
         *         4F 06 FF FF FF FF FF FF
         *         C1 14 B6 1B E3 4A D2 C2 0D 7A FE D8 49 3C 31 3A 13 7F 89 FA 27 65
         *      E3 10
         *         D0 01 01
         *         D1 01 01
         *         DB 08 00 00 00 00 00 00 00 01
         */
        final String hexString = "FF4034E232E11E4F06FFFFFFFFFFFFC114B61BE34AD2C20D7AFED84"
                + "93C313A137F89FA2765E310D00101D10101DB080000000000000001";

        testHelper(hexString);

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(0, mUiccCarrierPrivilegeRules.getPackageNames().size());
    }

    @Test
    @SmallTest
    public void testParseRule_With4FD0() {
        /**
         * FF40 31
         *   E2 2F
         *      E1 1E
         *         4F 06 FF FF FF FF FF FF
         *         C1 14 B6 1B E3 4A D2 C2 0D 7A FE D8 49 3C 31 3A 13 7F 89 FA 27 65
         *      E3 0D
         *         D0 01 01
         *         DB 08 00 00 00 00 00 00 00 01
         */
        final String hexString = "FF4031E22FE11E4F06FFFFFFFFFFFFC114B61BE34AD2C20D7AFED8493C313A"
                + "137F89FA2765E30DD00101DB080000000000000001";

        testHelper(hexString);

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(0, mUiccCarrierPrivilegeRules.getPackageNames().size());
    }

    @Test
    @SmallTest
    public void testParseRule_TwoMessages() {
        /**
         * FF40 68
         *   E2 39
         *      E1 2B
         *         4F 06 FFFFFFFFFFFF
         *         C1 02 B61B
         *         CA 1D 636F6D2E676F6F676C652E616E64726F69642E617070732E6D79617070
         *      E3 0A
         *         D0 01 01
         *         D1 01 01
         *         DB 02 0001
         *   E2 2B
         *      E1 23
         *         C1 02 ABCD
         *         CA 1D 636F6D2E676F6F676C652E616E64726F69642E617070732E6D79617070
         *      E3 04
         *         DB 02 0001
         */
        final String hexString =
                "FF4068E239E12B4F06FFFFFFFFFFFFC102B61BCA1D636F6D2E676F6F676C652E616E64726F69642"
                        + "E617070732E6D79617070E30AD00101D10101DB020001E22BE123C102ABCDCA1D636F"
                        + "6D2E676F6F676C652E616E64726F69642E617070732E6D79617070E304DB020001";

        testHelper(hexString);

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(4, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        Signature signature1 = new Signature("b61b");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature1,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));

        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(1));
        Signature signature2 = new Signature("abcd");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature2,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
    }

    @Test
    @SmallTest
    public void testParseRule_InvalidRulesWith4F00() {
        /**
         * FF40 24
         *   E2 22
         *      E1 18
         *         4F 00
         *         C1 14 75C073AFD219AEB221948E828F066E778ADFDF23
         *      E3 06
         *         D0 01 01
         *         D1 01 01
         */
        final String hexString = "FF4024E222E1184F00C11475C073AFD219AEB221948E828F066E778ADFDF23"
                + "E306D00101D10101";

        testHelper(hexString);

        assertTrue(!mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(0, mUiccCarrierPrivilegeRules.getPackageNames().size());
    }

    @Test
    @SmallTest
    public void testParseRule_InvalidRulesWithoutDB() {
        /**
         * FF40 2A
         *   E2 28
         *      E1 1E
         *         4F 06 FF FF FF FF FF FF
         *         C1 14 B6 1B E3 4A D2 C2 0D 7A FE D8 49 3C 31 3A 13 7F 89 FA 27 65
         *      E3 06
         *         D0 01 01
         *         D1 01 01
         */
        final String hexString = "FF402AE228E11E4F06FFFFFFFFFFFFC114B61BE34AD2C20D7AFED8493C313A"
                + "137F89FA2765E306D00101D10101";

        testHelper(hexString);

        assertTrue(!mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(0, mUiccCarrierPrivilegeRules.getPackageNames().size());
    }

    @Test
    @SmallTest
    public void testRetryARAM_shouldRetry() {
        AsyncResult ar1 = new AsyncResult(
                null,
                new int[]{0, 105, -123},
                new CommandException(CommandException.Error.NO_SUCH_ELEMENT));
        assertTrue(mUiccCarrierPrivilegeRules.shouldRetry(ar1, 0));

        AsyncResult ar2 = new AsyncResult(
                null,
                new int[]{0},
                new CommandException(CommandException.Error.MISSING_RESOURCE));
        assertTrue(mUiccCarrierPrivilegeRules.shouldRetry(ar2, 0));

        AsyncResult ar3 = new AsyncResult(
                null,
                new int[]{0, 105, 153},
                new CommandException(CommandException.Error.INTERNAL_ERR));
        assertTrue(mUiccCarrierPrivilegeRules.shouldRetry(ar3, 0));
    }

    @Test
    @SmallTest
    public void testRetryARAM_shouldNotRetry() {
        AsyncResult ar = new AsyncResult(
                null,
                new int[]{0, 106, -126},
                new CommandException(CommandException.Error.NO_SUCH_ELEMENT));
        assertTrue(!mUiccCarrierPrivilegeRules.shouldRetry(ar, 0));
    }

    private static final String ARAM = "A00000015141434C00";
    private static final String ARAD = "A00000015144414300";
    private static final String PKCS15_AID = "A000000063504B43532D3135";

    @Test
    @SmallTest
    public void testAID_OnlyARAM() {
        final String hexString =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30ADB080000000000000001";
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                String aid = (String) invocation.getArguments()[0];
                Message message = (Message) invocation.getArguments()[2];
                if (aid.equals(ARAM)) {
                    AsyncResult ar = new AsyncResult(null, new int[]{0}, null);
                    message.obj = ar;
                    message.arg2 = 1;
                    message.sendToTarget();
                } else {
                    AsyncResult ar = new AsyncResult(null, null, null);
                    message.obj = ar;
                    message.sendToTarget();
                }
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes(hexString));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                anyInt(), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));


        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(1, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        Signature signature = new Signature("abcd92cbb156b280fa4e1429a6eceeb6e5c1bfe4");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
    }

    @Test
    @SmallTest
    public void testAID_OnlyARAD() {
        final String hexString =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30ADB080000000000000001";
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                String aid = (String) invocation.getArguments()[0];
                Message message = (Message) invocation.getArguments()[2];
                if (aid.equals(ARAD)) {
                    AsyncResult ar = new AsyncResult(null, new int[]{0}, null);
                    message.obj = ar;
                    message.arg2 = 0;
                    message.sendToTarget();
                } else {
                    AsyncResult ar = new AsyncResult(null, null, null);
                    message.obj = ar;
                    message.arg2 = 1;
                    message.sendToTarget();
                }
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes(hexString));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                anyInt(), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));


        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(1, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        Signature signature = new Signature("abcd92cbb156b280fa4e1429a6eceeb6e5c1bfe4");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
    }

    @Test
    @SmallTest
    public void testAID_BothARAMandARAD() {
        final String hexString =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30ADB080000000000000001";
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                String aid = (String) invocation.getArguments()[0];
                Message message = (Message) invocation.getArguments()[2];
                AsyncResult ar = new AsyncResult(null, new int[]{0}, null);
                message.obj = ar;
                if (aid.equals(ARAD)) {
                    message.arg2 = 0;
                } else {
                    message.arg2 = 1;
                }
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00, IccUtils.hexStringToBytes(hexString));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                anyInt(), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));


        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();

        Signature signature = new Signature("abcd92cbb156b280fa4e1429a6eceeb6e5c1bfe4");
        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(2, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(1));
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(1)));
    }

    @Test
    @SmallTest
    public void testAID_NeitherARAMorARAD() {
        final String hexString =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30ADB080000000000000001";
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                String aid = (String) invocation.getArguments()[0];
                Message message = (Message) invocation.getArguments()[2];
                AsyncResult ar = new AsyncResult(null, null, null);
                if (aid.equals(ARAM)) {
                    message.arg2 = 1;
                } else if (aid.equals(ARAD)) {
                    message.arg2 = 0;
                } else {
                    // PKCS15
                    ar = new AsyncResult(null, null, new Throwable());
                }
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));

        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();

        assertTrue(!mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
    }

    private static final int P2 = 0x40;
    private static final int P2_EXTENDED_DATA = 0x60;
    @Test
    @SmallTest
    public void testAID_RetransmitLogicalChannel() {
        final String hexString1 =
                "FF4045E243E135C114ABCD92CBB156B280FA4E1429A6ECEEB6E5C1BFE4CA1D636F6D2E676F6F676"
                        + "C652E616E64726F69642E617070732E6D79617070E30A";

        final String hexString2 =
                "DB080000000000000001";
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                String aid = (String) invocation.getArguments()[0];
                Message message = (Message) invocation.getArguments()[2];
                if (aid.equals(ARAD)) {
                    AsyncResult ar = new AsyncResult(null, new int[]{0}, null);
                    message.obj = ar;
                    message.arg2 = 0;
                    message.sendToTarget();
                } else {
                    AsyncResult ar = new AsyncResult(null, null, null);
                    message.obj = ar;
                    message.arg2 = 1;
                    message.sendToTarget();
                }
                return null;
            }
        }).when(mUiccProfile).iccOpenLogicalChannel(anyString(), anyInt(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00,
                        IccUtils.hexStringToBytes(hexString1));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                eq(P2), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[7];
                IccIoResult iir = new IccIoResult(0x90, 0x00,
                        IccUtils.hexStringToBytes(hexString2));
                AsyncResult ar = new AsyncResult(null, iir, null);
                message.obj = ar;
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccTransmitApduLogicalChannel(anyInt(), anyInt(), anyInt(), anyInt(),
                eq(P2_EXTENDED_DATA), anyInt(), anyString(), any(Message.class));

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message message = (Message) invocation.getArguments()[1];
                message.sendToTarget();
                return null;
            }
        }).when(mUiccProfile).iccCloseLogicalChannel(anyInt(), any(Message.class));


        Message mCardOpenLogicalChannel = mHandler.obtainMessage(EVENT_OPEN_LOGICAL_CHANNEL_DONE);
        setReady(false);
        mCardOpenLogicalChannel.sendToTarget();
        waitUntilReady();

        assertTrue(mUiccCarrierPrivilegeRules.hasCarrierPrivilegeRules());
        assertEquals(1, mUiccCarrierPrivilegeRules.getPackageNames().size());
        assertEquals("com.google.android.apps.myapp",
                mUiccCarrierPrivilegeRules.getPackageNames().get(0));
        Signature signature = new Signature("abcd92cbb156b280fa4e1429a6eceeb6e5c1bfe4");
        assertEquals(0, mUiccCarrierPrivilegeRules.getCarrierPrivilegeStatus(signature,
                mUiccCarrierPrivilegeRules.getPackageNames().get(0)));
    }
}
