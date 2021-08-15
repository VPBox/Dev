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

/**
 * Unit tests for {@link BrowserUri}.
 */
@SmallTest
public class BrowserUriTest {
    private static final String BROWSER_COMMAND = "launchBrowserToURI";
    private static final String TEST_URL = "https://127.0.0.1:12345/index.htm";
    private BrowserUri mBrowserUri;
    private PropertyInfo mPropertyInfo;

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mPropertyInfo = new PropertyInfo();
        mPropertyInfo.setName(BROWSER_COMMAND);
        mPropertyInfo.setValue(TEST_URL);
    }

    /**
     * Verify if the returned Uri is same with the TEST_URL
     */
    @Test
    public void verifyGetUri() {
        mBrowserUri = BrowserUri.createInstance(mPropertyInfo);
        assertEquals(TEST_URL, mBrowserUri.getUri());
    }

    /**
     * Verify if the invalid message will return {@code null}
     */
    @Test
    public void verifyInvalidMessageReturnNull() {
        mPropertyInfo.setName("UnknownCommand");
        mBrowserUri = BrowserUri.createInstance(mPropertyInfo);

        assertNull(mBrowserUri);
    }
}
