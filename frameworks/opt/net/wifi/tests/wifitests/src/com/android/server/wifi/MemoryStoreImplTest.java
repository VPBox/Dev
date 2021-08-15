/*
 * Copyright 2019 The Android Open Source Project
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

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.net.IpMemoryStore;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

//import java.util.HashMap;
//import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.MemoryStoreImpl}.
 */
@SmallTest
public class MemoryStoreImplTest {
    @Mock Context mContext;
    @Mock WifiScoreCard mWifiScoreCard;
    @Mock WifiScoreCard.BlobListener mBlobListener;
    @Mock WifiInjector mWifiInjector;
    @Mock IpMemoryStore mIpMemoryStore;
    private MemoryStoreImpl mMemoryStoreImpl;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mMemoryStoreImpl = new MemoryStoreImpl(mContext, mWifiInjector, mWifiScoreCard);
    }

    /**
     * Attempt to start before ip memory service is ready should not call install.
     */
    @Test
    public void attemptToStartBeforeIpMemoryServiceIsReadyShouldNotCallInstall() throws Exception {
        mMemoryStoreImpl.start();
        verify(mWifiScoreCard, never()).installMemoryStore(any());
    }

    /**
     * Test start installs itself.
     */
    @Test
    public void testStartInstallsItself() throws Exception {
        when(mWifiInjector.getIpMemoryStore()).thenReturn(mIpMemoryStore);
        mMemoryStoreImpl.start();
        verify(mWifiScoreCard).installMemoryStore(eq(mMemoryStoreImpl));
    }

    /**
     * Test that stop does pending writes.
     *
     * Also check that stop is harmless if done before start or if done twice.
     */
    @Test
    public void testThatStopDoesPendingWrites() throws Exception {
        when(mWifiInjector.getIpMemoryStore()).thenReturn(mIpMemoryStore);

        // stop without start is a NOP
        mMemoryStoreImpl.stop();
        verifyNoMoreInteractions(mWifiScoreCard);

        // start / stop calls for pending writes
        mMemoryStoreImpl.start();
        mMemoryStoreImpl.stop();
        verify(mWifiScoreCard).installMemoryStore(eq(mMemoryStoreImpl));
        verify(mWifiScoreCard).doWrites();
        verifyNoMoreInteractions(mWifiScoreCard);

        // stopping again should do nothing more
        mMemoryStoreImpl.stop();
        verifyNoMoreInteractions(mWifiScoreCard);
    }

    /**
     * WifiScoreCard read should call ip memory store retrieveBlob.
     */
    @Test
    public void wifiScoreCardReadShouldCallIpMemoryStoreRetrieveBlob() throws Exception {
        final byte[] myBlob = new byte[]{0x0, 0x3};
        final String myL2Key = "L2Key:" + myBlob;
        final android.net.ipmemorystore.Status statusSuccess =
                new android.net.ipmemorystore.Status(android.net.ipmemorystore.Status.SUCCESS);

        when(mWifiInjector.getIpMemoryStore()).thenReturn(mIpMemoryStore);
        mMemoryStoreImpl.start();
        mMemoryStoreImpl.read(myL2Key, mBlobListener);
        verify(mIpMemoryStore).retrieveBlob(
                eq(myL2Key),                                                  // String l2Key
                eq(MemoryStoreImpl.WIFI_FRAMEWORK_IP_MEMORY_STORE_CLIENT_ID), // String clientId
                eq(MemoryStoreImpl.WIFI_FRAMEWORK_IP_MEMORY_STORE_DATA_NAME), // String name
                mOnBlobRetrievedListenerCaptor.capture());
        verifyNoMoreInteractions(mIpMemoryStore, mBlobListener);
        // Now simulate the completion of the read request
        final android.net.ipmemorystore.Blob wrappedBlob = new android.net.ipmemorystore.Blob();
        wrappedBlob.data = myBlob;
        mOnBlobRetrievedListenerCaptor.getValue()
                .onBlobRetrieved(
                        statusSuccess,
                        myL2Key,
                        MemoryStoreImpl.WIFI_FRAMEWORK_IP_MEMORY_STORE_DATA_NAME,
                        wrappedBlob);
        verify(mBlobListener).onBlobRetrieved(mBytesCaptor.capture());
        assertArrayEquals(myBlob, mBytesCaptor.getValue());
    }
    final ArgumentCaptor<android.net.ipmemorystore.OnBlobRetrievedListener>
            mOnBlobRetrievedListenerCaptor =
            ArgumentCaptor.forClass(android.net.ipmemorystore.OnBlobRetrievedListener.class);
    final ArgumentCaptor<byte[]>
            mBytesCaptor =
            ArgumentCaptor.forClass(byte[].class);

    /**
     * WifiScoreCard write should call ip memory store storeBlob.
     */
    @Test
    public void wifiScoreCardWriteShouldCallIpMemoryStoreStoreBlob() throws Exception {
        final byte[] myBlob = new byte[]{0x0, 0x3, 0x1};
        final String myL2Key = "L2Key:" + myBlob;
        when(mWifiInjector.getIpMemoryStore()).thenReturn(mIpMemoryStore);
        mMemoryStoreImpl.start();
        mMemoryStoreImpl.write(myL2Key, myBlob);
        verify(mIpMemoryStore).storeBlob(
                eq(myL2Key),
                eq(MemoryStoreImpl.WIFI_FRAMEWORK_IP_MEMORY_STORE_CLIENT_ID),
                eq(MemoryStoreImpl.WIFI_FRAMEWORK_IP_MEMORY_STORE_DATA_NAME),
                mIpMemoryStoreBlobCaptor.capture(),
                eq(null));
        android.net.ipmemorystore.Blob blob = mIpMemoryStoreBlobCaptor.getValue();
        assertArrayEquals(myBlob, blob.data);
        verifyNoMoreInteractions(mIpMemoryStore);
    }
    final ArgumentCaptor<android.net.ipmemorystore.Blob> mIpMemoryStoreBlobCaptor =
            ArgumentCaptor.forClass(android.net.ipmemorystore.Blob.class);

    /**
     * An exception should disable further operations.
     */
    @Test
    public void exceptionDisablesFurtherOperations() throws Exception {
        final byte[] myBlob = new byte[]{0x0, 0x3, 0x1};
        final String myL2Key = "L2Key:" + myBlob;
        when(mWifiInjector.getIpMemoryStore()).thenReturn(mIpMemoryStore);
        doThrow(new RuntimeException("Just a test"))
                .when(mIpMemoryStore).storeBlob(any(), any(), any(), any(), any());
        mMemoryStoreImpl.start();
        mMemoryStoreImpl.write(myL2Key, myBlob);
        // After the failed write, the read should do nothing.
        mMemoryStoreImpl.read(myL2Key, mBlobListener);
        verify(mIpMemoryStore, never())
                .retrieveBlob(any(), any(), any(), any());
    }
}
