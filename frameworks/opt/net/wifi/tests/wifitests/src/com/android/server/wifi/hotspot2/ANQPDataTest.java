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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.hotspot2.anqp.ANQPElement;
import com.android.server.wifi.hotspot2.anqp.Constants;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.ANQPData}.
 *
 * TODO(b/33000864): add more test once the ANQP elements cleanup are completed, which will
 * allow easy construction of ANQP elements for testing.
 */
@SmallTest
public class ANQPDataTest {
    @Mock Clock mClock;

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        // Returning the initial timestamp.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(0L);
    }

    /**
     * Verify creation of ANQPData with null elements.
     *
     * @throws Exception
     */
    @Test
    public void createWithNullElements() throws Exception {
        ANQPData data = new ANQPData(mClock, null);
        Map<Constants.ANQPElementType, ANQPElement> elements = data.getElements();
        assertTrue(elements.isEmpty());
    }

    /**
     * Verify the data expiration behavior.
     *
     * @throws Exception
     */
    @Test
    public void verifyExpiration() throws Exception {
        ANQPData data = new ANQPData(mClock, null);
        assertFalse(data.expired(ANQPData.DATA_LIFETIME_MILLISECONDS - 1));
        assertTrue(data.expired(ANQPData.DATA_LIFETIME_MILLISECONDS));
    }
}
