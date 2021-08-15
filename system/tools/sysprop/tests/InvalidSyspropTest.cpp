/*
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

#include <cstdio>
#include <string>

#include <android-base/file.h>
#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "Common.h"
#include "sysprop.pb.h"

namespace {

constexpr const char* kDuplicatedField =
    R"(
owner: Vendor
module: "com.error.DuplicatedField"
prop {
    api_name: "dup"
    type: Integer
    scope: Internal
    access: Readonly
}
prop {
    api_name: "dup"
    type: Long
    scope: Public
    access: ReadWrite
}
)";

constexpr const char* kEmptyProp =
    R"(
owner: Vendor
module: "com.google.EmptyProp"
)";

constexpr const char* kInvalidApiName =
    R"(
owner: Odm
module: "odm.invalid.prop.name"
prop {
    api_name: "!@#$"
    type: Integer
    scope: System
    access: ReadWrite
}
)";

constexpr const char* kEmptyEnumValues =
    R"(
owner: Odm
module: "test.manufacturer"
prop {
    api_name: "empty_enum_value"
    type: Enum
    scope: Internal
    access: ReadWrite
}
)";

constexpr const char* kDuplicatedEnumValue =
    R"(
owner: Vendor
module: "vendor.module.name"
prop {
    api_name: "status"
    type: Enum
    enum_values: "oN|off|intermediate|On"
    scope: Public
    access: ReadWrite
}
)";

constexpr const char* kInvalidModuleName =
    R"(
owner: Platform
module: ""
prop {
    api_name: "integer"
    type: Integer
    scope: Public
    access: ReadWrite
}
)";

constexpr const char* kInvalidNamespaceForPlatform =
    R"(
owner: Platform
module: "android.PlatformProperties"
prop {
    api_name: "vendor.build.utc_long"
    type: Long
    scope: System
    access: ReadWrite
}
)";

constexpr const char* kRoPrefixForReadWriteProperty =
    R"(
owner: Vendor
module: "com.android.VendorProp"
prop {
    api_name: "i_am_readwrite"
    type: Long
    scope: System
    prop_name: "ro.vendor.i_am_readwrite"
    access: ReadWrite
}
)";

constexpr const char* kIntegerAsBoolWithWrongType =
    R"(
owner: Platform
module: "android.os.LongProp"
prop {
    api_name: "longprop"
    type: Long
    scope: Internal
    prop_name: "long.prop"
    access: ReadWrite
    integer_as_bool: true
}
)";

/*
 * TODO: Some properties don't have prefix "ro." but not written in any
 * Java or C++ codes. They might be misnamed and should be readonly. Will
 * uncomment this check after fixing them all / or making a whitelist for
 * them
constexpr const char* kNoRoPrefixForReadonlyProperty =
    R"(
owner: Odm
module: "com.android.OdmProp"
prop {
    api_name: "i.am.readonly"
    type: Long
    scope: System
    prop_name: "odm.i_am_readwrite"
    access: Readonly
}
)";
*/

constexpr const char* kTestCasesAndExpectedErrors[][2] = {
    {kDuplicatedField, "Duplicated API name \"dup\""},
    {kEmptyProp, "There is no defined property"},
    {kInvalidApiName, "Invalid API name \"!@#$\""},
    {kEmptyEnumValues, "Invalid enum value \"\" for API \"empty_enum_value\""},
    {kDuplicatedEnumValue, "Duplicated enum value \"On\" for API \"status\""},
    {kInvalidModuleName, "Invalid module name \"\""},
    {kInvalidNamespaceForPlatform,
     "Prop \"vendor.build.utc_long\" owned by platform cannot have vendor. or "
     "odm. "
     "namespace"},
    {kRoPrefixForReadWriteProperty,
     "Prop \"ro.vendor.i_am_readwrite\" is ReadWrite and also have prefix "
     "\"ro.\""},
    {kIntegerAsBoolWithWrongType,
     "Prop \"long.prop\" has integer_as_bool: true, but not a boolean"},
    /*    {kNoRoPrefixForReadonlyProperty,
         "Prop \"odm.i_am_readwrite\" isn't ReadWrite, but don't have prefix "
         "\"ro.\""},*/
};

}  // namespace

TEST(SyspropTest, InvalidSyspropTest) {
  TemporaryFile file;
  close(file.fd);
  file.fd = -1;

  for (auto [test_case, expected_error] : kTestCasesAndExpectedErrors) {
    ASSERT_TRUE(android::base::WriteStringToFile(test_case, file.path));
    std::string err;
    sysprop::Properties props;
    EXPECT_FALSE(ParseProps(file.path, &props, &err));
    EXPECT_EQ(err, expected_error);
  }
}
