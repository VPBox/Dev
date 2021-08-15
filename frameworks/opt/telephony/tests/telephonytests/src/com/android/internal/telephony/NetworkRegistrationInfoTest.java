/*
 * Copyright 2018 The Android Open Source Project
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

package com.android.internal.telephony;

import static junit.framework.Assert.assertEquals;

import android.os.Parcel;
import android.telephony.AccessNetworkConstants;
import android.telephony.CellIdentityLte;
import android.telephony.NetworkRegistrationInfo;
import android.telephony.TelephonyManager;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.util.Arrays;

/** Unit tests for {@link NetworkRegistrationInfo}. */
public class NetworkRegistrationInfoTest {

    @Test
    @SmallTest
    public void testParcel() {
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setAvailableServices(Arrays.asList(NetworkRegistrationInfo.SERVICE_TYPE_DATA))
                .setCellIdentity(new CellIdentityLte())
                .build();

        Parcel p = Parcel.obtain();
        nri.writeToParcel(p, 0);
        p.setDataPosition(0);

        NetworkRegistrationInfo newNrs = NetworkRegistrationInfo.CREATOR.createFromParcel(p);
        assertEquals(nri, newNrs);
    }
}
