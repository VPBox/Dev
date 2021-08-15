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

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;

import android.content.Context;
import android.net.IpSecManager;
import android.net.IpSecManager.UdpEncapsulationSocket;
import android.os.HandlerThread;
import android.os.Looper;
import android.system.ErrnoException;
import android.system.Os;
import android.system.OsConstants;
import android.util.Log;
import android.util.LongSparseArray;

import androidx.test.InstrumentationRegistry;

import com.android.ike.ikev2.IkeSocket.PacketReceiver;
import com.android.ike.ikev2.message.IkeHeader;
import com.android.ike.ikev2.message.TestUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;

import java.io.FileDescriptor;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public final class IkeSocketTest {
    private static final int REMOTE_RECV_BUFF_SIZE = 2048;
    private static final int TIMEOUT = 1000;

    private static final String NON_ESP_MARKER_HEX_STRING = "00000000";
    private static final String IKE_REQ_MESSAGE_HEX_STRING =
            "5f54bf6d8b48e6e100000000000000002120220800000000"
                    + "00000150220000300000002c010100040300000c0100000c"
                    + "800e00800300000803000002030000080400000200000008"
                    + "020000022800008800020000b4a2faf4bb54878ae21d6385"
                    + "12ece55d9236fc5046ab6cef82220f421f3ce6361faf3656"
                    + "4ecb6d28798a94aad7b2b4b603ddeaaa5630adb9ece8ac37"
                    + "534036040610ebdd92f46bef84f0be7db860351843858f8a"
                    + "cf87056e272377f70c9f2d81e29c7b0ce4f291a3a72476bb"
                    + "0b278fd4b7b0a4c26bbeb08214c707137607958729000024"
                    + "c39b7f368f4681b89fa9b7be6465abd7c5f68b6ed5d3b4c7"
                    + "2cb4240eb5c464122900001c00004004e54f73b7d83f6beb"
                    + "881eab2051d8663f421d10b02b00001c00004005d915368c"
                    + "a036004cb578ae3e3fb268509aeab1900000002069936922"
                    + "8741c6d4ca094c93e242c9de19e7b7c60000000500000500";

    private static final String LOCAL_SPI = "0000000000000000";
    private static final String REMOTE_SPI = "5f54bf6d8b48e6e1";

    private static final String DATA_ONE = "one 1";
    private static final String DATA_TWO = "two 2";

    private static final String IPV4_LOOPBACK = "127.0.0.1";

    private byte[] mDataOne;
    private byte[] mDataTwo;

    private long mLocalSpi;
    private long mRemoteSpi;

    private LongSparseArray mSpiToIkeStateMachineMap;
    private PacketReceiver mPacketReceiver;

    private UdpEncapsulationSocket mClientUdpEncapSocket;
    private InetAddress mLocalAddress;
    private FileDescriptor mDummyRemoteServerFd;

    private IkeSessionStateMachine mMockIkeSessionStateMachine;

    @Before
    public void setUp() throws Exception {
        Context context = InstrumentationRegistry.getContext();
        IpSecManager ipSecManager = (IpSecManager) context.getSystemService(Context.IPSEC_SERVICE);
        mClientUdpEncapSocket = ipSecManager.openUdpEncapsulationSocket();

        mLocalAddress = InetAddress.getByName(IPV4_LOOPBACK);
        mDummyRemoteServerFd = getBoundUdpSocket(mLocalAddress);

        mDataOne = DATA_ONE.getBytes("UTF-8");
        mDataTwo = DATA_TWO.getBytes("UTF-8");

        ByteBuffer localSpiBuffer = ByteBuffer.wrap(TestUtils.hexStringToByteArray(LOCAL_SPI));
        mLocalSpi = localSpiBuffer.getLong();
        ByteBuffer remoteSpiBuffer = ByteBuffer.wrap(TestUtils.hexStringToByteArray(REMOTE_SPI));
        mRemoteSpi = remoteSpiBuffer.getLong();

        mMockIkeSessionStateMachine = mock(IkeSessionStateMachine.class);

        mSpiToIkeStateMachineMap = new LongSparseArray<IkeSessionStateMachine>();
        mSpiToIkeStateMachineMap.put(mLocalSpi, mMockIkeSessionStateMachine);

        mPacketReceiver = new IkeSocket.PacketReceiver();
    }

    @After
    public void tearDown() throws Exception {
        mClientUdpEncapSocket.close();
        IkeSocket.setPacketReceiver(mPacketReceiver);
        Os.close(mDummyRemoteServerFd);
    }

    private static FileDescriptor getBoundUdpSocket(InetAddress address) throws Exception {
        FileDescriptor sock =
                Os.socket(OsConstants.AF_INET, OsConstants.SOCK_DGRAM, OsConstants.IPPROTO_UDP);
        Os.bind(sock, address, IkeSocket.IKE_SERVER_PORT);
        return sock;
    }

    @Test
    public void testGetAndCloseIkeSocket() throws Exception {
        if (Looper.myLooper() == null) Looper.myLooper().prepare();

        IkeSocket ikeSocketOne = IkeSocket.getIkeSocket(mClientUdpEncapSocket);
        assertEquals(1, ikeSocketOne.mRefCount);

        IkeSocket ikeSocketTwo = IkeSocket.getIkeSocket(mClientUdpEncapSocket);
        assertEquals(ikeSocketOne, ikeSocketTwo);
        assertEquals(2, ikeSocketTwo.mRefCount);

        ikeSocketOne.releaseReference();
        assertEquals(1, ikeSocketOne.mRefCount);

        ikeSocketTwo.releaseReference();
        assertEquals(0, ikeSocketTwo.mRefCount);
    }

    @Test
    public void testSendIkePacket() throws Exception {
        if (Looper.myLooper() == null) Looper.myLooper().prepare();

        // Send IKE packet
        IkeSocket ikeSocket = IkeSocket.getIkeSocket(mClientUdpEncapSocket);
        ikeSocket.sendIkePacket(mDataOne, mLocalAddress);

        byte[] receivedData = receive(mDummyRemoteServerFd);

        // Verify received data
        ByteBuffer expectedBuffer =
                ByteBuffer.allocate(IkeSocket.NON_ESP_MARKER_LEN + mDataOne.length);
        expectedBuffer.put(IkeSocket.NON_ESP_MARKER).put(mDataOne);

        assertArrayEquals(expectedBuffer.array(), receivedData);

        ikeSocket.releaseReference();
    }

    @Test
    public void testReceiveIkePacket() throws Exception {
        // Create working thread.
        HandlerThread mIkeThread = new HandlerThread("IkeSocketTest");
        mIkeThread.start();

        // Create IkeSocket on working thread.
        IkeSocketReceiver socketReceiver = new IkeSocketReceiver();
        TestCountDownLatch createLatch = new TestCountDownLatch();
        mIkeThread
                .getThreadHandler()
                .post(
                        () -> {
                            try {
                                socketReceiver.setIkeSocket(
                                        IkeSocket.getIkeSocket(mClientUdpEncapSocket));
                                createLatch.countDown();
                                Log.d("IkeSocketTest", "IkeSocket created.");
                            } catch (ErrnoException e) {
                                Log.e("IkeSocketTest", "error encountered creating IkeSocket ", e);
                            }
                        });
        createLatch.await();

        IkeSocket ikeSocket = socketReceiver.getIkeSocket();
        assertNotNull(ikeSocket);

        // Configure IkeSocket
        TestCountDownLatch receiveLatch = new TestCountDownLatch();
        DummyPacketReceiver packetReceiver = new DummyPacketReceiver(receiveLatch);
        IkeSocket.setPacketReceiver(packetReceiver);

        // Send first packet.
        sendToIkeSocket(mDummyRemoteServerFd, mDataOne, mLocalAddress);
        receiveLatch.await();

        assertEquals(1, ikeSocket.numPacketsReceived());
        assertArrayEquals(mDataOne, packetReceiver.mReceivedData);

        // Send second packet.
        sendToIkeSocket(mDummyRemoteServerFd, mDataTwo, mLocalAddress);
        receiveLatch.await();

        assertEquals(2, ikeSocket.numPacketsReceived());
        assertArrayEquals(mDataTwo, packetReceiver.mReceivedData);

        // Close IkeSocket.
        TestCountDownLatch closeLatch = new TestCountDownLatch();
        ikeSocket
                .getHandler()
                .post(
                        () -> {
                            ikeSocket.releaseReference();
                            closeLatch.countDown();
                        });
        closeLatch.await();

        mIkeThread.quitSafely();
    }

    @Test
    public void testHandlePacket() throws Exception {
        byte[] recvBuf =
                TestUtils.hexStringToByteArray(
                        NON_ESP_MARKER_HEX_STRING + IKE_REQ_MESSAGE_HEX_STRING);

        mPacketReceiver.handlePacket(recvBuf, mSpiToIkeStateMachineMap);

        byte[] expectedIkePacketBytes = TestUtils.hexStringToByteArray(IKE_REQ_MESSAGE_HEX_STRING);
        ArgumentCaptor<IkeHeader> ikeHeaderCaptor = ArgumentCaptor.forClass(IkeHeader.class);
        verify(mMockIkeSessionStateMachine)
                .receiveIkePacket(ikeHeaderCaptor.capture(), eq(expectedIkePacketBytes));

        IkeHeader capturedIkeHeader = ikeHeaderCaptor.getValue();
        assertEquals(mRemoteSpi, capturedIkeHeader.ikeInitiatorSpi);
        assertEquals(mLocalSpi, capturedIkeHeader.ikeResponderSpi);
    }

    @Test
    public void testHandleEspPacket() throws Exception {
        byte[] recvBuf =
                TestUtils.hexStringToByteArray(
                        NON_ESP_MARKER_HEX_STRING + IKE_REQ_MESSAGE_HEX_STRING);
        // Modify Non-ESP Marker
        recvBuf[0] = 1;

        mPacketReceiver.handlePacket(recvBuf, mSpiToIkeStateMachineMap);

        verify(mMockIkeSessionStateMachine, never()).receiveIkePacket(any(), any());
    }

    @Test
    public void testHandlePacketWithMalformedHeader() throws Exception {
        String malformedIkePacketHexString = "5f54bf6d8b48e6e100000000000000002120220800000000";
        byte[] recvBuf =
                TestUtils.hexStringToByteArray(
                        NON_ESP_MARKER_HEX_STRING + malformedIkePacketHexString);

        mPacketReceiver.handlePacket(recvBuf, mSpiToIkeStateMachineMap);

        verify(mMockIkeSessionStateMachine, never()).receiveIkePacket(any(), any());
    }

    private byte[] receive(FileDescriptor mfd) throws Exception {
        byte[] receiveBuffer = new byte[REMOTE_RECV_BUFF_SIZE];
        AtomicInteger bytesRead = new AtomicInteger(-1);
        Thread receiveThread =
                new Thread(
                        () -> {
                            while (bytesRead.get() < 0) {
                                try {
                                    bytesRead.set(
                                            Os.recvfrom(
                                                    mDummyRemoteServerFd,
                                                    receiveBuffer,
                                                    0,
                                                    REMOTE_RECV_BUFF_SIZE,
                                                    0,
                                                    null));
                                } catch (Exception e) {
                                    Log.e(
                                            "IkeSocketTest",
                                            "Error encountered reading from socket",
                                            e);
                                }
                            }
                            Log.d(
                                    "IkeSocketTest",
                                    "Packet received with size of " + bytesRead.get());
                        });

        receiveThread.start();
        receiveThread.join(TIMEOUT);

        return Arrays.copyOfRange(receiveBuffer, 0, bytesRead.get());
    }

    private void sendToIkeSocket(FileDescriptor fd, byte[] data, InetAddress destAddress)
            throws Exception {
        Os.sendto(fd, data, 0, data.length, 0, destAddress, mClientUdpEncapSocket.getPort());
    }

    private static class IkeSocketReceiver {
        private IkeSocket mIkeSocket;

        void setIkeSocket(IkeSocket ikeSocket) {
            mIkeSocket = ikeSocket;
        }

        IkeSocket getIkeSocket() {
            return mIkeSocket;
        }
    }

    private static class DummyPacketReceiver implements IkeSocket.IPacketReceiver {
        byte[] mReceivedData = null;
        final TestCountDownLatch mLatch;

        DummyPacketReceiver(TestCountDownLatch latch) {
            mLatch = latch;
        }

        public void handlePacket(
                byte[] revbuf, LongSparseArray<IkeSessionStateMachine> spiToIkeSession) {
            mReceivedData = Arrays.copyOfRange(revbuf, 0, revbuf.length);
            mLatch.countDown();
            Log.d("IkeSocketTest", "Packet received");
        }
    }

    private static class TestCountDownLatch {
        private CountDownLatch mLatch;

        TestCountDownLatch() {
            reset();
        }

        private void reset() {
            mLatch = new CountDownLatch(1);
        }

        void countDown() {
            mLatch.countDown();
        }

        void await() {
            try {
                if (!mLatch.await(TIMEOUT, TimeUnit.MILLISECONDS)) {
                    fail("Time out");
                }
            } catch (InterruptedException e) {
                fail(e.toString());
            }
            reset();
        }
    }
}
