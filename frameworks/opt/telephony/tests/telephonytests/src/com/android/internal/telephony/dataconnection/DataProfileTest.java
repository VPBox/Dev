/*
 * Copyright (C) 2010 The Android Open Source Project
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

package com.android.internal.telephony.dataconnection;

import android.telephony.data.ApnSetting;
import android.telephony.data.DataProfile;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.RILConstants;

import junit.framework.TestCase;

public class DataProfileTest extends TestCase {

    private ApnSetting mApn1 = ApnSetting.makeApnSetting(
            2163,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "fake_apn",             // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "user",                 // user
            "passwd",               // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IPV6,                 // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            1234,                   // profile_id
            false,                  // modem_cognitive
            321,                    // max_conns
            456,                    // wait_time
            789,                    // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn2 = ApnSetting.makeApnSetting(
            2163,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "fake_apn",             // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "user",                 // user
            "passwd",               // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IP,                 // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            1234,                   // profile_id
            false,                  // modem_cognitive
            111,                    // max_conns
            456,                    // wait_time
            789,                    // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn3 = ApnSetting.makeApnSetting(
            2163,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "fake_apn",             // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "user",                 // user
            "passwd",               // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IP,                 // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            276600,                      // networktype_bitmask
            1234,                   // profile_id
            false,                  // modem_cognitive
            111,                    // max_conns
            456,                    // wait_time
            789,                    // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    @SmallTest
    public void testCreateFromApnSetting() throws Exception {
        DataProfile dp = DcTracker.createDataProfile(mApn1, mApn1.getProfileId(), false);
        assertEquals(mApn1.getProfileId(), dp.getProfileId());
        assertEquals(mApn1.getApnName(), dp.getApn());
        assertEquals(mApn1.getProtocol(), dp.getProtocolType());
        assertEquals(RILConstants.SETUP_DATA_AUTH_PAP_CHAP, dp.getAuthType());
        assertEquals(mApn1.getUser(), dp.getUserName());
        assertEquals(mApn1.getPassword(), dp.getPassword());
        assertEquals(0, dp.getType());  // TYPE_COMMON
        assertEquals(mApn1.getWaitTime(), dp.getWaitTime());
        assertEquals(mApn1.isEnabled(), dp.isEnabled());
        assertFalse(dp.isPersistent());
        assertFalse(dp.isPreferred());
    }

    @SmallTest
    public void testCreateFromApnSettingWithNetworkTypeBitmask() throws Exception {
        DataProfile dp = DcTracker.createDataProfile(mApn3, mApn3.getProfileId(), false);
        assertEquals(mApn3.getProfileId(), dp.getProfileId());
        assertEquals(mApn3.getApnName(), dp.getApn());
        assertEquals(mApn3.getProtocol(), dp.getProtocolType());
        assertEquals(RILConstants.SETUP_DATA_AUTH_PAP_CHAP, dp.getAuthType());
        assertEquals(mApn3.getUser(), dp.getUserName());
        assertEquals(mApn3.getPassword(), dp.getPassword());
        assertEquals(2, dp.getType());  // TYPE_3GPP2
        assertEquals(mApn3.getWaitTime(), dp.getWaitTime());
        assertEquals(mApn3.isEnabled(), dp.isEnabled());
        int expectedBearerBitmap = mApn3.getNetworkTypeBitmask();
        assertEquals(expectedBearerBitmap, dp.getBearerBitmask());
    }

    @SmallTest
    public void testEquals() throws Exception {
        DataProfile dp1 = DcTracker.createDataProfile(mApn1, mApn1.getProfileId(), false);
        DataProfile dp2 = DcTracker.createDataProfile(mApn1, mApn1.getProfileId(), false);
        assertEquals(dp1, dp2);

        dp2 = DcTracker.createDataProfile(mApn2, mApn2.getProfileId(), false);
        assertFalse(dp1.equals(dp2));
    }
}
