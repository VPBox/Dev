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

package com.android.server.wifi.util;

import static org.junit.Assert.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.verify;

import android.net.wifi.ISoftApCallback;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.util.ExternalCallbackTracker}.
 */
@SmallTest
public class ExternalCallbackTrackerTest {
    private static final int TEST_CALLBACK_IDENTIFIER = 56;
    @Mock Handler mHandler;
    @Mock ISoftApCallback mCallback;
    @Mock IBinder mBinder;
    private TestLooper mTestLooper;

    private ExternalCallbackTracker<ISoftApCallback> mExternalCallbackTracker;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mTestLooper = new TestLooper();
        mHandler = new Handler(mTestLooper.getLooper());
        mExternalCallbackTracker = new ExternalCallbackTracker<ISoftApCallback>(mHandler);
    }

    /**
     * Test adding a callback.
     */
    @Test
    public void testAddCallback() throws Exception {
        assertTrue(mExternalCallbackTracker.add(mBinder, mCallback, TEST_CALLBACK_IDENTIFIER));
        assertEquals(1, mExternalCallbackTracker.getNumCallbacks());
        assertEquals(mCallback, mExternalCallbackTracker.getCallbacks().get(0));
        verify(mBinder).linkToDeath(any(), anyInt());
    }

    /**
     * Test that adding a callback returns failure when binder death linking fails.
     */
    @Test
    public void testAddCallbackFailureOnLinkToDeath() throws Exception {
        doThrow(new RemoteException()).when(mBinder).linkToDeath(any(), anyInt());
        assertFalse(mExternalCallbackTracker.add(mBinder, mCallback, TEST_CALLBACK_IDENTIFIER));
        assertEquals(0, mExternalCallbackTracker.getNumCallbacks());
        assertTrue(mExternalCallbackTracker.getCallbacks().isEmpty());
    }

    /**
     * Test removing a callback.
     */
    @Test
    public void testRemoveCallback() throws Exception {
        testAddCallback();

        assertTrue(mExternalCallbackTracker.remove(TEST_CALLBACK_IDENTIFIER));
        assertEquals(0, mExternalCallbackTracker.getNumCallbacks());
        assertTrue(mExternalCallbackTracker.getCallbacks().isEmpty());
        verify(mBinder).unlinkToDeath(any(), anyInt());
    }

    /**
     * Test removing a callback returns failure when the identifier provided doesn't match the one
     * used to add the callback.
     */
    @Test
    public void testRemoveCallbackFailureOnWrongIdentifier() throws Exception {
        testAddCallback();

        assertFalse(mExternalCallbackTracker.remove(TEST_CALLBACK_IDENTIFIER + 5));
        assertEquals(1, mExternalCallbackTracker.getNumCallbacks());
        assertEquals(mCallback, mExternalCallbackTracker.getCallbacks().get(0));
    }

    /**
     * Test that the callback is automatically removed when the associated binder object is dead.
     */
    @Test
    public void testCallbackRemovalOnDeath() throws Exception {
        assertTrue(mExternalCallbackTracker.add(mBinder, mCallback, TEST_CALLBACK_IDENTIFIER));

        // Trigger the death.
        ArgumentCaptor<IBinder.DeathRecipient> deathCaptor =
                ArgumentCaptor.forClass(IBinder.DeathRecipient.class);
        verify(mBinder).linkToDeath(deathCaptor.capture(), anyInt());
        deathCaptor.getValue().binderDied();
        mTestLooper.dispatchAll();

        assertEquals(0, mExternalCallbackTracker.getNumCallbacks());
        assertTrue(mExternalCallbackTracker.getCallbacks().isEmpty());
        verify(mBinder).unlinkToDeath(any(), anyInt());
    }
}
