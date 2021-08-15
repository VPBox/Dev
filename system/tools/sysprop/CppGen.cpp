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

#define LOG_TAG "sysprop_cpp_gen"

#include "CppGen.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <cerrno>
#include <regex>
#include <string>

#include "CodeWriter.h"
#include "Common.h"
#include "sysprop.pb.h"

namespace {

constexpr const char* kIndent = "    ";

constexpr const char* kCppHeaderIncludes =
    R"(#include <cstdint>
#include <optional>
#include <string>
#include <vector>

)";

constexpr const char* kCppSourceIncludes =
    R"(#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <limits>
#include <utility>

#include <strings.h>
#include <sys/system_properties.h>

#include <android-base/parseint.h>
#include <log/log.h>

)";

constexpr const char* kCppParsersAndFormatters =
    R"(template <typename T> constexpr bool is_vector = false;

template <typename T> constexpr bool is_vector<std::vector<T>> = true;

template <> [[maybe_unused]] std::optional<bool> DoParse(const char* str) {
    static constexpr const char* kYes[] = {"1", "true"};
    static constexpr const char* kNo[] = {"0", "false"};

    for (const char* yes : kYes) {
        if (strcasecmp(yes, str) == 0) return std::make_optional(true);
    }

    for (const char* no : kNo) {
        if (strcasecmp(no, str) == 0) return std::make_optional(false);
    }

    return std::nullopt;
}

template <> [[maybe_unused]] std::optional<std::int32_t> DoParse(const char* str) {
    std::int32_t ret;
    return android::base::ParseInt(str, &ret) ? std::make_optional(ret) : std::nullopt;
}

template <> [[maybe_unused]] std::optional<std::int64_t> DoParse(const char* str) {
    std::int64_t ret;
    return android::base::ParseInt(str, &ret) ? std::make_optional(ret) : std::nullopt;
}

template <> [[maybe_unused]] std::optional<double> DoParse(const char* str) {
    int old_errno = errno;
    errno = 0;
    char* end;
    double ret = std::strtod(str, &end);
    if (errno != 0) {
        return std::nullopt;
    }
    if (str == end || *end != '\0') {
        errno = EINVAL;
        return std::nullopt;
    }
    errno = old_errno;
    return std::make_optional(ret);
}

template <> [[maybe_unused]] std::optional<std::string> DoParse(const char* str) {
    return *str == '\0' ? std::nullopt : std::make_optional(str);
}

template <typename Vec> [[maybe_unused]] Vec DoParseList(const char* str) {
    Vec ret;
    const char* p = str;
    for (;;) {
        const char* found = p;
        while (*found != '\0' && *found != ',') {
            ++found;
        }
        std::string value(p, found);
        ret.emplace_back(DoParse<typename Vec::value_type>(value.c_str()));
        if (*found == '\0') break;
        p = found + 1;
    }
    return ret;
}

template <typename T> inline T TryParse(const char* str) {
    if constexpr(is_vector<T>) {
        return DoParseList<T>(str);
    } else {
        return DoParse<T>(str);
    }
}

[[maybe_unused]] std::string FormatValue(const std::optional<std::int32_t>& value) {
    return value ? std::to_string(*value) : "";
}

[[maybe_unused]] std::string FormatValue(const std::optional<std::int64_t>& value) {
    return value ? std::to_string(*value) : "";
}

[[maybe_unused]] std::string FormatValue(const std::optional<double>& value) {
    if (!value) return "";
    char buf[1024];
    std::sprintf(buf, "%.*g", std::numeric_limits<double>::max_digits10, *value);
    return buf;
}

[[maybe_unused]] std::string FormatValue(const std::optional<bool>& value) {
    return value ? (*value ? "true" : "false") : "";
}

template <typename T>
[[maybe_unused]] std::string FormatValue(const std::vector<T>& value) {
    if (value.empty()) return "";

    std::string ret;
    bool first = true;

    for (auto&& element : value) {
        if (!first) ret += ",";
        else first = false;
        if constexpr(std::is_same_v<T, std::optional<std::string>>) {
            if (element) ret += *element;
        } else {
            ret += FormatValue(element);
        }
    }

    return ret;
}

template <typename T>
T GetProp(const char* key) {
    T ret;
    auto pi = __system_property_find(key);
    if (pi != nullptr) {
        __system_property_read_callback(pi, [](void* cookie, const char*, const char* value, std::uint32_t) {
            *static_cast<T*>(cookie) = TryParse<T>(value);
        }, &ret);
    }
    return ret;
}

)";

const std::regex kRegexDot{"\\."};
const std::regex kRegexUnderscore{"_"};

std::string GetCppEnumName(const sysprop::Property& prop);
std::string GetCppPropTypeName(const sysprop::Property& prop);
std::string GetCppNamespace(const sysprop::Properties& props);

std::string GenerateHeader(const sysprop::Properties& props,
                           sysprop::Scope scope);
std::string GenerateSource(const sysprop::Properties& props,
                           const std::string& include_name);

std::string GetCppEnumName(const sysprop::Property& prop) {
  return ApiNameToIdentifier(prop.api_name()) + "_values";
}

std::string GetCppPropTypeName(const sysprop::Property& prop) {
  switch (prop.type()) {
    case sysprop::Boolean:
      return "std::optional<bool>";
    case sysprop::Integer:
      return "std::optional<std::int32_t>";
    case sysprop::Long:
      return "std::optional<std::int64_t>";
    case sysprop::Double:
      return "std::optional<double>";
    case sysprop::String:
      return "std::optional<std::string>";
    case sysprop::Enum:
      return "std::optional<" + GetCppEnumName(prop) + ">";
    case sysprop::BooleanList:
      return "std::vector<std::optional<bool>>";
    case sysprop::IntegerList:
      return "std::vector<std::optional<std::int32_t>>";
    case sysprop::LongList:
      return "std::vector<std::optional<std::int64_t>>";
    case sysprop::DoubleList:
      return "std::vector<std::optional<double>>";
    case sysprop::StringList:
      return "std::vector<std::optional<std::string>>";
    case sysprop::EnumList:
      return "std::vector<std::optional<" + GetCppEnumName(prop) + ">>";
    default:
      __builtin_unreachable();
  }
}

std::string GetCppNamespace(const sysprop::Properties& props) {
  return std::regex_replace(props.module(), kRegexDot, "::");
}

std::string GenerateHeader(const sysprop::Properties& props,
                           sysprop::Scope scope) {
  CodeWriter writer(kIndent);

  writer.Write("%s", kGeneratedFileFooterComments);

  writer.Write("#pragma once\n\n");
  writer.Write("%s", kCppHeaderIncludes);

  std::string cpp_namespace = GetCppNamespace(props);
  writer.Write("namespace %s {\n\n", cpp_namespace.c_str());

  bool first = true;

  for (int i = 0; i < props.prop_size(); ++i) {
    const sysprop::Property& prop = props.prop(i);

    // Scope: Internal > System > Public
    if (prop.scope() > scope) continue;

    if (!first) {
      writer.Write("\n");
    } else {
      first = false;
    }

    std::string prop_id = ApiNameToIdentifier(prop.api_name());
    std::string prop_type = GetCppPropTypeName(prop);

    if (prop.type() == sysprop::Enum || prop.type() == sysprop::EnumList) {
      writer.Write("enum class %s {\n", GetCppEnumName(prop).c_str());
      writer.Indent();
      for (const std::string& name :
           android::base::Split(prop.enum_values(), "|")) {
        writer.Write("%s,\n", ToUpper(name).c_str());
      }
      writer.Dedent();
      writer.Write("};\n\n");
    }

    writer.Write("%s %s();\n", prop_type.c_str(), prop_id.c_str());
    if (prop.access() != sysprop::Readonly && scope == sysprop::Internal) {
      writer.Write("bool %s(const %s& value);\n", prop_id.c_str(),
                   prop_type.c_str());
    }
  }

  writer.Write("\n}  // namespace %s\n", cpp_namespace.c_str());

  return writer.Code();
}

std::string GenerateSource(const sysprop::Properties& props,
                           const std::string& include_name) {
  CodeWriter writer(kIndent);
  writer.Write("%s", kGeneratedFileFooterComments);
  writer.Write("#include <%s>\n\n", include_name.c_str());
  writer.Write("%s", kCppSourceIncludes);

  std::string cpp_namespace = GetCppNamespace(props);

  writer.Write("namespace {\n\n");
  writer.Write("using namespace %s;\n\n", cpp_namespace.c_str());
  writer.Write("template <typename T> T DoParse(const char* str);\n\n");

  for (int i = 0; i < props.prop_size(); ++i) {
    const sysprop::Property& prop = props.prop(i);
    if (prop.type() != sysprop::Enum && prop.type() != sysprop::EnumList) {
      continue;
    }

    std::string prop_id = ApiNameToIdentifier(prop.api_name());
    std::string enum_name = GetCppEnumName(prop);

    writer.Write("constexpr const std::pair<const char*, %s> %s_list[] = {\n",
                 enum_name.c_str(), prop_id.c_str());
    writer.Indent();
    for (const std::string& name :
         android::base::Split(prop.enum_values(), "|")) {
      writer.Write("{\"%s\", %s::%s},\n", name.c_str(), enum_name.c_str(),
                   ToUpper(name).c_str());
    }
    writer.Dedent();
    writer.Write("};\n\n");

    writer.Write("template <>\n");
    writer.Write("std::optional<%s> DoParse(const char* str) {\n",
                 enum_name.c_str());
    writer.Indent();
    writer.Write("for (auto [name, val] : %s_list) {\n", prop_id.c_str());
    writer.Indent();
    writer.Write("if (strcmp(str, name) == 0) {\n");
    writer.Indent();
    writer.Write("return val;\n");
    writer.Dedent();
    writer.Write("}\n");
    writer.Dedent();
    writer.Write("}\n");
    writer.Write("return std::nullopt;\n");
    writer.Dedent();
    writer.Write("}\n\n");

    if (prop.access() != sysprop::Readonly) {
      writer.Write("std::string FormatValue(std::optional<%s> value) {\n",
                   enum_name.c_str());
      writer.Indent();
      writer.Write("if (!value) return \"\";\n");
      writer.Write("for (auto [name, val] : %s_list) {\n", prop_id.c_str());
      writer.Indent();
      writer.Write("if (val == *value) {\n");
      writer.Indent();
      writer.Write("return name;\n");
      writer.Dedent();
      writer.Write("}\n");
      writer.Dedent();
      writer.Write("}\n");

      writer.Write(
          "LOG_ALWAYS_FATAL(\"Invalid value %%d for property %s\", "
          "static_cast<std::int32_t>(*value));\n",
          prop.prop_name().c_str());

      writer.Write("__builtin_unreachable();\n");
      writer.Dedent();
      writer.Write("}\n\n");
    }
  }
  writer.Write("%s", kCppParsersAndFormatters);
  writer.Write("}  // namespace\n\n");

  writer.Write("namespace %s {\n\n", cpp_namespace.c_str());

  for (int i = 0; i < props.prop_size(); ++i) {
    if (i > 0) writer.Write("\n");

    const sysprop::Property& prop = props.prop(i);
    std::string prop_id = ApiNameToIdentifier(prop.api_name());
    std::string prop_type = GetCppPropTypeName(prop);

    writer.Write("%s %s() {\n", prop_type.c_str(), prop_id.c_str());
    writer.Indent();
    writer.Write("return GetProp<%s>(\"%s\");\n", prop_type.c_str(),
                 prop.prop_name().c_str());
    writer.Dedent();
    writer.Write("}\n");

    if (prop.access() != sysprop::Readonly) {
      writer.Write("\nbool %s(const %s& value) {\n", prop_id.c_str(),
                   prop_type.c_str());
      writer.Indent();

      const char* format_expr = "FormatValue(value).c_str()";

      // Specialized formatters here
      if (prop.type() == sysprop::String) {
        format_expr = "value ? value->c_str() : \"\"";
      } else if (prop.integer_as_bool()) {
        if (prop.type() == sysprop::Boolean) {
          // optional<bool> -> optional<int>
          format_expr = "FormatValue(std::optional<int>(value)).c_str()";
        } else if (prop.type() == sysprop::BooleanList) {
          // vector<optional<bool>> -> vector<optional<int>>
          format_expr =
              "FormatValue(std::vector<std::optional<int>>("
              "value.begin(), value.end())).c_str()";
        }
      }

      writer.Write("return __system_property_set(\"%s\", %s) == 0;\n",
                   prop.prop_name().c_str(), format_expr);
      writer.Dedent();
      writer.Write("}\n");
    }
  }

  writer.Write("\n}  // namespace %s\n", cpp_namespace.c_str());

  return writer.Code();
}

}  // namespace

bool GenerateCppFiles(const std::string& input_file_path,
                      const std::string& header_dir,
                      const std::string& system_header_dir,
                      const std::string& source_output_dir,
                      const std::string& include_name, std::string* err) {
  sysprop::Properties props;

  if (!ParseProps(input_file_path, &props, err)) {
    return false;
  }

  std::string output_basename = android::base::Basename(input_file_path);

  for (auto&& [scope, dir] : {
           std::pair(sysprop::Internal, header_dir),
           std::pair(sysprop::System, system_header_dir),
       }) {
    if (!IsDirectory(dir) && !CreateDirectories(dir)) {
      *err = "Creating directory to " + dir + " failed: " + strerror(errno);
      return false;
    }

    std::string path = dir + "/" + output_basename + ".h";
    std::string result = GenerateHeader(props, scope);

    if (!android::base::WriteStringToFile(result, path)) {
      *err =
          "Writing generated header to " + path + " failed: " + strerror(errno);
      return false;
    }
  }

  std::string source_path = source_output_dir + "/" + output_basename + ".cpp";
  std::string source_result = GenerateSource(props, include_name);

  if (!android::base::WriteStringToFile(source_result, source_path)) {
    *err = "Writing generated source to " + source_path +
           " failed: " + strerror(errno);
    return false;
  }

  return true;
}
