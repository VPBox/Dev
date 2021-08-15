/*
 * Copyright (C) 2017 The Android Open Source Project
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

import static android.preference.PreferenceManager.getDefaultSharedPreferences;

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.DownloadManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.HandlerThread;
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.ImsiEncryptionInfo;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Pair;

import com.android.org.bouncycastle.util.io.pem.PemReader;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Matchers;
import org.mockito.MockitoAnnotations;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.security.PublicKey;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class CarrierKeyDownloadMgrTest extends TelephonyTest {

    private static final String LOG_TAG = "CarrierKeyDownloadManager";

    private CarrierKeyDownloadManager mCarrierKeyDM;
    private CarrierActionAgentHandler mCarrierActionAgentHandler;

    private String mURL = "http://www.google.com";

    private static final String CERT = "-----BEGIN CERTIFICATE-----\r\nMIIFjzCCBHegAwIBAgIUPxj3SLif82Ky1RlUy8p2EWJCh8MwDQYJKoZIhvcNAQELBQAwgY0xCzAJBgNVBAYTAk5MMRIwEAYDVQQHEwlBbXN0ZXJkYW0xJTAjBgNVBAoTHFZlcml6b24gRW50ZXJwcmlzZSBTb2x1dGlvbnMxEzARBgNVBAsTCkN5YmVydHJ1c3QxLjAsBgNVBAMTJVZlcml6b24gUHVibGljIFN1cmVTZXJ2ZXIgQ0EgRzE0LVNIQTIwHhcNMTcwODE0MTc0MzM4WhcNMTkwODE0MTc0MzM4WjCBmTELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFjAUBgNVBAcTDUJhc2tpbmcgUmlkZ2UxIjAgBgNVBAoTGVZlcml6b24gRGF0YSBTZXJ2aWNlcyBMTEMxHzAdBgNVBAsTFk5ldHdvcmsgU3lzdGVtIFN1cHBvcnQxGDAWBgNVBAMTD3ZpMWx2Lmltc3ZtLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALUQKWTHi4Hjpd1LQwJ87RXa0Rs3rVonvVevliqdUH5BikjhAzvIqwPSXeRQqkaRTFIyp0NKcNqGdjAaHRo43gdHeWSH331sS6CMZDg988gZznskzCqJJo6ii5FuLC8qe2YDsHxT+CefXev2rn6Bj1ei2X74uZsy5KlkBRZfFHtPdK6/EK5TpzrvcXfDyOK1rn8FTno1bQOTAhL39GPcLhdrXV7AN+lu+EBpdCqlTdcoDxsqavi/91MwUIVEzxJmycKloT6OWfU44r7+L5SYYgc88NTaGL/BvCFwHRIa1ZgYSGeAPes45792MGG7tfr/ttAGp9UEwTv2zWTxzWnRP/UCAwEAAaOCAdcwggHTMAwGA1UdEwEB/wQCMAAwTAYDVR0gBEUwQzBBBgkrBgEEAbE+ATIwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly9zZWN1cmUub21uaXJvb3QuY29tL3JlcG9zaXRvcnkwgakGCCsGAQUFBwEBBIGcMIGZMC0GCCsGAQUFBzABhiFodHRwOi8vdnBzc2cxNDIub2NzcC5vbW5pcm9vdC5jb20wMwYIKwYBBQUHMAKGJ2h0dHA6Ly9jYWNlcnQub21uaXJvb3QuY29tL3Zwc3NnMTQyLmNydDAzBggrBgEFBQcwAoYnaHR0cDovL2NhY2VydC5vbW5pcm9vdC5jb20vdnBzc2cxNDIuZGVyMBoGA1UdEQQTMBGCD3ZpMWx2Lmltc3ZtLmNvbTAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMB8GA1UdIwQYMBaAFOQtu5EBZSYftHo/oxUlpM6MRDM7MD4GA1UdHwQ3MDUwM6AxoC+GLWh0dHA6Ly92cHNzZzE0Mi5jcmwub21uaXJvb3QuY29tL3Zwc3NnMTQyLmNybDAdBgNVHQ4EFgQUv5SaSyNM/yXw1v0N9TNpjsFCaPcwDQYJKoZIhvcNAQELBQADggEBACNJusTULj1KyV4RwiskKfp4wI9Hsz3ESbZS/ijF9D57BQ0UwkELU9r6rEAhsYLUvMq4sDhDbYIdupgP4MBzFnjkKult7VQm5W3nCcuHgXYFAJ9Y1a4OZAo/4hrHj70W9TsQ1ioSMjUT4F8bDUYZI0kcyH8e/+2DaTsLUpHw3L+Keu8PsJVBLnvcKJjWrZD/Bgd6JuaTX2G84i0rY0GJuO9CxLNJa6n61Mz5cqLYIuwKgiVgTA2n71YITyFICOFPFX1vSx35AWvD6aVYblxtC8mpCdF2h4s1iyrpXeji2GCJLwsNVtTtNQ4zWX3Gnq683wzkYZeyOHUyftIgAQZ+HsY=\r\n-----END CERTIFICATE-----";
    private static final long CERT_EXPIRATION = 1565804618000L; //milliseconds since the epoch
    private String mJsonStr = "{ \"carrier-keys\": [ { \"certificate\": \"" + CERT + "\", \"key-type\": \"WLAN\", \"key-identifier\": \"key1=value\", \"expiration-date\": 1502577746000 }, { \"certificate\": \"" + CERT + "\", \"key-type\": \"WLAN\", \"key-identifier\": \"key1=value\", \"expiration-date\": 1502577746000 }]}";

    private String mJsonStr1 = "{ \"carrier-keys\": [ { \"public-key\": \"" + CERT + "\", \"key-type\": \"WLAN\", \"key-identifier\": \"key1=value\", \"expiration-date\": 1502577746000 }, { \"public-key\": \"" + CERT + "\", \"key-type\": \"WLAN\", \"key-identifier\": \"key1=value\", \"expiration-date\": 1502577746000 }]}";

    private String mJsonStr3GppSpec = "{ \"carrier-keys\": [ { \"key-identifier\": \"key1=value\", "
            + "\"public-key\": \"" + CERT + "\"}]}";

    private class CarrierActionAgentHandler extends HandlerThread {

        private CarrierActionAgentHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mCarrierKeyDM = new CarrierKeyDownloadManager(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        logd("CarrierActionAgentTest +Setup!");
        MockitoAnnotations.initMocks(this);
        super.setUp(getClass().getSimpleName());
        mCarrierActionAgentHandler = new CarrierActionAgentHandler(getClass().getSimpleName());
        mCarrierActionAgentHandler.start();
        waitUntilReady();
        logd("CarrierActionAgentTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        mCarrierActionAgentHandler.quit();
        super.tearDown();
    }

    /* Checks if the expiration date is calculated correctly
     * In this case the expiration date should be the next day.
     */
    @Test
    @SmallTest
    public void testExpirationDate1Day() {
        java.security.PublicKey publicKey = null;
        mCarrierKeyDM.mKeyAvailability = 3;
        SimpleDateFormat dt = new SimpleDateFormat("yyyy-mm-dd");
        Calendar cal = new GregorianCalendar();
        cal.add(Calendar.DATE, 6);
        Date date = cal.getTime();
        Calendar expectedCal = new GregorianCalendar();
        expectedCal.add(Calendar.DATE, 1);
        String dateExpected = dt.format(expectedCal.getTime());
        ImsiEncryptionInfo imsiEncryptionInfo = new ImsiEncryptionInfo("mcc", "mnc", 1,
                "keyIdentifier", publicKey, date);
        when(mPhone.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(imsiEncryptionInfo);
        Date expirationDate = new Date(mCarrierKeyDM.getExpirationDate());
        assertTrue(dt.format(expirationDate).equals(dateExpected));
    }

    /**
     * Checks if the expiration date is calculated correctly
     * In this case the expiration date should be within the window (7 to 21 days).
     **/
    @Test
    @SmallTest
    public void testExpirationDate7Day() {
        java.security.PublicKey publicKey = null;
        mCarrierKeyDM.mKeyAvailability = 3;
        SimpleDateFormat dt = new SimpleDateFormat("yyyy-mm-dd");
        Calendar cal = new GregorianCalendar();
        cal.add(Calendar.DATE, 30);
        Date date = cal.getTime();
        Calendar minExpirationCal = new GregorianCalendar();
        Calendar maxExpirationCal = new GregorianCalendar();
        minExpirationCal.add(Calendar.DATE, 23);
        maxExpirationCal.add(Calendar.DATE, 9);
        Date minExpirationDate = minExpirationCal.getTime();
        Date maxExpirationDate = maxExpirationCal.getTime();
        ImsiEncryptionInfo imsiEncryptionInfo = new ImsiEncryptionInfo("mcc", "mnc", 1,
                "keyIdentifier", publicKey, date);
        when(mPhone.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(imsiEncryptionInfo);
        Date expirationDate = new Date(mCarrierKeyDM.getExpirationDate());
        assertTrue(expirationDate.before(minExpirationDate));
        assertTrue(expirationDate.after(maxExpirationDate));
    }

    /**
     * Checks if the json is parse correctly.
     * Verify that setCarrierInfoForImsiEncryption is called with the right params
     **/
    @Test
    @SmallTest
    public void testParseJson() {
        ByteArrayInputStream certBytes = new ByteArrayInputStream(CERT.getBytes());
        Reader fRd = new BufferedReader(new InputStreamReader(certBytes));
        PemReader reader = new PemReader(fRd);
        Pair<PublicKey, Long> keyInfo = null;
        try {
            keyInfo = mCarrierKeyDM.getKeyInformation(reader.readPemObject().getContent());
        } catch (Exception e) {
            fail(LOG_TAG + "exception creating public key");
        }
        ImsiEncryptionInfo imsiEncryptionInfo = new ImsiEncryptionInfo("310", "270", 2,
                "key1=value", keyInfo.first, new Date(keyInfo.second));
        String mccMnc = "310:270";
        mCarrierKeyDM.parseJsonAndPersistKey(mJsonStr, mccMnc);
        verify(mPhone, times(2)).setCarrierInfoForImsiEncryption(
                (Matchers.refEq(imsiEncryptionInfo)));
    }

    /**
     * Checks if the json is parse correctly.
     * Same as testParseJason, except that the test looks for the "public-key" field.
     **/
    @Test
    @SmallTest
    public void testParseJsonPublicKey() {
        ByteArrayInputStream certBytes = new ByteArrayInputStream(CERT.getBytes());
        Reader fRd = new BufferedReader(new InputStreamReader(certBytes));
        PemReader reader = new PemReader(fRd);
        Pair<PublicKey, Long> keyInfo = null;
        try {
            keyInfo = mCarrierKeyDM.getKeyInformation(reader.readPemObject().getContent());
        } catch (Exception e) {
            fail(LOG_TAG + "exception creating public key");
        }
        ImsiEncryptionInfo imsiEncryptionInfo = new ImsiEncryptionInfo("310", "270", 2,
                "key1=value", keyInfo.first, new Date(keyInfo.second));
        String mccMnc = "310:270";
        mCarrierKeyDM.parseJsonAndPersistKey(mJsonStr1, mccMnc);
        verify(mPhone, times(2)).setCarrierInfoForImsiEncryption(
                (Matchers.refEq(imsiEncryptionInfo)));
    }

    /**
     * Checks if the json is parse correctly.
     * Since the json is bad, we want to verify that savePublicKey is not called.
     **/
    @Test
    @SmallTest
    public void testParseBadJsonFail() {
        String mccMnc = "310:290";
        String badJsonStr = "{badJsonString}";
        mCarrierKeyDM.parseJsonAndPersistKey(badJsonStr, mccMnc);
        verify(mPhone, times(0)).setCarrierInfoForImsiEncryption(any());
    }

    /**
     * Checks if the download is valid.
     * returns true since the mnc/mcc is valid.
     **/
    @Test
    @SmallTest
    public void testIsValidDownload() {
        String mccMnc = "310:260";
        when(mTelephonyManager.getSimOperator(anyInt())).thenReturn("310260");
        assertTrue(mCarrierKeyDM.isValidDownload(mccMnc));
    }

    /**
     * Checks if the download is valid.
     * returns false since the mnc/mcc is in-valid.
     **/
    @Test
    @SmallTest
    public void testIsValidDownloadFail() {
        String mccMnc = "310:290";
        when(mTelephonyManager.getSimOperator(anyInt())).thenReturn("310260");
        assertFalse(mCarrierKeyDM.isValidDownload(mccMnc));
    }

    /**
     * Tests if the key is enabled.
     * tests for all bit-mask value.
     **/
    @Test
    @SmallTest
    public void testIsKeyEnabled() {
        mCarrierKeyDM.mKeyAvailability = 3;
        assertTrue(mCarrierKeyDM.isKeyEnabled(1));
        assertTrue(mCarrierKeyDM.isKeyEnabled(2));
        mCarrierKeyDM.mKeyAvailability = 2;
        assertFalse(mCarrierKeyDM.isKeyEnabled(1));
        assertTrue(mCarrierKeyDM.isKeyEnabled(2));
        mCarrierKeyDM.mKeyAvailability = 1;
        assertTrue(mCarrierKeyDM.isKeyEnabled(1));
        assertFalse(mCarrierKeyDM.isKeyEnabled(2));
    }

    /**
     * Tests sending the ACTION_DOWNLOAD_COMPLETE intent.
     * Verify that the alarm will kick-off the next day.
     **/
    @Test
    @SmallTest
    public void testDownloadComplete() {
        SharedPreferences.Editor editor = getDefaultSharedPreferences(mContext).edit();
        String mccMnc = "310:260";
        int slotId = mPhone.getPhoneId();
        editor.putString("CARRIER_KEY_DM_MCC_MNC" + slotId, mccMnc);
        editor.commit();

        SimpleDateFormat dt = new SimpleDateFormat("yyyy-mm-dd");
        Calendar expectedCal = new GregorianCalendar();
        expectedCal.add(Calendar.DATE, 1);
        String dateExpected = dt.format(expectedCal.getTime());

        when(mTelephonyManager.getSimOperator(anyInt())).thenReturn("310260");
        Intent mIntent = new Intent(DownloadManager.ACTION_DOWNLOAD_COMPLETE);
        mContext.sendBroadcast(mIntent);
        waitForMs(200);
        Date expirationDate = new Date(mCarrierKeyDM.getExpirationDate());
        assertTrue(dt.format(expirationDate).equals(dateExpected));
    }

    /**
     * Test sending the ACTION_CARRIER_CONFIG_CHANGED intent.
     * Verify that the right mnc/mcc gets stored in the preferences.
     **/
    @Test
    @SmallTest
    public void testCarrierConfigChanged() {
        CarrierConfigManager carrierConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        int slotId = mPhone.getPhoneId();
        PersistableBundle bundle = carrierConfigManager.getConfigForSubId(slotId);
        bundle.putInt(CarrierConfigManager.IMSI_KEY_AVAILABILITY_INT, 3);
        bundle.putString(CarrierConfigManager.IMSI_KEY_DOWNLOAD_URL_STRING, mURL);

        when(mTelephonyManager.getSimOperator(anyInt())).thenReturn("310260");
        Intent mIntent = new Intent(CarrierConfigManager.ACTION_CARRIER_CONFIG_CHANGED);
        mIntent.putExtra(PhoneConstants.PHONE_KEY, 0);
        mContext.sendBroadcast(mIntent);
        waitForMs(200);
        SharedPreferences preferences = getDefaultSharedPreferences(mContext);
        String mccMnc = preferences.getString("CARRIER_KEY_DM_MCC_MNC" + slotId, null);
        assertTrue(mccMnc.equals("310:260"));
    }

    /**
     * Tests sending the INTENT_KEY_RENEWAL_ALARM_PREFIX intent.
     * Verify that the right mnc/mcc gets stored in the preferences.
     **/
    @Test
    @SmallTest
    public void testAlarmRenewal() {
        CarrierConfigManager carrierConfigManager = (CarrierConfigManager)
                mContext.getSystemService(Context.CARRIER_CONFIG_SERVICE);
        int slotId = mPhone.getPhoneId();
        PersistableBundle bundle = carrierConfigManager.getConfigForSubId(slotId);
        bundle.putInt(CarrierConfigManager.IMSI_KEY_AVAILABILITY_INT, 3);
        bundle.putString(CarrierConfigManager.IMSI_KEY_DOWNLOAD_URL_STRING, mURL);

        when(mTelephonyManager.getSimOperator(anyInt())).thenReturn("310260");
        Intent mIntent = new Intent("com.android.internal.telephony.carrier_key_download_alarm"
                + slotId);
        mContext.sendBroadcast(mIntent);
        waitForMs(200);
        SharedPreferences preferences = getDefaultSharedPreferences(mContext);
        String mccMnc = preferences.getString("CARRIER_KEY_DM_MCC_MNC" + slotId, null);
        assertTrue(mccMnc.equals("310:260"));
    }

    /**
     * Checks if the JSON in 3GPP spec format is parsed correctly, and that WLAN is the key type.
     **/
    @Test
    @SmallTest
    public void testParseJson3GppFormat() {
        ByteArrayInputStream certBytes = new ByteArrayInputStream(CERT.getBytes());
        Reader fRd = new BufferedReader(new InputStreamReader(certBytes));
        PemReader reader = new PemReader(fRd);
        Pair<PublicKey, Long> keyInfo = null;
        try {
            keyInfo = mCarrierKeyDM.getKeyInformation(reader.readPemObject().getContent());
        } catch (Exception e) {
            fail(LOG_TAG + "exception creating public key");
        }
        ImsiEncryptionInfo imsiEncryptionInfo = new ImsiEncryptionInfo("310", "270",
                TelephonyManager.KEY_TYPE_WLAN, "key1=value", keyInfo.first,
                new Date(CERT_EXPIRATION));
        String mccMnc = "310:270";
        mCarrierKeyDM.parseJsonAndPersistKey(mJsonStr3GppSpec, mccMnc);
        verify(mPhone).setCarrierInfoForImsiEncryption(
                (Matchers.refEq(imsiEncryptionInfo)));
    }

}
