/*
 * Copyright 2017 The Android Open Source Project
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

#include <iostream>

#include <gtest/gtest.h>

#include <keymaster/wrapped_key.h>

#include "android_keymaster_test_utils.h"

using ::std::cout;
using ::std::endl;
using ::std::string;

namespace keymaster {
namespace test {

string test_wrapped_key = hex2str(
    "3082015E020100048201005930C4FFE73B214575A66FB1DC07FD72F2508488F927926DB8DE8A78D780169FFC79728E"
    "63BE14280C5481856CB51886BB1FF7D7F0BB73013DAE5386C7F63CD7D12E7FCC9AF89A7A52E68AEBB3CD3C08819FB2"
    "A1D10EA717FF662D9FCF00194B7D7B75F6A898EF3295454642F697123758FB172EF015B515A2AC791BE35077346503"
    "7D25B45375B7E00472C5250F7FD9053ECEA62D59EE3734C919A124A1659EF4F031F137DB661C0E846DFEE46C4CC85F"
    "99B47708ADDEF2B21E1143F59A0EE12E0AB5ADF9E03C26642FC36905F38EE60A9B385FF4785FDF6611B60BD9DB283D"
    "EDD4481DFCBCCBB51166F475A94898EC759BB9125520304FF82124559D27BE2B040CD796B02C370F1FA4CC0124F130"
    "13020103300EA1023100A203020120A3030201200420CCD540855F833A5E1480BFD2D36FAF3AEEE15DF5BEABE2691B"
    "C82DDE2A7AA910041094CD97F58DE55B737B60B3AD127B1C59");

string test_tag = hex2str("94CD97F58DE55B737B60B3AD127B1C59");
string test_iv = hex2str("D796B02C370F1FA4CC0124F1");
string test_transit_key =
    hex2str("78421A8124D7960B4CBFC8F4F16B421B3511A3D29CFB329C3CCD90724FD9E8E440"
            "F0058F2035645EDAD7BCF62D0ED23D39B049069B2B0F8607F32B084804824A6620"
            "F2658FC74ECBFCE9533FE220E981EF1E05170988CA5EB42480FCD711B7668140DF"
            "5DC5D23DCAFC536A971DDB4FD65E5B5F7C01E5C13079CA03C301A28C2463885663"
            "BD649400113A8AF4FDF0D3A8B1964D48D4B5EF696D6CE4F7EF943966E7CAB4A9EA"
            "88AD0364E454452D5D5A2EFB57049C5EDDF6AEFB068B4D5A739E5B9ACFB3F0891B"
            "972B1A1F65167EAC34FD73BDB3D60CE6886293F755A3EA6D6CF216CB00E3A28A05"
            "9A41818BEFE3A159329A335CF3BA87B65C53D691FC12FF1911");
string test_secure_key =
    hex2str("CCD540855F833A5E1480BFD2D36FAF3AEEE15DF5BEABE2691BC82DDE2A7AA910");

string blob2string(keymaster_blob_t& blob) {
    string s(reinterpret_cast<const char*>(blob.data), blob.data_length);
    return s;
}

string keyblob2string(keymaster_key_blob_t& blob) {
    string s(reinterpret_cast<const char*>(blob.key_material), blob.key_material_size);
    return s;
}

TEST(WrappedKeyTest, Simple) {

    KeymasterKeyBlob asn1;
    size_t asn1_len;

    KeymasterBlob iv = {reinterpret_cast<const uint8_t*>(test_iv.c_str()), test_iv.size()};
    KeymasterKeyBlob tk = {reinterpret_cast<const uint8_t*>(test_transit_key.c_str()),
                           test_transit_key.size()};
    KeymasterKeyBlob secure_key = {reinterpret_cast<const uint8_t*>(test_secure_key.c_str()),
                                   test_secure_key.size()};
    KeymasterBlob tag = {reinterpret_cast<const uint8_t*>(test_tag.c_str()), test_tag.size()};
    AuthorizationSet authorization_list = AuthorizationSetBuilder().AesEncryptionKey(256).build();

    EXPECT_EQ(
        build_wrapped_key(tk, iv, KM_KEY_FORMAT_RAW, secure_key, tag, authorization_list, &asn1),
        KM_ERROR_OK);

    KeymasterBlob iv2;
    KeymasterBlob tag2;
    KeymasterBlob wrapped_key_description;
    KeymasterKeyBlob secure_key2;
    KeymasterKeyBlob transit_key2;
    AuthorizationSet auth_list;
    keymaster_key_format_t key_format;
    EXPECT_EQ(parse_wrapped_key(asn1, &iv2, &transit_key2, &secure_key2, &tag2, &auth_list,
                                &key_format, &wrapped_key_description),
              KM_ERROR_OK);

    uint32_t key_size;
    auth_list.GetTagValue(TAG_KEY_SIZE, &key_size);
    EXPECT_EQ(key_size, (uint32_t)256);

    keymaster_algorithm_t algorithm;
    auth_list.GetTagValue(TAG_ALGORITHM, &algorithm);
    EXPECT_EQ(algorithm, KM_ALGORITHM_AES);

    EXPECT_EQ(key_format, (uint32_t)KM_KEY_FORMAT_RAW);
    EXPECT_EQ(blob2string(tag2), test_tag);
    EXPECT_EQ(blob2string(iv2), test_iv);
}

TEST(WrappedKeyTest, Unwrap) {
    KeymasterKeyBlob wrapped_key = {reinterpret_cast<const uint8_t*>(test_wrapped_key.c_str()),
                                    test_wrapped_key.size()};

    KeymasterKeyBlob secure_key;
    KeymasterKeyBlob transit_key;
    KeymasterBlob iv;
    KeymasterBlob tag;
    KeymasterBlob wrapped_key_description;
    AuthorizationSet auth_list;
    keymaster_key_format_t key_format;
    EXPECT_EQ(parse_wrapped_key(wrapped_key, &iv, &transit_key, &secure_key, &tag, &auth_list,
                                &key_format, &wrapped_key_description),
              KM_ERROR_OK);

    EXPECT_EQ(blob2string(tag), test_tag);
    EXPECT_EQ(blob2string(iv), test_iv);
    EXPECT_EQ(keyblob2string(secure_key), test_secure_key);
}

}  // namespace test
}  // namespace keymaster
