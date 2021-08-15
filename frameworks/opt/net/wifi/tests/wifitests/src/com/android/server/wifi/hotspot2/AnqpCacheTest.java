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

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.hotspot2.ANQPData;
import com.android.server.wifi.hotspot2.AnqpCache;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.AnqpCache}.
 *
 * TODO(b/33000864): add more test once the ANQP elements cleanup are completed, which will
 * allow easy construction of ANQP elements for testing.
 */
@SmallTest
public class AnqpCacheTest {
    private static final ANQPNetworkKey ENTRY_KEY = new ANQPNetworkKey("test", 0L, 0L, 1);

    @Mock Clock mClock;
    AnqpCache mCache;

    /**
     * Sets up test.
     */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        // Returning the initial timestamp.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(0L);
        mCache = new AnqpCache(mClock);
    }

    /**
     * Verify expectation for addEntry and getEntry.
     *
     * @throws Exception
     */
    @Test
    public void addAndGetEntry() throws Exception {
        mCache.addEntry(ENTRY_KEY, null);
        ANQPData data = mCache.getEntry(ENTRY_KEY);
        assertNotNull(data);
        assertTrue(data.getElements().isEmpty());
    }

    /**
     * Verify that getting a non-existing entry will return a null.
     *
     * @throws Exception
     */
    @Test
    public void getNonExistEntry() throws Exception {
        assertNull(mCache.getEntry(ENTRY_KEY));
    }

    /**
     * Verify the expectation for the sweep function (expired entries will be removed).
     *
     * @throws Exception
     */
    @Test
    public void sweepRemoveExpiredEntry() throws Exception {
        mCache.addEntry(ENTRY_KEY, null);

        // Sweep the cache when the entry is not expired.
        when(mClock.getElapsedSinceBootMillis())
                .thenReturn(AnqpCache.CACHE_SWEEP_INTERVAL_MILLISECONDS);
        mCache.sweep();
        assertNotNull(mCache.getEntry(ENTRY_KEY));

        // Sweep the cache when the entry is expired.
        when(mClock.getElapsedSinceBootMillis()).thenReturn(ANQPData.DATA_LIFETIME_MILLISECONDS);
        mCache.sweep();
        assertNull(mCache.getEntry(ENTRY_KEY));
    }
}
