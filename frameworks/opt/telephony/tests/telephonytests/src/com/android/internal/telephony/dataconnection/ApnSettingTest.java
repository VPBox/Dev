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

import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;
import static junit.framework.Assert.fail;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.doReturn;

import android.net.Uri;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.telephony.data.ApnSetting;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;

public class ApnSettingTest extends TelephonyTest {

    private PersistableBundle mBundle;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mBundle = mContextFixture.getCarrierConfigBundle();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    static ApnSetting createApnSetting(int apnTypesBitmask) {
        return createApnSettingInternal(apnTypesBitmask, true);
    }

    private static ApnSetting createDisabledApnSetting(int apnTypesBitmask) {
        return createApnSettingInternal(apnTypesBitmask, false);
    }

    private static ApnSetting createApnSettingInternal(int apnTypeBitmask, boolean carrierEnabled) {
        return ApnSetting.makeApnSetting(
                2163,                   // id
                "44010",                // numeric
                "sp-mode",              // name
                "spmode.ne.jp",         // apn
                null,                     // proxy
                -1,                     // port
                null,                     // mmsc
                null,                     // mmsproxy
                -1,                     // mmsport
                "",                     // user
                "",                     // password
                -1,                     // authtype
                apnTypeBitmask,               // types
                ApnSetting.PROTOCOL_IP,                   // protocol
                ApnSetting.PROTOCOL_IP,                   // roaming_protocol
                carrierEnabled,         // carrier_enabled
                0,                      // networktype_bitmask
                0,                      // profile_id
                false,                  // modem_cognitive
                0,                      // max_conns
                0,                      // wait_time
                0,                      // max_conns_time
                0,                      // mtu
                -1,                     // mvno_type
                "");                    // mnvo_match_data
    }

    private static void assertApnSettingsEqual(List<ApnSetting> a1, List<ApnSetting> a2) {
        assertEquals(a1.size(), a2.size());
        for (int i = 0; i < a1.size(); ++i) {
            assertApnSettingEqual(a1.get(i), a2.get(i));
        }
    }

    private static void assertApnSettingEqual(ApnSetting a1, ApnSetting a2) {
        assertEquals(a1.getEntryName(), a2.getEntryName());
        assertEquals(a1.getApnName(), a2.getApnName());
        assertEquals(a1.getProxyAddressAsString(), a2.getProxyAddressAsString());
        assertEquals(a1.getProxyPort(), a2.getProxyPort());
        assertEquals(a1.getMmsc(), a2.getMmsc());
        assertEquals(a1.getMmsProxyAddressAsString(), a2.getMmsProxyAddressAsString());
        assertEquals(a1.getMmsProxyPort(), a2.getMmsProxyPort());
        assertEquals(a1.getUser(), a2.getUser());
        assertEquals(a1.getPassword(), a2.getPassword());
        assertEquals(a1.getAuthType(), a2.getAuthType());
        assertEquals(a1.getId(), a2.getId());
        assertEquals(a1.getOperatorNumeric(), a2.getOperatorNumeric());
        assertEquals(a1.getProtocol(), a2.getProtocol());
        assertEquals(a1.getRoamingProtocol(), a2.getRoamingProtocol());
        assertEquals(a1.getApnTypeBitmask(), a2.getApnTypeBitmask());
        assertEquals(a1.isEnabled(), a2.isEnabled());
        assertEquals(a1.getProfileId(), a2.getProfileId());
        assertEquals(a1.isPersistent(), a2.isPersistent());
        assertEquals(a1.getMaxConns(), a2.getMaxConns());
        assertEquals(a1.getWaitTime(), a2.getWaitTime());
        assertEquals(a1.getMaxConnsTime(), a2.getMaxConnsTime());
        assertEquals(a1.getMtu(), a2.getMtu());
        assertEquals(a1.getMvnoType(), a2.getMvnoType());
        assertEquals(a1.getMvnoMatchData(), a2.getMvnoMatchData());
        assertEquals(a1.getNetworkTypeBitmask(), a2.getNetworkTypeBitmask());
        assertEquals(a1.getApnSetId(), a2.getApnSetId());
        assertEquals(a1.getSkip464Xlat(), a2.getSkip464Xlat());
    }

    @Test
    @SmallTest
    public void testFromString() throws Exception {
        final int dunTypesBitmask = ApnSetting.TYPE_DUN;
        final int mmsTypesBitmask = ApnSetting.TYPE_MMS | ApnSetting.TYPE_ALL;

        ApnSetting expectedApn;
        String testString;

        // A real-world v1 example string.
        testString = "Vodafone IT,web.omnitel.it,,,,,,,,,222,10,,DUN";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "22210", "Vodafone IT", "web.omnitel.it", "", -1, null, "", -1, "", "", 0,
                dunTypesBitmask, ApnSetting.PROTOCOL_IP, ApnSetting.PROTOCOL_IP, true,
                0, 0, false, 0, 0, 0, 0, -1, "");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v2 string.
        testString = "[ApnSettingV2] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,14";
        int networkTypeBitmask = 1 << (13 - 1);
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, -1, "");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v2 string with spaces.
        testString = "[ApnSettingV2] Name,apn, ,,,,,,,,123,45,,mms|*,IPV6, IP,true,14";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, -1, "");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v3 string.
        testString = "[ApnSettingV3] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,14,,,,,,,spn,testspn";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v4 string with network type bitmask.
        testString =
                "[ApnSettingV4] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,6";
        networkTypeBitmask = 1 << (6 - 1);
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        testString =
                "[ApnSettingV4] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,"
                        + "4|5|6|7|8|12|13|14|19";
        // The value was calculated by adding "4|5|6|7|8|12|13|14|19".
        networkTypeBitmask = 276728;
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v4 string with network type bitmask and compatible bearer bitmask.
        testString =
                "[ApnSettingV4] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,8,,,,,,,spn,testspn, 6";
        networkTypeBitmask = 1 << (6 - 1);
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v4 string with network type bitmask and incompatible bearer bitmask.
        testString =
                "[ApnSettingV4] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,9,,,,,,,spn,testspn, 6";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0,
                0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v5 string with apnSetId=0
        testString =
                "[ApnSettingV5] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,0,0";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                0, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn");
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v5 string with apnSetId=3
        testString =
                "[ApnSettingV5] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,0,3";
        expectedApn = ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                0, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn", 3, -1, -1);
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v6 string with carrierId=100
        testString =
            "[ApnSettingV5] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,0,3,"
                + "100";
        expectedApn = ApnSetting.makeApnSetting(
            -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
            mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
            0, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn", 3, 100, -1);
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // A v7 string with skip_464xlat=1
        testString =
            "[ApnSettingV7] Name,apn,,,,,,,,,123,45,,mms|*,IPV6,IP,true,0,,,,,,,spn,testspn,0,3,"
                + "-1, 1";
        expectedApn = ApnSetting.makeApnSetting(
            -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
            mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
            0, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn", 3, -1, 1);
        assertApnSettingEqual(expectedApn, ApnSetting.fromString(testString));

        // Return no apn if insufficient fields given.
        testString = "[ApnSettingV3] Name,apn,,,,,,,,,123, 45,,mms|*";
        assertEquals(null, ApnSetting.fromString(testString));

        testString = "Name,apn,,,,,,,,,123, 45,";
        assertEquals(null, ApnSetting.fromString(testString));
    }

    @Test
    @SmallTest
    public void testArrayFromString() throws Exception {
        final int mmsTypesBitmask = ApnSetting.TYPE_MMS;
        // Test a multiple v3 string.
        String testString =
                "[ApnSettingV3] Name,apn,,,,,,,,,123,45,,mms,IPV6,IP,true,14,,,,,,,spn,testspn";
        testString +=
                " ;[ApnSettingV3] Name1,apn1,,,,,,,,,123,46,,mms,IPV6,IP,true,12,,,,,,,gid,testGid";
        testString +=
                " ;[ApnSettingV3] Name1,apn2,,,,,,,,,123,46,,mms,IPV6,IP,true,12,,,,,,,,";
        testString +=
                " ;[ApnSettingV5] Name1,apn2,,,,,,,,,123,46,,mms,IPV6,IP,true,0,,,,,,,,,,3";
        List<ApnSetting> expectedApns = new ArrayList<ApnSetting>();
        expectedApns.add(ApnSetting.makeApnSetting(
                -1, "12345", "Name", "apn", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                1 << (13 - 1), 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "testspn"));
        expectedApns.add(ApnSetting.makeApnSetting(
                -1, "12346", "Name1", "apn1", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                1 << (12 - 1), 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_GID, "testGid"));
        expectedApns.add(ApnSetting.makeApnSetting(
                -1, "12346", "Name1", "apn2", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                1 << (12 - 1), 0, false, 0, 0, 0, 0, -1, ""));
        expectedApns.add(ApnSetting.makeApnSetting(
                -1, "12346", "Name1", "apn2", "", -1, null, "", -1, "", "", 0,
                mmsTypesBitmask, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                0, 0, false, 0, 0, 0, 0, -1, "", 3, -1, -1));
        assertApnSettingsEqual(expectedApns, ApnSetting.arrayFromString(testString));
    }

    @Test
    @SmallTest
    public void testToString() throws Exception {
        // Use default apn_set_id constructor.
        ApnSetting apn = ApnSetting.makeApnSetting(
                99, "12345", "Name", "apn", null, 10,
                null, null, -1, "user", "password", 0,
                ApnSetting.TYPE_DEFAULT, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                4096, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "");
        String expected = "[ApnSettingV7] Name, 99, 12345, apn, null, "
                + "null, null, null, 10, 0, hipri | default, "
                + "IPV6, IP, true, 0, false, 0, 0, 0, 0, spn, , false, 4096, 0, -1, -1";
        assertEquals(expected, apn.toString());

        final int networkTypeBitmask = 1 << (14 - 1);
        apn = ApnSetting.makeApnSetting(
                99, "12345", "Name", "apn", null, 10,
                null, null, -1, "user", "password", 0,
                ApnSetting.TYPE_DEFAULT, ApnSetting.PROTOCOL_IPV6, ApnSetting.PROTOCOL_IP, true,
                networkTypeBitmask, 0, false, 0, 0, 0, 0, ApnSetting.MVNO_TYPE_SPN, "", 3, -1, 1);
        expected = "[ApnSettingV7] Name, 99, 12345, apn, null, "
                + "null, null, null, 10, 0, hipri | default, "
                + "IPV6, IP, true, 0, false, 0, 0, 0, 0, spn, , false, 8192, 3, -1, 1";
        assertEquals(expected, apn.toString());
    }

    @Test
    @SmallTest
    public void testIsMetered() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});

        doReturn(false).when(mServiceState).getDataRoaming();
        doReturn(1).when(mPhone).getSubId();

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_DEFAULT), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_DUN), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_ALL), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_FOTA), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_IA | ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DEFAULT, mPhone));
        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_MMS, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_SUPL, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_CBS, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DUN, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_FOTA, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_IA, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_HIPRI, mPhone));

        // Carrier config settings changes.
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT});

        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DEFAULT, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_MMS, mPhone));
    }

    @Test
    @SmallTest
    public void testIsRoamingMetered() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_DEFAULT, PhoneConstants.APN_TYPE_MMS});
        doReturn(true).when(mServiceState).getDataRoaming();
        doReturn(1).when(mPhone).getSubId();

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_DEFAULT), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_MMS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_DUN), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_ALL), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_FOTA), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_IA | ApnSetting.TYPE_CBS), mPhone));

        // Carrier config settings changes.
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_FOTA});

        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DEFAULT, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_MMS, mPhone));
        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_FOTA, mPhone));
    }

    @Test
    @SmallTest
    public void testIsMeteredAnother() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_SUPL, PhoneConstants.APN_TYPE_CBS});

        doReturn(false).when(mServiceState).getDataRoaming();
        doReturn(1).when(mPhone).getSubId();

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_SUPL), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_FOTA | ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_IA), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_ALL), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_IMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_IMS), mPhone));
    }

    @Test
    @SmallTest
    public void testIsRoamingMeteredAnother() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{PhoneConstants.APN_TYPE_SUPL, PhoneConstants.APN_TYPE_CBS});
        doReturn(true).when(mServiceState).getDataRoaming();
        doReturn(2).when(mPhone).getSubId();

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_SUPL), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_FOTA | ApnSetting.TYPE_CBS), mPhone));

        assertTrue(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_SUPL | ApnSetting.TYPE_IA), mPhone));

        assertTrue(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_ALL), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_IMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_IMS), mPhone));

        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_SUPL, mPhone));
        assertTrue(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_CBS, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DEFAULT, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_MMS, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_DUN, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_FOTA, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_IA, mPhone));
        assertFalse(ApnSettingUtils.isMeteredApnType(ApnSetting.TYPE_HIPRI, mPhone));
    }

    @Test
    @SmallTest
    public void testIsMeteredNothingCharged() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_APN_TYPES_STRINGS,
                new String[]{});

        doReturn(false).when(mServiceState).getDataRoaming();
        doReturn(3).when(mPhone).getSubId();

        assertFalse(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_IMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_IMS | ApnSetting.TYPE_MMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_FOTA), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_ALL), mPhone));
    }

    @Test
    @SmallTest
    public void testIsRoamingMeteredNothingCharged() throws Exception {
        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_METERED_ROAMING_APN_TYPES_STRINGS,
                new String[]{});
        doReturn(true).when(mServiceState).getDataRoaming();
        doReturn(3).when(mPhone).getSubId();

        assertFalse(ApnSettingUtils.isMetered(createApnSetting(ApnSetting.TYPE_IMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_IMS | ApnSetting.TYPE_MMS), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_FOTA), mPhone));

        assertFalse(ApnSettingUtils.isMetered(
                createApnSetting(ApnSetting.TYPE_ALL), mPhone));
    }

    @Test
    @SmallTest
    public void testCanHandleType() throws Exception {
        String types[] = {"mms"};

        assertTrue(createApnSetting(ApnSetting.TYPE_ALL)
                .canHandleType(ApnSetting.TYPE_MMS));

        assertFalse(createApnSetting(ApnSetting.TYPE_DEFAULT)
                .canHandleType(ApnSetting.TYPE_MMS));

        assertTrue(createApnSetting(ApnSetting.TYPE_DEFAULT)
                .canHandleType(ApnSetting.TYPE_DEFAULT));

        // Hipri is asymmetric
        assertTrue(createApnSetting(ApnSetting.TYPE_DEFAULT)
                .canHandleType(ApnSetting.TYPE_HIPRI));
        assertFalse(createApnSetting(ApnSetting.TYPE_HIPRI)
                .canHandleType(ApnSetting.TYPE_DEFAULT));


        assertTrue(createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS)
                .canHandleType(ApnSetting.TYPE_DEFAULT));

        assertTrue(createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS)
                .canHandleType(ApnSetting.TYPE_MMS));

        assertFalse(createApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS)
                .canHandleType(ApnSetting.TYPE_SUPL));

        // special IA case - doesn't match wildcards
        assertFalse(createApnSetting(ApnSetting.TYPE_ALL)
                .canHandleType(ApnSetting.TYPE_IA));
        assertTrue(createApnSetting(
                ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS | ApnSetting.TYPE_IA)
                .canHandleType(ApnSetting.TYPE_IA));

        // same for emergency and mcx
        assertFalse(createApnSetting(ApnSetting.TYPE_ALL)
                .canHandleType(ApnSetting.TYPE_EMERGENCY));
        assertTrue(createApnSetting(
                ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS | ApnSetting.TYPE_EMERGENCY)
                .canHandleType(ApnSetting.TYPE_EMERGENCY));
        assertFalse(createApnSetting(ApnSetting.TYPE_ALL)
                .canHandleType(ApnSetting.TYPE_MCX));
        assertTrue(createApnSetting(
                ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS | ApnSetting.TYPE_MCX)
                .canHandleType(ApnSetting.TYPE_MCX));

        // check carrier disabled
        assertFalse(createDisabledApnSetting(ApnSetting.TYPE_ALL)
                .canHandleType(ApnSetting.TYPE_MMS));
        assertFalse(createDisabledApnSetting(ApnSetting.TYPE_DEFAULT)
                .canHandleType(ApnSetting.TYPE_DEFAULT));
        assertFalse(createDisabledApnSetting(ApnSetting.TYPE_DEFAULT)
                .canHandleType(ApnSetting.TYPE_HIPRI));
        assertFalse(createDisabledApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS)
                .canHandleType(ApnSetting.TYPE_DEFAULT));
        assertFalse(createDisabledApnSetting(ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS)
                .canHandleType(ApnSetting.TYPE_MMS));
        assertFalse(createDisabledApnSetting(
                ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_MMS | ApnSetting.TYPE_IA)
                .canHandleType(ApnSetting.TYPE_IA));
    }

    @Test
    @SmallTest
    public void testEquals() throws Exception {
        final int dummyInt = 1;
        final String dummyString = "dummy";
        final String[] dummyStringArr = new String[] {"dummy"};
        final InetAddress dummyProxyAddress = InetAddress.getByAddress(new byte[]{0, 0, 0, 0});
        final Uri dummyUri = Uri.parse("www.google.com");
        // base apn
        ApnSetting baseApn = createApnSetting(ApnSetting.TYPE_MMS | ApnSetting.TYPE_DEFAULT);
        Field[] fields = ApnSetting.class.getDeclaredFields();
        for (Field f : fields) {
            int modifiers = f.getModifiers();
            if (Modifier.isStatic(modifiers) || !Modifier.isFinal(modifiers)) {
                continue;
            }
            f.setAccessible(true);
            ApnSetting testApn = null;
            if (int.class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.setInt(testApn, dummyInt + f.getInt(testApn));
            } else if (boolean.class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.setBoolean(testApn, !f.getBoolean(testApn));
            } else if (String.class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.set(testApn, dummyString);
            } else if (String[].class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.set(testApn, dummyStringArr);
            } else if (InetAddress.class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.set(testApn, dummyProxyAddress);
            } else if (Uri.class.equals(f.getType())) {
                testApn = ApnSetting.makeApnSetting(baseApn);
                f.set(testApn, dummyUri);
            } else {
                fail("Unsupported field:" + f.getName());
            }
            if (testApn != null) {
                assertFalse(f.getName() + " is NOT checked", testApn.equals(baseApn));
            }
        }
    }

    @Test
    @SmallTest
    public void testEqualsRoamingProtocol() throws Exception {
        ApnSetting apn1 = ApnSetting.makeApnSetting(
                1234,
                "310260",
                "",
                "ims",
                null,
                -1,
                null,
                null,
                -1,
                "",
                "",
                -1,
                ApnSetting.TYPE_IMS,
                ApnSetting.PROTOCOL_IPV6,
                -1,
                true,
                ServiceState.convertBearerBitmaskToNetworkTypeBitmask(131071),
                0,
                false,
                0,
                0,
                0,
                1440,
                -1,
                "");

        ApnSetting apn2 = ApnSetting.makeApnSetting(
                1235,
                "310260",
                "",
                "ims",
                null,
                -1,
                null,
                null,
                -1,
                "",
                "",
                -1,
                ApnSetting.TYPE_IMS,
                ApnSetting.PROTOCOL_IPV6,
                ApnSetting.PROTOCOL_IPV6,
                true,
                ServiceState.convertBearerBitmaskToNetworkTypeBitmask(131072),
                0,
                false,
                0,
                0,
                0,
                1440,
                -1,
                "");

        assertTrue(apn1.equals(apn2, false));
        assertFalse(apn1.equals(apn2, true));
    }

    @Test
    @SmallTest
    public void testCanHandleNetwork() throws Exception {
        ApnSetting apn1 = ApnSetting.makeApnSetting(
                1234,
                "310260",
                "",
                "ims",
                null,
                -1,
                null,
                null,
                -1,
                "",
                "",
                -1,
                ApnSetting.TYPE_IMS,
                ApnSetting.PROTOCOL_IPV6,
                -1,
                true,
                (int) (TelephonyManager.NETWORK_TYPE_BITMASK_LTE
                        | TelephonyManager.NETWORK_TYPE_BITMASK_UMTS),
                0,
                false,
                0,
                0,
                0,
                1440,
                -1,
                "");

        ApnSetting apn2 = ApnSetting.makeApnSetting(
                1235,
                "310260",
                "",
                "ims",
                null,
                -1,
                null,
                null,
                -1,
                "",
                "",
                -1,
                ApnSetting.TYPE_IMS,
                ApnSetting.PROTOCOL_IPV6,
                ApnSetting.PROTOCOL_IPV6,
                true,
                (int) (TelephonyManager.NETWORK_TYPE_BITMASK_EDGE
                        | TelephonyManager.NETWORK_TYPE_BITMASK_GPRS),
                0,
                false,
                0,
                0,
                0,
                1440,
                -1,
                "");

        assertFalse(apn1.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_1xRTT));
        assertTrue(apn1.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_LTE));
        assertTrue(apn1.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_UMTS));

        assertFalse(apn2.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_1xRTT));
        assertFalse(apn2.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_LTE));
        assertTrue(apn2.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_GPRS));
        assertTrue(apn2.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_EDGE));

        assertTrue(apn2.canSupportNetworkType(TelephonyManager.NETWORK_TYPE_GSM));
    }
}
