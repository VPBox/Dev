/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.server.wifi.util;

import static com.google.common.truth.Truth.assertThat;

import static org.mockito.Mockito.when;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.time.Duration;

/**
 * Unit tests for {@link TimedQuotaManager}.
 */
@SmallTest
public class TimedQuotaManagerTest {

    private static final long DAY_MILLIS = Duration.ofDays(1).toMillis();

    @Mock private Clock mClock;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mClock.getElapsedSinceBootMillis()).thenReturn(100000000L);
    }

    /**
     * Tests that {@link TimedQuotaManager#requestQuota()} returns true before the quota is exceeded
     * and returns false after it is exceeded. Tests that upon entering a new time period, the quota
     * is reset correctly.
     */
    @Test
    public void exhaustQuota_newDay_exhaustQuotaAgain() {
        TimedQuotaManager qm = new TimedQuotaManager(mClock, 10, DAY_MILLIS);

        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isTrue();
        }
        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isFalse();
        }

        long now = mClock.getElapsedSinceBootMillis();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(now + DAY_MILLIS + 1000);

        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isTrue();
        }
        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isFalse();
        }
    }

    /**
     * Tests that {@link TimedQuotaManager#requestQuota()} returns true before the quota is exceeded
     * and returns false after it is exceeded. Tests when advancing time within the same time
     * period, the quota is still enforced.
     */
    @Test
    public void exhaustQuota_sameDay_stillExhausted() {
        TimedQuotaManager qm = new TimedQuotaManager(mClock, 10, DAY_MILLIS);

        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isTrue();
        }
        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isFalse();
        }

        long now = mClock.getElapsedSinceBootMillis();
        when(mClock.getElapsedSinceBootMillis()).thenReturn(now + DAY_MILLIS - 1000);
        for (int i = 0; i < 10; i++) {
            assertThat(qm.requestQuota()).isFalse();
        }
    }
}
