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

package com.android.server.wifi;

import static org.mockito.Mockito.*;

import android.content.Context;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/** Unit tests for {@link WifiInjector}. */
@SmallTest
public class WifiInjectorTest {

    @Mock private Context mContext;
    private WifiInjector mInjector;

    /**
     * Method to initialize mocks for tests.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
    }

    /**
     * Test that attempting to get the instance of the WifiInjector throws an IllegalStateException
     * if it is not initialized.
     */
    @Test(expected = IllegalStateException.class)
    public void testGetInstanceWithUninitializedWifiInjector() {
        WifiInjector.getInstance();
    }

    /**
     * Test that attempting to call the WifiInjector a second time throws an exception.
     */
    @Test(expected = IllegalStateException.class)
    public void testShouldNotBeAbleToCreateMoreThanOneWifiInjector() {
        try {
            WifiInjector willThrowNullPointerException = new WifiInjector(mContext);
        } catch (NullPointerException e) {
        }
        WifiInjector shouldThrowIllegalStateException = new WifiInjector(mContext);
    }

    /**
     * Test that a WifiInjector cannot be created with a null Context.
     */
    @Test(expected = IllegalStateException.class)
    public void testShouldNotCreateWifiInjectorWithNullContext() {
        new WifiInjector(null);
    }
}
