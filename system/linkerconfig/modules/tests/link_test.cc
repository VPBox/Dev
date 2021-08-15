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

#include <gtest/gtest.h>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/link.h"

constexpr const char* kSharedLibsExpectedResult =
    R"(namespace.originalNamespace.link.targetNamespace.shared_libs = lib1.so
namespace.originalNamespace.link.targetNamespace.shared_libs += lib2.so
namespace.originalNamespace.link.targetNamespace.shared_libs += lib3.so
)";

TEST(linkerconfig_link, link_with_all_shared_libs) {
  android::linkerconfig::modules::ConfigWriter writer;

  auto link = std::make_shared<android::linkerconfig::modules::Link>(
      "originalNamespace", "targetNamespace", true);

  link->WriteConfig(writer);
  auto config_text = writer.ToString();

  ASSERT_EQ(config_text,
            "namespace.originalNamespace.link.targetNamespace.allow_all_shared_"
            "libs = true\n");
}

TEST(linkerconfig_link, link_with_shared_libs) {
  android::linkerconfig::modules::ConfigWriter writer;
  auto link = std::make_shared<android::linkerconfig::modules::Link>(
      "originalNamespace", "targetNamespace");
  link->AddSharedLib("lib1.so", "lib2.so", "lib3.so");

  link->WriteConfig(writer);
  auto config_text = writer.ToString();

  ASSERT_EQ(config_text, kSharedLibsExpectedResult);
}