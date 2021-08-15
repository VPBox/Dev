/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.internal.telephony.mocks;

import android.os.Handler;
import android.os.Registrant;
import android.os.RegistrantList;

import com.android.internal.annotations.VisibleForTesting;
import com.android.internal.telephony.SubscriptionMonitor;

public class SubscriptionMonitorMock extends SubscriptionMonitor {
    private final int mNumPhones;
    private final RegistrantList mSubscriptionsChangedRegistrants[];
    private final RegistrantList mDefaultSubscriptionRegistrants[];

    public SubscriptionMonitorMock(int numPhones) {
        super();
        mNumPhones = numPhones;
        mSubscriptionsChangedRegistrants = new RegistrantList[numPhones];
        mDefaultSubscriptionRegistrants = new RegistrantList[numPhones];

        for (int i = 0; i < numPhones; i++) {
            mSubscriptionsChangedRegistrants[i] = new RegistrantList();
            mDefaultSubscriptionRegistrants[i] = new RegistrantList();
        }
    }

    @Override
    public void registerForSubscriptionChanged(int phoneId, Handler h, int what, Object o) {
        validatePhoneId(phoneId);
        Registrant r = new Registrant(h, what, o);
        mSubscriptionsChangedRegistrants[phoneId].add(r);
        r.notifyRegistrant();
    }

    @Override
    public void unregisterForSubscriptionChanged(int phoneId, Handler h) {
        validatePhoneId(phoneId);
        mSubscriptionsChangedRegistrants[phoneId].remove(h);
    }

    @Override
    public void registerForDefaultDataSubscriptionChanged(int phoneId, Handler h, int what,
            Object o) {
        validatePhoneId(phoneId);
        Registrant r = new Registrant(h, what, o);
        mDefaultSubscriptionRegistrants[phoneId].add(r);
        r.notifyRegistrant();
    }

    @Override
    public void unregisterForDefaultDataSubscriptionChanged(int phoneId, Handler h) {
        validatePhoneId(phoneId);
        mDefaultSubscriptionRegistrants[phoneId].remove(h);
    }

    @VisibleForTesting
    public void notifySubscriptionChanged(int phoneId) {
        validatePhoneId(phoneId);
        mSubscriptionsChangedRegistrants[phoneId].notifyRegistrants();
    }

    @VisibleForTesting
    public void notifyDefaultSubscriptionChanged(int phoneId) {
        validatePhoneId(phoneId);
        mDefaultSubscriptionRegistrants[phoneId].notifyRegistrants();
    }

    private void validatePhoneId(int phoneId) {
        if (phoneId < 0 || phoneId >= mNumPhones) {
            throw new IllegalArgumentException("Invalid PhoneId");
        }
    }
}
