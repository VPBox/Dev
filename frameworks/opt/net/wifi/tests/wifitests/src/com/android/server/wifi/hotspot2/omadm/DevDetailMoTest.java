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

package com.android.server.wifi.hotspot2.omadm;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertNull;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.content.Context;
import android.os.Build;
import android.telephony.TelephonyManager;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.SystemInfo;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

/**
 * Unit tests for {@link DevDetailMo}.
 * TODO(b/80300806): change the test to verify the XML in terms of the structure of XML.
 */
@SmallTest
public class DevDetailMoTest {
    private static final String TEST_DEV_ID = "12312341";
    private static final String TEST_MANUFACTURER = Build.MANUFACTURER;
    private static final String TEST_HW_VERSION = "Test HW 1.0";
    private static final String TEST_MAC_ADDR = "11:22:33:44:55:66";
    private static final String TEST_IMSI = "310150123456789";
    private static final String TEST_SW_VERSION = "Android Test 1.0";
    private static final String TEST_FW_VERSION = "Test FW 1.0";
    private static final String TEST_REDIRECT_URL = "http://127.0.0.1:12345/index.htm";

    @Mock
    private SystemInfo mSystemInfo;
    @Mock
    private Context mContext;
    @Mock
    private TelephonyManager mTelephonyManager;
    @Mock
    private TelephonyManager mDataTelephonyManager;
    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        when(mContext.getSystemService(TelephonyManager.class)).thenReturn(mTelephonyManager);
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
        when(mSystemInfo.getDeviceId()).thenReturn(TEST_DEV_ID);
        when(mSystemInfo.getDeviceManufacturer()).thenReturn(TEST_MANUFACTURER);
        when(mSystemInfo.getHwVersion()).thenReturn(TEST_HW_VERSION);
        when(mSystemInfo.getMacAddress(any(String.class))).thenReturn(TEST_MAC_ADDR);
        when(mSystemInfo.getSoftwareVersion()).thenReturn(TEST_SW_VERSION);
        when(mSystemInfo.getFirmwareVersion()).thenReturn(TEST_FW_VERSION);
        when(mDataTelephonyManager.getSubscriberId()).thenReturn(TEST_IMSI);
        DevDetailMo.setAllowToSendImsiImeiInfo(false);
    }

    /**
     * Verify the serialization will return null if wifi mac address was missed.
     */
    @Test
    public void serializeDevDetailMoWithoutMacaddress() {
        when(mSystemInfo.getMacAddress(any(String.class))).thenReturn(null);
        assertNull(DevDetailMo.serializeToXml(mContext, mSystemInfo, TEST_REDIRECT_URL));
    }

    /**
     * Verify the serialization will include IMSI and IMEI if it is allowed to send the information.
     */
    @Test
    public void serializeDevDetailMoForHomeNetworkCarrier() {
        DevDetailMo.setAllowToSendImsiImeiInfo(true);
        String expected = String.format("<MgmtTree>"
                        + "<VerDTD>%s</VerDTD>"
                        + "<Node><NodeName>DevDetail</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>ext</NodeName>"
                        + "<Node><NodeName>org.wi-fi</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>Wi-Fi</NodeName>"
                        + "<Node><NodeName>EAPMethodList</NodeName>"
                        + "<Node><NodeName>EAPMethod01</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP-V2</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>EAPMethod02</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>EAPMethod03</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>PAP</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod04</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>13</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod05</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>18</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod06</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>23</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod07</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>50</Value></Node></Node>"
                        + "</Node>"
                        + "<Node><NodeName>ManufacturingCertificate</NodeName><Value>FALSE</Value>"
                        + "</Node>"
                        + "<Node><NodeName>ClientTriggerRedirectURI</NodeName>"
                        + "<Value>%s</Value></Node>"
                        + "<Node><NodeName>Wi-FiMACAddress</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>IMSI</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>IMEI_MEID</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Ops</NodeName>"
                        + "<Node><NodeName>launchBrowserToURI</NodeName><Value/></Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "<Node><NodeName>URI</NodeName>"
                        + "<Node><NodeName>MaxDepth</NodeName><Value>32</Value></Node>"
                        + "<Node><NodeName>MaxTotLen</NodeName><Value>2048</Value></Node>"
                        + "<Node><NodeName>MaxSegLen</NodeName><Value>64</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>DevType</NodeName><Value>Smartphone</Value></Node>"
                        + "<Node><NodeName>OEM</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>FwV</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>SwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>HwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>LrgOrj</NodeName><Value>TRUE</Value></Node>"
                        + "</Node>"
                        + "</MgmtTree>",
                MoSerializer.DM_VERSION, DevDetailMo.URN, DevDetailMo.HS20_URN,
                TEST_REDIRECT_URL, TEST_MAC_ADDR.replace(":", ""), TEST_IMSI, TEST_DEV_ID,
                TEST_MANUFACTURER, TEST_FW_VERSION, TEST_SW_VERSION, TEST_HW_VERSION);
        assertEquals(expected,
                DevDetailMo.serializeToXml(mContext, mSystemInfo, TEST_REDIRECT_URL));
    }

    /**
     * Verify the serialization will not include IMSI and IMEI if the HS2.0 network is not a home
     * carrier network.
     */
    @Test
    public void serializeDevDetailMoForOtherCarrier() {
        String expected = String.format("<MgmtTree>"
                        + "<VerDTD>%s</VerDTD>"
                        + "<Node><NodeName>DevDetail</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>ext</NodeName>"
                        + "<Node><NodeName>org.wi-fi</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>Wi-Fi</NodeName>"
                        + "<Node><NodeName>EAPMethodList</NodeName>"
                        + "<Node><NodeName>EAPMethod01</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP-V2</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>EAPMethod02</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP</Value>"
                        + "</Node></Node>"
                        + "<Node><NodeName>EAPMethod03</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>PAP</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod04</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>13</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod05</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>18</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod06</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>23</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod07</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>50</Value></Node></Node>"
                        + "</Node>"
                        + "<Node><NodeName>ManufacturingCertificate</NodeName><Value>FALSE</Value>"
                        + "</Node>"
                        + "<Node><NodeName>ClientTriggerRedirectURI</NodeName>"
                        + "<Value>%s</Value></Node>"
                        + "<Node><NodeName>Wi-FiMACAddress</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Ops</NodeName>"
                        + "<Node><NodeName>launchBrowserToURI</NodeName><Value/></Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "<Node><NodeName>URI</NodeName>"
                        + "<Node><NodeName>MaxDepth</NodeName><Value>32</Value></Node>"
                        + "<Node><NodeName>MaxTotLen</NodeName><Value>2048</Value></Node>"
                        + "<Node><NodeName>MaxSegLen</NodeName><Value>64</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>DevType</NodeName><Value>Smartphone</Value></Node>"
                        + "<Node><NodeName>OEM</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>FwV</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>SwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>HwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>LrgOrj</NodeName><Value>TRUE</Value></Node>"
                        + "</Node>"
                        + "</MgmtTree>",
                MoSerializer.DM_VERSION, DevDetailMo.URN, DevDetailMo.HS20_URN, TEST_REDIRECT_URL,
                TEST_MAC_ADDR.replace(":", ""), TEST_MANUFACTURER,
                TEST_FW_VERSION, TEST_SW_VERSION, TEST_HW_VERSION);
        assertEquals(expected,
                DevDetailMo.serializeToXml(mContext, mSystemInfo, TEST_REDIRECT_URL));
    }

    /**
     * Verify the serialization will not include IMSI and IMEI if the device has no SIM.
     */
    @Test
    public void serializeDevDetailMoWithoutSim() {
        DevDetailMo.setAllowToSendImsiImeiInfo(true);
        when(mDataTelephonyManager.getSubscriberId()).thenReturn(null);
        String expected = String.format("<MgmtTree>"
                        + "<VerDTD>%s</VerDTD>"
                        + "<Node><NodeName>DevDetail</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>ext</NodeName>"
                        + "<Node><NodeName>org.wi-fi</NodeName>"
                        + "<RTProperties>"
                        + "<Type><DDFName>%s</DDFName></Type>"
                        + "</RTProperties>"
                        + "<Node><NodeName>Wi-Fi</NodeName>"
                        + "<Node><NodeName>EAPMethodList</NodeName>"
                        + "<Node><NodeName>EAPMethod01</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP-V2</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>EAPMethod02</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>MS-CHAP</Value>"
                        + "</Node></Node>"
                        + "<Node><NodeName>EAPMethod03</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>21</Value></Node>"
                        + "<Node><NodeName>InnerMethod</NodeName><Value>PAP</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod04</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>13</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod05</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>18</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod06</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>23</Value></Node></Node>"
                        + "<Node><NodeName>EAPMethod07</NodeName>"
                        + "<Node><NodeName>EAPType</NodeName><Value>50</Value></Node></Node>"
                        + "</Node>"
                        + "<Node><NodeName>ManufacturingCertificate</NodeName><Value>FALSE</Value>"
                        + "</Node>"
                        + "<Node><NodeName>ClientTriggerRedirectURI</NodeName>"
                        + "<Value>%s</Value></Node>"
                        + "<Node><NodeName>Wi-FiMACAddress</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Ops</NodeName>"
                        + "<Node><NodeName>launchBrowserToURI</NodeName><Value/></Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "</Node>"
                        + "<Node><NodeName>URI</NodeName>"
                        + "<Node><NodeName>MaxDepth</NodeName><Value>32</Value></Node>"
                        + "<Node><NodeName>MaxTotLen</NodeName><Value>2048</Value></Node>"
                        + "<Node><NodeName>MaxSegLen</NodeName><Value>64</Value></Node>"
                        + "</Node>"
                        + "<Node><NodeName>DevType</NodeName><Value>Smartphone</Value></Node>"
                        + "<Node><NodeName>OEM</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>FwV</NodeName><Value>%s</Value></Node><Node>"
                        + "<NodeName>SwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>HwV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>LrgOrj</NodeName><Value>TRUE</Value></Node>"
                        + "</Node>"
                        + "</MgmtTree>",
                MoSerializer.DM_VERSION, DevDetailMo.URN, DevDetailMo.HS20_URN, TEST_REDIRECT_URL,
                TEST_MAC_ADDR.replace(":", ""), TEST_MANUFACTURER,
                TEST_FW_VERSION, TEST_SW_VERSION, TEST_HW_VERSION);
        assertEquals(expected,
                DevDetailMo.serializeToXml(mContext, mSystemInfo, TEST_REDIRECT_URL));
    }
}
