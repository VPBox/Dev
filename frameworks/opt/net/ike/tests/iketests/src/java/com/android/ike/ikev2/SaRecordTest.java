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

package com.android.ike.ikev2;

import static org.junit.Assert.assertArrayEquals;

import com.android.ike.ikev2.message.TestUtils;

import org.junit.Test;

public final class SaRecordTest {
    private static final String IKE_INIT_SPI = "5F54BF6D8B48E6E1";
    private static final String IKE_RESP_SPI = "909232B3D1EDCB5C";

    private static final String IKE_NONCE_INIT_HEX_STRING =
            "C39B7F368F4681B89FA9B7BE6465ABD7C5F68B6ED5D3B4C72CB4240EB5C46412";
    private static final String IKE_NONCE_RESP_HEX_STRING =
            "9756112CA539F5C25ABACC7EE92B73091942A9C06950F98848F1AF1694C4DDFF";

    private static final String IKE_SHARED_DH_KEY_HEX_STRING =
            "C14155DEA40056BD9C76FB4819687B7A397582F4CD5AFF4B"
                    + "8F441C56E0C08C84234147A0BA249A555835A048E3CA2980"
                    + "7D057A61DD26EEFAD9AF9C01497005E52858E29FB42EB849"
                    + "6731DF96A11CCE1F51137A9A1B900FA81AEE7898E373D4E4"
                    + "8B899BBECA091314ECD4B6E412EF4B0FEF798F54735F3180"
                    + "7424A318287F20E8";

    private static final String IKE_SKEYSEED_HEX_STRING =
            "8C42F3B1F5F81C7BAAC5F33E9A4F01987B2F9657";
    private static final String IKE_SK_D_HEX_STRING = "C86B56EFCF684DCC2877578AEF3137167FE0EBF6";
    private static final String IKE_SK_AUTH_INIT_HEX_STRING =
            "554FBF5A05B7F511E05A30CE23D874DB9EF55E51";
    private static final String IKE_SK_AUTH_RESP_HEX_STRING =
            "36D83420788337CA32ECAA46892C48808DCD58B1";
    private static final String IKE_SK_ENCR_INIT_HEX_STRING = "5CBFD33F75796C0188C4A3A546AEC4A1";
    private static final String IKE_SK_ENCR_RESP_HEX_STRING = "C33B35FCF29514CD9D8B4A695E1A816E";
    private static final String IKE_SK_PRF_INIT_HEX_STRING =
            "094787780EE466E2CB049FA327B43908BC57E485";
    private static final String IKE_SK_PRF_RESP_HEX_STRING =
            "A30E6B08BE56C0E6BFF4744143C75219299E1BEB";
    private static final String IKE_KEY_MAT =
            IKE_SK_D_HEX_STRING
                    + IKE_SK_AUTH_INIT_HEX_STRING
                    + IKE_SK_AUTH_RESP_HEX_STRING
                    + IKE_SK_ENCR_INIT_HEX_STRING
                    + IKE_SK_ENCR_RESP_HEX_STRING
                    + IKE_SK_PRF_INIT_HEX_STRING
                    + IKE_SK_PRF_RESP_HEX_STRING;

    private static final int IKE_AUTH_ALGO_KEY_LEN = 20;
    private static final int IKE_ENCR_ALGO_KEY_LEN = 16;
    private static final int IKE_PRF_KEY_LEN = 20;
    private static final int IKE_SK_D_KEY_LEN = IKE_PRF_KEY_LEN;

    private static final String FIRST_CHILD_ENCR_INIT_HEX_STRING =
            "1B865CEA6E2C23973E8C5452ADC5CD7D";
    private static final String FIRST_CHILD_ENCR_RESP_HEX_STRING =
            "5E82FEDACC6DCB0756DDD7553907EBD1";
    private static final String FIRST_CHILD_AUTH_INIT_HEX_STRING =
            "A7A5A44F7EF4409657206C7DC52B7E692593B51E";
    private static final String FIRST_CHILD_AUTH_RESP_HEX_STRING =
            "CDE612189FD46DE870FAEC04F92B40B0BFDBD9E1";
    private static final String FIRST_CHILD_KEY_MAT =
            FIRST_CHILD_ENCR_INIT_HEX_STRING
                    + FIRST_CHILD_AUTH_INIT_HEX_STRING
                    + FIRST_CHILD_ENCR_RESP_HEX_STRING
                    + FIRST_CHILD_AUTH_RESP_HEX_STRING;

    private static final int FIRST_CHILD_AUTH_ALGO_KEY_LEN = 20;
    private static final int FIRST_CHILD_ENCR_ALGO_KEY_LEN = 16;

    private static final String PRF_HMAC_SHA1_ALGO_NAME = "HmacSHA1";

    @Test
    public void testCalculateSKeySeed() throws Exception {
        byte[] nonceInit = TestUtils.hexStringToByteArray(IKE_NONCE_INIT_HEX_STRING);
        byte[] nonceResp = TestUtils.hexStringToByteArray(IKE_NONCE_RESP_HEX_STRING);
        byte[] sharedDhKey = TestUtils.hexStringToByteArray(IKE_SHARED_DH_KEY_HEX_STRING);

        byte[] calculatedSKeySeed =
                SaRecord.generateSKeySeed(
                        PRF_HMAC_SHA1_ALGO_NAME, nonceInit, nonceResp, sharedDhKey);

        byte[] expectedSKeySeed = TestUtils.hexStringToByteArray(IKE_SKEYSEED_HEX_STRING);
        assertArrayEquals(expectedSKeySeed, calculatedSKeySeed);
    }

    @Test
    public void testSignWithPrfPlusForIke() throws Exception {
        byte[] prfKey = TestUtils.hexStringToByteArray(IKE_SKEYSEED_HEX_STRING);
        byte[] prfData =
                TestUtils.hexStringToByteArray(
                        IKE_NONCE_INIT_HEX_STRING
                                + IKE_NONCE_RESP_HEX_STRING
                                + IKE_INIT_SPI
                                + IKE_RESP_SPI);
        int keyMaterialLen =
                IKE_SK_D_KEY_LEN
                        + IKE_AUTH_ALGO_KEY_LEN * 2
                        + IKE_ENCR_ALGO_KEY_LEN * 2
                        + IKE_PRF_KEY_LEN * 2;

        byte[] calculatedKeyMat =
                SaRecord.generateKeyMat(PRF_HMAC_SHA1_ALGO_NAME, prfKey, prfData, keyMaterialLen);

        byte[] expectedKeyMat = TestUtils.hexStringToByteArray(IKE_KEY_MAT);
        assertArrayEquals(expectedKeyMat, calculatedKeyMat);
    }

    @Test
    public void testSignWithPrfPlusForFirstChild() throws Exception {
        byte[] prfKey = TestUtils.hexStringToByteArray(IKE_SK_D_HEX_STRING);
        byte[] prfData =
                TestUtils.hexStringToByteArray(
                        IKE_NONCE_INIT_HEX_STRING + IKE_NONCE_RESP_HEX_STRING);
        int keyMaterialLen = FIRST_CHILD_AUTH_ALGO_KEY_LEN * 2 + FIRST_CHILD_ENCR_ALGO_KEY_LEN * 2;

        byte[] calculatedKeyMat =
                SaRecord.generateKeyMat(PRF_HMAC_SHA1_ALGO_NAME, prfKey, prfData, keyMaterialLen);

        byte[] expectedKeyMat = TestUtils.hexStringToByteArray(FIRST_CHILD_KEY_MAT);
        assertArrayEquals(expectedKeyMat, calculatedKeyMat);
    }
}
