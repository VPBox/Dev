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
import static org.junit.Assert.assertNull;

import androidx.test.filters.SmallTest;

import org.junit.Test;
import org.ksoap2.serialization.SoapObject;

/**
 * Unit tests for {@link ExchangeCompleteMessage}.
 */
@SmallTest
public class ExchangeCompleteMessageTest {
    private static final String TEST_STATUS = "OK";
    private static final String TEST_SESSION_ID = "D215D696517BA138F1D28442DF0F4E07";
    private static final String TEST_VERSION = "1.0";

    private ExchangeCompleteMessage mExchangeCompleteMessage;

    /**
     * Verify if the ExchangeCompleteMessage message is properly parsed.
     */
    @Test
    public void verifyValidExchangeComplete() {
        SoapObject response = new SoapObject();
        response.addAttribute(SppResponseMessage.SPPStatusAttribute, TEST_STATUS);
        response.addAttribute(SppResponseMessage.SPPSessionIDAttribute, TEST_SESSION_ID);
        response.addAttribute(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);

        mExchangeCompleteMessage = ExchangeCompleteMessage.createInstance(response);

        assertNotNull(mExchangeCompleteMessage);
    }

    /**
     * Verify if the exchangeComplete message missing session id will return {@code null}
     */
    @Test
    public void verifyInvalidExchangeCompleteReturnNull() {
        SoapObject response = new SoapObject();
        response.addAttribute(SppResponseMessage.SPPStatusAttribute, TEST_STATUS);
        response.addAttribute(SppResponseMessage.SPPVersionAttribute, TEST_VERSION);
        mExchangeCompleteMessage = ExchangeCompleteMessage.createInstance(response);

        assertNull(mExchangeCompleteMessage);
    }
}
