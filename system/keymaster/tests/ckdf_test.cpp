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

#include <keymaster/km_openssl/ckdf.h>

#include <gtest/gtest.h>
#include <string.h>

#include "android_keymaster_test_utils.h"

using std::string;

namespace keymaster {
namespace test {

struct CkdfTest {
    const char* key;
    const char* label;
    const char* context;
    const char* output;
};

static const CkdfTest kCkdfTests[] = {{"80583f389dd797a3d18abd7b9399da02"
                                       "6fecb1eade7bc2f0ef091ad39e613c35",
                                       "c16e6e02c5a3dcc8d78b9ac1306877761310455b4e41469951d9e6",
                                       "c2245a064b33fd8c3b01203a7824485bf0a64060c4648b707d260793",
                                       "5d42a1941670917d746c7278e75f4879"
                                       "750469dcb59c129e42edb7a3273f38d4"
                                       "ea6fbcba9f422f735fc2db23603c63e5"
                                       "86ff39cc048f4bc18690e478dd1108fa"
                                       "fc635b29acb6b29784fdf8184296fa7f"
                                       "772b62cdd1a8bd1a2d073830fac0409b"
                                       "45acedf53a70676de96d7cb7e337cec4"
                                       "08d5e3d626ac6c775baf71368b1d5851"
                                       "47585f06b305ad5f547cb40644d2e048"
                                       "7a9ded9778ddbfac15a6a7aee399fc7d"
                                       "92610b028c624fd68cb573b830d842c2"
                                       "ceb34da13efd50db13165a4f19d38cea"
                                       "3293a073ba2d1bb31642297764b0fc17"
                                       "e941ba73d703ba77455f30f9293a41fe"
                                       "2915358c99f95a55075811d57ddff3d3"
                                       "67d0a59e5b2f4e1c697b1e9955aa972c"
                                       "f43d5b81c242a2b8eda917b25dc689be"
                                       "f514b39979b7181eb5db62eb39cd0c3a"
                                       "3dcb8013b19bdb262a890fce3360a351"
                                       "cb3ddf76c13606177479b6e1345a2705"
                                       "eaf97715af161b17b715ab6ef006e697"
                                       "a1a779ea879a10c258069c4d9522d411"
                                       "70aa69132d6e5cecb7ada5d16973d77f"
                                       "3d7cc647175604d7151480473e61e73f"
                                       "36227324058f38f578198a19e083db2b"
                                       "8454ee2a00b527a99e3ec9addbfbd3a6"
                                       "8c51cab16a720b7f47fe6fbfb4ca541c"
                                       "2ec4683588ce2106fc907d987620ee48"
                                       "aa506b8a246a18e2fa156d66b5add15c"
                                       "2305615bb1c7c76d95aa679545eac38b"
                                       "806cd02e5ef89897e278a536c25553f4"
                                       "05d12474"}};

template <class Blob> Blob hex2span(const char* hex) {
    string bytes = hex2str(hex);
    Blob retval(reinterpret_cast<const uint8_t*>(bytes.data()), bytes.size());
    return std::move(retval);
}

KeymasterKeyBlob hex2key(const char* hex) {
    return hex2span<KeymasterKeyBlob>(hex);
}

KeymasterBlob hex2blob(const char* hex) {
    return hex2span<KeymasterBlob>(hex);
}

TEST(CkdfTest, Ckdf) {
    for (auto& test : kCkdfTests) {
        auto key = hex2key(test.key);
        auto label = hex2blob(test.label);
        auto context = hex2blob(test.context);
        auto expected = hex2blob(test.output);

        KeymasterKeyBlob output;
        output.Reset(expected.data_length);

        ASSERT_EQ(KM_ERROR_OK, ckdf(key, label, context, &output));
        EXPECT_TRUE(std::equal(output.begin(), output.end(), expected.begin()));
    }
}

}  // namespace test
}  // namespace keymaster
