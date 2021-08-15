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
#include <set>
#include <string>
#include <vector>

#include <android-base/stringprintf.h>
#include <gtest/gtest.h>

#include "aidl.h"
#include "aidl_apicheck.h"
#include "aidl_language.h"
#include "aidl_to_cpp.h"
#include "tests/fake_io_delegate.h"
#include "type_cpp.h"
#include "type_java.h"
#include "type_namespace.h"

using android::aidl::test::FakeIoDelegate;
using android::base::StringPrintf;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;
using android::aidl::internals::parse_preprocessed_file;

namespace android {
namespace aidl {
namespace {

const char kExpectedDepFileContents[] =
R"(place/for/output/p/IFoo.java : \
  p/IFoo.aidl

p/IFoo.aidl :
)";

const char kExpectedNinjaDepFileContents[] =
R"(place/for/output/p/IFoo.java : \
  p/IFoo.aidl
)";

const char kExpectedParcelableDeclarationDepFileContents[] =
    R"( : \
  p/Foo.aidl

p/Foo.aidl :
)";

const char kExpectedStructuredParcelableDepFileContents[] =
    R"(place/for/output/p/Foo.java : \
  p/Foo.aidl

p/Foo.aidl :
)";

const char kExepectedJavaParcelableOutputContests[] =
    R"(/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
@android.annotation.SystemApi
public class Rect implements android.os.Parcelable
{
  // Comment

  @android.annotation.SystemApi
  public int x = 5;

  @android.annotation.UnsupportedAppUsage
  @android.annotation.SystemApi
  public int y;
  public static final android.os.Parcelable.Creator<Rect> CREATOR = new android.os.Parcelable.Creator<Rect>() {
    @Override
    public Rect createFromParcel(android.os.Parcel _aidl_source) {
      Rect _aidl_out = new Rect();
      _aidl_out.readFromParcel(_aidl_source);
      return _aidl_out;
    }
    @Override
    public Rect[] newArray(int _aidl_size) {
      return new Rect[_aidl_size];
    }
  };
  @Override public final void writeToParcel(android.os.Parcel _aidl_parcel, int _aidl_flag)
  {
    int _aidl_start_pos = _aidl_parcel.dataPosition();
    _aidl_parcel.writeInt(0);
    _aidl_parcel.writeInt(x);
    _aidl_parcel.writeInt(y);
    int _aidl_end_pos = _aidl_parcel.dataPosition();
    _aidl_parcel.setDataPosition(_aidl_start_pos);
    _aidl_parcel.writeInt(_aidl_end_pos - _aidl_start_pos);
    _aidl_parcel.setDataPosition(_aidl_end_pos);
  }
  public final void readFromParcel(android.os.Parcel _aidl_parcel)
  {
    int _aidl_start_pos = _aidl_parcel.dataPosition();
    int _aidl_parcelable_size = _aidl_parcel.readInt();
    if (_aidl_parcelable_size < 0) return;
    try {
      x = _aidl_parcel.readInt();
      if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
      y = _aidl_parcel.readInt();
      if (_aidl_parcel.dataPosition() - _aidl_start_pos >= _aidl_parcelable_size) return;
    } finally {
      _aidl_parcel.setDataPosition(_aidl_start_pos + _aidl_parcelable_size);
    }
  }
  @Override public int describeContents()
  {
    return 0;
  }
}
)";

}  // namespace

class AidlTest : public ::testing::Test {
 protected:
  void SetUp() override {
    java_types_.Init();
    cpp_types_.Init();
  }

  AidlDefinedType* Parse(const string& path, const string& contents, TypeNamespace* types,
                         AidlError* error = nullptr,
                         const vector<string> additional_arguments = {}) {
    io_delegate_.SetFileContents(path, contents);
    vector<string> args;
    if (types == &java_types_) {
      args.emplace_back("aidl");
    } else {
      args.emplace_back("aidl-cpp");
    }
    for (const string& s : additional_arguments) {
      args.emplace_back(s);
    }
    for (const string& f : preprocessed_files_) {
      args.emplace_back("--preprocessed=" + f);
    }
    for (const string& i : import_paths_) {
      args.emplace_back("--include=" + i);
    }
    args.emplace_back(path);
    Options options = Options::From(args);
    vector<AidlDefinedType*> defined_types;
    vector<string> imported_files;
    ImportResolver import_resolver{io_delegate_, path, import_paths_, {}};
    AidlError actual_error = ::android::aidl::internals::load_and_validate_aidl(
        path, options, io_delegate_, types, &defined_types, &imported_files);

    if (error != nullptr) {
      *error = actual_error;
    }

    if (actual_error != AidlError::OK) {
      return nullptr;
    }

    EXPECT_EQ(1ul, defined_types.size());

    return defined_types.front();
  }

  FakeIoDelegate io_delegate_;
  vector<string> preprocessed_files_;
  set<string> import_paths_;
  java::JavaTypeNamespace java_types_;
  cpp::TypeNamespace cpp_types_;
};

TEST_F(AidlTest, AcceptMissingPackage) {
  EXPECT_NE(nullptr, Parse("IFoo.aidl", "interface IFoo { }", &java_types_));
  EXPECT_NE(nullptr, Parse("IFoo.aidl", "interface IFoo { }", &cpp_types_));
}

TEST_F(AidlTest, RejectsArraysOfBinders) {
  import_paths_.emplace("");
  io_delegate_.SetFileContents("bar/IBar.aidl",
                               "package bar; interface IBar {}");
  string path = "foo/IFoo.aidl";
  string contents = "package foo;\n"
                    "import bar.IBar;\n"
                    "interface IFoo { void f(in IBar[] input); }";
  EXPECT_EQ(nullptr, Parse(path, contents, &java_types_));
  EXPECT_EQ(nullptr, Parse(path, contents, &cpp_types_));
}

TEST_F(AidlTest, RejectsOnewayOutParameters) {
  string oneway_interface =
      "package a; oneway interface IFoo { void f(out int bar); }";
  string oneway_method =
      "package a; interface IBar { oneway void f(out int bar); }";
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_interface, &cpp_types_));
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_interface, &java_types_));
  EXPECT_EQ(nullptr, Parse("a/IBar.aidl", oneway_method, &cpp_types_));
  EXPECT_EQ(nullptr, Parse("a/IBar.aidl", oneway_method, &java_types_));
}

TEST_F(AidlTest, RejectsOnewayNonVoidReturn) {
  string oneway_method = "package a; interface IFoo { oneway int f(); }";
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, &cpp_types_));
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, &java_types_));
}

TEST_F(AidlTest, RejectsNullablePrimitive) {
  string oneway_method = "package a; interface IFoo { @nullable int f(); }";
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, &cpp_types_));
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", oneway_method, &java_types_));
}

TEST_F(AidlTest, RejectsDuplicatedArgumentNames) {
  string method = "package a; interface IFoo { void f(int a, int a); }";
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, &cpp_types_));
  EXPECT_EQ(nullptr, Parse("a/IFoo.aidl", method, &java_types_));
}

TEST_F(AidlTest, ParsesNullableAnnotation) {
  for (auto is_nullable: {true, false}) {
    auto parse_result = Parse(
        "a/IFoo.aidl",
        StringPrintf( "package a; interface IFoo {%s String f(); }",
                     (is_nullable) ? "@nullable" : ""),
        &cpp_types_);
    ASSERT_NE(nullptr, parse_result);
    const AidlInterface* interface = parse_result->AsInterface();
    ASSERT_NE(nullptr, interface);
    ASSERT_FALSE(interface->GetMethods().empty());
    EXPECT_EQ(interface->GetMethods()[0]->GetType().IsNullable(), is_nullable);
    cpp_types_.typenames_.Reset();
  }
}

TEST_F(AidlTest, ParsesUtf8Annotations) {
  for (auto is_utf8: {true, false}) {
    auto parse_result = Parse(
        "a/IFoo.aidl",
        StringPrintf( "package a; interface IFoo {%s String f(); }",
                     (is_utf8) ? "@utf8InCpp" : ""),
        &cpp_types_);
    ASSERT_NE(nullptr, parse_result);
    const AidlInterface* interface = parse_result->AsInterface();
    ASSERT_NE(nullptr, interface);
    ASSERT_FALSE(interface->GetMethods().empty());
    EXPECT_EQ(interface->GetMethods()[0]->GetType().IsUtf8InCpp(), is_utf8);
    cpp_types_.typenames_.Reset();
  }
}

TEST_F(AidlTest, AcceptsOneway) {
  string oneway_method = "package a; interface IFoo { oneway void f(int a); }";
  string oneway_interface =
      "package a; oneway interface IBar { void f(int a); }";
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", oneway_method, &cpp_types_));
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", oneway_method, &java_types_));
  EXPECT_NE(nullptr, Parse("a/IBar.aidl", oneway_interface, &cpp_types_));
  EXPECT_NE(nullptr, Parse("a/IBar.aidl", oneway_interface, &java_types_));
}

TEST_F(AidlTest, ParsesPreprocessedFile) {
  string simple_content = "parcelable a.Foo;\ninterface b.IBar;";
  io_delegate_.SetFileContents("path", simple_content);
  EXPECT_FALSE(java_types_.HasTypeByCanonicalName("a.Foo"));
  EXPECT_TRUE(parse_preprocessed_file(io_delegate_, "path", &java_types_, java_types_.typenames_));
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("a.Foo"));
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("b.IBar"));
}

TEST_F(AidlTest, ParsesPreprocessedFileWithWhitespace) {
  string simple_content = "parcelable    a.Foo;\n  interface b.IBar  ;\t";
  io_delegate_.SetFileContents("path", simple_content);
  EXPECT_FALSE(java_types_.HasTypeByCanonicalName("a.Foo"));
  EXPECT_TRUE(parse_preprocessed_file(io_delegate_, "path", &java_types_, java_types_.typenames_));
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("a.Foo"));
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("b.IBar"));
}

TEST_F(AidlTest, PreferImportToPreprocessed) {
  io_delegate_.SetFileContents("preprocessed", "interface another.IBar;");
  io_delegate_.SetFileContents("one/IBar.aidl", "package one; "
                                                "interface IBar {}");
  preprocessed_files_.push_back("preprocessed");
  import_paths_.emplace("");
  auto parse_result = Parse(
      "p/IFoo.aidl", "package p; import one.IBar; interface IFoo {}",
      &java_types_);
  EXPECT_NE(nullptr, parse_result);
  // We expect to know about both kinds of IBar
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("one.IBar"));
  EXPECT_TRUE(java_types_.HasTypeByCanonicalName("another.IBar"));
  // But if we request just "IBar" we should get our imported one.
  AidlTypeSpecifier ambiguous_type(AIDL_LOCATION_HERE, "IBar", false, nullptr, "");
  const java::Type* type = java_types_.Find(ambiguous_type);
  ASSERT_TRUE(type);
  EXPECT_EQ("one.IBar", type->CanonicalName());
}

TEST_F(AidlTest, WritePreprocessedFile) {
  io_delegate_.SetFileContents("p/Outer.aidl",
                               "package p; parcelable Outer.Inner;");
  io_delegate_.SetFileContents("one/IBar.aidl", "package one; import p.Outer;"
                                                "interface IBar {}");

  vector<string> args {
    "aidl",
    "--preprocess",
    "preprocessed",
    "p/Outer.aidl",
    "one/IBar.aidl"};
  Options options = Options::From(args);
  EXPECT_TRUE(::android::aidl::preprocess_aidl(options, io_delegate_));

  string output;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("preprocessed", &output));
  EXPECT_EQ("parcelable p.Outer.Inner;\ninterface one.IBar;\n", output);
}

TEST_F(AidlTest, JavaParcelableOutput) {
  io_delegate_.SetFileContents("Rect.aidl",
                               "@SystemApi\n"
                               "parcelable Rect {\n"
                               "  // Comment\n"
                               "  @SystemApi\n"
                               "  int x=5;\n"
                               "  @SystemApi\n"
                               "  @UnsupportedAppUsage\n"
                               "  int y;\n"
                               "}");

  vector<string> args{"aidl", "Rect.aidl"};
  Options options = Options::From(args);
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string output;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("Rect.java", &output));
  EXPECT_EQ(kExepectedJavaParcelableOutputContests, output);
}

TEST_F(AidlTest, RequireOuterClass) {
  io_delegate_.SetFileContents("p/Outer.aidl",
                               "package p; parcelable Outer.Inner;");
  import_paths_.emplace("");
  auto parse_result = Parse(
      "p/IFoo.aidl",
      "package p; import p.Outer; interface IFoo { void f(in Inner c); }",
      &java_types_);
  EXPECT_EQ(nullptr, parse_result);
}

TEST_F(AidlTest, ParseCompoundParcelableFromPreprocess) {
  io_delegate_.SetFileContents("preprocessed",
                               "parcelable p.Outer.Inner;");
  preprocessed_files_.push_back("preprocessed");
  auto parse_result = Parse(
      "p/IFoo.aidl",
      "package p; interface IFoo { void f(in Inner c); }",
      &java_types_);
  // TODO(wiley): This should actually return nullptr because we require
  //              the outer class name.  However, for legacy reasons,
  //              this behavior must be maintained.  b/17415692
  EXPECT_NE(nullptr, parse_result);
}

TEST_F(AidlTest, FailOnParcelable) {
  io_delegate_.SetFileContents("p/IFoo.aidl", "package p; parcelable IFoo;");

  // By default, we shouldn't fail on parcelable.
  Options options1 = Options::From("aidl p/IFoo.aidl");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options1, io_delegate_));

  // -b considers this an error
  Options options2 = Options::From("aidl -b p/IFoo.aidl");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options2, io_delegate_));

  io_delegate_.SetFileContents("p/IBar.aidl", "package p; parcelable Foo; interface IBar{}");

  // Regardless of '-b', a parcelable and an interface should fail.
  Options options3 = Options::From("aidl p/IBar.aidl");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options3, io_delegate_));
  Options options4 = Options::From("aidl -b p/IBar.aidl");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options4, io_delegate_));
}

TEST_F(AidlTest, StructuredFailOnUnstructuredParcelable) {
  io_delegate_.SetFileContents("o/WhoKnowsWhat.aidl", "package o; parcelable WhoKnowsWhat;");
  import_paths_.emplace("");
  AidlError reported_error;
  auto parse_result =
      Parse("p/IFoo.aidl",
            "package p; import o.WhoKnowsWhat; interface IFoo { void f(in WhoKnowsWhat thisIs); }",
            &java_types_, &reported_error, {"--structured"});
  EXPECT_EQ(nullptr, parse_result);
  EXPECT_EQ(AidlError::NOT_STRUCTURED, reported_error);
}

TEST_F(AidlTest, FailOnDuplicateConstantNames) {
  AidlError reported_error;
  EXPECT_EQ(nullptr,
            Parse("p/IFoo.aidl",
                   R"(package p;
                      interface IFoo {
                        const String DUPLICATED = "d";
                        const int DUPLICATED = 1;
                      }
                   )",
                   &cpp_types_,
                   &reported_error));
  EXPECT_EQ(AidlError::BAD_TYPE, reported_error);
}

TEST_F(AidlTest, FailOnManyDefinedTypes) {
  AidlError reported_error;
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl",
                           R"(package p;
                      interface IFoo {}
                      parcelable Bar;
                      parcelable IBar {}
                      parcelable StructuredParcelable {}
                      interface IBaz {}
                   )",
                           &cpp_types_, &reported_error));
  // Parse success is important for clear error handling even if the cases aren't
  // actually supported in code generation.
  EXPECT_EQ(AidlError::BAD_TYPE, reported_error);
}

TEST_F(AidlTest, FailOnNoDefinedTypes) {
  AidlError reported_error;
  EXPECT_EQ(nullptr, Parse("p/IFoo.aidl", R"(package p;)", &cpp_types_, &reported_error));
  EXPECT_EQ(AidlError::PARSE_ERROR, reported_error);
}

TEST_F(AidlTest, FailOnMalformedConstHexValue) {
  AidlError reported_error;
  EXPECT_EQ(nullptr,
            Parse("p/IFoo.aidl",
                   R"(package p;
                      interface IFoo {
                        const int BAD_HEX_VALUE = 0xffffffffffffffffff;
                      }
                   )",
                   &cpp_types_,
                   &reported_error));
  EXPECT_EQ(AidlError::BAD_TYPE, reported_error);
}

TEST_F(AidlTest, ParsePositiveConstHexValue) {
  AidlError reported_error;
  auto cpp_parse_result =
    Parse("p/IFoo.aidl",
           R"(package p;
              interface IFoo {
                const int POSITIVE_HEX_VALUE = 0xf5;
              }
           )",
           &cpp_types_,
           &reported_error);
  EXPECT_NE(nullptr, cpp_parse_result);
  const AidlInterface* interface = cpp_parse_result->AsInterface();
  ASSERT_NE(nullptr, interface);
  const auto& cpp_constants = interface->GetConstantDeclarations();
  EXPECT_EQ((size_t)1, cpp_constants.size());
  EXPECT_EQ("POSITIVE_HEX_VALUE", cpp_constants[0]->GetName());
  EXPECT_EQ("245", cpp_constants[0]->ValueString(cpp::ConstantValueDecorator));
}

TEST_F(AidlTest, ParseNegativeConstHexValue) {
  AidlError reported_error;
  auto cpp_parse_result =
    Parse("p/IFoo.aidl",
           R"(package p;
              interface IFoo {
                const int NEGATIVE_HEX_VALUE = 0xffffffff;
              }
           )",
           &cpp_types_,
           &reported_error);
  EXPECT_NE(nullptr, cpp_parse_result);
  const AidlInterface* interface = cpp_parse_result->AsInterface();
  ASSERT_NE(nullptr, interface);
  const auto& cpp_constants = interface->GetConstantDeclarations();
  EXPECT_EQ((size_t)1, cpp_constants.size());
  EXPECT_EQ("NEGATIVE_HEX_VALUE", cpp_constants[0]->GetName());
  EXPECT_EQ("-1", cpp_constants[0]->ValueString(cpp::ConstantValueDecorator));
}

TEST_F(AidlTest, UnderstandsNestedParcelables) {
  io_delegate_.SetFileContents(
      "p/Outer.aidl",
      "package p; parcelable Outer.Inner cpp_header \"baz/header\";");
  import_paths_.emplace("");
  const string input_path = "p/IFoo.aidl";
  const string input = "package p; import p.Outer; interface IFoo"
                       " { Outer.Inner get(); }";

  auto cpp_parse_result = Parse(input_path, input, &cpp_types_);
  EXPECT_NE(nullptr, cpp_parse_result);
  auto cpp_type = cpp_types_.FindTypeByCanonicalName("p.Outer.Inner");
  ASSERT_NE(nullptr, cpp_type);
  // C++ uses "::" instead of "." to refer to a inner class.
  EXPECT_EQ("::p::Outer::Inner", cpp_type->CppType());
}

TEST_F(AidlTest, UnderstandsNativeParcelables) {
  io_delegate_.SetFileContents(
      "p/Bar.aidl",
      "package p; parcelable Bar cpp_header \"baz/header\";");
  import_paths_.emplace("");
  const string input_path = "p/IFoo.aidl";
  const string input = "package p; import p.Bar; interface IFoo { }";

  // C++ understands C++ specific stuff
  auto cpp_parse_result = Parse(input_path, input, &cpp_types_);
  EXPECT_NE(nullptr, cpp_parse_result);
  auto cpp_type = cpp_types_.FindTypeByCanonicalName("p.Bar");
  ASSERT_NE(nullptr, cpp_type);
  EXPECT_EQ("::p::Bar", cpp_type->CppType());
  set<string> headers;
  cpp_type->GetHeaders(&headers);
  EXPECT_EQ(1u, headers.size());
  EXPECT_EQ(1u, headers.count("baz/header"));

  // Java ignores C++ specific stuff
  auto java_parse_result = Parse(input_path, input, &java_types_);
  EXPECT_NE(nullptr, java_parse_result);
  auto java_type = java_types_.FindTypeByCanonicalName("p.Bar");
  ASSERT_NE(nullptr, java_type);
  EXPECT_EQ("p.Bar", java_type->InstantiableName());
}

TEST_F(AidlTest, WritesCorrectDependencyFile) {
  // While the in tree build system always gives us an output file name,
  // other android tools take advantage of our ability to infer the intended
  // file name.  This test makes sure we handle this correctly.
  vector<string> args = {
    "aidl",
    "-d dep/file/path",
    "-o place/for/output",
    "p/IFoo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; interface IFoo {}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedDepFileContents);
}

TEST_F(AidlTest, WritesCorrectDependencyFileNinja) {
  // While the in tree build system always gives us an output file name,
  // other android tools take advantage of our ability to infer the intended
  // file name.  This test makes sure we handle this correctly.
  vector<string> args = {
    "aidl",
    "-d dep/file/path",
    "--ninja",
    "-o place/for/output",
    "p/IFoo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; interface IFoo {}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedNinjaDepFileContents);
}

TEST_F(AidlTest, WritesTrivialDependencyFileForParcelableDeclaration) {
  // The SDK uses aidl to decide whether a .aidl file is a parcelable.  It does
  // this by calling aidl with every .aidl file it finds, then parsing the
  // generated dependency files.  Those that reference .java output files are
  // for interfaces and those that do not are parcelables.  However, for both
  // parcelables and interfaces, we *must* generate a non-empty dependency file.
  vector<string> args = {
    "aidl",
    "-o place/for/output",
    "-d dep/file/path",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo;");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedParcelableDeclarationDepFileContents);
}

TEST_F(AidlTest, WritesDependencyFileForStructuredParcelable) {
  vector<string> args = {
    "aidl",
    "--structured",
    "-o place/for/output",
    "-d dep/file/path",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo {int a;}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string actual_dep_file_contents;
  EXPECT_TRUE(io_delegate_.GetWrittenContents(options.DependencyFile(), &actual_dep_file_contents));
  EXPECT_EQ(actual_dep_file_contents, kExpectedStructuredParcelableDepFileContents);
}

TEST_F(AidlTest, NoJavaOutputForParcelableDeclaration) {
 vector<string> args = {
    "aidl",
    "--lang=java",
    "-o place/for/output",
    "p/Foo.aidl"};
  Options options = Options::From(args);
  io_delegate_.SetFileContents(options.InputFiles().front(), "package p; parcelable Foo;");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
  string output_file_contents;
  EXPECT_FALSE(io_delegate_.GetWrittenContents(options.OutputFile(), &output_file_contents));
}

/* not working until type_namespace.h is fixed
TEST_F(AidlTest, AcceptsNestedContainerType) {
  string nested_in_iface = "package a; interface IFoo {\n"
                           "  List<int, List<String, bool>> foo(); }";
  string nested_in_parcelable = "package a; parcelable IData {\n"
                                "  List<int, List<String, bool>> foo;}";
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", nested_in_iface, &java_types_));
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", nested_in_iface, &cpp_types_));
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", nested_in_parcelable, &java_types_));
  EXPECT_NE(nullptr, Parse("a/IFoo.aidl", nested_in_parcelable, &cpp_types_));
}
*/

TEST_F(AidlTest, ApiDump) {
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "// comment\n"
      "interface IFoo {\n"
      "    int foo(out int[] a, String b, boolean c, inout List<String>  d);\n"
      "    int foo2(@utf8InCpp String x, inout List<String>  y);\n"
      "    IFoo foo3(IFoo foo);\n"
      "    Data getData();\n"
      "    const int A = 1;\n"
      "    const String STR = \"Hello\";\n"
      "}\n");
  io_delegate_.SetFileContents("foo/bar/Data.aidl",
                               "package foo.bar;\n"
                               "import foo.bar.IFoo;\n"
                               "parcelable Data {\n"
                               "   int x = 10;\n"
                               "   int y;\n"
                               "   IFoo foo;\n"
                               "   List<IFoo> a;\n"
                               "   List<foo.bar.IFoo> b;\n"
                               "   @nullable String[] c;\n"
                               "}\n");
  io_delegate_.SetFileContents("api.aidl", "");
  vector<string> args = {"aidl", "--dumpapi", "--out=dump", "foo/bar/IFoo.aidl",
                         "foo/bar/Data.aidl"};
  Options options = Options::From(args);
  bool result = dump_api(options, io_delegate_);
  ASSERT_TRUE(result);
  string actual;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/IFoo.aidl", &actual));
  EXPECT_EQ(actual, R"(package foo.bar;
interface IFoo {
  int foo(out int[] a, String b, boolean c, inout List<String> d);
  int foo2(@utf8InCpp String x, inout List<String> y);
  foo.bar.IFoo foo3(foo.bar.IFoo foo);
  foo.bar.Data getData();
  const int A = 1;
  const String STR = "Hello";
}
)");

  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/Data.aidl", &actual));
  EXPECT_EQ(actual, R"(package foo.bar;
parcelable Data {
  int x = 10;
  int y;
  foo.bar.IFoo foo;
  List<foo.bar.IFoo> a;
  List<foo.bar.IFoo> b;
  @nullable String[] c;
}
)");
}

TEST_F(AidlTest, ApiDumpWithManualIds) {
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "interface IFoo {\n"
      "    int foo() = 1;\n"
      "    int bar() = 2;\n"
      "    int baz() = 10;\n"
      "}\n");

  vector<string> args = {"aidl", "--dumpapi", "-o dump", "foo/bar/IFoo.aidl"};
  Options options = Options::From(args);
  bool result = dump_api(options, io_delegate_);
  ASSERT_TRUE(result);
  string actual;
  EXPECT_TRUE(io_delegate_.GetWrittenContents("dump/foo/bar/IFoo.aidl", &actual));
  EXPECT_EQ(actual, R"(package foo.bar;
interface IFoo {
  int foo() = 1;
  int bar() = 2;
  int baz() = 10;
}
)");
}

TEST_F(AidlTest, ApiDumpWithManualIdsOnlyOnSomeMethods) {
  io_delegate_.SetFileContents(
      "foo/bar/IFoo.aidl",
      "package foo.bar;\n"
      "interface IFoo {\n"
      "    int foo() = 1;\n"
      "    int bar();\n"
      "    int baz() = 10;\n"
      "}\n");

  vector<string> args = {"aidl", "--dumpapi", "-o dump", "foo/bar/IFoo.aidl"};
  Options options = Options::From(args);
  EXPECT_FALSE(dump_api(options, io_delegate_));
}

TEST_F(AidlTest, CheckNumGenericTypeSecifier) {
  Options options = Options::From("aidl p/IFoo.aidl IFoo.java");
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "void foo(List<String, String> a);}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));

  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "void foo(Map<String> a);}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));

  Options options2 = Options::From("aidl p/Data.aidl Data.java");
  io_delegate_.SetFileContents(options2.InputFiles().front(),
                               "package p; parcelable Data {"
                               "List<String, String> foo;}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options2, io_delegate_));

  io_delegate_.SetFileContents(options2.InputFiles().front(),
                               "package p; parcelable Data {"
                               "Map<String> foo;}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options2, io_delegate_));
}

TEST_F(AidlTest, MultipleTypesInSingleFile) {
  Options options = Options::From("aidl --lang=java -o out foo/bar/Foo.aidl");
  io_delegate_.SetFileContents(options.InputFiles().front(),
      "package foo.bar;\n"
      "interface IFoo1 { int foo(); }\n"
      "interface IFoo2 { int foo(); }\n"
      "parcelable Data { int a; int b;}\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file :
    {"out/foo/bar/IFoo1.java", "out/foo/bar/IFoo2.java", "out/foo/bar/Data.java"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, MultipleTypesInSingleFileCpp) {
  Options options = Options::From("aidl --lang=cpp -o out -h out/include foo/bar/Foo.aidl");
  io_delegate_.SetFileContents(options.InputFiles().front(),
      "package foo.bar;\n"
      "interface IFoo1 { int foo(); }\n"
      "interface IFoo2 { int foo(); }\n"
      "parcelable Data { int a; int b;}\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file : {
    "out/foo/bar/IFoo1.cpp", "out/foo/bar/IFoo2.cpp", "out/foo/bar/Data.cpp",
    "out/include/foo/bar/IFoo1.h", "out/include/foo/bar/IFoo2.h", "out/include/foo/bar/Data.h",
    "out/include/foo/bar/BpFoo1.h", "out/include/foo/bar/BpFoo2.h", "out/include/foo/bar/BpData.h",
    "out/include/foo/bar/BnFoo1.h", "out/include/foo/bar/BnFoo2.h", "out/include/foo/bar/BnData.h"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, MultipleInputFiles) {
  Options options = Options::From(
      "aidl --lang=java -o out foo/bar/IFoo.aidl foo/bar/Data.aidl");

  io_delegate_.SetFileContents(options.InputFiles().at(0),
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "interface IFoo { Data getData(); }\n");

  io_delegate_.SetFileContents(options.InputFiles().at(1),
        "package foo.bar;\n"
        "import foo.bar.IFoo;\n"
        "parcelable Data { IFoo foo; }\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file : {
    "out/foo/bar/IFoo.java", "out/foo/bar/Data.java"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, MultipleInputFilesCpp) {
  Options options = Options::From("aidl --lang=cpp -o out -h out/include "
      "foo/bar/IFoo.aidl foo/bar/Data.aidl");

  io_delegate_.SetFileContents(options.InputFiles().at(0),
      "package foo.bar;\n"
      "import foo.bar.Data;\n"
      "interface IFoo { Data getData(); }\n");

  io_delegate_.SetFileContents(options.InputFiles().at(1),
        "package foo.bar;\n"
        "import foo.bar.IFoo;\n"
        "parcelable Data { IFoo foo; }\n");

  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));

  string content;
  for (const auto file : {
    "out/foo/bar/IFoo.cpp", "out/foo/bar/Data.cpp",
    "out/include/foo/bar/IFoo.h", "out/include/foo/bar/Data.h",
    "out/include/foo/bar/BpFoo.h", "out/include/foo/bar/BpData.h",
    "out/include/foo/bar/BnFoo.h", "out/include/foo/bar/BnData.h"}) {
    content.clear();
    EXPECT_TRUE(io_delegate_.GetWrittenContents(file, &content));
    EXPECT_FALSE(content.empty());
  }
}

TEST_F(AidlTest, ConflictWithMetaTransactions) {
  Options options = Options::From("aidl --lang=java -o place/for/output p/IFoo.aidl");
  // int getInterfaceVersion() is one of the meta transactions
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "int getInterfaceVersion(); }");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));

  // boolean getInterfaceVersion() is not, but should be prevented
  // because return type is not part of a method signature
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "boolean getInterfaceVersion(); }");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));

  // this is another reserved name
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "String getTransactionName(int code); }");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));

  // this is not a meta interface method as it differs type arguments
  io_delegate_.SetFileContents(options.InputFiles().front(),
                               "package p; interface IFoo {"
                               "String getTransactionName(); }");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, DiffrentOrderAnnotationsInCheckAPI) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p; interface IFoo{ @utf8InCpp @nullable String foo();}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p; interface IFoo{ @nullable @utf8InCpp String foo();}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
}

TEST_F(AidlTest, SuccessOnIdenticalApiDumps) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{ void foo();}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo();}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
}

TEST_F(AidlTest, SuccessOnCompatibleChanges) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");

  // new type
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/p/IBar.aidl",
                               "package p;"
                               "interface IBar {"
                               "  void bar();"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");
  io_delegate_.SetFileContents("new/p/IBar.aidl", "");

  // new method
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "  void bar();"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // new field
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // new package
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  io_delegate_.SetFileContents("new/q/IFoo.aidl",
                               "package q;"
                               "interface IFoo {"
                               "  void foo(int a);"
                               "}");
  io_delegate_.SetFileContents("new/q/Data.aidl",
                               "package q;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");
  io_delegate_.SetFileContents("new/q/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/q/Data.aidl", "");

  // arg name change
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(int b);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  io_delegate_.SetFileContents("old/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("old/p/Data.aidl", "");

  // added const value
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; }");
  io_delegate_.SetFileContents("new/p/I.aidl",
                               "package p ; interface I {"
                               "const int A = 1; const int B = 2;}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("old/p/I.aidl", "");
  io_delegate_.SetFileContents("new/p/I.aidl", "");

  // changed const value order
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; const int B = 2;}");
  io_delegate_.SetFileContents("new/p/I.aidl",
                               "package p ; interface I {"
                               "const int B = 2; const int A = 1;}");
  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));
}

TEST_F(AidlTest, FailOnIncompatibleChanges) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("old/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");

  // removed type
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");

  // removed method
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // removed field
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // renamed method
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar2(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // renamed field
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar2;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // renamed type
  io_delegate_.SetFileContents("new/p/IFoo2.aidl",
                               "package p;"
                               "interface IFoo2 {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo2.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // reorderd method
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void bar(@utf8InCpp String str);"
                               "  void foo(in String[] str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // reorderd field
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int bar;"
                               "  int foo;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // changed direction specifier
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(out String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // added annotation
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in @utf8InCpp String[] str);"
                               "  void bar(@utf8InCpp String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // removed annotation
  io_delegate_.SetFileContents("new/p/IFoo.aidl",
                               "package p;"
                               "interface IFoo {"
                               "  void foo(in String[] str);"
                               "  void bar(String str);"
                               "}");
  io_delegate_.SetFileContents("new/p/Data.aidl",
                               "package p;"
                               "parcelable Data {"
                               "  int foo;"
                               "  int bar;"
                               "}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/Data.aidl", "");

  // removed package
  io_delegate_.SetFileContents("old/p/Data.aidl", "");
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{}");
  io_delegate_.SetFileContents("old/q/IFoo.aidl", "package q; interface IFoo{}");

  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "");
  io_delegate_.SetFileContents("old/q/IFoo.aidl", "");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "");

  // changed default value
  io_delegate_.SetFileContents("old/p/D.aidl", "package p; parcelable D { int a = 1; }");
  io_delegate_.SetFileContents("new/p/D.aidl", "package p; parcelable D { int a = 2; }");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("old/p/D.aidl", "");
  io_delegate_.SetFileContents("new/p/D.aidl", "");

  // removed const value
  io_delegate_.SetFileContents("old/p/I.aidl",
                               "package p; interface I {"
                               "const int A = 1; const int B = 2;}");
  io_delegate_.SetFileContents("new/p/I.aidl", "package p; interface I { const int A = 1; }");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("old/p/I.aidl", "");
  io_delegate_.SetFileContents("new/p/I.aidl", "");

  // changed const value
  io_delegate_.SetFileContents("old/p/I.aidl", "package p; interface I { const int A = 1; }");
  io_delegate_.SetFileContents("new/p/I.aidl", "package p; interface I { const int A = 2; }");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
  io_delegate_.SetFileContents("old/p/I.aidl", "");
  io_delegate_.SetFileContents("new/p/I.aidl", "");
}

TEST_F(AidlTest, RejectAmbiguousImports) {
  Options options = Options::From("aidl --lang=java -o out -I dir1 -I dir2 p/IFoo.aidl");
  io_delegate_.SetFileContents("p/IFoo.aidl", "package p; import q.IBar; interface IFoo{}");
  io_delegate_.SetFileContents("dir1/q/IBar.aidl", "package q; interface IBar{}");
  io_delegate_.SetFileContents("dir2/q/IBar.aidl", "package q; interface IBar{}");

  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, HandleManualIdAssignments) {
  Options options = Options::From("aidl --checkapi old new");
  io_delegate_.SetFileContents("old/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 10;}");
  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 10;}");

  EXPECT_TRUE(::android::aidl::check_api(options, io_delegate_));

  io_delegate_.SetFileContents("new/p/IFoo.aidl", "package p; interface IFoo{ void foo() = 11;}");
  EXPECT_FALSE(::android::aidl::check_api(options, io_delegate_));
}

TEST_F(AidlTest, ParcelFileDescriptorIsBuiltinType) {
  Options javaOptions = Options::From("aidl --lang=java -o out p/IFoo.aidl");
  Options cppOptions = Options::From("aidl --lang=cpp -h out -o out p/IFoo.aidl");

  // use without import
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p; interface IFoo{ void foo(in ParcelFileDescriptor fd);}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));

  // use without impot but with full name
  io_delegate_.SetFileContents(
      "p/IFoo.aidl",
      "package p; interface IFoo{ void foo(in android.os.ParcelFileDescriptor fd);}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));

  // use with import (as before)
  io_delegate_.SetFileContents("p/IFoo.aidl",
                               "package p;"
                               "import android.os.ParcelFileDescriptor;"
                               "interface IFoo{"
                               "  void foo(in ParcelFileDescriptor fd);"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(javaOptions, io_delegate_));
  EXPECT_EQ(0, ::android::aidl::compile_aidl(cppOptions, io_delegate_));
}

TEST_F(AidlTest, ManualIds) {
  Options options = Options::From("aidl --lang=java -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 0;\n"
                               "  void bar() = 1;\n"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, ManualIdsWithMetaTransactions) {
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 0;\n"
                               "  void bar() = 1;\n"
                               "}");
  EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnDuplicatedIds) {
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar() = 3;\n"
                               "}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnOutOfRangeIds) {
  // 16777115 is kLastMetaMethodId + 1
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar() = 16777115;\n"
                               "}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

TEST_F(AidlTest, FailOnPartiallyAssignedIds) {
  Options options = Options::From("aidl --lang=java --version 10 -o out IFoo.aidl");
  io_delegate_.SetFileContents("IFoo.aidl",
                               "interface IFoo {\n"
                               "  void foo() = 3;\n"
                               "  void bar();\n"
                               "}");
  EXPECT_NE(0, ::android::aidl::compile_aidl(options, io_delegate_));
}

class AidlOutputPathTest : public AidlTest {
 protected:
  void SetUp() override {
    AidlTest::SetUp();
    io_delegate_.SetFileContents("sub/dir/foo/bar/IFoo.aidl", "package foo.bar; interface IFoo {}");
  }

  void Test(const Options& options, const std::string expected_output_path) {
    EXPECT_EQ(0, ::android::aidl::compile_aidl(options, io_delegate_));
    // check the existence
    EXPECT_TRUE(io_delegate_.GetWrittenContents(expected_output_path, nullptr));
  }
};

TEST_F(AidlOutputPathTest, OutDirWithNoOutputFile) {
  // <out_dir> / <package_name> / <type_name>.java
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl"), "out/foo/bar/IFoo.java");
}

TEST_F(AidlOutputPathTest, OutDirWithOutputFile) {
  // when output file is explicitly set, it is always respected. -o option is
  // ignored.
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl output/IFoo.java"), "output/IFoo.java");
}

TEST_F(AidlOutputPathTest, NoOutDirWithOutputFile) {
  Test(Options::From("aidl -o out sub/dir/foo/bar/IFoo.aidl output/IFoo.java"), "output/IFoo.java");
}

TEST_F(AidlOutputPathTest, NoOutDirWithNoOutputFile) {
  // output is the same as the input file except for the suffix
  Test(Options::From("aidl sub/dir/foo/bar/IFoo.aidl"), "sub/dir/foo/bar/IFoo.java");
}

}  // namespace aidl
}  // namespace android
