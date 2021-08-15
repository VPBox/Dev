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
package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;

import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Test;

public class CallStateExceptionTest {
    private CallStateException mCallStateException;

    @After
    public void tearDown() throws Exception {
        mCallStateException = null;
    }

    @Test
    @SmallTest
    public void testCallStateExceptionDefault() {
        mCallStateException = new CallStateException("sanity test");
        assertEquals("sanity test", mCallStateException.getMessage());
        assertEquals(mCallStateException.ERROR_INVALID, mCallStateException.getError());
    }

    @Test
    @SmallTest
    public void testCallStateExceptionWithErrCode() {
        mCallStateException = new CallStateException(mCallStateException.ERROR_OUT_OF_SERVICE,
                                                     "sanity test with err code");
        assertEquals("sanity test with err code", mCallStateException.getMessage());
        assertEquals(mCallStateException.ERROR_OUT_OF_SERVICE, mCallStateException.getError());
    }
}
