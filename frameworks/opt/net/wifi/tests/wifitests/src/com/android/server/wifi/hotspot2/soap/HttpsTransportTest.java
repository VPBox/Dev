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

import static org.junit.Assert.assertNotNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.net.Network;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.io.IOException;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;

/**
 * Unit tests for {@link HttpsTransport}.
 */
@SmallTest
public class HttpsTransportTest {
    private static final String TEST_URL = "https://127.0.0.1:12345/index.htm";
    private URL mUrl;
    private HttpsTransport mHttpsTransport;

    @Mock Network mNetwork;
    @Mock HttpsURLConnection mHttpsURLConnection;

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mUrl = new URL(TEST_URL);
        when(mNetwork.openConnection(any(URL.class))).thenReturn(mHttpsURLConnection);
        mHttpsTransport = HttpsTransport.createInstance(mNetwork, mUrl);
    }

    /**
     * Verify if the getServiceConnection never returns {@code null}.
     */
    @Test
    public void verifyGetServiceConnection() throws IOException {
        assertNotNull(mHttpsTransport.getServiceConnection());
    }
}
