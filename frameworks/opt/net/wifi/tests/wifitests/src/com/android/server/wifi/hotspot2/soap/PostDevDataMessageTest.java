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

package com.android.server.wifi.hotspot2.soap;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.content.Context;
import android.os.Build;
import android.telephony.TelephonyManager;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.SystemInfo;
import com.android.server.wifi.hotspot2.omadm.DevDetailMo;
import com.android.server.wifi.hotspot2.omadm.DevInfoMo;

import org.junit.Before;
import org.junit.Test;
import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.AttributeInfo;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapPrimitive;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.mockito.Mock;

/**
 * Unit tests for {@link PostDevDataMessage}.
 */
@SmallTest
public class PostDevDataMessageTest {
    private static final String TEST_DEV_ID = "12312341";
    private static final String TEST_MANUFACTURER = Build.MANUFACTURER;
    private static final String TEST_MODEL = Build.MODEL;
    private static final String TEST_LANGUAGE = "en";
    private static final String TEST_SESSION_ID = "123456";
    private static final String TEST_REQUEST_REASON = "test Reason";
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
        when(mSystemInfo.getDeviceModel()).thenReturn(TEST_MODEL);
        when(mSystemInfo.getLanguage()).thenReturn(TEST_LANGUAGE);

        when(mSystemInfo.getDeviceId()).thenReturn(TEST_DEV_ID);
        when(mSystemInfo.getDeviceManufacturer()).thenReturn(TEST_MANUFACTURER);
        when(mSystemInfo.getHwVersion()).thenReturn(TEST_HW_VERSION);
        when(mSystemInfo.getMacAddress(any(String.class))).thenReturn(TEST_MAC_ADDR);
        when(mSystemInfo.getSoftwareVersion()).thenReturn(TEST_SW_VERSION);
        when(mSystemInfo.getFirmwareVersion()).thenReturn(TEST_FW_VERSION);
        when(mDataTelephonyManager.getSubscriberId()).thenReturn(TEST_IMSI);
    }

    /**
     * Verify the request is organized correctly with no session id.
     */
    @Test
    public void serializePostDevDataRequestWithoutSessionId() {
        SoapSerializationEnvelope request = PostDevDataMessage.serializeToSoapEnvelope(mContext,
                mSystemInfo, TEST_REDIRECT_URL,
                TEST_REQUEST_REASON, null);
        assertNotNull(request);
        assertEquals(SoapEnvelope.VER12, request.version);
        SoapObject soapRequest = (SoapObject) request.bodyOut;
        verifyCommonFields(soapRequest);
        // Should not have the session id attribute
        assertFalse(soapRequest.hasAttribute(SoapEnvelope.NS20, SppConstants.ATTRIBUTE_SESSION_ID));
    }

    /**
     * Verify the request is organized correctly with a session id.
     */
    @Test
    public void serializePostDevDataRequestWithSessionId() {
        SoapSerializationEnvelope request = PostDevDataMessage.serializeToSoapEnvelope(mContext,
                mSystemInfo, TEST_REDIRECT_URL,
                TEST_REQUEST_REASON, TEST_SESSION_ID);
        assertNotNull(request);
        assertEquals(SoapEnvelope.VER12, request.version);
        SoapObject soapRequest = (SoapObject) request.bodyOut;
        verifyCommonFields(soapRequest);
        // Should have session id attribute
        assertEquals(TEST_SESSION_ID, soapRequest.getAttributeAsString(SoapEnvelope.NS20,
                SppConstants.ATTRIBUTE_SESSION_ID));

    }

    private void verifyCommonFields(SoapObject request) {
        assertEquals(request.getNamespace(), SoapEnvelope.NS20);
        assertEquals(request.getName(), SppConstants.METHOD_POST_DEV_DATA);
        assertEquals(SppConstants.SUPPORTED_SPP_VERSION,
                request.getAttributeAsString(SoapEnvelope.NS20,
                        SppConstants.ATTRIBUTE_SPP_VERSION));

        int moCounter = 0;
        PropertyInfo propertyInfo = new AttributeInfo();
        for (int i = 0; i < request.getPropertyCount(); i++) {
             request.getPropertyInfo(i, propertyInfo);
            if (propertyInfo.name.equals(SppConstants.PROPERTY_MO_CONTAINER)) {
                SoapPrimitive soapPrimitive = (SoapPrimitive) propertyInfo.getValue();
                String moUrn = soapPrimitive.getAttributeAsString(SppConstants.ATTRIBUTE_MO_URN);
                assertEquals(soapPrimitive.getNamespace(), SoapEnvelope.NS20);
                if (moUrn.equals(DevDetailMo.URN) || moUrn.equals(DevInfoMo.URN)) {
                    moCounter++;
                }
            }
        }
        // DevInfoMo + DevDetailMo
        assertEquals(2, moCounter);

        assertTrue(
                request.hasProperty(SoapEnvelope.NS20, SppConstants.PROPERTY_MO_CONTAINER));

        assertEquals(TEST_REQUEST_REASON,
                request.getAttribute(SppConstants.ATTRIBUTE_REQUEST_REASON));
        assertEquals(TEST_REDIRECT_URL, request.getAttribute(SppConstants.ATTRIBUTE_REDIRECT_URI));
    }
}
