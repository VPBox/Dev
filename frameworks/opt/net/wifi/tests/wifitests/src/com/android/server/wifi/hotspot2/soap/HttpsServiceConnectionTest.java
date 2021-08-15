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
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.net.Network;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.ksoap2.HeaderProperty;
import org.mockito.Mock;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;

/**
 * Unit tests for {@link HttpsServiceConnection}.
 */
@SmallTest
public class HttpsServiceConnectionTest {
    private static final String TEST_URL = "https://127.0.0.1:12345/index.htm";

    private HttpsServiceConnection mHttpsServiceConnection;
    private URL mUrl;

    @Mock Network mNetwork;
    @Mock HttpsURLConnection mHttpsURLConnection;

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);

        mUrl = new URL(TEST_URL);

        when(mHttpsURLConnection.getURL()).thenReturn(mUrl);
        when(mNetwork.openConnection(any(URL.class))).thenReturn(mHttpsURLConnection);
        mHttpsServiceConnection = new HttpsServiceConnection(mNetwork, mUrl);
    }

    /**
     * Verify if the connect function actually calls {@link HttpsURLConnection#connect()}
     */
    @Test
    public void verifyConnect() throws IOException {
        mHttpsServiceConnection.connect();
        verify(mHttpsURLConnection).connect();
    }

    /**
     * Verify if the disconnect function actually calls {@link HttpsURLConnection#disconnect()}
     */
    @Test
    public void verifyDisconnect() {
        mHttpsServiceConnection.disconnect();
        verify(mHttpsURLConnection).disconnect();
    }

    /**
     * Verify if the response code returned is same with response code from
     * {@link HttpsURLConnection#getResponseCode()}.
     */
    @Test
    public void verifyGetResponseProperties() {
        Map<String, List<String>> testProperties = new HashMap<>();
        testProperties.put("test1", Arrays.asList("value1", "value2"));
        testProperties.put("test2", Arrays.asList("value3"));
        when(mHttpsURLConnection.getHeaderFields()).thenReturn(testProperties);

        Map<String, List<String>> actualResultsProperties = new HashMap<>();
        List<HeaderProperty> actualResults = mHttpsServiceConnection.getResponseProperties();
        actualResults.forEach(h -> {
            if (actualResultsProperties.containsKey(h.getKey())) {
                actualResultsProperties.get(h.getKey()).add(h.getValue());
            } else {
                actualResultsProperties.put(h.getKey(),
                        new ArrayList<>(Arrays.asList(h.getValue())));
            }
        });
        assertEquals(testProperties, actualResultsProperties);
    }

    /**
     * Verify if the response code returned is same with response code from
     * {@link HttpsURLConnection#getResponseCode()}.
     */
    @Test
    public void verifyGetResponseCode() throws IOException {
        int testResponseCode = 400;
        when(mHttpsURLConnection.getResponseCode()).thenReturn(testResponseCode);

        assertEquals(testResponseCode, mHttpsServiceConnection.getResponseCode());
    }

    /**
     * Verify if the InputStream returned is same with InputStream from
     * {@link HttpsURLConnection#getInputStream()}.
     */
    @Test
    public void verifyOpenInputStream() throws IOException {
        InputStream testInputStream = new InputStream() {
            @Override
            public int read() {
                return 0;
            }
        };

        when(mHttpsURLConnection.getInputStream()).thenReturn(testInputStream);

        assertEquals(testInputStream, mHttpsServiceConnection.openInputStream());
    }

    /**
     * Verify if the OutputStream returned is same with OutputStream from
     * {@link HttpsURLConnection#getOutputStream()}.
     */
    @Test
    public void verifyOpenOutputStream() throws IOException {
        OutputStream testOutputStream = new OutputStream() {
            @Override
            public void write(int b) {

            }
        };

        when(mHttpsURLConnection.getOutputStream()).thenReturn(testOutputStream);

        assertEquals(testOutputStream, mHttpsServiceConnection.openOutputStream());
    }

    /**
     * Verify if the InputStream returned is same with InputStream from
     * {@link HttpsURLConnection#getErrorStream()}.
     */
    @Test
    public void verifyGetErrorStream() {
        InputStream testInputStream = new InputStream() {
            @Override
            public int read() {
                return 0;
            }
        };

        when(mHttpsURLConnection.getErrorStream()).thenReturn(testInputStream);

        assertEquals(testInputStream, mHttpsServiceConnection.getErrorStream());
    }

    /**
     * Verify if the host returned is same with host from test URL.
     */
    @Test
    public void verifyGetHost() {
        assertEquals(mUrl.getHost(), mHttpsServiceConnection.getHost());
    }

    /**
     * Verify if the port returned is same with port from test URL.
     */
    @Test
    public void verifyGetPort() {
        assertEquals(mUrl.getPort(), mHttpsServiceConnection.getPort());
    }

    /**
     * Verify if the path returned is same with path from test URL.
     */
    @Test
    public void verifyGetPath() {
        assertEquals(mUrl.getPath(), mHttpsServiceConnection.getPath());
    }

    /**
     * Verify if the setRequestProperty never get called for Connection:close property.
     */
    @Test
    public void verifyNotUpdatingRequestPropertyForIgnorableProperty() {
        String testName = "Connection";
        String testValue = "close";
        mHttpsServiceConnection.setRequestProperty(testName, testValue);

        verify(mHttpsURLConnection, never()).setRequestProperty(any(String.class),
                any(String.class));
    }

    /**
     * Verify if the setRequestProperty get called for other properties but "Connection:close"
     * property.
     */
    @Test
    public void verifyCallingSetRequestPropertyForProperty() {
        String testName = "Content-Type";
        String testValue = "application/x-www-form-urlencoded";
        mHttpsServiceConnection.setRequestProperty(testName, testValue);

        verify(mHttpsURLConnection).setRequestProperty(any(String.class),
                any(String.class));
    }
}
