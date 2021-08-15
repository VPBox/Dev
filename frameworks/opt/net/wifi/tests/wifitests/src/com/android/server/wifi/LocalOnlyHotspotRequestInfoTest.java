/*
 * Copyright (C) 2017 The Android Open Source Project
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

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.Process;
import android.os.RemoteException;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/*
 * Unit tests for {@link com.android.server.wifi.LocalOnlyHotspotRequestInfo}.
 */
@SmallTest
public class LocalOnlyHotspotRequestInfoTest {

    private static final String TAG = "LocalOnlyHotspotRequestInfoTest";
    @Mock IBinder mAppBinder;
    @Mock LocalOnlyHotspotRequestInfo.RequestingApplicationDeathCallback mCallback;
    private Handler mHandler;
    private Messenger mMessenger;
    private TestLooper mTestLooper;
    RemoteException mRemoteException;
    private LocalOnlyHotspotRequestInfo mLOHSRequestInfo;

    /**
     * Setup test.
     */
    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mTestLooper = new TestLooper();
        mHandler = new Handler(mTestLooper.getLooper());
        mMessenger = new Messenger(mHandler);
        mRemoteException = new RemoteException("Test Remote Exception");
    }

    /**
     * Make sure we link the call to request LocalOnlyHotspot by an app is linked to their Binder
     * call.  This allows us to clean up if the app dies.
     */
    @Test
    public void verifyBinderLinkToDeathIsCalled() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        verify(mAppBinder).linkToDeath(eq(mLOHSRequestInfo), eq(0));
    }

    /**
     * Calls to create the requestor to binder death should not pass null callback.
     */
    @Test(expected = NullPointerException.class)
    public void verifyNullCallbackChecked() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, null);
    }

    /**
     * Calls to create the request info object should not pass a null messenger.
     */
    @Test(expected = NullPointerException.class)
    public void verifyNullMessengerChecked() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, null, mCallback);
    }

    /**
     * Calls to link the requestor to binder death should not pass null binder
     */
    @Test(expected = NullPointerException.class)
    public void verifyNullBinderChecked() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(null, mMessenger, mCallback);
    }

    /**
     * Calls to unlink the DeathRecipient should call to unlink from Binder.
     */
    @Test
    public void verifyUnlinkDeathRecipientUnlinksFromBinder() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        mLOHSRequestInfo.unlinkDeathRecipient();
        verify(mAppBinder).unlinkToDeath(eq(mLOHSRequestInfo), eq(0));
    }

    /**
     * Binder death notification should trigger a callback on the requestor.
     */
    @Test
    public void verifyBinderDeathTriggersCallback() {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        mLOHSRequestInfo.binderDied();
        verify(mCallback).onLocalOnlyHotspotRequestorDeath(eq(mLOHSRequestInfo));
    }

    /**
     * Verify a RemoteException when calling linkToDeath triggers the callback.
     */
    @Test
    public void verifyRemoteExceptionTriggersCallback() throws Exception {
        doThrow(mRemoteException).when(mAppBinder)
                .linkToDeath(any(IBinder.DeathRecipient.class), eq(0));
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        verify(mCallback).onLocalOnlyHotspotRequestorDeath(eq(mLOHSRequestInfo));
    }

    /**
     * Verify the pid is properly set.
     */
    @Test
    public void verifyPid() {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        assertEquals(Process.myPid(), mLOHSRequestInfo.getPid());
    }

    /**
     * Verify that sendHotspotFailedMessage does send a Message properly
     */
    @Test
    public void verifySendFailedMessage() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        mLOHSRequestInfo.sendHotspotFailedMessage(
                WifiManager.LocalOnlyHotspotCallback.ERROR_GENERIC);
        Message message = mTestLooper.nextMessage();
        assertEquals(WifiManager.HOTSPOT_FAILED, message.what);
        assertEquals(WifiManager.LocalOnlyHotspotCallback.ERROR_GENERIC, message.arg1);
    }

    /**
     * Verify that sendHotspotStartedMessage does send a Message properly
     */
    @Test
    public void verifySendStartedMessage() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        WifiConfiguration config = mock(WifiConfiguration.class);
        mLOHSRequestInfo.sendHotspotStartedMessage(config);
        Message message = mTestLooper.nextMessage();
        assertEquals(WifiManager.HOTSPOT_STARTED, message.what);
        assertEquals(config, (WifiConfiguration) message.obj);
    }

    /**
     * Verify that sendHotspotStoppedMessage does send a Message properly
     */
    @Test
    public void verifySendStoppedMessage() throws Exception {
        mLOHSRequestInfo = new LocalOnlyHotspotRequestInfo(mAppBinder, mMessenger, mCallback);
        mLOHSRequestInfo.sendHotspotStoppedMessage();
        Message message = mTestLooper.nextMessage();
        assertEquals(WifiManager.HOTSPOT_STOPPED, message.what);
    }
}
