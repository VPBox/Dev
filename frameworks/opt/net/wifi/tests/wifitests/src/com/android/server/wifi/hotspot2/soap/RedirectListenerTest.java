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

package com.android.server.wifi.hotspot2.soap;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.os.Handler;
import android.os.Looper;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.io.IOException;
import java.net.URL;

import fi.iki.elonen.NanoHTTPD;

/**
 * Unit tests for {@link RedirectListener}.
 */
@SmallTest
public class RedirectListenerTest {
    private static final int TEST_PORT = 1010;

    private RedirectListenerSpy mRedirectListener;
    private URL mServerUrl;
    private TestLooper mLooper = new TestLooper();
    private Handler mHandler = new Handler(mLooper.getLooper());

    @Mock RedirectListener.RedirectCallback mListener;
    @Mock NanoHTTPD.IHTTPSession mIHTTPSession;

    /** Spy class to avoid start/stop {@link NanoHTTPD} server */
    private class RedirectListenerSpy extends RedirectListener {
        boolean mIsStart = false;
        RedirectListenerSpy(Looper looper, int port) throws IOException {
            super(looper, port);
        }

        @Override
        public void start() {
            mIsStart = true;
        }

        @Override
        public void stop() {
            mIsStart = false;
        }

        @Override
        public boolean isServerAlive() {
            return mIsStart;
        }
    }

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mRedirectListener = new RedirectListenerSpy(mLooper.getLooper(), TEST_PORT);
        mServerUrl = mRedirectListener.getServerUrl();
    }

    private void verifyStartServer() {
        mRedirectListener.startServer(mListener, mHandler);
        mLooper.dispatchAll();

        assertTrue(mRedirectListener.mIsStart);
    }

    private void verifyStopServer() {
        mRedirectListener.stopServer(mHandler);
        mLooper.dispatchAll();

        assertFalse(mRedirectListener.mIsStart);

        // true if there are pending messages in the message queue
        assertFalse(mLooper.isIdle());
    }

    /**
     * Verifies that Timeout handler will be invoked when There is no a known GET request received
     * in a {@link RedirectListener#USER_TIMEOUT_MILLIS}.
     */
    @Test
    public void timeOutForKnownGetRequest() {
        when(mIHTTPSession.getMethod()).thenReturn(NanoHTTPD.Method.PUT);
        verifyStartServer();
        mRedirectListener.serve(mIHTTPSession);

        verify(mListener, never()).onRedirectReceived();

        // Timeout has expired.
        mLooper.moveTimeForward(RedirectListener.USER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();

        verify(mListener).onRedirectTimedOut();
        verifyStopServer();
    }

    /**
     * Verifies that {@link RedirectListener.RedirectCallback#onRedirectReceived()} will not be
     * invoked when receiving a GET request with an unexpected path.
     */
    @Test
    public void receiveUnknownGetRequest() {
        when(mIHTTPSession.getMethod()).thenReturn(NanoHTTPD.Method.GET);
        when(mIHTTPSession.getUri()).thenReturn("/test");
        verifyStartServer();

        mRedirectListener.serve(mIHTTPSession);

        verify(mListener, never()).onRedirectReceived();

        // Timeout has expired.
        mLooper.moveTimeForward(RedirectListener.USER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();

        verify(mListener).onRedirectTimedOut();
        verifyStopServer();
    }

    /**
     * Verifies that a {@link RedirectListener.RedirectCallback#onRedirectReceived()} callback will
     * be invoked when receiving a GET request with an expected path.
     */
    @Test
    public void receiveKnownGetRequest() {
        when(mIHTTPSession.getMethod()).thenReturn(NanoHTTPD.Method.GET);
        when(mIHTTPSession.getUri()).thenReturn(mServerUrl.getPath());
        verifyStartServer();

        mRedirectListener.serve(mIHTTPSession);

        verify(mListener).onRedirectReceived();

        mLooper.moveTimeForward(RedirectListener.USER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();

        // TimeoutTask is cancelled once receiving HTTP redirect response.
        verify(mListener, never()).onRedirectTimedOut();
        verifyStopServer();
    }
}








