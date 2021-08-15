/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <android-base/file.h>

#include "dso.h"
#include "environment.h"

TEST(environment, GetCpusFromString) {
  ASSERT_EQ(GetCpusFromString(""), std::vector<int>());
  ASSERT_EQ(GetCpusFromString("0-2"), std::vector<int>({0, 1, 2}));
  ASSERT_EQ(GetCpusFromString("0,2-3"), std::vector<int>({0, 2, 3}));
  ASSERT_EQ(GetCpusFromString("1,0-3,3,4"), std::vector<int>({0, 1, 2, 3, 4}));
}

TEST(environment, PrepareVdsoFile) {
  std::string content;
  ASSERT_TRUE(android::base::ReadFileToString("/proc/self/maps", &content));
  if (content.find("[vdso]") == std::string::npos) {
    // Vdso isn't used, no need to test.
    return;
  }
  TemporaryDir tmpdir;
  ScopedTempFiles scoped_temp_files(tmpdir.path);
  PrepareVdsoFile();
  std::unique_ptr<Dso> dso = Dso::CreateDso(DSO_ELF_FILE, "[vdso]",
                                            sizeof(size_t) == sizeof(uint64_t));
  ASSERT_TRUE(dso != nullptr);
  ASSERT_NE(dso->GetDebugFilePath(), "[vdso]");
}

TEST(environment, GetHardwareFromCpuInfo) {
  std::string cpu_info = "CPU revision : 10\n\n"
      "Hardware : Symbol i.MX6 Freeport_Plat Quad/DualLite (Device Tree)\n";
  ASSERT_EQ("Symbol i.MX6 Freeport_Plat Quad/DualLite (Device Tree)",
            GetHardwareFromCpuInfo(cpu_info));
}

TEST(environment, MappedFileOnlyExistInMemory) {
  ASSERT_TRUE(MappedFileOnlyExistInMemory(""));
  ASSERT_TRUE(MappedFileOnlyExistInMemory("[stack]"));
  ASSERT_TRUE(MappedFileOnlyExistInMemory("[anon:.bss]"));
  ASSERT_FALSE(MappedFileOnlyExistInMemory("[vdso]"));
  ASSERT_TRUE(MappedFileOnlyExistInMemory("/dev/__properties__/u:object_r"));
  ASSERT_TRUE(MappedFileOnlyExistInMemory("//anon"));
  ASSERT_TRUE(MappedFileOnlyExistInMemory("/memfd:/jit-cache"));
  ASSERT_FALSE(MappedFileOnlyExistInMemory("./TemporaryFile-12345"));
  ASSERT_FALSE(MappedFileOnlyExistInMemory("/system/lib64/libc.so"));
}

TEST(environment, SetPerfEventLimits) {
#if defined(__ANDROID__)
  if (GetAndroidVersion() <= kAndroidVersionP) {
    return;
  }
  uint64_t orig_freq = 100000;
  size_t orig_percent = 25;
  uint64_t orig_mlock_kb = 516;
  bool has_freq = GetMaxSampleFrequency(&orig_freq);
  bool has_percent = GetCpuTimeMaxPercent(&orig_percent);
  bool has_mlock_kb = GetPerfEventMlockKb(&orig_mlock_kb);

  ASSERT_TRUE(SetPerfEventLimits(orig_freq + 1, orig_percent + 1, orig_mlock_kb + 1));
  if (has_freq) {
    uint64_t value;
    ASSERT_TRUE(GetMaxSampleFrequency(&value));
    ASSERT_EQ(value, orig_freq + 1);
  }
  if (has_percent) {
    size_t value;
    ASSERT_TRUE(GetCpuTimeMaxPercent(&value));
    ASSERT_EQ(value, orig_percent + 1);
  }
  if (has_mlock_kb) {
    uint64_t value;
    ASSERT_TRUE(GetPerfEventMlockKb(&value));
    ASSERT_EQ(value, orig_mlock_kb + 1);
  }
  // Restore the environment.
  ASSERT_TRUE(SetPerfEventLimits(orig_freq, orig_percent, orig_mlock_kb));
#else  // !defined(__ANDROID__)
  GTEST_LOG_(INFO) << "This test tests setting properties on Android.";
#endif
}
