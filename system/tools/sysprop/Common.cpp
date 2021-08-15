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

#include "Common.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <google/protobuf/text_format.h>

#include "sysprop.pb.h"

namespace {

std::string GenerateDefaultPropName(const sysprop::Properties& props,
                                    const sysprop::Property& prop);
bool IsCorrectIdentifier(const std::string& name);
bool ValidateProp(const sysprop::Properties& props,
                  const sysprop::Property& prop, std::string* err);
bool ValidateProps(const sysprop::Properties& props, std::string* err);

std::string GenerateDefaultPropName(const sysprop::Properties& props,
                                    const sysprop::Property& prop) {
  std::string ret;

  if (prop.access() != sysprop::ReadWrite) ret = "ro.";

  switch (props.owner()) {
    case sysprop::Vendor:
      ret += "vendor.";
      break;
    case sysprop::Odm:
      ret += "odm.";
      break;
    default:
      break;
  }

  ret += prop.api_name();

  return ret;
}

bool IsCorrectIdentifier(const std::string& name) {
  if (name.empty()) return false;
  if (std::isalpha(name[0]) == 0 && name[0] != '_') return false;

  return std::all_of(name.begin() + 1, name.end(), [](char ch) {
    return std::isalnum(ch) != 0 || ch == '_';
  });
}

bool IsCorrectPropertyOrApiName(const std::string& name) {
  if (name.empty()) return false;

  static std::unordered_set<char> allowed{'_', '-', '.'};

  return std::all_of(name.begin(), name.end(), [](char ch) {
    return std::isalnum(ch) != 0 || allowed.count(ch) != 0;
  });
}

bool ValidateProp(const sysprop::Properties& props,
                  const sysprop::Property& prop, std::string* err) {
  if (!IsCorrectPropertyOrApiName(prop.api_name())) {
    if (err) *err = "Invalid API name \"" + prop.api_name() + "\"";
    return false;
  }

  if (prop.type() == sysprop::Enum || prop.type() == sysprop::EnumList) {
    std::vector<std::string> names =
        android::base::Split(prop.enum_values(), "|");
    if (names.empty()) {
      if (err)
        *err = "Enum values are empty for API \"" + prop.api_name() + "\"";
      return false;
    }

    for (const std::string& name : names) {
      if (!IsCorrectIdentifier(name)) {
        if (err)
          *err = "Invalid enum value \"" + name + "\" for API \"" +
                 prop.api_name() + "\"";
        return false;
      }
    }

    std::unordered_set<std::string> name_set;
    for (const std::string& name : names) {
      if (!name_set.insert(ToUpper(name)).second) {
        if (err)
          *err = "Duplicated enum value \"" + name + "\" for API \"" +
                 prop.api_name() + "\"";
        return false;
      }
    }
  }

  std::string prop_name = prop.prop_name();
  if (prop_name.empty()) prop_name = GenerateDefaultPropName(props, prop);

  if (!IsCorrectPropertyOrApiName(prop_name)) {
    if (err) *err = "Invalid prop name \"" + prop.prop_name() + "\"";
    return false;
  }

  static const std::regex vendor_regex(
      "(init\\.svc\\.|ro\\.|persist\\.)?vendor\\..+|ro\\.hardware\\..+");
  static const std::regex odm_regex(
      "(init\\.svc\\.|ro\\.|persist\\.)?odm\\..+|ro\\.hardware\\..+");

  switch (props.owner()) {
    case sysprop::Platform:
      if (std::regex_match(prop_name, vendor_regex) ||
          std::regex_match(prop_name, odm_regex)) {
        if (err)
          *err = "Prop \"" + prop_name +
                 "\" owned by platform cannot have vendor. or odm. namespace";
        return false;
      }
      break;
    case sysprop::Vendor:
      if (!std::regex_match(prop_name, vendor_regex)) {
        if (err)
          *err = "Prop \"" + prop_name +
                 "\" owned by vendor should have vendor. namespace";
        return false;
      }
      break;
    case sysprop::Odm:
      if (!std::regex_match(prop_name, odm_regex)) {
        if (err)
          *err = "Prop \"" + prop_name +
                 "\" owned by odm should have odm. namespace";
        return false;
      }
      break;
    default:
      break;
  }

  switch (prop.access()) {
    case sysprop::ReadWrite:
      if (android::base::StartsWith(prop_name, "ro.")) {
        if (err) {
          *err = "Prop \"" + prop_name +
                 "\" is ReadWrite and also have prefix \"ro.\"";
        }
        return false;
      }
      break;
    default:
      /*
       * TODO: Some properties don't have prefix "ro." but not written in any
       * Java or C++ codes. They might be misnamed and should be readonly. Will
       * uncomment this check after fixing them all / or making a whitelist for
       * them
      if (!android::base::StartsWith(prop_name, "ro.")) {
        if (err) {
          *err = "Prop \"" + prop_name +
                 "\" isn't ReadWrite, but don't have prefix \"ro.\"";
        }
        return false;
      }
      */
      break;
  }

  if (prop.integer_as_bool() && !(prop.type() == sysprop::Boolean ||
                                  prop.type() == sysprop::BooleanList)) {
    if (err) {
      *err = "Prop \"" + prop_name +
             "\" has integer_as_bool: true, but not a boolean";
    }
    return false;
  }

  return true;
}

bool ValidateProps(const sysprop::Properties& props, std::string* err) {
  std::vector<std::string> names = android::base::Split(props.module(), ".");
  if (names.size() <= 1) {
    if (err) *err = "Invalid module name \"" + props.module() + "\"";
    return false;
  }

  for (const auto& name : names) {
    if (!IsCorrectIdentifier(name)) {
      if (err) *err = "Invalid name \"" + name + "\" in module";
      return false;
    }
  }

  if (props.prop_size() == 0) {
    if (err) *err = "There is no defined property";
    return false;
  }

  for (int i = 0; i < props.prop_size(); ++i) {
    const auto& prop = props.prop(i);
    if (!ValidateProp(props, prop, err)) return false;
  }

  std::unordered_set<std::string> prop_names;

  for (int i = 0; i < props.prop_size(); ++i) {
    const auto& prop = props.prop(i);
    auto res = prop_names.insert(ApiNameToIdentifier(prop.api_name()));

    if (!res.second) {
      if (err) *err = "Duplicated API name \"" + prop.api_name() + "\"";
      return false;
    }
  }

  return true;
}

}  // namespace

// For directory functions, we could use <filesystem> of C++17 if supported..
bool CreateDirectories(const std::string& path) {
  struct stat st;

  // If already exists..
  if (stat(path.c_str(), &st) == 0) {
    return false;
  }

  size_t last_slash = path.rfind('/');
  if (last_slash > 0 && last_slash != std::string::npos) {
    std::string parent = path.substr(0, last_slash);
    if (!IsDirectory(parent) && !CreateDirectories(parent)) return false;
  }

  // It's very unlikely, but if path contains ".." or any symbolic links, it
  // might already be created before this line.
  return mkdir(path.c_str(), 0755) == 0 || IsDirectory(path);
}

bool IsDirectory(const std::string& path) {
  struct stat st;

  if (stat(path.c_str(), &st) == -1) return false;
  return S_ISDIR(st.st_mode);
}

bool IsListProp(const sysprop::Property& prop) {
  switch (prop.type()) {
    case sysprop::BooleanList:
    case sysprop::IntegerList:
    case sysprop::LongList:
    case sysprop::DoubleList:
    case sysprop::StringList:
    case sysprop::EnumList:
      return true;
    default:
      return false;
  }
}

std::string GetModuleName(const sysprop::Properties& props) {
  const std::string& module = props.module();
  return module.substr(module.rfind('.') + 1);
}

bool ParseProps(const std::string& input_file_path, sysprop::Properties* props,
                std::string* err) {
  std::string file_contents;

  if (!android::base::ReadFileToString(input_file_path, &file_contents, true)) {
    *err = "Error reading file " + input_file_path + ": " + strerror(errno);
    return false;
  }

  if (!google::protobuf::TextFormat::ParseFromString(file_contents, props)) {
    *err = "Error parsing file " + input_file_path;
    return false;
  }

  if (!ValidateProps(*props, err)) {
    return false;
  }

  for (int i = 0; i < props->prop_size(); ++i) {
    // set each optional field to its default value
    sysprop::Property& prop = *props->mutable_prop(i);
    if (prop.prop_name().empty())
      prop.set_prop_name(GenerateDefaultPropName(*props, prop));
  }

  return true;
}

std::string ToUpper(std::string str) {
  for (char& ch : str) {
    ch = toupper(ch);
  }
  return str;
}

std::string ApiNameToIdentifier(const std::string& name) {
  static const std::regex kRegexAllowed{"-|\\."};
  return (isdigit(name[0]) ? "_" : "") +
         std::regex_replace(name, kRegexAllowed, "_");
}
