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

package com.android.internal.telephony.dataconnection;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.clearInvocations;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.verify;

import android.os.HandlerThread;
import android.telephony.data.ApnSetting;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;

import java.util.Objects;

public class DataEnabledSettingsTest extends TelephonyTest {

    private DataEnabledSettings mDataEnabledSettingsUT;

    private DataEnabledSettingsTestHandler mDataEnabledSettingsTestHandler;

    private String mRules = "";

    private class DataEnabledSettingsTestHandler extends HandlerThread {

        private DataEnabledSettingsTestHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mDataEnabledSettingsUT = new DataEnabledSettings(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());

        doReturn(mRules).when(mSubscriptionController).getDataEnabledOverrideRules(anyInt());

        doAnswer(invocation -> {
            String rules = (String) invocation.getArguments()[1];
            boolean changed = !Objects.equals(mRules, rules);
            mRules = rules;
            return changed;
        }).when(mSubscriptionController).setDataEnabledOverrideRules(anyInt(), anyString());

        mDataEnabledSettingsTestHandler = new DataEnabledSettingsTestHandler(
                getClass().getSimpleName());
        mDataEnabledSettingsTestHandler.start();
        waitUntilReady();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testSetDataAllowedInVoiceCall() throws Exception {
        mDataEnabledSettingsUT.setAllowDataDuringVoiceCall(true);
        ArgumentCaptor<String> stringCaptor = ArgumentCaptor.forClass(String.class);
        verify(mSubscriptionController).setDataEnabledOverrideRules(anyInt(),
                stringCaptor.capture());
        assertEquals("*=nonDefault&inVoiceCall&DefaultDataOn&dsdsEnabled", stringCaptor.getValue());

        clearInvocations(mSubscriptionController);

        mDataEnabledSettingsUT.setAllowDataDuringVoiceCall(false);
        verify(mSubscriptionController).setDataEnabledOverrideRules(anyInt(),
                stringCaptor.capture());
        assertEquals("", stringCaptor.getValue());
    }

    @Test
    @SmallTest
    public void testSetAlwaysAllowMmsData() throws Exception {
        mDataEnabledSettingsUT.setUserDataEnabled(false);
        assertTrue(mDataEnabledSettingsUT.setAlwaysAllowMmsData(true));
        ArgumentCaptor<String> stringCaptor = ArgumentCaptor.forClass(String.class);
        verify(mSubscriptionController).setDataEnabledOverrideRules(anyInt(),
                stringCaptor.capture());
        assertEquals("mms=unconditionally", stringCaptor.getValue());
        assertTrue(mDataEnabledSettingsUT.isDataEnabled(ApnSetting.TYPE_MMS));

        clearInvocations(mSubscriptionController);

        assertTrue(mDataEnabledSettingsUT.setAlwaysAllowMmsData(false));
        verify(mSubscriptionController).setDataEnabledOverrideRules(anyInt(),
                stringCaptor.capture());
        assertEquals("", stringCaptor.getValue());
        assertFalse(mDataEnabledSettingsUT.isDataEnabled(ApnSetting.TYPE_MMS));

        mDataEnabledSettingsUT.setUserDataEnabled(true);
        assertTrue(mDataEnabledSettingsUT.isDataEnabled(ApnSetting.TYPE_MMS));
    }
}
