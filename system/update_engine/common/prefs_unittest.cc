//
// Copyright (C) 2012 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/common/prefs.h"

#include <inttypes.h>

#include <limits>
#include <string>

#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <base/macros.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::string;
using testing::_;
using testing::Eq;

namespace {
// Test key used along the tests.
const char kKey[] = "test-key";
}  // namespace

namespace chromeos_update_engine {

class PrefsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    prefs_dir_ = temp_dir_.GetPath();
    ASSERT_TRUE(prefs_.Init(prefs_dir_));
  }

  bool SetValue(const string& key, const string& value) {
    return base::WriteFile(prefs_dir_.Append(key),
                           value.data(),
                           value.length()) == static_cast<int>(value.length());
  }

  base::ScopedTempDir temp_dir_;
  base::FilePath prefs_dir_;
  Prefs prefs_;
};

TEST_F(PrefsTest, GetFileNameForKey) {
  const char kAllvalidCharsKey[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
  base::FilePath path;
  EXPECT_TRUE(prefs_.file_storage_.GetFileNameForKey(kAllvalidCharsKey, &path));
  EXPECT_EQ(prefs_dir_.Append(kAllvalidCharsKey).value(), path.value());
}

TEST_F(PrefsTest, GetFileNameForKeyBadCharacter) {
  base::FilePath path;
  EXPECT_FALSE(prefs_.file_storage_.GetFileNameForKey("ABC abc", &path));
}

TEST_F(PrefsTest, GetFileNameForKeyEmpty) {
  base::FilePath path;
  EXPECT_FALSE(prefs_.file_storage_.GetFileNameForKey("", &path));
}

TEST_F(PrefsTest, GetString) {
  const string test_data = "test data";
  ASSERT_TRUE(SetValue(kKey, test_data));
  string value;
  EXPECT_TRUE(prefs_.GetString(kKey, &value));
  EXPECT_EQ(test_data, value);
}

TEST_F(PrefsTest, GetStringBadKey) {
  string value;
  EXPECT_FALSE(prefs_.GetString(",bad", &value));
}

TEST_F(PrefsTest, GetStringNonExistentKey) {
  string value;
  EXPECT_FALSE(prefs_.GetString("non-existent-key", &value));
}

TEST_F(PrefsTest, SetString) {
  const char kValue[] = "some test value\non 2 lines";
  EXPECT_TRUE(prefs_.SetString(kKey, kValue));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ(kValue, value);
}

TEST_F(PrefsTest, SetStringBadKey) {
  const char kKeyWithDots[] = ".no-dots";
  EXPECT_FALSE(prefs_.SetString(kKeyWithDots, "some value"));
  EXPECT_FALSE(base::PathExists(prefs_dir_.Append(kKeyWithDots)));
}

TEST_F(PrefsTest, SetStringCreateDir) {
  const char kValue[] = "test value";
  base::FilePath subdir = prefs_dir_.Append("subdir1").Append("subdir2");
  EXPECT_TRUE(prefs_.Init(subdir));
  EXPECT_TRUE(prefs_.SetString(kKey, kValue));
  string value;
  EXPECT_TRUE(base::ReadFileToString(subdir.Append(kKey), &value));
  EXPECT_EQ(kValue, value);
}

TEST_F(PrefsTest, SetStringDirCreationFailure) {
  EXPECT_TRUE(prefs_.Init(base::FilePath("/dev/null")));
  EXPECT_FALSE(prefs_.SetString(kKey, "test value"));
}

TEST_F(PrefsTest, SetStringFileCreationFailure) {
  base::CreateDirectory(prefs_dir_.Append(kKey));
  EXPECT_FALSE(prefs_.SetString(kKey, "test value"));
  EXPECT_TRUE(base::DirectoryExists(prefs_dir_.Append(kKey)));
}

TEST_F(PrefsTest, GetInt64) {
  ASSERT_TRUE(SetValue(kKey, " \n 25 \t "));
  int64_t value;
  EXPECT_TRUE(prefs_.GetInt64(kKey, &value));
  EXPECT_EQ(25, value);
}

TEST_F(PrefsTest, GetInt64BadValue) {
  ASSERT_TRUE(SetValue(kKey, "30a"));
  int64_t value;
  EXPECT_FALSE(prefs_.GetInt64(kKey, &value));
}

TEST_F(PrefsTest, GetInt64Max) {
  ASSERT_TRUE(SetValue(
      kKey,
      base::StringPrintf("%" PRIi64, std::numeric_limits<int64_t>::max())));
  int64_t value;
  EXPECT_TRUE(prefs_.GetInt64(kKey, &value));
  EXPECT_EQ(std::numeric_limits<int64_t>::max(), value);
}

TEST_F(PrefsTest, GetInt64Min) {
  ASSERT_TRUE(SetValue(
      kKey,
      base::StringPrintf("%" PRIi64, std::numeric_limits<int64_t>::min())));
  int64_t value;
  EXPECT_TRUE(prefs_.GetInt64(kKey, &value));
  EXPECT_EQ(std::numeric_limits<int64_t>::min(), value);
}

TEST_F(PrefsTest, GetInt64Negative) {
  ASSERT_TRUE(SetValue(kKey, " \t -100 \n "));
  int64_t value;
  EXPECT_TRUE(prefs_.GetInt64(kKey, &value));
  EXPECT_EQ(-100, value);
}

TEST_F(PrefsTest, GetInt64NonExistentKey) {
  int64_t value;
  EXPECT_FALSE(prefs_.GetInt64("random-key", &value));
}

TEST_F(PrefsTest, SetInt64) {
  EXPECT_TRUE(prefs_.SetInt64(kKey, -123));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ("-123", value);
}

TEST_F(PrefsTest, SetInt64BadKey) {
  const char kKeyWithSpaces[] = "s p a c e s";
  EXPECT_FALSE(prefs_.SetInt64(kKeyWithSpaces, 20));
  EXPECT_FALSE(base::PathExists(prefs_dir_.Append(kKeyWithSpaces)));
}

TEST_F(PrefsTest, SetInt64Max) {
  EXPECT_TRUE(prefs_.SetInt64(kKey, std::numeric_limits<int64_t>::max()));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ(base::StringPrintf("%" PRIi64, std::numeric_limits<int64_t>::max()),
            value);
}

TEST_F(PrefsTest, SetInt64Min) {
  EXPECT_TRUE(prefs_.SetInt64(kKey, std::numeric_limits<int64_t>::min()));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ(base::StringPrintf("%" PRIi64, std::numeric_limits<int64_t>::min()),
            value);
}

TEST_F(PrefsTest, GetBooleanFalse) {
  ASSERT_TRUE(SetValue(kKey, " \n false \t "));
  bool value;
  EXPECT_TRUE(prefs_.GetBoolean(kKey, &value));
  EXPECT_FALSE(value);
}

TEST_F(PrefsTest, GetBooleanTrue) {
  const char kKey[] = "test-key";
  ASSERT_TRUE(SetValue(kKey, " \t true \n "));
  bool value;
  EXPECT_TRUE(prefs_.GetBoolean(kKey, &value));
  EXPECT_TRUE(value);
}

TEST_F(PrefsTest, GetBooleanBadValue) {
  const char kKey[] = "test-key";
  ASSERT_TRUE(SetValue(kKey, "1"));
  bool value;
  EXPECT_FALSE(prefs_.GetBoolean(kKey, &value));
}

TEST_F(PrefsTest, GetBooleanBadEmptyValue) {
  const char kKey[] = "test-key";
  ASSERT_TRUE(SetValue(kKey, ""));
  bool value;
  EXPECT_FALSE(prefs_.GetBoolean(kKey, &value));
}

TEST_F(PrefsTest, GetBooleanNonExistentKey) {
  bool value;
  EXPECT_FALSE(prefs_.GetBoolean("random-key", &value));
}

TEST_F(PrefsTest, SetBooleanTrue) {
  const char kKey[] = "test-bool";
  EXPECT_TRUE(prefs_.SetBoolean(kKey, true));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ("true", value);
}

TEST_F(PrefsTest, SetBooleanFalse) {
  const char kKey[] = "test-bool";
  EXPECT_TRUE(prefs_.SetBoolean(kKey, false));
  string value;
  EXPECT_TRUE(base::ReadFileToString(prefs_dir_.Append(kKey), &value));
  EXPECT_EQ("false", value);
}

TEST_F(PrefsTest, SetBooleanBadKey) {
  const char kKey[] = "s p a c e s";
  EXPECT_FALSE(prefs_.SetBoolean(kKey, true));
  EXPECT_FALSE(base::PathExists(prefs_dir_.Append(kKey)));
}

TEST_F(PrefsTest, ExistsWorks) {
  // test that the key doesn't exist before we set it.
  EXPECT_FALSE(prefs_.Exists(kKey));

  // test that the key exists after we set it.
  ASSERT_TRUE(prefs_.SetInt64(kKey, 8));
  EXPECT_TRUE(prefs_.Exists(kKey));
}

TEST_F(PrefsTest, DeleteWorks) {
  // test that it's alright to delete a non-existent key.
  EXPECT_TRUE(prefs_.Delete(kKey));

  // delete the key after we set it.
  ASSERT_TRUE(prefs_.SetInt64(kKey, 0));
  EXPECT_TRUE(prefs_.Delete(kKey));

  // make sure it doesn't exist anymore.
  EXPECT_FALSE(prefs_.Exists(kKey));
}

class MockPrefsObserver : public PrefsInterface::ObserverInterface {
 public:
  MOCK_METHOD1(OnPrefSet, void(const string&));
  MOCK_METHOD1(OnPrefDeleted, void(const string& key));
};

TEST_F(PrefsTest, ObserversCalled) {
  MockPrefsObserver mock_obserser;
  prefs_.AddObserver(kKey, &mock_obserser);

  EXPECT_CALL(mock_obserser, OnPrefSet(Eq(kKey)));
  EXPECT_CALL(mock_obserser, OnPrefDeleted(_)).Times(0);
  prefs_.SetString(kKey, "value");
  testing::Mock::VerifyAndClearExpectations(&mock_obserser);

  EXPECT_CALL(mock_obserser, OnPrefSet(_)).Times(0);
  EXPECT_CALL(mock_obserser, OnPrefDeleted(Eq(kKey)));
  prefs_.Delete(kKey);
  testing::Mock::VerifyAndClearExpectations(&mock_obserser);

  prefs_.RemoveObserver(kKey, &mock_obserser);
}

TEST_F(PrefsTest, OnlyCalledOnObservedKeys) {
  MockPrefsObserver mock_obserser;
  const char kUnusedKey[] = "unused-key";
  prefs_.AddObserver(kUnusedKey, &mock_obserser);

  EXPECT_CALL(mock_obserser, OnPrefSet(_)).Times(0);
  EXPECT_CALL(mock_obserser, OnPrefDeleted(_)).Times(0);
  prefs_.SetString(kKey, "value");
  prefs_.Delete(kKey);

  prefs_.RemoveObserver(kUnusedKey, &mock_obserser);
}

TEST_F(PrefsTest, RemovedObserversNotCalled) {
  MockPrefsObserver mock_obserser_a, mock_obserser_b;
  prefs_.AddObserver(kKey, &mock_obserser_a);
  prefs_.AddObserver(kKey, &mock_obserser_b);
  EXPECT_CALL(mock_obserser_a, OnPrefSet(_)).Times(2);
  EXPECT_CALL(mock_obserser_b, OnPrefSet(_)).Times(1);
  EXPECT_TRUE(prefs_.SetString(kKey, "value"));
  prefs_.RemoveObserver(kKey, &mock_obserser_b);
  EXPECT_TRUE(prefs_.SetString(kKey, "other value"));
  prefs_.RemoveObserver(kKey, &mock_obserser_a);
  EXPECT_TRUE(prefs_.SetString(kKey, "yet another value"));
}

TEST_F(PrefsTest, UnsuccessfulCallsNotObserved) {
  MockPrefsObserver mock_obserser;
  const char kInvalidKey[] = "no spaces or .";
  prefs_.AddObserver(kInvalidKey, &mock_obserser);

  EXPECT_CALL(mock_obserser, OnPrefSet(_)).Times(0);
  EXPECT_CALL(mock_obserser, OnPrefDeleted(_)).Times(0);
  EXPECT_FALSE(prefs_.SetString(kInvalidKey, "value"));
  EXPECT_FALSE(prefs_.Delete(kInvalidKey));

  prefs_.RemoveObserver(kInvalidKey, &mock_obserser);
}

class MemoryPrefsTest : public ::testing::Test {
 protected:
  MemoryPrefs prefs_;
};

TEST_F(MemoryPrefsTest, BasicTest) {
  EXPECT_FALSE(prefs_.Exists(kKey));
  int64_t value = 0;
  EXPECT_FALSE(prefs_.GetInt64(kKey, &value));

  EXPECT_TRUE(prefs_.SetInt64(kKey, 1234));
  EXPECT_TRUE(prefs_.Exists(kKey));
  EXPECT_TRUE(prefs_.GetInt64(kKey, &value));
  EXPECT_EQ(1234, value);

  EXPECT_TRUE(prefs_.Delete(kKey));
  EXPECT_FALSE(prefs_.Exists(kKey));
  EXPECT_FALSE(prefs_.Delete(kKey));
}

}  // namespace chromeos_update_engine
