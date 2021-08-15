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
#include <vector>

#include <android-base/logging.h>
#include <gtest/gtest.h>

#include "aidl.h"
#include "options.h"
#include "tests/fake_io_delegate.h"
#include "tests/test_data.h"
#include "tests/test_util.h"

using android::aidl::test::CanonicalNameToPath;
using android::aidl::test::FakeIoDelegate;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {

class EndToEndTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void AddStubAidls(const char** parcelables, const char** interfaces,
                    const char* cpp_header=nullptr) {
    for ( ; *parcelables; ++parcelables) {
      io_delegate_.AddStubParcelable(
          *parcelables, (cpp_header) ? cpp_header : "");
    }
    for ( ; *interfaces; ++interfaces) {
      io_delegate_.AddStubInterface(*interfaces);
    }
  }

  void CheckFileContents(const string& rel_path,
                         const string& expected_content) {
    string actual_content;
    ASSERT_TRUE(io_delegate_.GetWrittenContents(rel_path, &actual_content))
        << "Expected aidl to write to " << rel_path << " but it did not.";

    if (actual_content == expected_content) {
      return;  // success!
    }

    test::PrintDiff(expected_content, actual_content);
    FAIL() << "Actual contents of " << rel_path
           << " did not match expected content";
  }

  FakeIoDelegate io_delegate_;
};

TEST_F(EndToEndTest, IExampleInterface) {
  using namespace ::android::aidl::test_data::example_interface;

  vector<string> args = {
    "aidl",
    "-b",
    "-I .",
    "-d an/arbitrary/path/to/dep.P",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(options.InputFiles().front(), kInterfaceDefinition);
  io_delegate_.AddCompoundParcelable("android.test.CompoundParcelable",
                                     {"Subclass1", "Subclass2"});
  AddStubAidls(kImportedParcelables, kImportedInterfaces);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutput);
  CheckFileContents(options.DependencyFile(), kExpectedJavaDepsOutput);
}

TEST_F(EndToEndTest, IExampleInterface_WithTrace) {
  using namespace ::android::aidl::test_data::example_interface;

  vector<string> args = {
    "aidl",
    "-b",
    "-I .",
    "-t", //trace
    "-d an/arbitrary/path/to/dep.P",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(options.InputFiles().front(), kInterfaceDefinition);
  io_delegate_.AddCompoundParcelable("android.test.CompoundParcelable",
                                     {"Subclass1", "Subclass2"});
  AddStubAidls(kImportedParcelables, kImportedInterfaces);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutputWithTrace);
  CheckFileContents(options.DependencyFile(), kExpectedJavaDepsOutput);
}

TEST_F(EndToEndTest, IExampleInterface_WithTransactionNames) {
  using namespace ::android::aidl::test_data::example_interface;

  vector<string> args = {
    "aidl",
    "-b",
    "-I .",
    "--transaction_name", //trace
    "-d an/arbitrary/path/to/dep.P",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(options.InputFiles().front(), kInterfaceDefinition);
  io_delegate_.AddCompoundParcelable("android.test.CompoundParcelable",
                                     {"Subclass1", "Subclass2"});
  AddStubAidls(kImportedParcelables, kImportedInterfaces);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutputWithTransactionNames);
  CheckFileContents(options.DependencyFile(), kExpectedJavaDepsOutput);
}

TEST_F(EndToEndTest, IExampleInterface_Outlining) {
  using namespace ::android::aidl::test_data::example_interface;

  vector<string> args = {
    "aidl",
    "-b",
    "-I .",
    "-d an/arbitrary/path/to/dep.P",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);
  options.onTransact_outline_threshold_ = 4;
  options.onTransact_non_outline_count_ = 3;

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(options.InputFiles().front(), kInterfaceDefinitionOutlining);
  io_delegate_.AddCompoundParcelable("android.test.CompoundParcelable",
                                     {"Subclass1", "Subclass2"});
  AddStubAidls(kImportedParcelables, kImportedInterfaces);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutputOutlining);
  CheckFileContents(options.DependencyFile(), kExpectedJavaDepsOutput);
}

TEST_F(EndToEndTest, IExampleInterface_WithVersion) {
  using namespace ::android::aidl::test_data::example_interface;

  vector<string> args = {
    "aidl",
    "-b",
    "-I .",
    "-d an/arbitrary/path/to/dep.P",
    "--version=10",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);
  options.onTransact_outline_threshold_ = 4;
  options.onTransact_non_outline_count_ = 3;

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(options.InputFiles().front(), kInterfaceDefinitionOutlining);
  io_delegate_.AddCompoundParcelable("android.test.CompoundParcelable",
                                     {"Subclass1", "Subclass2"});
  AddStubAidls(kImportedParcelables, kImportedInterfaces);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutputWithVersion);
  CheckFileContents(options.DependencyFile(), kExpectedJavaDepsOutput);
}


TEST_F(EndToEndTest, IPingResponderCpp) {
  using namespace ::android::aidl::test_data::ping_responder;

  vector<string> args = {
    "aidl-cpp",
    "-d deps.P",
    "-I .",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kGenHeaderDir,
    kCppOutputPath};
  Options options = Options::From(args);

  // Set up input paths.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);
  AddStubAidls(kImportedParcelables, kImportedInterfaces, kCppParcelableHeader);

  // Check that we parse and generate code correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kCppOutputPath, kExpectedCppOutput);
  CheckFileContents(kGenInterfaceHeaderPath, kExpectedIHeaderOutput);
  CheckFileContents(kGenClientHeaderPath, kExpectedBpHeaderOutput);
  CheckFileContents(kGenServerHeaderPath, kExpectedBnHeaderOutput);
  CheckFileContents(options.DependencyFile(), kExpectedCppDepsOutput);
}

TEST_F(EndToEndTest, IPingResponderCpp_WithVersion) {
  using namespace ::android::aidl::test_data::ping_responder;

  vector<string> args = {
    "aidl-cpp",
    "-d deps.P",
    "-I .",
    "--version=10",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kGenHeaderDir,
    kCppOutputPath};
  Options options = Options::From(args);

  // Set up input paths.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);
  AddStubAidls(kImportedParcelables, kImportedInterfaces, kCppParcelableHeader);

  // Check that we parse and generate code correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kCppOutputPath, kExpectedCppOutputWithVersion);
  CheckFileContents(kGenInterfaceHeaderPath, kExpectedIHeaderOutputWithVersion);
  CheckFileContents(kGenClientHeaderPath, kExpectedBpHeaderOutputWithVersion);
  CheckFileContents(kGenServerHeaderPath, kExpectedBnHeaderOutputWithVersion);
  CheckFileContents(options.DependencyFile(), kExpectedCppDepsOutput);
}

TEST_F(EndToEndTest, StringConstantsInCpp) {
  using namespace ::android::aidl::test_data::string_constants;

  vector<string> args = {
    "aidl-cpp",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kGenHeaderDir,
    kCppOutputPath};
  Options options = Options::From(args);

  // Set up input paths.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);

  // Check that we parse and generate code correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kCppOutputPath, kExpectedCppOutput);
  CheckFileContents(kGenInterfaceHeaderPath, kExpectedIHeaderOutput);
}

TEST_F(EndToEndTest, StringConstantsInJava) {
  using namespace ::android::aidl::test_data::string_constants;

  vector<string> args = {
    "aidl",
    "-b",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutput);
}

TEST_F(EndToEndTest, StringConstantsInCpp_WithVersion) {
  using namespace ::android::aidl::test_data::string_constants;

  vector<string> args = {
    "aidl-cpp",
    "--version=10",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kGenHeaderDir,
    kCppOutputPath};
  Options options = Options::From(args);

  // Set up input paths.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);

  // Check that we parse and generate code correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kCppOutputPath, kExpectedCppOutputWithVersion);
  CheckFileContents(kGenInterfaceHeaderPath, kExpectedIHeaderOutputWithVersion);
}

TEST_F(EndToEndTest, StringConstantsInJava_WithVersion) {
  using namespace ::android::aidl::test_data::string_constants;

  vector<string> args = {
    "aidl",
    "-b",
    "--version=10",
    CanonicalNameToPath(kCanonicalName, ".aidl"),
    kJavaOutputPath};
  Options options = Options::From(args);

  // Load up our fake file system with data.
  io_delegate_.SetFileContents(CanonicalNameToPath(kCanonicalName, ".aidl"), kInterfaceDefinition);

  // Check that we parse correctly.
  EXPECT_EQ(android::aidl::compile_aidl(options, io_delegate_), 0);
  CheckFileContents(kJavaOutputPath, kExpectedJavaOutputWithVersion);
}

}  // namespace android
}  // namespace aidl
