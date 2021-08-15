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

package com.android.ike.ikev2;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.os.test.TestLooper;

import com.android.ike.ikev2.IkeSessionStateMachine.IChildSessionCallback;
import com.android.ike.ikev2.SaRecord.ChildSaRecord;
import com.android.ike.ikev2.SaRecord.ISaRecordHelper;
import com.android.ike.ikev2.SaRecord.SaRecordHelper;
import com.android.ike.ikev2.exceptions.IkeException;
import com.android.ike.ikev2.message.IkePayload;
import com.android.ike.ikev2.message.TestUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.List;

public final class ChildSessionStateMachineTest {
    private static final String IKE_AUTH_REQ_SA_PAYLOAD =
            "2c00002c00000028010304032ad4c0a20300000c0100000c800e0080"
                    + "03000008030000020000000805000000";
    private static final String IKE_AUTH_RESP_SA_PAYLOAD =
            "2c00002c0000002801030403cae7019f0300000c0100000c800e0080"
                    + "03000008030000020000000805000000";

    private static final String CURRENT_CHILD_SA_SPI_IN = "2ad4c0a2";
    private static final String CURRENT_CHILD_SA_SPI_OUT = "cae7019f";

    private TestLooper mLooper;
    private ChildSessionStateMachine mChildSessionStateMachine;

    private List<IkePayload> mAuthReqSaNegoPayloads = new LinkedList<>();
    private List<IkePayload> mAuthRespSaNegoPayloads = new LinkedList<>();

    private ChildSaRecord mSpyCurrentChildSaRecord;

    private ISaRecordHelper mMockSaRecordHelper;
    private IChildSessionCallback mMockChildSessionCallback;
    private ChildSessionOptions mChildSessionOptions;

    public ChildSessionStateMachineTest() {
        mMockSaRecordHelper = mock(SaRecord.ISaRecordHelper.class);
        mMockChildSessionCallback = mock(IChildSessionCallback.class);

        mChildSessionOptions = new ChildSessionOptions();
    }

    @Before
    public void setup() throws Exception {
        // Setup thread and looper
        mLooper = new TestLooper();
        mChildSessionStateMachine =
                new ChildSessionStateMachine(
                        "ChildSessionStateMachine", mLooper.getLooper(), mChildSessionOptions);
        mChildSessionStateMachine.setDbg(true);
        SaRecord.setSaRecordHelper(mMockSaRecordHelper);

        setUpPayloadLists();
        setUpChildSaRecords();

        mChildSessionStateMachine.start();
    }

    private void setUpPayloadLists() throws IkeException {
        mAuthReqSaNegoPayloads.add(
                TestUtils.hexStringToIkePayload(
                        IkePayload.PAYLOAD_TYPE_SA, false, IKE_AUTH_REQ_SA_PAYLOAD));
        mAuthRespSaNegoPayloads.add(
                TestUtils.hexStringToIkePayload(
                        IkePayload.PAYLOAD_TYPE_SA, true, IKE_AUTH_RESP_SA_PAYLOAD));
        // TODO: Build and add Traffic Selector Payloads to two payload lists.
    }

    private void setUpChildSaRecords() {
        mSpyCurrentChildSaRecord =
                spy(makeDummyChildSaRecord(CURRENT_CHILD_SA_SPI_IN, CURRENT_CHILD_SA_SPI_OUT));
    }

    private ChildSaRecord makeDummyChildSaRecord(String inboundSpiHex, String outboundSpiHex) {
        byte[] spiInBytes = TestUtils.hexStringToByteArray(CURRENT_CHILD_SA_SPI_IN);
        int spiIn = ByteBuffer.wrap(spiInBytes).getInt();

        byte[] spiOutBytes = TestUtils.hexStringToByteArray(CURRENT_CHILD_SA_SPI_OUT);
        int spiOut = ByteBuffer.wrap(spiOutBytes).getInt();

        return new ChildSaRecord(spiIn, spiOut, null, null);
    }

    @After
    public void tearDown() {
        mChildSessionStateMachine.quit();
        mChildSessionStateMachine.setDbg(false);

        SaRecord.setSaRecordHelper(new SaRecordHelper());
    }

    @Test
    public void testCreateFirstChild() throws Exception {
        when(mMockSaRecordHelper.makeChildSaRecord(any(), any()))
                .thenReturn(mSpyCurrentChildSaRecord);
        mChildSessionStateMachine.handleFirstChildExchange(
                mAuthReqSaNegoPayloads, mAuthRespSaNegoPayloads, mMockChildSessionCallback);

        mLooper.dispatchAll();
        verify(mMockChildSessionCallback)
                .onCreateChildSa(mSpyCurrentChildSaRecord.outboundSpi, mChildSessionStateMachine);
        assertTrue(
                mChildSessionStateMachine.getCurrentState()
                        instanceof ChildSessionStateMachine.Idle);
        assertEquals(mSpyCurrentChildSaRecord, mChildSessionStateMachine.mCurrentChildSaRecord);
    }
}
