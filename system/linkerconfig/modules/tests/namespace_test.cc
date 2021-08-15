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
#include "modules_testbase.h"

constexpr const char* kExpectedSimpleNamespaceConfig =
    R"(namespace.test_namespace.isolated = false
namespace.test_namespace.search.paths = /search_path1
namespace.test_namespace.search.paths += /search_path2
namespace.test_namespace.search.paths += /search_path3
namespace.test_namespace.permitted.paths = /permitted_path1
namespace.test_namespace.permitted.paths += /permitted_path2
namespace.test_namespace.permitted.paths += /permitted_path3
namespace.test_namespace.asan.search.paths = /search_path1
namespace.test_namespace.asan.search.paths += /data/asan/search_path1
namespace.test_namespace.asan.search.paths += /search_path2
namespace.test_namespace.asan.permitted.paths = /permitted_path1
namespace.test_namespace.asan.permitted.paths += /data/asan/permitted_path1
namespace.test_namespace.asan.permitted.paths += /permitted_path2
)";

constexpr const char* kExpectedNamespaceWithLinkConfig =
    R"(namespace.test_namespace.isolated = true
namespace.test_namespace.visible = true
namespace.test_namespace.search.paths = /search_path1
namespace.test_namespace.search.paths += /search_path2
namespace.test_namespace.search.paths += /search_path3
namespace.test_namespace.permitted.paths = /permitted_path1
namespace.test_namespace.permitted.paths += /permitted_path2
namespace.test_namespace.permitted.paths += /permitted_path3
namespace.test_namespace.asan.search.paths = /search_path1
namespace.test_namespace.asan.search.paths += /data/asan/search_path1
namespace.test_namespace.asan.search.paths += /search_path2
namespace.test_namespace.asan.permitted.paths = /permitted_path1
namespace.test_namespace.asan.permitted.paths += /data/asan/permitted_path1
namespace.test_namespace.asan.permitted.paths += /permitted_path2
namespace.test_namespace.links = target_namespace1,target_namespace2
namespace.test_namespace.link.target_namespace1.shared_libs = lib1.so
namespace.test_namespace.link.target_namespace1.shared_libs += lib2.so
namespace.test_namespace.link.target_namespace1.shared_libs += lib3.so
namespace.test_namespace.link.target_namespace2.allow_all_shared_libs = true
)";

TEST(linkerconfig_namespace, simple_namespace) {
  android::linkerconfig::modules::ConfigWriter writer;
  auto ns = std::make_shared<android::linkerconfig::modules::Namespace>(
      "test_namespace");

  DecorateNamespaceWithPaths(ns);
  ns->WriteConfig(writer);
  auto config = writer.ToString();

  ASSERT_EQ(config, kExpectedSimpleNamespaceConfig);
}

TEST(linkerconfig_namespace, namespace_with_links) {
  android::linkerconfig::modules::ConfigWriter writer;
  auto ns = std::make_shared<android::linkerconfig::modules::Namespace>(
      "test_namespace", /*is_isolated*/ true,
      /*is_visible*/ true);

  DecorateNamespaceWithPaths(ns);
  DecorateNamespaceWithLinks(ns, "target_namespace1", "target_namespace2");
  ns->WriteConfig(writer);
  auto config = writer.ToString();

  ASSERT_EQ(config, kExpectedNamespaceWithLinkConfig);
}