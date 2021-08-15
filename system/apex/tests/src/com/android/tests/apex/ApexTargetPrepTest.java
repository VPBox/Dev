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

package com.android.tests.apex;

import com.android.tradefed.testtype.DeviceJUnit4ClassRunner;
import com.android.tradefed.testtype.junit4.BaseHostJUnit4Test;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * A trivial test that makes apex-targetprep-tests a real test.
 */
@RunWith(DeviceJUnit4ClassRunner.class)
public class ApexTargetPrepTest extends BaseHostJUnit4Test {

    @Before
    public synchronized void setUp() throws Exception {
    }

    @Test
    public void trivialTest() {
        Assert.assertTrue("", true);
    }

    @After
    public void tearDown() throws Exception {
    }
}
