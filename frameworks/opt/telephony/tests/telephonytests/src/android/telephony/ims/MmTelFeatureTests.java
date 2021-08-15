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

package android.telephony.ims;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertTrue;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.telecom.TelecomManager;
import android.telephony.ims.aidl.IImsCapabilityCallback;
import android.telephony.ims.aidl.IImsMmTelFeature;
import android.telephony.ims.feature.MmTelFeature;
import android.telephony.ims.stub.ImsCallSessionImplBase;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import com.android.ims.internal.IImsCallSession;
import com.android.internal.telephony.ims.ImsTestBase;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

@RunWith(AndroidJUnit4.class)
public class MmTelFeatureTests extends ImsTestBase {

    // Public for Mockito testing
    public class CapabilityCallback extends IImsCapabilityCallback.Stub {

        @Override
        public void onQueryCapabilityConfiguration(int capability, int radioTech, boolean enabled) {

        }

        @Override
        public void onChangeCapabilityConfigurationError(int capability, int radioTech,
                int reason) {

        }

        @Override
        public void onCapabilitiesStatusChanged(int config) {

        }
    }

    private static final int TEST_CAPABILITY = 1;
    private static final int TEST_RADIO_TECH = 0;

    private static final int TEST_TTY_RESULT = 0;
    private static final int TEST_SEND_DTMF_RESULT = 1;
    private static final int TEST_RESULT_MAX = 2;

    private static final int TEST_RESULT_DELAY_MS = 5000;

    private android.telephony.ims.TestMmTelFeature mFeature;
    private IImsMmTelFeature mFeatureBinder;
    private CapabilityCallback mCapabilityCallback;
    private MmTelFeature.Listener mListener;

    // set to true when the handler receives a message back from the Feature.
    private boolean[] mHandlerResults;

    private class TestHandler extends Handler {

        TestHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case TEST_TTY_RESULT:
                    mHandlerResults[TEST_TTY_RESULT] = true;
                    break;
                case TEST_SEND_DTMF_RESULT:
                    mHandlerResults[TEST_SEND_DTMF_RESULT] = true;
                    break;
            }
        }
    }
    private final Handler mHandler = new TestHandler(Looper.getMainLooper());
    private final Messenger mHandlerMessenger = new Messenger(mHandler);

    @Before
    public void setup() throws Exception {
        super.setUp();
        mFeature = new TestMmTelFeature();
        mFeatureBinder = mFeature.getBinder();
        mCapabilityCallback = spy(new CapabilityCallback());
        mListener = spy(new MmTelFeature.Listener());
        mFeatureBinder.setListener(mListener);
        mHandlerResults = new boolean[TEST_RESULT_MAX];
    }

    @After
    public void tearDown() {
        mFeature = null;
        mFeatureBinder = null;
    }

    @SmallTest
    @Test
    public void testQueryCapabilityConfiguration() throws Exception {
        mFeature.queryConfigurationResult = true;

        mFeatureBinder.queryCapabilityConfiguration(TEST_CAPABILITY, TEST_RADIO_TECH,
                mCapabilityCallback);

        verify(mCapabilityCallback).onQueryCapabilityConfiguration(eq(TEST_CAPABILITY),
                eq(TEST_RADIO_TECH), eq(true));
    }

    @SmallTest
    @Test
    public void testNewIncomingCall() throws Exception {
        IImsCallSession sessionBinder = Mockito.mock(IImsCallSession.class);
        ImsCallSessionImplBase session = new ImsCallSessionImplBase();
        session.setServiceImpl(sessionBinder);

        mFeature.incomingCall(session);
        ArgumentCaptor<IImsCallSession> captor = ArgumentCaptor.forClass(IImsCallSession.class);
        verify(mListener).onIncomingCall(captor.capture(), any());

        assertEquals(sessionBinder, captor.getValue());
    }

    @SmallTest
    @Test
    public void testSetTtyMessageMessenger() throws Exception {
        Message resultMessage = Message.obtain(mHandler, TEST_TTY_RESULT);
        resultMessage.replyTo = mHandlerMessenger;
        mFeatureBinder.setUiTtyMode(TelecomManager.TTY_MODE_FULL, resultMessage);
        waitForHandlerAction(mHandler, TEST_RESULT_DELAY_MS);
        assertTrue(mHandlerResults[TEST_TTY_RESULT]);
    }

    @SmallTest
    @Test
    public void testSendDtmfMessageMessenger() throws Exception {
        Message resultMessage = Message.obtain(mHandler, TEST_SEND_DTMF_RESULT);
        resultMessage.replyTo = mHandlerMessenger;
        IImsCallSession callSession = mFeatureBinder.createCallSession(null);
        callSession.sendDtmf('0', resultMessage);
        waitForHandlerAction(mHandler, TEST_RESULT_DELAY_MS);
        assertTrue(mHandlerResults[TEST_SEND_DTMF_RESULT]);
    }
}
