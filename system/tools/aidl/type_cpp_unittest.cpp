/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "type_cpp.h"

using std::string;
using std::unique_ptr;

namespace android {
namespace aidl {
namespace cpp {

namespace {

string kParcelableDotName = "Outer.Inner";
string kParcelableColonName = "Outer::Inner";

}  // namespace

class CppTypeNamespaceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    types_.Init();
  }
  TypeNamespace types_;
};

TEST_F(CppTypeNamespaceTest, HasSomeBasicTypes) {
  EXPECT_TRUE(types_.HasTypeByCanonicalName("byte"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("int"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("long"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("float"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("double"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("boolean"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("char"));
  EXPECT_TRUE(types_.HasTypeByCanonicalName("String"));
}

TEST_F(CppTypeNamespaceTest, SupportsListString) {
  EXPECT_TRUE(
      types_.HasTypeByCanonicalName("java.util.List<java.lang.String>"));
}

TEST_F(CppTypeNamespaceTest, SupportsNestedParcelableClass) {
  unique_ptr<AidlParcelable> parcelable(new AidlParcelable(
      AIDL_LOCATION_HERE, new AidlQualifiedName(AIDL_LOCATION_HERE, kParcelableDotName, ""),
      {"a", "goog"}, ""));
  EXPECT_EQ(parcelable->GetCppName(), kParcelableColonName);
}

}  // namespace cpp
}  // namespace android
}  // namespace aidl
