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
import static org.junit.Assert.assertNotNull;

import androidx.test.filters.SmallTest;

import org.junit.Test;
import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;

/**
 * Unit tests for {@link UpdateResponseMessage}.
 */
@SmallTest
public class UpdateResponseMessageTest {
    private static final String TEST_SESSION_ID = "123456";

    /**
     * Verify the request is organized correctly with a session id.
     */
    @Test
    public void serializeUpdateResponseMessageWithSessionIdWithoutError() {
        SoapSerializationEnvelope request = UpdateResponseMessage.serializeToSoapEnvelope(
                TEST_SESSION_ID, false);
        assertNotNull(request);
        assertEquals(SoapEnvelope.VER12, request.version);
        SoapObject soapRequest = (SoapObject) request.bodyOut;
        verifyCommonFields(soapRequest);

        // Should have an OK status attribute
        assertEquals(SppConstants.mapStatusIntToString(SppConstants.SppStatus.OK),
                soapRequest.getAttributeAsString(SoapEnvelope.NS20,
                        SppConstants.ATTRIBUTE_SPP_STATUS));
    }

    /**
     * Verify the request is organized correctly with a session id and an error.
     */
    @Test
    public void serializeUpdateResponseMessageWithError() {
        SoapSerializationEnvelope request = UpdateResponseMessage.serializeToSoapEnvelope(
                TEST_SESSION_ID, true);
        assertNotNull(request);
        assertEquals(SoapEnvelope.VER12, request.version);
        SoapObject soapRequest = (SoapObject) request.bodyOut;
        verifyCommonFields(soapRequest);

        // Should have an error status attribute
        assertEquals(SppConstants.mapStatusIntToString(SppConstants.SppStatus.ERROR),
                soapRequest.getAttributeAsString(SoapEnvelope.NS20,
                        SppConstants.ATTRIBUTE_SPP_STATUS));
        assertEquals(1, soapRequest.getPropertyCount());

        SoapObject sppError = (SoapObject) soapRequest.getProperty(0);
        assertNotNull(sppError);
        assertEquals(sppError.getNamespace(), SoapEnvelope.NS20);
        assertEquals(
                SppConstants.mapErrorIntToString(SppConstants.SppError.MO_ADD_OR_UPDATE_FAILED),
                sppError.getAttributeAsString(SppConstants.ATTRIBUTE_ERROR_CODE));
    }

    private void verifyCommonFields(SoapObject request) {
        assertEquals(request.getNamespace(), SoapEnvelope.NS20);
        assertEquals(request.getName(), SppConstants.METHOD_UPDATE_RESPONSE);
        assertEquals(SppConstants.SUPPORTED_SPP_VERSION,
                request.getAttributeAsString(SoapEnvelope.NS20,
                        SppConstants.ATTRIBUTE_SPP_VERSION));

        // Should have a session id attribute
        assertEquals(TEST_SESSION_ID, request.getAttributeAsString(SoapEnvelope.NS20,
                SppConstants.ATTRIBUTE_SESSION_ID));
    }
}
