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
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.doReturn;

import android.telephony.data.ApnSetting;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class DataEnabledOverrideTest extends TelephonyTest {

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testCreateByRules() throws Exception {
        DataEnabledOverride deo1 = new DataEnabledOverride(
                "mms=nonDefault, default=inVoiceCall&nonDefault");
        DataEnabledOverride deo2 = new DataEnabledOverride(
                "mms=nonDefault, default=inVoiceCall");
        DataEnabledOverride deo3 = new DataEnabledOverride(
                "default=inVoiceCall&nonDefault, mms=nonDefault");
        assertEquals(deo1, deo3);
        assertNotEquals(deo1, deo2);
    }

    @Test
    @SmallTest
    public void testOverrideEnabled() throws Exception {
        DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                "mms=nonDefault, default=inVoiceCall&nonDefault");
        doReturn(1).when(mPhone).getSubId();
        doReturn(2).when(mSubscriptionController).getDefaultSmsSubId();
        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_MMS));

        doReturn(PhoneConstants.State.IDLE).when(mPhone).getState();

        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));

        doReturn(PhoneConstants.State.OFFHOOK).when(mPhone).getState();

        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));
    }

    @Test
    @SmallTest
    public void testGetRules() throws Exception {
        DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                "mms=nonDefault, default=inVoiceCall&nonDefault");
        String rules = dataEnabledOverride.getRules();
        assertEquals(dataEnabledOverride, new DataEnabledOverride(rules));
    }

    @Test
    @SmallTest
    public void testUpdateRules() throws Exception {
        DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                "mms=nonDefault, default=inVoiceCall&nonDefault");
        doReturn(1).when(mPhone).getSubId();
        doReturn(2).when(mSubscriptionController).getDefaultSmsSubId();
        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_MMS));

        doReturn(PhoneConstants.State.IDLE).when(mPhone).getState();

        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));

        doReturn(PhoneConstants.State.OFFHOOK).when(mPhone).getState();

        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));

        dataEnabledOverride.updateRules("");

        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_MMS));
        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));
    }

    @Test
    @SmallTest
    public void testAlwaysEnabled() throws Exception {
        DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                "mms   =unconditionally,    default=  unconditionally ,   ");
        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_MMS));
        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));

        dataEnabledOverride.updateRules("");

        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_MMS));
        assertFalse(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));
    }

    @Test
    @SmallTest
    public void testAllApnTypesInRule() throws Exception {
        DataEnabledOverride dataEnabledOverride = new DataEnabledOverride("*=inVoiceCall");
        doReturn(PhoneConstants.State.OFFHOOK).when(mPhone).getState();

        assertTrue(dataEnabledOverride.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_FOTA));
    }

    @Test
    @SmallTest
    public void testInvalidRules() throws Exception {
        try {
            DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                    "default=xyz");
            fail("Invalid conditions but not threw IllegalArgumentException.");
        } catch (IllegalArgumentException ex) {

        }

        try {
            DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                    "mms=");
            fail("Invalid conditions but not threw IllegalArgumentException.");
        } catch (IllegalArgumentException ex) {

        }

        try {
            DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                    "abc=nonDefault");
            fail("Invalid APN type but not threw IllegalArgumentException.");
        } catch (IllegalArgumentException ex) {

        }

        try {
            DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                    " =nonDefault");
            fail("Invalid APN type but not threw IllegalArgumentException.");
        } catch (IllegalArgumentException ex) {

        }

        try {
            DataEnabledOverride dataEnabledOverride = new DataEnabledOverride(
                    "Invalid rule");
            fail("Invalid rule but not threw IllegalArgumentException.");
        } catch (IllegalArgumentException ex) {

        }
    }

    @Test
    @SmallTest
    public void testSetAlwaysAllowMms() throws Exception {
        DataEnabledOverride deo = new DataEnabledOverride("");
        deo.setAlwaysAllowMms(true);
        assertTrue(deo.shouldOverrideDataEnabledSettings(mPhone, ApnSetting.TYPE_MMS));
        deo.setAlwaysAllowMms(false);
        assertFalse(deo.shouldOverrideDataEnabledSettings(mPhone, ApnSetting.TYPE_MMS));
    }

    @Test
    @SmallTest
    public void testSetDataAllowedInVoiceCall() throws Exception {
        DataEnabledOverride deo = new DataEnabledOverride("");
        deo.setDataAllowedInVoiceCall(true);
        assertFalse(deo.getRules(), deo.shouldOverrideDataEnabledSettings(mPhone,
                ApnSetting.TYPE_DEFAULT));
        assertTrue(deo.isDataAllowedInVoiceCall());
        doReturn(1).when(mPhone).getSubId();
        doReturn(2).when(mSubscriptionController).getDefaultSmsSubId();

        doReturn(PhoneConstants.State.OFFHOOK).when(mPhone).getState();
        deo.setDataAllowedInVoiceCall(false);
        assertFalse(deo.getRules(), deo.shouldOverrideDataEnabledSettings(
                mPhone, ApnSetting.TYPE_DEFAULT));
        assertFalse(deo.isDataAllowedInVoiceCall());
    }
}
