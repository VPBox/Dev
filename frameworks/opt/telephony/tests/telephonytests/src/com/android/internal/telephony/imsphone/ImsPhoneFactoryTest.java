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
package com.android.internal.telephony.imsphone;

import android.os.HandlerThread;
import android.test.suitebuilder.annotation.SmallTest;
import com.android.internal.telephony.PhoneNotifier;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

public class ImsPhoneFactoryTest extends TelephonyTest {

    @Mock
    private PhoneNotifier mPhoneNotifer;
    private ImsPhone mImsPhoneUT;
    private ImsPhoneFactoryHandler mImsPhoneFactoryHandler;

    private class ImsPhoneFactoryHandler extends HandlerThread {

        private ImsPhoneFactoryHandler(String name) {
            super(name);
        }
        @Override
        public void onLooperPrepared() {
            mImsPhoneUT = ImsPhoneFactory.makePhone(mContext, mPhoneNotifer, mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        mImsPhoneFactoryHandler = new ImsPhoneFactoryHandler(this.getClass().getSimpleName());
        mImsPhoneFactoryHandler.start();

        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mImsPhoneFactoryHandler.quit();
        super.tearDown();
    }

    @Test @SmallTest
    public void testMakeImsPhone() throws Exception {
        assertNotNull(mImsPhoneUT);
        assertEquals(mPhone, mImsPhoneUT.getDefaultPhone());

        mImsPhoneUT.notifyDataActivity();
        verify(mPhoneNotifer, times(1)).notifyDataActivity(eq(mImsPhoneUT));
    }
}
