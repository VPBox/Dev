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

package com.android.ike.ikev2.message;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import com.android.ike.ikev2.exceptions.AuthenticationFailedException;

import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.Base64;

public final class IkeCertX509CertPayloadTest {
    private static final String CERT_PAYLOAD_BODY_HEX_STRING =
            "043082042130820209a003020102020827a2b30cdd5043ab300d06092a864886"
                    + "f70d01010c05003035310b3009060355040613024e5a310e300c060355040a13"
                    + "054576697461311630140603550403130d457669746120526f6f74204341301e"
                    + "170d3138313033303139303034305a170d3230313032393139303034305a3036"
                    + "310b3009060355040613024e5a310e300c060355040a13054576697461311730"
                    + "150603550403130e3139322e3136382e34332e31303330820122300d06092a86"
                    + "4886f70d01010105000382010f003082010a0282010100b15dd29d25ef546532"
                    + "2b85b2db5aefbff85b2f8e90a67c5ce48a7baee696d645602b1122501e10aec4"
                    + "7733a875a6f02432a4d684e67bdab36549996f3a2fc919cf769dc3a3b270791a"
                    + "3682c868225e5478df1d2467b1afafd608db2696c484a0134f8069f9ed0b55fc"
                    + "656b15fef6b9b151debfd1d96f6bba0f032432ec6497573d47f3539b4754f33a"
                    + "89ea4d3986ad33137a750b36073fe50188c169a851c092506ea383ceb58b55ad"
                    + "f55956e037118f2f747be6dc3f0f741ec1d658defcd9197b8750a75ae9c1b1c7"
                    + "2f2ff91f561d1b3d77d54037e8cfe3738668a3de42cf7eab21353ae322e40eb4"
                    + "8dcf38eb34572118315047133f7727cab46f10f9f70de30203010001a3343032"
                    + "301f0603551d23041830168014fbcb6762ffde82b4d63d069beeac583228ab32"
                    + "25300f0603551d11040830068704c0a82b67300d06092a864886f70d01010c05"
                    + "0003820201006f26b5973346030b7297d50e3ce4cd8ca5b1527fd9045b4bb540"
                    + "a7cd0193e16732346d6fb99c2058a17f0129460ba6e4832b6b4a6c51731ceea4"
                    + "5464e0f179b860b3f0bf10956b1f4bae8cecdfc08261a3c00e8aa0ddbbe9ca08"
                    + "6a954ec9e3bcb0ea907e617e98a97ec1c2b7c2190b10a8117b12e56640830c1f"
                    + "c6492a8f7062e92b7e2d03f22173fb333b9a45d96c9842fc16c0c6658428fbdf"
                    + "6351be6fe65f7e1f329d7a07dad617837208e72539774ef46a72bc8e279fca74"
                    + "d68fbfba91de8d62331c2df135ce234b007afc330f96d3ee80888ba102334c32"
                    + "2d25a78d26a2c9434fafdfa9ea35acadf4938a69623de5ee966d1d550d851df3"
                    + "d99b477f804a6eb2d8c16f80161a9511674bfa514f5d127c856ca6c0dff07eba"
                    + "87672e2154c4fd0f3b906ff888978315a041b3fdbc7cf8e2306e0b20b3a79d2b"
                    + "5a59f7f6b8e784af57d43be4be37f9381a6ff6c3fa477fed151d586c42634e0a"
                    + "88e699a9f3b38459589ea014d549b7ed7fd551bd544d464d955476ed1c051fa1"
                    + "a7351d5d4f13efe232bc847a245c85a4a04abf66abd7d983b254a67d0189206c"
                    + "8fc8989f38e63bd827552e209a2aa119d0622f0defe08cef0bf48a3459c09ad9"
                    + "8f729b51bb57f2518385abd790ff3d80d1cdce1218f61ea45c0c6fc9814c300d"
                    + "abc24a747560744e9861c9395dd2f849b4d1196fe302ac8a063afeea9bc9d637"
                    + "2fedb79130bb";

    private static final String CLIENT_END_CERTIFICATE =
            "MIIEITCCAgmgAwIBAgIIJ6KzDN1QQ6swDQYJKoZIhvcNAQEMBQAwNTELMAkGA1UE"
                    + "BhMCTloxDjAMBgNVBAoTBUV2aXRhMRYwFAYDVQQDEw1Fdml0YSBSb290IENBMB4X"
                    + "DTE4MTAzMDE5MDA0MFoXDTIwMTAyOTE5MDA0MFowNjELMAkGA1UEBhMCTloxDjAM"
                    + "BgNVBAoTBUV2aXRhMRcwFQYDVQQDEw4xOTIuMTY4LjQzLjEwMzCCASIwDQYJKoZI"
                    + "hvcNAQEBBQADggEPADCCAQoCggEBALFd0p0l71RlMiuFstta77/4Wy+OkKZ8XOSK"
                    + "e67mltZFYCsRIlAeEK7EdzOodabwJDKk1oTme9qzZUmZbzovyRnPdp3Do7JweRo2"
                    + "gshoIl5UeN8dJGexr6/WCNsmlsSEoBNPgGn57QtV/GVrFf72ubFR3r/R2W9rug8D"
                    + "JDLsZJdXPUfzU5tHVPM6iepNOYatMxN6dQs2Bz/lAYjBaahRwJJQbqODzrWLVa31"
                    + "WVbgNxGPL3R75tw/D3QewdZY3vzZGXuHUKda6cGxxy8v+R9WHRs9d9VAN+jP43OG"
                    + "aKPeQs9+qyE1OuMi5A60jc846zRXIRgxUEcTP3cnyrRvEPn3DeMCAwEAAaM0MDIw"
                    + "HwYDVR0jBBgwFoAU+8tnYv/egrTWPQab7qxYMiirMiUwDwYDVR0RBAgwBocEwKgr"
                    + "ZzANBgkqhkiG9w0BAQwFAAOCAgEAbya1lzNGAwtyl9UOPOTNjKWxUn/ZBFtLtUCn"
                    + "zQGT4WcyNG1vuZwgWKF/ASlGC6bkgytrSmxRcxzupFRk4PF5uGCz8L8QlWsfS66M"
                    + "7N/AgmGjwA6KoN276coIapVOyeO8sOqQfmF+mKl+wcK3whkLEKgRexLlZkCDDB/G"
                    + "SSqPcGLpK34tA/Ihc/szO5pF2WyYQvwWwMZlhCj732NRvm/mX34fMp16B9rWF4Ny"
                    + "COclOXdO9GpyvI4nn8p01o+/upHejWIzHC3xNc4jSwB6/DMPltPugIiLoQIzTDIt"
                    + "JaeNJqLJQ0+v36nqNayt9JOKaWI95e6WbR1VDYUd89mbR3+ASm6y2MFvgBYalRFn"
                    + "S/pRT10SfIVspsDf8H66h2cuIVTE/Q87kG/4iJeDFaBBs/28fPjiMG4LILOnnSta"
                    + "Wff2uOeEr1fUO+S+N/k4Gm/2w/pHf+0VHVhsQmNOCojmmanzs4RZWJ6gFNVJt+1/"
                    + "1VG9VE1GTZVUdu0cBR+hpzUdXU8T7+IyvIR6JFyFpKBKv2ar19mDslSmfQGJIGyP"
                    + "yJifOOY72CdVLiCaKqEZ0GIvDe/gjO8L9Io0WcCa2Y9ym1G7V/JRg4Wr15D/PYDR"
                    + "zc4SGPYepFwMb8mBTDANq8JKdHVgdE6YYck5XdL4SbTRGW/jAqyKBjr+6pvJ1jcv"
                    + "7beRMLs=";
    private static final int CERTIFICATE_OFFSET = 1;

    @Test
    public void testDecodeX509Certificate() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(CERT_PAYLOAD_BODY_HEX_STRING);
        IkeCertPayload certPayload = IkeCertPayload.getIkeCertPayload(false, inputPacket);

        assertTrue(certPayload instanceof IkeCertX509CertPayload);
        X509Certificate expectedCert = pemStringToCertificate(CLIENT_END_CERTIFICATE);
        assertEquals(expectedCert, ((IkeCertX509CertPayload) certPayload).certificate);
    }

    @Test
    public void testDecodeX509CertificateWithUnexpectedTrailing() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(CERT_PAYLOAD_BODY_HEX_STRING + "ffff");
        try {
            IkeCertPayload.getIkeCertPayload(false, inputPacket);
            fail("Expected AuthenticationFailedException: " + "Unexpected trailing bytes.");
        } catch (AuthenticationFailedException expected) {
        }
    }

    @Test
    public void testDecodeGetNoX509Certificate() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(CERT_PAYLOAD_BODY_HEX_STRING);
        inputPacket[CERTIFICATE_OFFSET] = 0;
        try {
            IkeCertPayload.getIkeCertPayload(false, inputPacket);
            fail("Expected AuthenticationFailedException: " + "No certificate got.");
        } catch (AuthenticationFailedException expected) {
        }
    }

    @Test
    public void testDecodeInvalidX509Certificate() throws Exception {
        byte[] inputPacket = TestUtils.hexStringToByteArray(CERT_PAYLOAD_BODY_HEX_STRING);
        try {
            IkeCertPayload.getIkeCertPayload(
                    false, Arrays.copyOfRange(inputPacket, 0, inputPacket.length - 1));
            fail("Expected AuthenticationFailedException: " + "Certificate parsing exception.");
        } catch (AuthenticationFailedException expected) {
        }
    }

    private X509Certificate pemStringToCertificate(String certPemStr) throws Exception {
        CertificateFactory factory =
                CertificateFactory.getInstance("X.509", IkeMessage.getSecurityProvider());
        Base64.Decoder bs64Decoder = Base64.getDecoder();
        byte[] decodedBytes = bs64Decoder.decode(certPemStr);
        return (X509Certificate)
                factory.generateCertificate(new ByteArrayInputStream(decodedBytes));
    }
}
