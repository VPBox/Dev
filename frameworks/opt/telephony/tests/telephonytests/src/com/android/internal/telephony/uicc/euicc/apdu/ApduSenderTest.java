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

package com.android.internal.telephony.uicc.euicc.apdu;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.Handler;
import android.os.HandlerThread;

import com.android.internal.telephony.CommandException;
import com.android.internal.telephony.CommandsInterface;
import com.android.internal.telephony.uicc.IccIoResult;
import com.android.internal.telephony.uicc.IccUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class ApduSenderTest {
    private static final long WAIT_TIMEOUT_MLLIS = 5000;

    private static class ResponseCaptor extends ApduSenderResultCallback {
        public byte[] response;
        public Throwable exception;
        public int stopApduIndex = -1;

        private final CountDownLatch mLatch = new CountDownLatch(1);

        public void await() {
            try {
                if (!mLatch.await(WAIT_TIMEOUT_MLLIS, TimeUnit.MILLISECONDS)) {
                    fail("Execution timed out.");
                }
            } catch (InterruptedException e) {
                fail("Execution interrupted: " + e);
            }
        }

        @Override
        public boolean shouldContinueOnIntermediateResult(IccIoResult result) {
            if (stopApduIndex < 0) {
                return true;
            }
            if (stopApduIndex == 0) {
                return false;
            }
            stopApduIndex--;
            return true;
        }

        @Override
        public void onResult(byte[] bytes) {
            response = bytes;
            mLatch.countDown();
        }

        @Override
        public void onException(Throwable e) {
            exception = e;
            mLatch.countDown();
        }
    }

    @Mock
    private CommandsInterface mMockCi;

    private HandlerThread mThread;
    private Handler mHandler;
    private ResponseCaptor mResponseCaptor;
    private byte[] mSelectResponse;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);

        mThread = new HandlerThread("ApduSenderTest");
        mThread.start();
        mHandler = mThread.getThreadHandler();

        mResponseCaptor = new ResponseCaptor();
        mSelectResponse = null;
    }

    @After
    public void tearDown() {
        mThread.quit();
    }

    @Test
    public void testSendEmptyCommands() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "A1A1A19000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        sender.send((selectResponse, requestBuilder) -> mSelectResponse = selectResponse,
                mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("A1A1A19000", IccUtils.bytesToHexString(mSelectResponse));
        assertNull(mResponseCaptor.response);
        assertNull(mResponseCaptor.exception);
        verify(mMockCi).iccOpenLogicalChannel(eq(aid), anyInt(), any());
        verify(mMockCi).iccCloseLogicalChannel(eq(channel), any());
    }

    @Test
    public void testOpenChannelErrorStatus() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi,
                new CommandException(CommandException.Error.NO_SUCH_ELEMENT));

        sender.send((selectResponse, requestBuilder) -> mSelectResponse = new byte[0],
                mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertNull("Request provider should not be called when failed to open channel.",
                mSelectResponse);
        assertTrue(mResponseCaptor.exception instanceof ApduException);
        verify(mMockCi).iccOpenLogicalChannel(eq(aid), anyInt(), any());
    }

    @Test
    public void testSend() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A1A1A19000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        sender.send((selectResponse, requestBuilder) -> requestBuilder.addApdu(10, 1, 2, 3, 0, "a"),
                mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("A1A1A1", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
    }

    @Test
    public void testSendMultiApdus() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A19000", "A29000",
                "A39000", "A49000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addApdu(10, 1, 2, 3, 0, "a");
            requestBuilder.addApdu(10, 1, 2, 3, "ab");
            requestBuilder.addApdu(10, 1, 2, 3);
            requestBuilder.addStoreData("abcd");
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("A4", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(1), eq("ab"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq(""), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x91),
                eq(0), eq(2), eq("abcd"), any());
    }

    @Test
    public void testSendMultiApdusStopEarly() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A19000", "A29000",
                "A39000", "A49000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);
        mResponseCaptor.stopApduIndex = 2;

        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addApdu(10, 1, 2, 3, 0, "a");
            requestBuilder.addApdu(10, 1, 2, 3, "ab");
            requestBuilder.addApdu(10, 1, 2, 3);
            requestBuilder.addStoreData("abcd");
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("A3", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(1), eq("ab"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq(""), any());
    }

    @Test
    public void testSendLongResponse() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A1A1A16104",
                "B2B2B2B26102", "C3C39000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        sender.send((selectResponse, requestBuilder) -> requestBuilder.addApdu(10, 1, 2, 3, 0, "a"),
                mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("A1A1A1B2B2B2B2C3C3", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel), eq(0xC0), eq(0),
                eq(0), eq(4), eq(""), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel), eq(0xC0), eq(0),
                eq(0), eq(2), eq(""), any());
    }

    @Test
    public void testSendStoreDataLongDataLongResponse() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A19000", "9000", "9000",
                "B22B6103", "B2222B9000", "C39000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        // Each segment has 0xFF (the limit of a single command) bytes.
        String s1 = new String(new char[0xFF]).replace("\0", "AA");
        String s2 = new String(new char[0xFF]).replace("\0", "BB");
        String s3 = new String(new char[16]).replace("\0", "CC");
        String longData = s1 + s2 + s3;
        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addApdu(10, 1, 2, 3, 0, "a");
            requestBuilder.addApdu(10, 1, 2, 3, 0, "b");
            requestBuilder.addStoreData(longData);
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("C3", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("b"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x11),
                eq(0), eq(0xFF), eq(s1), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x11),
                eq(1), eq(0xFF), eq(s2), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x91),
                eq(2), eq(16), eq(s3), any());
    }

    @Test
    public void testSendStoreDataLongDataMod0() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "9000", "B2222B9000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        // Each segment has 0xFF (the limit of a single command) bytes.
        String s1 = new String(new char[0xFF]).replace("\0", "AA");
        String s2 = new String(new char[0xFF]).replace("\0", "BB");
        String longData = s1 + s2;
        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addStoreData(longData);
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("B2222B", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x11),
                eq(0), eq(0xFF), eq(s1), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x91),
                eq(1), eq(0xFF), eq(s2), any());
    }

    @Test
    public void testSendStoreDataLen0() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "B2222B9000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addStoreData("");
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals("B2222B", IccUtils.bytesToHexString(mResponseCaptor.response));
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x91),
                eq(0), eq(0), eq(""), any());
    }

    @Test
    public void testSendErrorResponseInMiddle() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockSendToLogicalChannel(mMockCi, channel, "A19000", "9000",
                "B22B6103", "6985");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        // Each segment has 0xFF (the limit of a single command) bytes.
        String s1 = new String(new char[0xFF]).replace("\0", "AA");
        String s2 = new String(new char[0xFF]).replace("\0", "BB");
        String s3 = new String(new char[16]).replace("\0", "CC");
        String longData = s1 + s2 + s3;
        sender.send((selectResponse, requestBuilder) -> {
            requestBuilder.addApdu(10, 1, 2, 3, 0, "a");
            requestBuilder.addStoreData(longData);
        }, mResponseCaptor, mHandler);
        mResponseCaptor.await();

        assertEquals(0x6985, ((ApduException) mResponseCaptor.exception).getApduStatus());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(channel | 10), eq(1), eq(2),
                eq(3), eq(0), eq("a"), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x11),
                eq(0), eq(0xFF), eq(s1), any());
        verify(mMockCi).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2), eq(0x11),
                eq(1), eq(0xFF), eq(s2), any());
        verify(mMockCi, never()).iccTransmitApduLogicalChannel(eq(channel), eq(0x81), eq(0xE2),
                eq(0x91), eq(2), eq(16), eq(s3), any());
    }

    @Test
    public void testChannelAlreadyOpened() throws InterruptedException {
        String aid = "B2C3D4";
        ApduSender sender = new ApduSender(mMockCi, aid, false /* supportExtendedApdu */);

        int channel = LogicalChannelMocker.mockOpenLogicalChannelResponse(mMockCi, "9000");
        LogicalChannelMocker.mockCloseLogicalChannel(mMockCi, channel);

        ResponseCaptor outerResponseCaptor = new ResponseCaptor();
        sender.send(
                (selectResponse, requestBuilder) -> sender.send(
                        (selectResponseOther, requestBuilderOther) ->
                                mSelectResponse = selectResponseOther,
                        mResponseCaptor, mHandler),
                outerResponseCaptor, mHandler);
        mResponseCaptor.await();
        outerResponseCaptor.await();

        assertNull("Should not open channel when another one is already opened.", mSelectResponse);
        assertTrue(mResponseCaptor.exception instanceof ApduException);
        verify(mMockCi, times(1)).iccOpenLogicalChannel(eq(aid), anyInt(), any());
    }
}
