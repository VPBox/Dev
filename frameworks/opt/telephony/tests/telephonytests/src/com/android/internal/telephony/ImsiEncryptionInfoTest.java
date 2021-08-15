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

import static org.junit.Assert.assertEquals;

import android.os.Parcel;
import android.telephony.ImsiEncryptionInfo;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Base64;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.security.PublicKey;
import java.security.cert.CertificateFactory;
import java.util.Date;

public class ImsiEncryptionInfoTest {
    private ImsiEncryptionInfo mImsiEncryptionInfo;
    private PublicKey mPublicKey;
    private Date mDate = new Date(1496795015);

    private static final String TEST_CERT = ""
            + "MIIDsjCCAxugAwIBAgIJAPLf2gS0zYGUMA0GCSqGSIb3DQEBBQUAMIGYMQswCQYDVQQGEwJVUzET"
            + "MBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29v"
            + "Z2xlMRAwDgYDVQQLEwd0ZXN0aW5nMRYwFAYDVQQDEw1HZXJlbXkgQ29uZHJhMSEwHwYJKoZIhvcN"
            + "AQkBFhJnY29uZHJhQGdvb2dsZS5jb20wHhcNMTIwNzE0MTc1MjIxWhcNMTIwODEzMTc1MjIxWjCB"
            + "mDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDU1vdW50YWluIFZp"
            + "ZXcxDzANBgNVBAoTBkdvb2dsZTEQMA4GA1UECxMHdGVzdGluZzEWMBQGA1UEAxMNR2VyZW15IENv"
            + "bmRyYTEhMB8GCSqGSIb3DQEJARYSZ2NvbmRyYUBnb29nbGUuY29tMIGfMA0GCSqGSIb3DQEBAQUA"
            + "A4GNADCBiQKBgQCjGGHATBYlmas+0sEECkno8LZ1KPglb/mfe6VpCT3GhSr+7br7NG/ZwGZnEhLq"
            + "E7YIH4fxltHmQC3Tz+jM1YN+kMaQgRRjo/LBCJdOKaMwUbkVynAH6OYsKevjrOPk8lfM5SFQzJMG"
            + "sA9+Tfopr5xg0BwZ1vA/+E3mE7Tr3M2UvwIDAQABo4IBADCB/TAdBgNVHQ4EFgQUhzkS9E6G+x8W"
            + "L4EsmRjDxu28tHUwgc0GA1UdIwSBxTCBwoAUhzkS9E6G+x8WL4EsmRjDxu28tHWhgZ6kgZswgZgx"
            + "CzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3"
            + "MQ8wDQYDVQQKEwZHb29nbGUxEDAOBgNVBAsTB3Rlc3RpbmcxFjAUBgNVBAMTDUdlcmVteSBDb25k"
            + "cmExITAfBgkqhkiG9w0BCQEWEmdjb25kcmFAZ29vZ2xlLmNvbYIJAPLf2gS0zYGUMAwGA1UdEwQF"
            + "MAMBAf8wDQYJKoZIhvcNAQEFBQADgYEAYiugFDmbDOQ2U/+mqNt7o8ftlEo9SJrns6O8uTtK6AvR"
            + "orDrR1AXTXkuxwLSbmVfedMGOZy7Awh7iZa8hw5x9XmUudfNxvmrKVEwGQY2DZ9PXbrnta/dwbhK"
            + "mWfoepESVbo7CKIhJp8gRW0h1Z55ETXD57aGJRvQS4pxkP8ANhM=";

    @After
    public void tearDown() throws Exception {
        mImsiEncryptionInfo = null;
    }

    @Before
    public void setUp() throws Exception {

        mPublicKey = createPublicKey(TEST_CERT);
        mImsiEncryptionInfo = new ImsiEncryptionInfo("310", "270", TelephonyManager.KEY_TYPE_WLAN,
                "key1=value", mPublicKey, mDate);
    }

    private static PublicKey createPublicKey(String cert) throws Exception {
        byte[] derCert = Base64.decode(cert.getBytes(), Base64.DEFAULT);
        InputStream istream = new ByteArrayInputStream(derCert);
        CertificateFactory cf = CertificateFactory.getInstance("X.509");
        return cf.generateCertificate(istream).getPublicKey();
    }

    /**
     * Tests that all the class variables are set correctly.
     */
    @Test
    @SmallTest
    public void testSubProperties() {
        assertEquals("310", mImsiEncryptionInfo.getMcc());
        assertEquals("270", mImsiEncryptionInfo.getMnc());
        assertEquals(TelephonyManager.KEY_TYPE_WLAN, mImsiEncryptionInfo.getKeyType());
        assertEquals("key1=value", mImsiEncryptionInfo.getKeyIdentifier());
        Date date = mImsiEncryptionInfo.getExpirationTime();
        assertEquals(mDate, mImsiEncryptionInfo.getExpirationTime());
    }

    /**
     * Tests the parceling/un-parceling of the object.
     */
    @Test
    @SmallTest
    public void testParcel() {
        Parcel p = Parcel.obtain();
        p.setDataPosition(0);
        mImsiEncryptionInfo.writeToParcel(p, 0);
        p.setDataPosition(0);
        ImsiEncryptionInfo nw = new ImsiEncryptionInfo(p);
        assertEquals("310", mImsiEncryptionInfo.getMcc());
        assertEquals("270", mImsiEncryptionInfo.getMnc());
        assertEquals(TelephonyManager.KEY_TYPE_WLAN, mImsiEncryptionInfo.getKeyType());
        assertEquals("key1=value", mImsiEncryptionInfo.getKeyIdentifier());
        assertEquals(mPublicKey, mImsiEncryptionInfo.getPublicKey());
        assertEquals(mDate, mImsiEncryptionInfo.getExpirationTime());
    }
}
