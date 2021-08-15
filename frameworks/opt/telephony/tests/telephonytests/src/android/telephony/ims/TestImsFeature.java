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

import android.os.IInterface;
import android.telephony.ims.feature.CapabilityChangeRequest;
import android.telephony.ims.feature.ImsFeature;

public class TestImsFeature extends ImsFeature {


    public static final int CAPABILITY_TEST_1 = 1 << 0;
    public static final int CAPABILITY_TEST_2 = 1 << 1;

    public int setCapabilitiesResult = ImsFeature.CAPABILITY_SUCCESS;
    public CapabilityChangeRequest lastRequest;
    public int onFeatureRemovedCount = 0;
    public int onFeatureReadyCount = 0;

    public void testSetFeatureState(int featureState) {
        setFeatureState(featureState);
    }

    public void capabilitiesStatusChanged(Capabilities c) {
        notifyCapabilitiesStatusChanged(c);
    }

    @Override
    public void changeEnabledCapabilities(CapabilityChangeRequest request,
            CapabilityCallbackProxy c) {
        lastRequest = request;
        if (setCapabilitiesResult != ImsFeature.CAPABILITY_SUCCESS) {
            // Take the first value to enable and return it as an error.
            CapabilityChangeRequest.CapabilityPair capPair = request.getCapabilitiesToEnable()
                    .get(0);
            c.onChangeCapabilityConfigurationError(capPair.getCapability(), capPair.getRadioTech(),
                    ImsFeature.CAPABILITY_ERROR_GENERIC);
        }
    }

    @Override
    public void onFeatureRemoved() {
        onFeatureRemovedCount++;
    }

    @Override
    public void onFeatureReady() {
        onFeatureReadyCount++;
    }

    @Override
    public IInterface getBinder() {
        return null;
    }
}
