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

package com.android.server.wifi.hotspot2.anqp;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.DomainNameElement}.
 */
@SmallTest
public class DomainNameElementTest {
    private static final String TEST_DOMAIN_NAME1 = "test1.com";
    private static final String TEST_DOMAIN_NAME2 = "test2.com";

    /**
     * Helper function for appending a Domain Name to an output stream.
     *
     * @param stream Stream to write to
     * @param domain The domain name string
     * @throws IOException
     */
    private void appendDomain(ByteArrayOutputStream stream, String domain) throws IOException {
        byte[] domainBytes = domain.getBytes(StandardCharsets.ISO_8859_1);
        stream.write((byte) domainBytes.length);
        stream.write(domainBytes);
    }

    /**
     * Helper function for generating test data.
     *
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getTestData(String[] domains) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (String domain : domains) {
            appendDomain(stream, domain);
        }
        return stream.toByteArray();
    }

    /**
     * Verify that a DomainNameElement with empty domain list will be returned when parsing an
     * empty buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseEmptyBuffer() throws Exception {
        assertTrue(DomainNameElement.parse(ByteBuffer.allocate(0)).getDomains().isEmpty());
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(new String[] {TEST_DOMAIN_NAME1}));
        buffer.limit(buffer.remaining() - 1);
        DomainNameElement.parse(buffer);
    }

    /**
     * Verify that a DomainNameElement with expected domain list will be returned when parsing a
     * buffer contained valid domain name list.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithValidDomainNames() throws Exception {
        byte[] testData = getTestData(new String[] {TEST_DOMAIN_NAME1, TEST_DOMAIN_NAME2});
        ByteBuffer buffer = ByteBuffer.wrap(testData);

        // Setup expected element.
        List<String> domainList = new ArrayList<>();
        domainList.add(TEST_DOMAIN_NAME1);
        domainList.add(TEST_DOMAIN_NAME2);
        DomainNameElement expectedElement = new DomainNameElement(domainList);

        assertEquals(expectedElement, DomainNameElement.parse(buffer));
    }
}
