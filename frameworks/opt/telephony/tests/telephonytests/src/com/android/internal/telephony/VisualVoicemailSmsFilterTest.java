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
 * limitations under the License
 */

package com.android.internal.telephony;

import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.when;

import android.content.ComponentName;
import android.content.Context;
import android.content.res.Resources;
import android.telecom.PhoneAccountHandle;
import android.telephony.TelephonyManager;
import android.telephony.VisualVoicemailSmsFilterSettings;

import com.android.internal.telephony.VisualVoicemailSmsFilter.PhoneAccountHandleConverter;

import junit.framework.TestCase;

import org.mockito.Mockito;

import java.util.Arrays;

/**
 * Unit test for {@link VisualVoicemailSmsFilter}
 */
public class VisualVoicemailSmsFilterTest extends TestCase {

    /**
     * PDU for the following message:
     * <p>originating number: 129
     * <p>message: //VVM:SYNC:ev=NM;id=143;c=6;t=v;s=11111111111;dt=07/03/2017 18:17 -0800;l=4
     */
    private static final byte[][] SYNC_PDU = {{
            (byte) 0x07, (byte) 0x91, (byte) 0x41, (byte) 0x50, (byte) 0x74, (byte) 0x02,
            (byte) 0x50, (byte) 0xF5, (byte) 0x44, (byte) 0x03, (byte) 0xC9, (byte) 0x21,
            (byte) 0xF9, (byte) 0x00, (byte) 0x00, (byte) 0x71, (byte) 0x30, (byte) 0x70,
            (byte) 0x81, (byte) 0x71, (byte) 0x81, (byte) 0x2B, (byte) 0x53, (byte) 0x06,
            (byte) 0x05, (byte) 0x04, (byte) 0x07, (byte) 0x10, (byte) 0x01, (byte) 0x01,
            (byte) 0xAF, (byte) 0x97, (byte) 0xD5, (byte) 0xDA, (byte) 0xD4, (byte) 0x4D,
            (byte) 0xB3, (byte) 0xCE, (byte) 0xA1, (byte) 0xAE, (byte) 0x6C, (byte) 0xEF,
            (byte) 0x39, (byte) 0x9B, (byte) 0xBB, (byte) 0x34, (byte) 0xB9, (byte) 0x17,
            (byte) 0xA3, (byte) 0xCD, (byte) 0x76, (byte) 0xE3, (byte) 0x9E, (byte) 0x6D,
            (byte) 0x47, (byte) 0xEF, (byte) 0xD9, (byte) 0x77, (byte) 0xF3, (byte) 0x5E,
            (byte) 0x2C, (byte) 0x16, (byte) 0x8B, (byte) 0xC5, (byte) 0x62, (byte) 0xB1,
            (byte) 0x58, (byte) 0x2C, (byte) 0x16, (byte) 0xDB, (byte) 0x91, (byte) 0xE9,
            (byte) 0x3D, (byte) 0xD8, (byte) 0xED, (byte) 0x05, (byte) 0x9B, (byte) 0xBD,
            (byte) 0x64, (byte) 0xB0, (byte) 0xD8, (byte) 0x0D, (byte) 0x14, (byte) 0xC3,
            (byte) 0xE9, (byte) 0x62, (byte) 0x37, (byte) 0x50, (byte) 0x0B, (byte) 0x86,
            (byte) 0x83, (byte) 0xC1, (byte) 0x76, (byte) 0xEC, (byte) 0x1E, (byte) 0x0D}};

    /**
     * PDU for the following message:
     * <p>originating number: 129
     * <p>message: //VZWVVM
     */
    private static final byte[][] VZWVVM_PDU = {{
            (byte) 0x07, (byte) 0x91, (byte) 0x41, (byte) 0x50, (byte) 0x74, (byte) 0x02,
            (byte) 0x50, (byte) 0xF5, (byte) 0x04, (byte) 0x03, (byte) 0xC9, (byte) 0x21,
            (byte) 0xF9, (byte) 0x00, (byte) 0x00, (byte) 0x71, (byte) 0x30, (byte) 0x70,
            (byte) 0x81, (byte) 0x71, (byte) 0x81, (byte) 0x2B, (byte) 0x08, (byte) 0xAF,
            (byte) 0x97, (byte) 0x55, (byte) 0x7B, (byte) 0xB5, (byte) 0x5A, (byte) 0x9B}};

    private static final String SIM_MCC_MNC = "001002";

    private static final String[] VVM_PATTERN_REGEXP = {SIM_MCC_MNC + ";^//VZWVVM.*"};

    private Context mContext;
    private TelephonyManager mTelephonyManager;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        mContext = Mockito.mock(Context.class);
        mTelephonyManager = Mockito.mock(TelephonyManager.class);
        when(mContext.getSystemServiceName(TelephonyManager.class))
                .thenReturn(Context.TELEPHONY_SERVICE);
        when(mContext.getSystemService(Context.TELEPHONY_SERVICE))
                .thenReturn(mTelephonyManager);

        VisualVoicemailSmsFilter.setPhoneAccountHandleConverterForTest(
                new PhoneAccountHandleConverter() {
                    @Override
                    public PhoneAccountHandle fromSubId(int subId) {
                        return new PhoneAccountHandle(
                                new ComponentName("com.android.internal.telephony",
                                        "VisualVoicemailSmsFilterTest"), "foo");
                    }
                });
    }

    @Override
    public void tearDown() throws Exception {
        VisualVoicemailSmsFilter.setPhoneAccountHandleConverterForTest(null);
        super.tearDown();
    }


    /**
     * Notification SMS targeting over devices do not follow 3GPP or 3GPP2 standards, but instead
     * use a plain text message. {@link android.telephony.SmsMessage#createFromPdu(byte[], String)}
     * will fail to parse it and return an invalid object, causing {@link NullPointerException} on
     * any operation if not handled.
     */
    public void testUnsupportedPdu() {

        setSettings(new VisualVoicemailSmsFilterSettings.Builder().build());

        byte[][] pdus = {
                ("MBOXUPDATE?m=11;server=example.com;"
                        + "port=143;name=1234567890@example.com;pw=CphQJKnYS4jEiDO").getBytes()};
        assertFalse(
                VisualVoicemailSmsFilter.filter(mContext, pdus, SmsConstants.FORMAT_3GPP, 0, 0));
    }

    public void testFilterNotSet_matchesVvmPattern_filtered() {
        setSettings(null);
        Resources resources = Mockito.mock(Resources.class);
        when(mTelephonyManager.getSimOperator(anyInt()))
                .thenReturn(SIM_MCC_MNC);
        when(mContext.getResources())
                .thenReturn(resources);
        when(resources.getStringArray(com.android.internal.R.array.config_vvmSmsFilterRegexes))
                .thenReturn(VVM_PATTERN_REGEXP);

        assertTrue(
                VisualVoicemailSmsFilter.filter(mContext, VZWVVM_PDU, SmsConstants.FORMAT_3GPP, 0,
                        0));
    }

    public void testOriginatingNumber_unspecified_filtered() {
        setSettings(new VisualVoicemailSmsFilterSettings.Builder().build());
        assertTrue(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 0, 0));
    }

    public void testOriginatingNumber_match_filtered() {
        setSettings(
                new VisualVoicemailSmsFilterSettings.Builder().setOriginatingNumbers(
                        Arrays.asList("129")
                ).build());
        assertTrue(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 0, 0));
    }

    public void testOriginatingNumber_mismatch_notFiltered() {
        setSettings(
                new VisualVoicemailSmsFilterSettings.Builder().setOriginatingNumbers(
                        Arrays.asList("128")
                ).build());
        assertFalse(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 0, 0));
    }

    public void testDestinationPort_anyMatch_filtered() {
        setSettings(new VisualVoicemailSmsFilterSettings.Builder()
                .setDestinationPort(123).build());
        assertTrue(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 123, 0));
    }

    public void testDestinationPort_anyData_filtered() {
        setSettings(new VisualVoicemailSmsFilterSettings.Builder()
                .setDestinationPort(VisualVoicemailSmsFilterSettings.DESTINATION_PORT_DATA_SMS)
                .build());
        assertTrue(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 456, 0));
    }

    public void testDestinationPort_anyData_textReceived_notFiltered() {
        setSettings(new VisualVoicemailSmsFilterSettings.Builder()
                .setDestinationPort(VisualVoicemailSmsFilterSettings.DESTINATION_PORT_DATA_SMS)
                .build());
        assertFalse(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, -1, 0));
    }


    public void testDestinationPort_mismatch_notFiltered() {
        setSettings(new VisualVoicemailSmsFilterSettings.Builder()
                .setDestinationPort(123).build());
        assertFalse(VisualVoicemailSmsFilter
                .filter(mContext, SYNC_PDU, SmsConstants.FORMAT_3GPP, 456, 0));
    }

    private void setSettings(VisualVoicemailSmsFilterSettings settings) {
        when(mTelephonyManager.getActiveVisualVoicemailSmsFilterSettings(anyInt()))
                .thenReturn(settings);
    }
}
