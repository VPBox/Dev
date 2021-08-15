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
package com.android.internal.telephony.uicc;

import android.os.HandlerThread;
import android.os.Message;
import android.os.AsyncResult;

import com.android.internal.telephony.IccPhoneBookInterfaceManager;
import com.android.internal.telephony.TelephonyTest;

import static org.mockito.Matchers.anyObject;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;

import android.test.suitebuilder.annotation.SmallTest;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.Mockito.anyInt;

import java.util.Arrays;
import java.util.List;

public class IccPhoneBookInterfaceManagerTest extends TelephonyTest {

    @Mock
    private AdnRecordCache mAdnRecordCache;
    @Mock
    private AdnRecord mAdnRecord;
    private IccPhoneBookInterfaceManager mIccPhoneBookInterfaceMgr;
    private IccPhoneBookInterfaceManagerHandler mIccPhoneBookInterfaceManagerHandler;
    private List<AdnRecord> mAdnList = Arrays.asList(mAdnRecord);

    private class IccPhoneBookInterfaceManagerHandler extends HandlerThread {

        private IccPhoneBookInterfaceManagerHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mIccPhoneBookInterfaceMgr = new IccPhoneBookInterfaceManager(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(this.getClass().getSimpleName());
        //APP_FAM_3GPP default mPhone is GSM
        doReturn(mSimRecords).when(mPhone).getIccRecords();
        doReturn(mAdnRecordCache).when(mSimRecords).getAdnCache();

        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message response = (Message) invocation.getArguments()[2];
                //set result for load ADN EF
                AsyncResult.forMessage(response).result = mAdnList;
                response.sendToTarget();
                return null;
            }
        }).when(mAdnRecordCache).requestLoadAllAdnLike(anyInt(), anyInt(), (Message) anyObject());

        mIccPhoneBookInterfaceManagerHandler = new IccPhoneBookInterfaceManagerHandler(TAG);
        mIccPhoneBookInterfaceManagerHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        mIccPhoneBookInterfaceManagerHandler.quit();
        super.tearDown();
    }
    @Test
    @SmallTest
    public void testAdnEFLoadWithFailure() {
        List<AdnRecord> adnListResult = mIccPhoneBookInterfaceMgr.getAdnRecordsInEf(
                IccConstants.EF_ADN);
        assertEquals(mAdnList, adnListResult);
        //mock a ADN Ef load failure
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Throwable {
                Message response = (Message) invocation.getArguments()[2];
                AsyncResult.forMessage(response).exception = new RuntimeException();
                response.sendToTarget();
                return null;
            }
        }).when(mAdnRecordCache).requestLoadAllAdnLike(anyInt(), anyInt(), (Message) anyObject());
        List<AdnRecord> adnListResultNew = mIccPhoneBookInterfaceMgr.getAdnRecordsInEf(
                IccConstants.EF_ADN);
        //the later read return null due to exception
        assertNull(adnListResultNew);
        //verify the previous read is not got affected
        assertEquals(mAdnList, adnListResult);
    }
}
