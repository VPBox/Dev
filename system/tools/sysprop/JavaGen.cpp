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

#define LOG_TAG "sysprop_java_gen"

#include "JavaGen.h"

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

constexpr const char* kJavaFileImports =
    R"(import android.annotation.SystemApi;

import android.os.SystemProperties;
import java.util.ArrayList;
import java.util.function.Function;
import java.util.List;
import java.util.Locale;
import java.util.Optional;
import java.util.StringJoiner;
import java.util.stream.Collectors;

)";

constexpr const char* kJavaParsersAndFormatters =
    R"(private static Boolean tryParseBoolean(String str) {
    switch (str.toLowerCase(Locale.US)) {
        case "1":
        case "true":
            return Boolean.TRUE;
        case "0":
        case "false":
            return Boolean.FALSE;
        default:
            return null;
    }
}

private static Integer tryParseInteger(String str) {
    try {
        return Integer.valueOf(str);
    } catch (NumberFormatException e) {
        return null;
    }
}

private static Long tryParseLong(String str) {
    try {
        return Long.valueOf(str);
    } catch (NumberFormatException e) {
        return null;
    }
}

private static Double tryParseDouble(String str) {
    try {
        return Double.valueOf(str);
    } catch (NumberFormatException e) {
        return null;
    }
}

private static String tryParseString(String str) {
    return "".equals(str) ? null : str;
}

private static <T extends Enum<T>> T tryParseEnum(Class<T> enumType, String str) {
    try {
        return Enum.valueOf(enumType, str.toUpperCase(Locale.US));
    } catch (IllegalArgumentException e) {
        return null;
    }
}

private static <T> List<T> tryParseList(Function<String, T> elementParser, String str) {
    if ("".equals(str)) return new ArrayList<>();

    List<T> ret = new ArrayList<>();

    for (String element : str.split(",")) {
        ret.add(elementParser.apply(element));
    }

    return ret;
}

private static <T extends Enum<T>> List<T> tryParseEnumList(Class<T> enumType, String str) {
    if ("".equals(str)) return new ArrayList<>();

    List<T> ret = new ArrayList<>();

    for (String element : str.split(",")) {
        ret.add(tryParseEnum(enumType, element));
    }

    return ret;
}

private static <T> String formatList(List<T> list) {
    StringJoiner joiner = new StringJoiner(",");

    for (T element : list) {
        joiner.add(element == null ? "" : element.toString());
    }

    return joiner.toString();
}

private static <T extends Enum<T>> String formatEnumList(List<T> list, Function<T, String> elementFormatter) {
    StringJoiner joiner = new StringJoiner(",");

    for (T element : list) {
        joiner.add(element == null ? "" : elementFormatter.apply(element));
    }

    return joiner.toString();
}
)";

const std::regex kRegexDot{"\\."};
const std::regex kRegexUnderscore{"_"};

std::string GetJavaTypeName(const sysprop::Property& prop);
std::string GetJavaEnumTypeName(const sysprop::Property& prop);
std::string GetJavaPackageName(const sysprop::Properties& props);
std::string GetJavaClassName(const sysprop::Properties& props);
std::string GetParsingExpression(const sysprop::Property& prop);
std::string GetFormattingExpression(const sysprop::Property& prop);
void WriteJavaAnnotation(CodeWriter& writer, sysprop::Scope scope);
bool GenerateJavaClass(const sysprop::Properties& props,
                       std::string* java_result, std::string* err);

std::string GetJavaEnumTypeName(const sysprop::Property& prop) {
  return ApiNameToIdentifier(prop.api_name()) + "_values";
}

std::string GetJavaTypeName(const sysprop::Property& prop) {
  switch (prop.type()) {
    case sysprop::Boolean:
      return "Boolean";
    case sysprop::Integer:
      return "Integer";
    case sysprop::Long:
      return "Long";
    case sysprop::Double:
      return "Double";
    case sysprop::String:
      return "String";
    case sysprop::Enum:
      return GetJavaEnumTypeName(prop);
    case sysprop::BooleanList:
      return "List<Boolean>";
    case sysprop::IntegerList:
      return "List<Integer>";
    case sysprop::LongList:
      return "List<Long>";
    case sysprop::DoubleList:
      return "List<Double>";
    case sysprop::StringList:
      return "List<String>";
    case sysprop::EnumList:
      return "List<" + GetJavaEnumTypeName(prop) + ">";
    default:
      __builtin_unreachable();
  }
}

std::string GetParsingExpression(const sysprop::Property& prop) {
  switch (prop.type()) {
    case sysprop::Boolean:
      return "tryParseBoolean(value)";
    case sysprop::Integer:
      return "tryParseInteger(value)";
    case sysprop::Long:
      return "tryParseLong(value)";
    case sysprop::Double:
      return "tryParseDouble(value)";
    case sysprop::String:
      return "tryParseString(value)";
    case sysprop::Enum:
      return "tryParseEnum(" + GetJavaEnumTypeName(prop) + ".class, value)";
    case sysprop::EnumList:
      return "tryParseEnumList(" + GetJavaEnumTypeName(prop) +
             ".class, "
             "value)";
    default:
      break;
  }

  // The remaining cases are lists for types other than Enum which share the
  // same parsing function "tryParseList"
  std::string element_parser;

  switch (prop.type()) {
    case sysprop::BooleanList:
      element_parser = "v -> tryParseBoolean(v)";
      break;
    case sysprop::IntegerList:
      element_parser = "v -> tryParseInteger(v)";
      break;
    case sysprop::LongList:
      element_parser = "v -> tryParseLong(v)";
      break;
    case sysprop::DoubleList:
      element_parser = "v -> tryParseDouble(v)";
      break;
    case sysprop::StringList:
      element_parser = "v -> tryParseString(v)";
      break;
    default:
      __builtin_unreachable();
  }

  return "tryParseList(" + element_parser + ", value)";
}

std::string GetFormattingExpression(const sysprop::Property& prop) {
  if (prop.integer_as_bool()) {
    if (prop.type() == sysprop::Boolean) {
      // Boolean -> Integer String
      return "(value ? \"1\" : \"0\")";
    } else {
      // List<Boolean> -> String directly
      return "value.stream().map("
             "x -> x == null ? \"\" : (x ? \"1\" : \"0\"))"
             ".collect(Collectors.joining(\",\"))";
    }
  } else if (prop.type() == sysprop::Enum) {
    return "value.getPropValue()";
  } else if (prop.type() == sysprop::EnumList) {
    return "formatEnumList(value, " + GetJavaEnumTypeName(prop) +
           "::getPropValue)";
  } else if (IsListProp(prop)) {
    return "formatList(value)";
  } else {
    return "value.toString()";
  }
}

std::string GetJavaPackageName(const sysprop::Properties& props) {
  const std::string& module = props.module();
  return module.substr(0, module.rfind('.'));
}

std::string GetJavaClassName(const sysprop::Properties& props) {
  const std::string& module = props.module();
  return module.substr(module.rfind('.') + 1);
}

void WriteJavaAnnotation(CodeWriter& writer, sysprop::Scope scope) {
  switch (scope) {
    case sysprop::System:
      writer.Write("/** @hide */\n");
      writer.Write("@SystemApi\n");
      break;
    case sysprop::Internal:
      writer.Write("/** @hide */\n");
      break;
    default:
      break;
  }
}

bool GenerateJavaClass(const sysprop::Properties& props,
                       std::string* java_result,
                       [[maybe_unused]] std::string* err) {
  sysprop::Scope classScope = sysprop::Internal;

  for (int i = 0; i < props.prop_size(); ++i) {
    // Get least restrictive scope among props. For example, if all props
    // are internal, class can be as well internal. However, class should
    // be public or system if at least one prop is so.
    classScope = std::min(classScope, props.prop(i).scope());
  }

  std::string package_name = GetJavaPackageName(props);
  std::string class_name = GetJavaClassName(props);

  CodeWriter writer(kIndent);
  writer.Write("%s", kGeneratedFileFooterComments);
  writer.Write("package %s;\n\n", package_name.c_str());
  writer.Write("%s", kJavaFileImports);
  WriteJavaAnnotation(writer, classScope);
  writer.Write("public final class %s {\n", class_name.c_str());
  writer.Indent();
  writer.Write("private %s () {}\n\n", class_name.c_str());
  writer.Write("%s", kJavaParsersAndFormatters);

  for (int i = 0; i < props.prop_size(); ++i) {
    writer.Write("\n");

    const sysprop::Property& prop = props.prop(i);

    std::string prop_id = ApiNameToIdentifier(prop.api_name()).c_str();
    std::string prop_type = GetJavaTypeName(prop);

    if (prop.type() == sysprop::Enum || prop.type() == sysprop::EnumList) {
      if (prop.scope() != classScope) {
        WriteJavaAnnotation(writer, prop.scope());
      }
      writer.Write("public static enum %s {\n",
                   GetJavaEnumTypeName(prop).c_str());
      writer.Indent();
      std::vector<std::string> values =
          android::base::Split(prop.enum_values(), "|");
      for (int i = 0; i < values.size(); ++i) {
        const std::string& name = values[i];
        writer.Write("%s(\"%s\")", ToUpper(name).c_str(), name.c_str());
        if (i + 1 < values.size()) {
          writer.Write(",\n");
        } else {
          writer.Write(";\n");
        }
      }
      writer.Write(
          "private final String propValue;\n"
          "private %s(String propValue) {\n",
          GetJavaEnumTypeName(prop).c_str());
      writer.Indent();
      writer.Write("this.propValue = propValue;\n");
      writer.Dedent();
      writer.Write(
          "}\n"
          "public String getPropValue() {\n");
      writer.Indent();
      writer.Write("return propValue;\n");
      writer.Dedent();
      writer.Write("}\n");
      writer.Dedent();
      writer.Write("}\n\n");
    }

    if (prop.scope() != classScope) {
      WriteJavaAnnotation(writer, prop.scope());
    }

    if (IsListProp(prop)) {
      writer.Write("public static %s %s() {\n", prop_type.c_str(),
                   prop_id.c_str());
      writer.Indent();
      writer.Write("String value = SystemProperties.get(\"%s\");\n",
                   prop.prop_name().c_str());
      writer.Write("return %s;\n", GetParsingExpression(prop).c_str());
      writer.Dedent();
      writer.Write("}\n");
    } else {
      writer.Write("public static Optional<%s> %s() {\n", prop_type.c_str(),
                   prop_id.c_str());
      writer.Indent();
      writer.Write("String value = SystemProperties.get(\"%s\");\n",
                   prop.prop_name().c_str());
      writer.Write("return Optional.ofNullable(%s);\n",
                   GetParsingExpression(prop).c_str());
      writer.Dedent();
      writer.Write("}\n");
    }

    if (prop.access() != sysprop::Readonly) {
      writer.Write("\n");
      if (classScope != sysprop::Internal) {
        WriteJavaAnnotation(writer, sysprop::Internal);
      }
      writer.Write("public static void %s(%s value) {\n", prop_id.c_str(),
                   prop_type.c_str());
      writer.Indent();
      writer.Write("SystemProperties.set(\"%s\", value == null ? \"\" : %s);\n",
                   prop.prop_name().c_str(),
                   GetFormattingExpression(prop).c_str());
      writer.Dedent();
      writer.Write("}\n");
    }
  }

  writer.Dedent();
  writer.Write("}\n");

  *java_result = writer.Code();
  return true;
}

}  // namespace

bool GenerateJavaLibrary(const std::string& input_file_path,
                         const std::string& java_output_dir, std::string* err) {
  sysprop::Properties props;

  if (!ParseProps(input_file_path, &props, err)) {
    return false;
  }

  std::string java_result;

  if (!GenerateJavaClass(props, &java_result, err)) {
    return false;
  }

  std::string package_name = GetJavaPackageName(props);
  std::string java_package_dir =
      java_output_dir + "/" + std::regex_replace(package_name, kRegexDot, "/");

  if (!IsDirectory(java_package_dir) && !CreateDirectories(java_package_dir)) {
    *err = "Creating directory to " + java_package_dir +
           " failed: " + strerror(errno);
    return false;
  }

  std::string class_name = GetJavaClassName(props);
  std::string java_output_file = java_package_dir + "/" + class_name + ".java";
  if (!android::base::WriteStringToFile(java_result, java_output_file)) {
    *err = "Writing generated java class to " + java_output_file +
           " failed: " + strerror(errno);
    return false;
  }

  return true;
}
