/*
 * Copyright (C) 2016 The Android Open Source Project
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

import android.os.Bundle;
import android.os.Parcel;
import android.telephony.AccessNetworkConstants;
import android.telephony.LteVopsSupportInfo;
import android.telephony.NetworkRegistrationInfo;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Pair;

import junit.framework.TestCase;

import java.util.ArrayList;
import java.util.Arrays;

public class ServiceStateTest extends TestCase {

    @SmallTest
    public void testRoaming() {
        ServiceState ss = new ServiceState();
        // add data registration state
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_ROAMING)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        assertTrue(ss.getDataRoamingFromRegistration());

        ss.setCdmaDefaultRoamingIndicator(1);
        assertEquals(1, ss.getCdmaDefaultRoamingIndicator());

        ss.setCdmaEriIconIndex(2);
        assertEquals(2, ss.getCdmaEriIconIndex());

        ss.setCdmaEriIconMode(3);
        assertEquals(3, ss.getCdmaEriIconMode());

        ss.setCdmaRoamingIndicator(4);
        assertEquals(4, ss.getCdmaRoamingIndicator());

        ss.setDataRoamingType(ServiceState.ROAMING_TYPE_DOMESTIC);
        assertTrue(ss.getDataRoaming());
        assertEquals(ServiceState.ROAMING_TYPE_DOMESTIC, ss.getDataRoamingType());

        ss.setVoiceRoamingType(ServiceState.ROAMING_TYPE_DOMESTIC);
        assertTrue(ss.getVoiceRoaming());
        assertEquals(ServiceState.ROAMING_TYPE_DOMESTIC, ss.getVoiceRoamingType());
    }

    @SmallTest
    public void testRegState() {
        ServiceState ss = new ServiceState();

        ss.setDataRegState(ServiceState.STATE_IN_SERVICE);
        assertEquals(ServiceState.STATE_IN_SERVICE, ss.getDataRegState());

        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        assertEquals(ServiceState.STATE_IN_SERVICE, ss.getVoiceRegState());
    }

    @SmallTest
    public void testBitmaskFromString() {
        String networkTypeList = "4|7|5|6|12|13|14|19";
        int networkTypeBitmask = 1 << (4 - 1) | 1 << (7 - 1) | 1 << (5 - 1) | 1 << (6 - 1)
                | 1 << (12 - 1) | 1 << (14 - 1) | 1 << (13 - 1) | 1 << (19 - 1);
        assertEquals(networkTypeBitmask,
                ServiceState.getBitmaskFromString(networkTypeList));

        networkTypeList = "13";
        networkTypeBitmask = 1 << (13 - 1);
        assertEquals(networkTypeBitmask,
                ServiceState.getBitmaskFromString(networkTypeList));

        networkTypeList = "";
        networkTypeBitmask = 0;
        assertEquals(networkTypeBitmask,
                ServiceState.getBitmaskFromString(networkTypeList));
    }

    @SmallTest
    public void testConvertNetworkTypeBitmaskToBearerBitmask() {
        // The value was calculated by adding "4|4|7|5|6|12|14|13|19".
        int networkTypeBitmask = 276600;
        // The value was calculated by adding "4|5|6|7|8|12|13|14|19".
        int bearerBitmask = 276728;
        assertEquals(bearerBitmask,
                ServiceState.convertNetworkTypeBitmaskToBearerBitmask(networkTypeBitmask));

        networkTypeBitmask = 0;
        bearerBitmask = 0;
        assertEquals(bearerBitmask,
                ServiceState.convertNetworkTypeBitmaskToBearerBitmask(networkTypeBitmask));
    }

    @SmallTest
    public void testConvertBearerBitmaskToNetworkTypeBitmask() {
        // The value was calculated by adding "4|4|7|5|6|12|14|13|19".
        int networkTypeBitmask = 276600;
        // The value was calculated by adding "4|5|6|7|8|12|13|14|19".
        int bearerBitmask = 276728;
        assertEquals(networkTypeBitmask,
                ServiceState.convertBearerBitmaskToNetworkTypeBitmask(bearerBitmask));

        networkTypeBitmask = 0;
        bearerBitmask = 0;
        assertEquals(networkTypeBitmask,
                ServiceState.convertBearerBitmaskToNetworkTypeBitmask(bearerBitmask));
    }

    @SmallTest
    public void testRAT() {
        ServiceState ss = new ServiceState();

        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);
        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_LTE, ss.getRilDataRadioTechnology());
        assertEquals(TelephonyManager.NETWORK_TYPE_LTE, ss.getDataNetworkType());

        nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_1xRTT)
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        assertEquals(ServiceState.RIL_RADIO_TECHNOLOGY_1xRTT, ss.getRilVoiceRadioTechnology());
        assertEquals(TelephonyManager.NETWORK_TYPE_1xRTT, ss.getVoiceNetworkType());

        ArrayList<Pair<Integer, Boolean>> rats = new ArrayList<Pair<Integer, Boolean>>();

        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_IS95A, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_IS95B, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_1xRTT, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_EVDO_0, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_EVDO_A, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_EVDO_B, true));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_EHRPD, true));

        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_GPRS, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_EDGE, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_UMTS, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_HSDPA, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_HSUPA, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_HSPA, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_LTE, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_HSPAP, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_GSM, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_TD_SCDMA, false));
        rats.add(new Pair<Integer, Boolean>(ServiceState.RIL_RADIO_TECHNOLOGY_IWLAN, false));

        for (Pair<Integer, Boolean> rat : rats) {
            boolean isCdma = rat.second;

            if (isCdma) {
                assertTrue("RAT " + rat + " should be CDMA", ServiceState.isCdma(rat.first));
                assertFalse("RAT " + rat + " should not be GSM", ServiceState.isGsm(rat.first));
            } else {
                assertFalse("RAT " + rat + " should not be CDMA", ServiceState.isCdma(rat.first));
                assertTrue("RAT " + rat + " should be GSM", ServiceState.isGsm(rat.first));
            }
        }
    }
    @SmallTest
    public void testGetCellBandwidths() {
        ServiceState ss = new ServiceState();

        ss.setCellBandwidths(null);
        assertTrue(Arrays.equals(ss.getCellBandwidths(), new int[0]));

        int[] cellBandwidths = new int[]{5000, 10000};
        ss.setCellBandwidths(cellBandwidths);
        assertTrue(Arrays.equals(ss.getCellBandwidths(), cellBandwidths));
    }

    @SmallTest
    public void testOperatorName() {
        ServiceState ss = new ServiceState();

        ss.setDataOperatorAlphaLong("abc");
        assertEquals("abc", ss.getDataOperatorAlphaLong());

        ss.setDataOperatorName("def", "xyz", "123456");
        assertEquals("xyz", ss.getDataOperatorAlphaShort());

        ss.setOperatorName("long", "short", "numeric");
        assertEquals("long", ss.getVoiceOperatorAlphaLong());
        assertEquals("short", ss.getVoiceOperatorAlphaShort());
        assertEquals("numeric", ss.getVoiceOperatorNumeric());
        assertEquals("long", ss.getDataOperatorAlphaLong());
        assertEquals("short", ss.getDataOperatorAlphaShort());
        assertEquals("numeric", ss.getDataOperatorNumeric());
        assertEquals("long", ss.getOperatorAlpha());

        ss.setOperatorName("", "short", "");
        assertEquals("short", ss.getOperatorAlpha());
    }

    @SmallTest
    public void testMisc() {
        ServiceState ss = new ServiceState();

        ss.setCssIndicator(100);
        assertEquals(1, ss.getCssIndicator());

        ss.setIsManualSelection(true);
        assertTrue(ss.getIsManualSelection());

        ss.setCdmaSystemAndNetworkId(123, 456);
        assertEquals(123, ss.getCdmaSystemId());
        assertEquals(456, ss.getCdmaNetworkId());

        ss.setEmergencyOnly(true);
        assertTrue(ss.isEmergencyOnly());
    }

    @SmallTest
    public void testParcel() {

        ServiceState ss = new ServiceState();
        ss.setVoiceRegState(ServiceState.STATE_IN_SERVICE);
        ss.setDataRegState(ServiceState.STATE_OUT_OF_SERVICE);
        ss.setVoiceRoamingType(ServiceState.ROAMING_TYPE_INTERNATIONAL);
        ss.setDataRoamingType(ServiceState.ROAMING_TYPE_UNKNOWN);
        ss.setOperatorName("long", "short", "numeric");
        ss.setIsManualSelection(true);
        ss.setCssIndicator(1);
        ss.setCdmaSystemAndNetworkId(2, 3);
        ss.setCdmaRoamingIndicator(4);
        ss.setCdmaDefaultRoamingIndicator(5);
        ss.setCdmaEriIconIndex(6);
        ss.setCdmaEriIconMode(7);
        ss.setEmergencyOnly(true);
        ss.setChannelNumber(2100);
        ss.setCellBandwidths(new int[]{1400, 5000, 10000});

        Parcel p = Parcel.obtain();
        ss.writeToParcel(p, 0);
        p.setDataPosition(0);

        ServiceState newSs = new ServiceState(p);
        assertEquals(ss, newSs);
    }

    @SmallTest
    public void testBundle() {
        ServiceState ss = new ServiceState();

        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_1xRTT)
                .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
                .setDomain(NetworkRegistrationInfo.DOMAIN_CS)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_EVDO_0)
                .setRegistrationState(
                        NetworkRegistrationInfo.REGISTRATION_STATE_NOT_REGISTERED_OR_SEARCHING)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        ss.setVoiceRoamingType(ServiceState.ROAMING_TYPE_INTERNATIONAL);
        ss.setDataRoamingType(ServiceState.ROAMING_TYPE_UNKNOWN);
        ss.setOperatorName("long", "short", "numeric");
        ss.setIsManualSelection(true);

        ss.setCssIndicator(1);
        ss.setCdmaSystemAndNetworkId(2, 3);
        ss.setCdmaRoamingIndicator(4);
        ss.setCdmaDefaultRoamingIndicator(5);
        ss.setCdmaEriIconIndex(6);
        ss.setCdmaEriIconMode(7);
        ss.setEmergencyOnly(true);
        ss.setChannelNumber(2100);
        ss.setCellBandwidths(new int[]{3, 4, 10});

        Bundle b = new Bundle();
        ss.fillInNotifierBundle(b);
        ServiceState newSs = ServiceState.newFromBundle(b);

        assertEquals(ss, newSs);
    }

    @SmallTest
    public void testNetworkRegistrationInfo() {
        NetworkRegistrationInfo wwanVoiceRegState = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_CS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                0, 0, 0, false,
                null, null, true, 0, 0, 0);

        LteVopsSupportInfo lteVopsSupportInfo =
                new LteVopsSupportInfo(LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE,
                        LteVopsSupportInfo.LTE_STATUS_NOT_AVAILABLE);

        NetworkRegistrationInfo wwanDataRegState = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .build();

        NetworkRegistrationInfo wlanRegState = new NetworkRegistrationInfo.Builder()
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WLAN)
                .build();

        ServiceState ss = new ServiceState();

        ss.addNetworkRegistrationInfo(wwanVoiceRegState);
        ss.addNetworkRegistrationInfo(wwanDataRegState);
        ss.addNetworkRegistrationInfo(wlanRegState);

        assertEquals(ss.getNetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_CS,
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN), wwanVoiceRegState);
        assertEquals(ss.getNetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_PS,
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN), wwanDataRegState);
        assertEquals(ss.getNetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_PS,
                AccessNetworkConstants.TRANSPORT_TYPE_WLAN), wlanRegState);

        wwanDataRegState = new NetworkRegistrationInfo(
                NetworkRegistrationInfo.DOMAIN_PS, AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                0, 0, 0, true, null, null, 0, false, false, false, lteVopsSupportInfo, false);
        ss.addNetworkRegistrationInfo(wwanDataRegState);
        assertEquals(ss.getNetworkRegistrationInfo(NetworkRegistrationInfo.DOMAIN_PS,
                AccessNetworkConstants.TRANSPORT_TYPE_WWAN), wwanDataRegState);
    }

    @SmallTest
    public void testDuplexMode_notLte() {
        ServiceState ss = new ServiceState();
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_GSM)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        ss.setChannelNumber(2400);

        assertEquals(ss.getDuplexMode(), ServiceState.DUPLEX_MODE_UNKNOWN);
    }

    @SmallTest
    public void testDuplexMode_invalidEarfcn() {
        ServiceState ss = new ServiceState();
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);

        ss.setChannelNumber(-1);

        assertEquals(ss.getDuplexMode(), ServiceState.DUPLEX_MODE_UNKNOWN);

        ss.setChannelNumber(Integer.MAX_VALUE);

        assertEquals(ss.getDuplexMode(), ServiceState.DUPLEX_MODE_UNKNOWN);
    }

    @SmallTest
    public void testDuplexMode_FddChannel() {
        ServiceState ss = new ServiceState();
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);
        ss.setChannelNumber(2400); // band 5

        assertEquals(ss.getDuplexMode(), ServiceState.DUPLEX_MODE_FDD);
    }

    @SmallTest
    public void testDuplexMode_TddChannel() {
        ServiceState ss = new ServiceState();
        NetworkRegistrationInfo nri = new NetworkRegistrationInfo.Builder()
                .setTransportType(AccessNetworkConstants.TRANSPORT_TYPE_WWAN)
                .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
                .setDomain(NetworkRegistrationInfo.DOMAIN_PS)
                .build();
        ss.addNetworkRegistrationInfo(nri);
        ss.setChannelNumber(36000); // band 33

        assertEquals(ss.getDuplexMode(), ServiceState.DUPLEX_MODE_TDD);
    }
}
