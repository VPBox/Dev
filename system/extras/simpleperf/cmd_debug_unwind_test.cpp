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

#include <gtest/gtest.h>

#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include <android-base/file.h>

#include "command.h"
#include "get_test_data.h"
#include "record_file.h"
#include "test_util.h"

static std::unique_ptr<Command> DebugUnwindCmd() {
  return CreateCommandInstance("debug-unwind");
}

TEST(cmd_debug_unwind, smoke) {
  std::string input_data = GetTestData(PERF_DATA_NO_UNWIND);
  CaptureStdout capture;
  TemporaryFile tmp_file;
  ASSERT_TRUE(capture.Start());
  ASSERT_TRUE(DebugUnwindCmd()->Run({"-i", input_data, "-o", tmp_file.path}));
  ASSERT_NE(capture.Finish().find("Unwinding sample count: 8"), std::string::npos);

  ASSERT_TRUE(capture.Start());
  ASSERT_TRUE(DebugUnwindCmd()->Run({"-i", input_data, "-o", tmp_file.path, "--time",
                                     "1516379654300997"}));
  ASSERT_NE(capture.Finish().find("Unwinding sample count: 1"), std::string::npos);
}

TEST(cmd_debug_unwind, symfs_option) {
  std::string input_data = GetTestData(NATIVELIB_IN_APK_PERF_DATA);
  CaptureStdout capture;
  TemporaryFile tmp_file;
  ASSERT_TRUE(capture.Start());
  ASSERT_TRUE(DebugUnwindCmd()->Run({"-i", input_data, "-o", tmp_file.path, "--symfs",
                                     GetTestDataDir()}));
  ASSERT_NE(capture.Finish().find("Unwinding sample count: 55"), std::string::npos);
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmp_file.path);
  ASSERT_TRUE(reader);
  const std::map<int, PerfFileFormat::SectionDesc>& features = reader->FeatureSectionDescriptors();
  ASSERT_NE(features.find(PerfFileFormat::FEAT_FILE), features.end());
  ASSERT_NE(features.find(PerfFileFormat::FEAT_META_INFO), features.end());
  std::unordered_map<std::string, std::string> info_map;
  ASSERT_TRUE(reader->ReadMetaInfoFeature(&info_map));
  ASSERT_EQ(info_map["debug_unwind"], "true");
}

TEST(cmd_debug_unwind, unwind_with_ip_zero_in_callchain) {
  TemporaryFile tmp_file;
  CaptureStdout capture;
  ASSERT_TRUE(capture.Start());
  ASSERT_TRUE(DebugUnwindCmd()->Run({"-i", GetTestData(PERF_DATA_WITH_IP_ZERO_IN_CALLCHAIN),
                                     "-o", tmp_file.path}));
  ASSERT_NE(capture.Finish().find("Unwinding sample count: 1"), std::string::npos);
}
