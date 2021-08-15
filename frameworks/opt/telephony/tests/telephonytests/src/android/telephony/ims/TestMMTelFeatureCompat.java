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

import android.telephony.ims.compat.feature.MMTelFeature;

import com.android.ims.internal.IImsCallSession;
import com.android.ims.internal.IImsCallSessionListener;

/**
 * MMTelFeature implementation used by mockito to test functionality.
 */

public class TestMMTelFeatureCompat extends MMTelFeature {

    public boolean isConnectedCalled = false;
    public boolean isCreateCallSessionCalled = false;

    @Override
    public boolean isConnected(int callSessinType, int callType) {
        isConnectedCalled = true;
        return true;
    }

    @Override
    public void onFeatureReady() {
    }

    @Override
    public void onFeatureRemoved() {
    }

    public void sendSetFeatureState(int state) {
        setFeatureState(state);
    }

    // Compatibility Method
    @Override
    public IImsCallSession createCallSession(int sessionId, ImsCallProfile profile,
            IImsCallSessionListener listener) {
        isCreateCallSessionCalled = true;
        return null;
    }
}
