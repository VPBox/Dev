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
#pragma once

#include <memory>

#include "linkerconfig/namespace.h"

inline void DecorateNamespaceWithPaths(
    std::shared_ptr<android::linkerconfig::modules::Namespace> ns) {
  ns->AddSearchPath("/search_path1");
  ns->AddSearchPath("/search_path2", true, false);
  ns->AddSearchPath("/search_path3", false, false);
  ns->AddPermittedPath("/permitted_path1");
  ns->AddPermittedPath("/permitted_path2", true, false);
  ns->AddPermittedPath("/permitted_path3", false, false);
}

inline void DecorateNamespaceWithLinks(
    std::shared_ptr<android::linkerconfig::modules::Namespace> ns,
    std::string target_1, std::string target_2) {
  auto link = ns->CreateLink(target_1, false);
  link->AddSharedLib("lib1.so", "lib2.so", "lib3.so");

  ns->CreateLink(target_2, true);
}