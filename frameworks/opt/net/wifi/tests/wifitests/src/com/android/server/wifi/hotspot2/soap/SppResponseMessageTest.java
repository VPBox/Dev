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
import static org.junit.Assert.fail;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.ksoap2.serialization.SoapObject;

import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link SppResponseMessage}.
 */
@SmallTest
public class SppResponseMessageTest {
    private static final String TEST_STATUS = "OK";
    private static final String TEST_ERROR_STATUS = "Error occurred";
    private static final String TEST_SESSION_ID = "D215D696517BA138F1D28442DF0F4E07";
    private static final String TEST_VERSION = "1.0";
    private static final String TEST_ERROR = "SPP version not supported";

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
    }

    /**
     * Verify if the SppSoapResponse message without error attribute is properly parsed.
     */
    @Test
    public void verifySppSoapResponseWithoutErrorAttribute() {
        SoapObject soapObject = new SoapObject();
        soapObject.addAttribute(SppResponseMessage.SPPStatusAttribute, TEST_STATUS);
        soapObject.addAttribute(SppResponseMessage.SPPSessionIDAttribute, TEST_SESSION_ID);
        soapObject.addAttribute(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);
        Map<String, String> expectedAttributes = new HashMap<>();
        expectedAttributes.put(SppResponseMessage.SPPStatusAttribute, TEST_STATUS);
        expectedAttributes.put(SppResponseMessage.SPPSessionIDAttribute, TEST_SESSION_ID);
        expectedAttributes.put(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);

        SppResponseMessage sppResponseMessage = new SppResponseMessage(soapObject,
                SppResponseMessage.MessageType.POST_DEV_DATA_RESPONSE);

        assertEquals(SppConstants.mapStatusStringToInt(TEST_STATUS),
                sppResponseMessage.getStatus());
        assertEquals(SppResponseMessage.MessageType.POST_DEV_DATA_RESPONSE,
                sppResponseMessage.getMessageType());
        assertEquals(TEST_SESSION_ID, sppResponseMessage.getSessionID());
        assertEquals(TEST_VERSION, sppResponseMessage.getVersion());
        assertEquals(expectedAttributes, sppResponseMessage.getAttributes());
        assertEquals(SppConstants.INVALID_SPP_CONSTANT, sppResponseMessage.getError());
    }

    /**
     * Verify if the SppSoapResponse message with error attribute is properly parsed.
     */
    @Test
    public void verifySppSoapResponseWithErrorAttribute() {
        SoapObject soapObject = new SoapObject();
        soapObject.addAttribute(SppResponseMessage.SPPStatusAttribute, TEST_ERROR_STATUS);
        soapObject.addAttribute(SppResponseMessage.SPPSessionIDAttribute, TEST_SESSION_ID);
        soapObject.addAttribute(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);
        SoapObject errorInfo = new SoapObject();
        errorInfo.addAttribute(SppResponseMessage.SPPErrorCodeAttribute, TEST_ERROR);
        soapObject.addProperty(SppResponseMessage.SPPErrorProperty, errorInfo);

        Map<String, String> expectedAttributes = new HashMap<>();
        expectedAttributes.put(SppResponseMessage.SPPStatusAttribute, TEST_ERROR_STATUS);
        expectedAttributes.put(SppResponseMessage.SPPSessionIDAttribute, TEST_SESSION_ID);
        expectedAttributes.put(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);

        SppResponseMessage sppResponseMessage = new SppResponseMessage(soapObject,
                SppResponseMessage.MessageType.POST_DEV_DATA_RESPONSE);

        assertEquals(SppConstants.mapStatusStringToInt(TEST_ERROR_STATUS),
                sppResponseMessage.getStatus());
        assertEquals(SppResponseMessage.MessageType.POST_DEV_DATA_RESPONSE,
                sppResponseMessage.getMessageType());
        assertEquals(TEST_SESSION_ID, sppResponseMessage.getSessionID());
        assertEquals(TEST_VERSION, sppResponseMessage.getVersion());
        assertEquals(expectedAttributes, sppResponseMessage.getAttributes());
        assertEquals(SppConstants.mapErrorStringToInt(TEST_ERROR), sppResponseMessage.getError());
    }

    /**
     * Verify if the incomplete SppSoapResponse message will raise
     * {@code IllegalArgumentException}.
     */
    @Test
    public void verifyInCompleteSppSoapResponseRaiseException() {
        SoapObject soapObject = new SoapObject();
        soapObject.addAttribute(SppResponseMessage.SPPStatusAttribute, TEST_STATUS);
        soapObject.addAttribute(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);
        try {
            new SppResponseMessage(soapObject,
                    SppResponseMessage.MessageType.POST_DEV_DATA_RESPONSE);
            fail("expected IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // do nothing
        }
    }
}
