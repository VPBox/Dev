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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.database.MatrixCursor;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.util.HexDump;

import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

public class InboundSmsTrackerTest {
    InboundSmsTracker mInboundSmsTracker;

    private static final byte[] FAKE_PDU = new byte[]{1, 2, 3};
    private static final long FAKE_TIMESTAMP = 123456L;
    private static final int FAKE_DEST_PORT = 1234;
    private static final String FAKE_ADDRESS = "address";
    private static final String FAKE_DISPLAY_ADDRESS = "disp_addr";
    private static final int FAKE_REFERENCE_NUMBER = 345;
    private static final int FAKE_SEQUENCE_NUMBER = 3;
    private static final int FAKE_MESSAGE_COUNT = 5;
    private static final String FAKE_MESSAGE_BODY = "message body";
    private static final int FAKE_SUBID = 0;

    @Before
    public void setUp() throws Exception {
        mInboundSmsTracker = new InboundSmsTracker(FAKE_PDU, FAKE_TIMESTAMP, FAKE_DEST_PORT, false,
                FAKE_ADDRESS, FAKE_DISPLAY_ADDRESS, FAKE_REFERENCE_NUMBER, FAKE_SEQUENCE_NUMBER,
                FAKE_MESSAGE_COUNT, false, FAKE_MESSAGE_BODY, false /* isClass0 */, FAKE_SUBID);
    }

    public static MatrixCursor createFakeCursor() {
        MatrixCursor mc = new MatrixCursor(
                new String[]{"pdu", "seq", "dest", "date", "ref", "cnt", "addr", "id", "msg_body",
                        "display_originating_addr", "sub_id"});
        mc.addRow(new Object[]{HexDump.toHexString(FAKE_PDU),
                FAKE_SEQUENCE_NUMBER, FAKE_DEST_PORT, FAKE_TIMESTAMP,
                FAKE_REFERENCE_NUMBER, FAKE_MESSAGE_COUNT, FAKE_ADDRESS, 1, FAKE_MESSAGE_BODY,
                FAKE_DISPLAY_ADDRESS, FAKE_SUBID});
        mc.addRow(new Object[]{HexDump.toHexString(FAKE_PDU),
                FAKE_SEQUENCE_NUMBER, FAKE_DEST_PORT, FAKE_TIMESTAMP,
                FAKE_REFERENCE_NUMBER, FAKE_MESSAGE_COUNT, FAKE_ADDRESS, 2, FAKE_MESSAGE_BODY,
                FAKE_DISPLAY_ADDRESS, FAKE_SUBID});
        mc.moveToFirst();
        return mc;
    }

    @Test
    @SmallTest
    public void testInitialization() {
        assertTrue(Arrays.equals(FAKE_PDU, mInboundSmsTracker.getPdu()));
        assertEquals(FAKE_TIMESTAMP, mInboundSmsTracker.getTimestamp());
        assertEquals(FAKE_DEST_PORT, mInboundSmsTracker.getDestPort());
        assertFalse(mInboundSmsTracker.is3gpp2());
        assertEquals(FAKE_ADDRESS, mInboundSmsTracker.getAddress());
        assertEquals(FAKE_REFERENCE_NUMBER, mInboundSmsTracker.getReferenceNumber());
        assertEquals(FAKE_SEQUENCE_NUMBER, mInboundSmsTracker.getSequenceNumber());
        assertEquals(FAKE_MESSAGE_COUNT, mInboundSmsTracker.getMessageCount());
        assertEquals(1, mInboundSmsTracker.getIndexOffset());
        assertEquals(SmsConstants.FORMAT_3GPP, mInboundSmsTracker.getFormat());
        assertEquals(FAKE_MESSAGE_BODY, mInboundSmsTracker.getMessageBody());
        assertEquals(FAKE_DISPLAY_ADDRESS, mInboundSmsTracker.getDisplayAddress());
        assertEquals(false, mInboundSmsTracker.isClass0());
        assertEquals(FAKE_SUBID, mInboundSmsTracker.getSubId());

        String[] args = new String[]{"123"};
        mInboundSmsTracker.setDeleteWhere(InboundSmsHandler.SELECT_BY_ID, args);
        assertEquals(InboundSmsHandler.SELECT_BY_ID, mInboundSmsTracker.getDeleteWhere());
        assertTrue(Arrays.equals(args, mInboundSmsTracker.getDeleteWhereArgs()));
    }

    @Test
    @SmallTest
    public void testInitializationFromDb() {
        mInboundSmsTracker = new InboundSmsTracker(createFakeCursor(), false);
        testInitialization();
    }
}