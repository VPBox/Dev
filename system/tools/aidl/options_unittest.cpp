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

#include "options.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using std::cerr;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {
namespace {

const char kPreprocessCommandOutputFile[] = "output_file_name";
const char kPreprocessCommandInput1[] = "input1.aidl";
const char kPreprocessCommandInput2[] = "input2.aidl";
const char kPreprocessCommandInput3[] = "input3.aidl";
const char* kPreprocessCommand[] = {
    "aidl", "--preprocess",
    kPreprocessCommandOutputFile,
    kPreprocessCommandInput1,
    kPreprocessCommandInput2,
    kPreprocessCommandInput3,
    nullptr,
};

const char kCompileCommandInput[] = "directory/ITool.aidl";
const char kCompileCommandIncludePath[] = "-Iinclude_path";
const char* kCompileJavaCommand[] = {
    "aidl",
    "-b",
    kCompileCommandIncludePath,
    kCompileCommandInput,
    nullptr,
};
const char kCompileCommandJavaOutput[] = "directory/ITool.java";

const char kCompileDepFileNinja[] = "--ninja";
const char* kCompileJavaCommandNinja[] = {
    "aidl",
    "-b",
    kCompileDepFileNinja,
    kCompileCommandIncludePath,
    kCompileCommandInput,
    nullptr,
};

const char kCompileDepFile[] = "-doutput.deps";
const char kCompileCommandHeaderDir[] = "output/dir/";
const char kCompileCommandCppOutput[] = "some/file.cpp";
const char* kCompileCppCommand[] = {
    "aidl-cpp",
    kCompileCommandIncludePath,
    kCompileDepFile,
    kCompileCommandInput,
    kCompileCommandHeaderDir,
    kCompileCommandCppOutput,
    nullptr,
};
const char* kCompileCppCommandNinja[] = {
    "aidl-cpp",
    kCompileCommandIncludePath,
    kCompileDepFile,
    kCompileDepFileNinja,
    kCompileCommandInput,
    kCompileCommandHeaderDir,
    kCompileCommandCppOutput,
    nullptr,
};

unique_ptr<Options> GetOptions(const char* command[],
                               Options::Language default_lang = Options::Language::JAVA) {
  int argc = 0;
  const char** command_part = command;
  for (; *command_part; ++argc, ++command_part) {}
  unique_ptr<Options> ret(new Options(argc, command, default_lang));
  if (!ret->Ok()) {
    cerr << ret->GetErrorMessage();
    cerr << "Failed to parse command line:";
    for (int i = 0; i < argc; ++i) {
      cerr << " " << command[i];
      cerr << endl;
    }
  }
  EXPECT_NE(ret, nullptr) << "Failed to parse options!";
  return ret;
}

}  // namespace

TEST(OptionsTests, ParsesPreprocess) {
  unique_ptr<Options> options = GetOptions(kPreprocessCommand);
  EXPECT_EQ(Options::Task::PREPROCESS, options->GetTask());
  EXPECT_EQ(false, options->FailOnParcelable());
  EXPECT_EQ(0u, options->ImportDirs().size());
  EXPECT_EQ(0u, options->PreprocessedFiles().size());
  EXPECT_EQ(string{kPreprocessCommandOutputFile}, options->OutputFile());
  EXPECT_EQ(false, options->AutoDepFile());
  const vector<string> expected_input{kPreprocessCommandInput1,
                                      kPreprocessCommandInput2,
                                      kPreprocessCommandInput3};
  EXPECT_EQ(expected_input, options->InputFiles());
}

TEST(OptionsTests, ParsesCompileJava) {
  unique_ptr<Options> options = GetOptions(kCompileJavaCommand);
  EXPECT_EQ(Options::Task::COMPILE, options->GetTask());
  EXPECT_EQ(Options::Language::JAVA, options->TargetLanguage());
  EXPECT_EQ(true, options->FailOnParcelable());
  EXPECT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(0u, options->PreprocessedFiles().size());
  EXPECT_EQ(string{kCompileCommandInput}, options->InputFiles().front());
  EXPECT_EQ(string{kCompileCommandJavaOutput}, options->OutputFile());
  EXPECT_EQ(false, options->AutoDepFile());
  EXPECT_EQ(false, options->DependencyFileNinja());
}

TEST(OptionsTests, ParsesCompileJavaNinja) {
  unique_ptr<Options> options = GetOptions(kCompileJavaCommandNinja);
  EXPECT_EQ(Options::Task::COMPILE, options->GetTask());
  EXPECT_EQ(Options::Language::JAVA, options->TargetLanguage());
  EXPECT_EQ(true, options->FailOnParcelable());
  EXPECT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(0u, options->PreprocessedFiles().size());
  EXPECT_EQ(string{kCompileCommandInput}, options->InputFiles().front());
  EXPECT_EQ(string{kCompileCommandJavaOutput}, options->OutputFile());
  EXPECT_EQ(false, options->AutoDepFile());
  EXPECT_EQ(true, options->DependencyFileNinja());
}

TEST(OptionsTests, ParsesCompileCpp) {
  unique_ptr<Options> options = GetOptions(kCompileCppCommand, Options::Language::CPP);
  ASSERT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(string{kCompileCommandIncludePath}.substr(2), *options->ImportDirs().begin());
  EXPECT_EQ(string{kCompileDepFile}.substr(2), options->DependencyFile());
  EXPECT_EQ(false, options->DependencyFileNinja());
  EXPECT_EQ(kCompileCommandInput, options->InputFiles().front());
  EXPECT_EQ(kCompileCommandHeaderDir, options->OutputHeaderDir());
  EXPECT_EQ(kCompileCommandCppOutput, options->OutputFile());
}

TEST(OptionsTests, ParsesCompileCppNinja) {
  unique_ptr<Options> options = GetOptions(kCompileCppCommandNinja, Options::Language::CPP);
  ASSERT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(string{kCompileCommandIncludePath}.substr(2), *options->ImportDirs().begin());
  EXPECT_EQ(string{kCompileDepFile}.substr(2), options->DependencyFile());
  EXPECT_EQ(true, options->DependencyFileNinja());
  EXPECT_EQ(kCompileCommandInput, options->InputFiles().front());
  EXPECT_EQ(kCompileCommandHeaderDir, options->OutputHeaderDir());
  EXPECT_EQ(kCompileCommandCppOutput, options->OutputFile());
}

TEST(OptionsTests, ParsesCompileJavaMultiInput) {
  const char* argv[] = {
      "aidl",
      "--lang=java",
      kCompileCommandIncludePath,
      "-o src_out",
      "directory/input1.aidl",
      "directory/input2.aidl",
      "directory/input3.aidl",
      nullptr,
  };
  unique_ptr<Options> options = GetOptions(argv);
  EXPECT_EQ(Options::Task::COMPILE, options->GetTask());
  EXPECT_EQ(Options::Language::JAVA, options->TargetLanguage());
  EXPECT_EQ(false, options->FailOnParcelable());
  EXPECT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(0u, options->PreprocessedFiles().size());
  const vector<string> expected_input{"directory/input1.aidl", "directory/input2.aidl",
                                      "directory/input3.aidl"};
  EXPECT_EQ(expected_input, options->InputFiles());
  EXPECT_EQ(string{""}, options->OutputFile());
  EXPECT_EQ(false, options->AutoDepFile());
  EXPECT_EQ(false, options->DependencyFileNinja());
  EXPECT_EQ(string{""}, options->OutputHeaderDir());
  EXPECT_EQ(string{"src_out/"}, options->OutputDir());
}

TEST(OptionsTests, ParsesCompileJavaInvalid) {
  // -o option is required
  const char* arg_with_no_out_dir[] = {
      "aidl",
      "--lang=java",
      kCompileCommandIncludePath,
      "directory/input1.aidl",
      "directory/input2.aidl",
      "directory/input3.aidl",
      nullptr,
  };
  EXPECT_EQ(false, GetOptions(arg_with_no_out_dir)->Ok());

  // -h options is not for Java
  const char* arg_with_header_dir[] = {
      "aidl",          "--lang=java",           kCompileCommandIncludePath, "-o src_out",
      "-h header_out", "directory/input1.aidl", "directory/input2.aidl",    "directory/input3.aidl",
      nullptr,
  };
  EXPECT_EQ(false, GetOptions(arg_with_header_dir)->Ok());
}

TEST(OptionsTests, ParsesCompileCppMultiInput) {
  const char* argv[] = {
      "aidl",
      "--lang=cpp",
      kCompileCommandIncludePath,
      "-h header_out",
      "-o src_out",
      "directory/input1.aidl",
      "directory/input2.aidl",
      "directory/input3.aidl",
      nullptr,
  };
  unique_ptr<Options> options = GetOptions(argv);
  EXPECT_EQ(Options::Task::COMPILE, options->GetTask());
  EXPECT_EQ(Options::Language::CPP, options->TargetLanguage());
  EXPECT_EQ(false, options->FailOnParcelable());
  EXPECT_EQ(1u, options->ImportDirs().size());
  EXPECT_EQ(0u, options->PreprocessedFiles().size());
  const vector<string> expected_input{"directory/input1.aidl", "directory/input2.aidl",
                                      "directory/input3.aidl"};
  EXPECT_EQ(expected_input, options->InputFiles());
  EXPECT_EQ(string{""}, options->OutputFile());
  EXPECT_EQ(false, options->AutoDepFile());
  EXPECT_EQ(false, options->DependencyFileNinja());
  EXPECT_EQ(string{"header_out/"}, options->OutputHeaderDir());
  EXPECT_EQ(string{"src_out/"}, options->OutputDir());
}

TEST(OptionsTests, ParsesCompileCppInvalid) {
  // -o option is required
  const char* arg_with_no_out_dir[] = {
      "aidl",
      "--lang=cpp",
      kCompileCommandIncludePath,
      "directory/input1.aidl",
      "directory/input2.aidl",
      "directory/input3.aidl",
      nullptr,
  };
  EXPECT_EQ(false, GetOptions(arg_with_no_out_dir)->Ok());

  // -h options is required as well
  const char* arg_with_no_header_dir[] = {
      "aidl",
      "--lang=cpp",
      kCompileCommandIncludePath,
      "-o src_out",
      "directory/input1.aidl",
      "directory/input2.aidl",
      "directory/input3.aidl",
      nullptr,
  };
  EXPECT_EQ(false, GetOptions(arg_with_no_header_dir)->Ok());
}

}  // namespace android
}  // namespace aidl
