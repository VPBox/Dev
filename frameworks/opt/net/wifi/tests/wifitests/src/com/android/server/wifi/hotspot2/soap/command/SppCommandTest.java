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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.ksoap2.serialization.PropertyInfo;
import org.ksoap2.serialization.SoapObject;

/**
 * Unit tests for {@link SppCommand}.
 */
@SmallTest
public class SppCommandTest {
    private static final String EXEC = "exec";
    private static final String BROWSER_COMMAND = "launchBrowserToURI";
    private static final String GET_CERT_COMMAND = "getCertificate";
    private static final String TEST_URL = "https://127.0.0.1:12345/index.htm";

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
    }

    /**
     * Verify if the LaunchBrowserCommand message is properly parsed.
     */
    @Test
    public void verifyLaunchBrowserCommand() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName(EXEC);
        propertyInfo.setValue(new SoapObject().addProperty(BROWSER_COMMAND, TEST_URL));

        SppCommand result = SppCommand.createInstance(propertyInfo);

        assertEquals(SppCommand.CommandId.EXEC, result.getSppCommandId());
        assertEquals(SppCommand.ExecCommandId.BROWSER, result.getExecCommandId());
        assertEquals(BrowserUri.class, result.getCommandData().getClass());
        assertEquals(TEST_URL, ((BrowserUri) result.getCommandData()).getUri());
    }

    /**
     * Verify if the other command types, but LaunchBrowserCommand will return {@code null}
     */
    @Test
    public void verifyOtherCommandTypesReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        SoapObject subCommand = new SoapObject();
        propertyInfo.setName(EXEC);
        propertyInfo.setValue(subCommand);
        subCommand.addProperty(GET_CERT_COMMAND, "");

        SppCommand result = SppCommand.createInstance(propertyInfo);

        assertNull(result.getCommandData());
    }

    /**
     * Verify if the incomplete command message will return {@code null}.
     */
    @Test
    public void verifyIncompleteCommandReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        SoapObject subCommand = new SoapObject();
        propertyInfo.setName(EXEC);
        propertyInfo.setValue(subCommand);

        assertNull(SppCommand.createInstance(propertyInfo));
    }

    /**
     * Verify if the invalid command message will return {@code null}.
     */
    @Test
    public void verifyInvalidCommandReturnNull() {
        PropertyInfo propertyInfo = new PropertyInfo();
        propertyInfo.setName("invalidCommand");

        SppCommand result = SppCommand.createInstance(propertyInfo);
        assertNull(result);
    }
}
