/**
 * Copyright (C) 2018 The Android Open Source Project
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

#include <stdlib.h>
#include <string.h>

#include <android-base/logging.h>
#include <gtest/gtest.h>

#include "installd_constants.h"
#include "otapreopt_parameters.h"

namespace android {
namespace installd {

static bool ParseBool(const char* in) {
    if (strcmp(in, "true") == 0) {
        return true;
    }
    return false;
}

static const char* ParseNull(const char* arg) {
    return (strcmp(arg, "!") == 0) ? nullptr : arg;
}

class OTAPreoptTest : public testing::Test {
protected:
    virtual void SetUp() {
        setenv("ANDROID_LOG_TAGS", "*:f", 1);
        android::base::InitLogging(nullptr, android::base::StderrLogger);
    }

    void verifyPackageParameters(const OTAPreoptParameters& params,
                                 uint32_t version,
                                 bool versioned,
                                 const char** args) {
        //  otapreopt target-slot [version] dexopt {DEXOPT_PARAMETERS}
        int i = 0;
        if (version > 2 || (version == 2 && versioned)) {
            i += 4;
        } else {
            i += 3;
        }
        ASSERT_STREQ(params.target_slot.c_str(), args[1]);
        ASSERT_STREQ(params.apk_path, args[i++]);
        ASSERT_EQ(params.uid, static_cast<uid_t>(atoi(args[i++])));
        ASSERT_STREQ(params.pkgName, args[i++]);
        ASSERT_STREQ(params.instruction_set, args[i++]);
        ASSERT_EQ(params.dexopt_needed, atoi(args[i++]));
        ASSERT_STREQ(params.oat_dir, args[i++]);
        const int dexopt_flags = atoi(args[i++]);
        ASSERT_STREQ(params.compiler_filter, args[i++]);
        ASSERT_STREQ(params.volume_uuid, ParseNull(args[i++]));
        ASSERT_STREQ(params.shared_libraries, ParseNull(args[i++]));
        if (version > 1) {
            ASSERT_STREQ(params.se_info, ParseNull(args[i++]));
        } else {
            ASSERT_EQ(params.se_info, nullptr);
        }
        if (version > 2) {
            ASSERT_EQ(params.downgrade, ParseBool(args[i++]));
        } else {
            ASSERT_FALSE(params.downgrade);
        }
        if (version > 3) {
            ASSERT_EQ(params.target_sdk_version, atoi(args[i++]));
        } else {
            ASSERT_EQ(params.target_sdk_version, 0);
        }
        if (version > 4) {
            ASSERT_STREQ(params.profile_name, ParseNull(args[i++]));
        } else {
            ASSERT_STREQ(params.profile_name, "primary.prof");
        }
        if (version > 5) {
            ASSERT_STREQ(params.dex_metadata_path, ParseNull(args[i++]));
        } else {
            ASSERT_EQ(params.dex_metadata_path, nullptr);
        }
        if (version > 6) {
            ASSERT_STREQ(params.compilation_reason, ParseNull(args[i++]));
        } else {
            ASSERT_STREQ(params.compilation_reason, "ab-ota");
        }
        if (version > 7) {
            ASSERT_EQ(params.dexopt_flags, dexopt_flags);
        } else {
            ASSERT_EQ(params.dexopt_flags, dexopt_flags | DEXOPT_GENERATE_COMPACT_DEX);
        }
    }

    const char* getVersionCStr(uint32_t version) {
        switch (version) {
            case 1: return "1";
            case 2: return "2";
            case 3: return "3";
            case 4: return "4";
            case 5: return "5";
            case 6: return "6";
            case 7: return "7";
            case 8: return "8";
            case 9: return "9";
            case 10: return "10";
        }
        return nullptr;
    }

    std::vector<const char*> getArgs(uint32_t version,
                                     bool versioned,
                                     const char* shared_libs = "shared.lib") {
        std::vector<const char*> args;
        args.push_back("otapreopt");  // "otapreopt"
        args.push_back("a");  // slot
        if (versioned) {
            args.push_back(getVersionCStr(version));
        }
        args.push_back("dexopt");  // "dexopt"
        args.push_back("foo.apk");  // apk_path
        args.push_back("123");  // uid
        args.push_back("pkgname");  // pkg
        args.push_back("arm");  // isa
        args.push_back("1");  // dexopt_needed (DEX2OAT_FROM_SCRATCH)
        args.push_back("oat_dir");  // oat_dir
        args.push_back("0");  // dexopt_flags
        args.push_back("speed");  // filter
        args.push_back("!");  // volume
        args.push_back(shared_libs);  // libs

        if (version > 1) {
            args.push_back("!");  // seinfo
        }
        if (version > 2) {
            args.push_back("true");  // downgrade
        }
        if (version > 3) {
            args.push_back("28");  // sdk_version
        }
        if (version > 4) {
            args.push_back("split_a.prof");  // profile_name
        }
        if (version > 5) {
            args.push_back("dex_metadata.dm");  // dex_metadata_path
        }
        if (version > 6) {
            args.push_back("ab-ota-test");  // compilation_reason
        }
        args.push_back(nullptr);  // we have to end with null.
        return args;
    }

    void VerifyReadArguments(uint32_t version,
                             bool versioned,
                             const char* shared_libs = "shared.lib") {
        OTAPreoptParameters params;
        std::vector<const char*> args = getArgs(version, versioned, shared_libs);
        ASSERT_TRUE(params.ReadArguments(args.size() - 1, args.data()));
        verifyPackageParameters(params, version, versioned, args.data());
    }
};

TEST_F(OTAPreoptTest, ReadArgumentsV1) {
    VerifyReadArguments(1, false);
}

TEST_F(OTAPreoptTest, ReadArgumentsV2Unversioned) {
    VerifyReadArguments(2, false);
}

TEST_F(OTAPreoptTest, ReadArgumentsV2) {
    VerifyReadArguments(2, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV3) {
    VerifyReadArguments(3, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV4) {
    VerifyReadArguments(4, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV5) {
    VerifyReadArguments(5, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV6) {
    VerifyReadArguments(6, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV7) {
    VerifyReadArguments(7, true);
}

TEST_F(OTAPreoptTest, ReadArgumentsV9SharedLibsAmpersand) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(9, true, "&");
    ASSERT_FALSE(params.ReadArguments(args.size() - 1, args.data()));
}

TEST_F(OTAPreoptTest, ReadArgumentsV10SharedLibsAmpersand) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(10, true, "&");
    ASSERT_TRUE(params.ReadArguments(args.size() - 1, args.data()));
}

TEST_F(OTAPreoptTest, ReadArgumentsFailToManyArgs) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(5, true);
    args[2] = "3";  // pretend it's version 3. It should fail since there are too many args.
    ASSERT_FALSE(params.ReadArguments(args.size() - 1, args.data()));
}

TEST_F(OTAPreoptTest, ReadArgumentsFailInsufficientArgs) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(4, true);
    args[2] = "5";  // pretend it's version 5. It should fail since there are insufficient args.
    ASSERT_FALSE(params.ReadArguments(args.size() - 1, args.data()));
}

TEST_F(OTAPreoptTest, ReadArgumentsFailInvalidDexopt) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(4, true);
    args[3] = "dexopt-invalid";
    ASSERT_FALSE(params.ReadArguments(args.size() - 1, args.data()));
}

TEST_F(OTAPreoptTest, ReadArgumentsFailInvalidSlot) {
    OTAPreoptParameters params;
    std::vector<const char*> args = getArgs(3, true);
    args[1] = "invalid-slot???";
    ASSERT_FALSE(params.ReadArguments(args.size() - 1, args.data()));
}

}  // namespace installd
}  // namespace android
