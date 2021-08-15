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

package com.android.server.wifi.hotspot2.soap.command;

import static com.android.server.wifi.hotspot2.soap.command.PpsMoData.ADD_MO_COMMAND;
import static com.android.server.wifi.hotspot2.soap.command.PpsMoData.ATTRIBUTE_MANAGEMENT_TREE_URI;
import static com.android.server.wifi.hotspot2.soap.command.PpsMoData.ATTRIBUTE_MO_URN;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapPrimitive;

/**
 * Unit tests for {@link PpsMoData}.
 */
@SmallTest
public class PpsMoDataTest {
    private static final String TEST_PPS_MO_XML = "<MgmtTree>test</MgmtTree>";
    private static final String TEST_TREE_URI = "testTreeURI";
    private static final String TEST_MO_URN = "testMoURN";

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
    }

    /**
     * Verify if a message is valid format, it will return a PPS MO XML.
     */
    @Test
    public void verifyGetPpsMo() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName(ADD_MO_COMMAND);
        SoapPrimitive soapPrimitive = new SoapPrimitive("namespace", "name", TEST_PPS_MO_XML);
        soapPrimitive.addAttribute(ATTRIBUTE_MANAGEMENT_TREE_URI, TEST_TREE_URI);
        soapPrimitive.addAttribute(ATTRIBUTE_MO_URN, TEST_MO_URN);
        propertyInfo.setValue(soapPrimitive);

        assertEquals(TEST_PPS_MO_XML, PpsMoData.createInstance(propertyInfo).getPpsMoTree());
    }

    /**
     * Verify if a message does not have PPS MO XML, it will return {@code null}.
     */
    @Test
    public void verifyMissingPpsMoReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName(ADD_MO_COMMAND);
        SoapPrimitive soapPrimitive = new SoapPrimitive("namespace", "name", TEST_PPS_MO_XML);
        soapPrimitive.addAttribute(ATTRIBUTE_MANAGEMENT_TREE_URI, TEST_TREE_URI);
        soapPrimitive.addAttribute(ATTRIBUTE_MO_URN, TEST_MO_URN);
        propertyInfo.setValue(null);

        assertNull(PpsMoData.createInstance(propertyInfo));
    }

    /**
     * Verify if a message is missing {@link PpsMoData#ATTRIBUTE_MANAGEMENT_TREE_URI}, it will
     * return {@code null}.
     */
    @Test
    public void verifyMissingTreeURIAttributeReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName(ADD_MO_COMMAND);
        SoapPrimitive soapPrimitive = new SoapPrimitive("namespace", "name", TEST_PPS_MO_XML);
        soapPrimitive.addAttribute(ATTRIBUTE_MO_URN, TEST_MO_URN);
        propertyInfo.setValue(soapPrimitive);

        assertNull(PpsMoData.createInstance(propertyInfo));
    }

    /**
     * Verify if a message is missing {@link PpsMoData#ATTRIBUTE_MO_URN}, it will return
     * {@code null}.
     */
    @Test
    public void verifyMissingMoUrnAttributeReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName(ADD_MO_COMMAND);
        SoapPrimitive soapPrimitive = new SoapPrimitive("namespace", "name", TEST_PPS_MO_XML);
        soapPrimitive.addAttribute(ATTRIBUTE_MANAGEMENT_TREE_URI, TEST_TREE_URI);
        propertyInfo.setValue(soapPrimitive);

        assertNull(PpsMoData.createInstance(propertyInfo));
    }

    /**
     * Verify if a message that is not for addMO command, it will return {@code null}.
     */
    @Test
    public void verifyNonAddMoCommandMessageReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName("InvalidCommand");

        assertNull(PpsMoData.createInstance(propertyInfo));
    }
}
