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

package com.android.internal.telephony.ims;

import android.os.RemoteException;
import android.telephony.ims.aidl.IImsConfig;
import android.telephony.ims.aidl.IImsMmTelFeature;
import android.telephony.ims.aidl.IImsRcsFeature;
import android.telephony.ims.aidl.IImsRegistration;
import android.telephony.ims.aidl.IImsServiceController;
import android.telephony.ims.aidl.IImsServiceControllerListener;
import android.telephony.ims.stub.ImsConfigImplBase;
import android.telephony.ims.stub.ImsFeatureConfiguration;
import android.telephony.ims.stub.ImsRegistrationImplBase;

import com.android.ims.internal.IImsFeatureStatusCallback;

import static org.mockito.Mockito.spy;

/**
 * Test base implementation of the ImsServiceController, which is used as a mockito spy.
 */

public class TestImsServiceControllerAdapter {

    public IImsFeatureStatusCallback mStatusCallback;

    public class ImsServiceControllerBinder extends IImsServiceController.Stub {

        @Override
        public void setListener(IImsServiceControllerListener l) {
        }

        @Override
        public IImsMmTelFeature createMmTelFeature(int slotId, IImsFeatureStatusCallback c) {
            return TestImsServiceControllerAdapter.this.createMMTelFeature(slotId);
        }

        @Override
        public IImsRcsFeature createRcsFeature(int slotId, IImsFeatureStatusCallback c) {
            return TestImsServiceControllerAdapter.this.createRcsFeature(slotId);
        }

        @Override
        public void removeImsFeature(int slotId, int featureType, IImsFeatureStatusCallback c)
                throws RemoteException {
            TestImsServiceControllerAdapter.this.removeImsFeature(slotId, featureType);
        }

        @Override
        public ImsFeatureConfiguration querySupportedImsFeatures() {
            return null;
        }

        @Override
        public void notifyImsServiceReadyForFeatureCreation() {
        }

        @Override
        public IImsConfig getConfig(int slotId) throws RemoteException {
            return new ImsConfigImplBase().getIImsConfig();
        }

        @Override
        public IImsRegistration getRegistration(int slotId) throws RemoteException {
            return new ImsRegistrationImplBase().getBinder();
        }

        @Override
        public void enableIms(int slotId) {
        }

        @Override
        public void disableIms(int slotId) {

        }

    }

    private ImsServiceControllerBinder mBinder;

    public IImsServiceController getBinder() {
        if (mBinder == null) {
            mBinder = spy(new ImsServiceControllerBinder());
        }

        return mBinder;
    }

    // Used by Mockito for verification that this method is being called in spy
    public IImsMmTelFeature createMMTelFeature(int slotId) {
        return null;
    }

    // Used by Mockito for verification that this method is being called in spy
    public IImsRcsFeature createRcsFeature(int slotId) {
        return null;
    }

    // Used by Mockito for verification that this method is being called in spy
    public void removeImsFeature(int subId, int feature) throws RemoteException {
    }
}
