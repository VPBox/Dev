/* Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

package com.android.internal.telephony.uicc;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.*;

import android.os.HandlerThread;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class IccRecordsTest extends TelephonyTest {

    private IccRecords mIccRecords;

    private class IccRecordsTestHandler extends HandlerThread {
        private IccRecordsTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mIccRecords = new SIMRecords(mUiccCardApplication3gpp, mContext, mSimulatedCommands);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        new IccRecordsTestHandler(TAG).start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    public void testDisposeCallsUnregisterForIccRefresh() {
        // verify called below when IccRecords object is created
        verify(mSimulatedCommandsVerifier).registerForIccRefresh(eq(mIccRecords),
                    eq(IccRecords.EVENT_REFRESH), isNull());
        mIccRecords.dispose();
        // verify called within dispose
        verify(mSimulatedCommandsVerifier).unregisterForIccRefresh(eq(mIccRecords));

    }

    @Test
    public void testSetImsiInvalid() {
        mIccRecords.setImsi("0123456789FFFFFF");
        assertEquals(mIccRecords.getIMSI(), "0123456789");
        mIccRecords.setImsi("0123456789ffffff");
        assertEquals(mIccRecords.getIMSI(), "0123456789");
        mIccRecords.setImsi("ffffff");
        assertEquals(mIccRecords.getIMSI(), null);
        mIccRecords.setImsi("12F34F567890");
        assertEquals(mIccRecords.getIMSI(), null);
        mIccRecords.setImsi("123456ABCDEF");
        assertEquals(mIccRecords.getIMSI(), null);
    }
}
