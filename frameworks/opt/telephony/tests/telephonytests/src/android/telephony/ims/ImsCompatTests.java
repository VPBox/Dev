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

import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

import android.telephony.ims.aidl.IImsCallSessionListener;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import com.android.ims.internal.IImsMMTelFeature;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.MockitoAnnotations;

@RunWith(AndroidJUnit4.class)
public class ImsCompatTests {

    TestMMTelFeatureCompat mCompatMmTel;
    IImsMMTelFeature mCompatMmTelBinder;

    TestImsCallSessionCompat mCompatCallSession;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mCompatMmTel = new TestMMTelFeatureCompat();
        mCompatMmTelBinder = mCompatMmTel.getBinder();

        mCompatCallSession = new TestImsCallSessionCompat();
    }

    @After
    public void tearDown() {
        mCompatMmTel = null;
        mCompatMmTelBinder = null;

        mCompatCallSession = null;
    }

    @SmallTest
    @Test
    public void testCreateCallSessionCompat() throws Exception {
        mCompatMmTelBinder.createCallSession(0, new ImsCallProfile());
        assertTrue(mCompatMmTel.isCreateCallSessionCalled);
    }

    @SmallTest
    @Test
    public void testListenerCompatLayer() throws Exception {
        IImsCallSessionListener listener = mock(IImsCallSessionListener.class);
        ImsReasonInfo info = new ImsReasonInfo();
        mCompatCallSession.setListener(listener);

        mCompatCallSession.mListener.callSessionTerminated(null, info);

        verify(listener).callSessionTerminated(eq(info));
    }
}
