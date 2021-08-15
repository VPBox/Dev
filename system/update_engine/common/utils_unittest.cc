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

#include "update_engine/common/utils.h"

#include <fcntl.h>
#include <stdint.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <limits>
#include <string>
#include <vector>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"

using std::numeric_limits;
using std::string;
using std::vector;

namespace chromeos_update_engine {

class UtilsTest : public ::testing::Test {};

TEST(UtilsTest, CanParseECVersion) {
  // Should be able to parse and valid key value line.
  EXPECT_EQ("12345", utils::ParseECVersion("fw_version=12345"));
  EXPECT_EQ("123456",
            utils::ParseECVersion("b=1231a fw_version=123456 a=fasd2"));
  EXPECT_EQ("12345", utils::ParseECVersion("fw_version=12345"));
  EXPECT_EQ("00VFA616",
            utils::ParseECVersion("vendor=\"sam\" fw_version=\"00VFA616\""));

  // For invalid entries, should return the empty string.
  EXPECT_EQ("", utils::ParseECVersion("b=1231a fw_version a=fasd2"));
}

TEST(UtilsTest, WriteFileOpenFailure) {
  EXPECT_FALSE(utils::WriteFile("/this/doesn't/exist", "hello", 5));
}

TEST(UtilsTest, WriteFileReadFile) {
  test_utils::ScopedTempFile file;
  EXPECT_TRUE(utils::WriteFile(file.path().c_str(), "hello", 5));

  brillo::Blob readback;
  EXPECT_TRUE(utils::ReadFile(file.path().c_str(), &readback));
  EXPECT_EQ("hello", string(readback.begin(), readback.end()));
}

TEST(UtilsTest, ReadFileFailure) {
  brillo::Blob empty;
  EXPECT_FALSE(utils::ReadFile("/this/doesn't/exist", &empty));
}

TEST(UtilsTest, ReadFileChunk) {
  test_utils::ScopedTempFile file;
  brillo::Blob data;
  const size_t kSize = 1024 * 1024;
  for (size_t i = 0; i < kSize; i++) {
    data.push_back(i % 255);
  }
  EXPECT_TRUE(test_utils::WriteFileVector(file.path(), data));
  brillo::Blob in_data;
  EXPECT_TRUE(utils::ReadFileChunk(file.path().c_str(), kSize, 10, &in_data));
  EXPECT_TRUE(in_data.empty());
  EXPECT_TRUE(utils::ReadFileChunk(file.path().c_str(), 0, -1, &in_data));
  EXPECT_EQ(data, in_data);
  in_data.clear();
  EXPECT_TRUE(utils::ReadFileChunk(file.path().c_str(), 10, 20, &in_data));
  EXPECT_EQ(brillo::Blob(data.begin() + 10, data.begin() + 10 + 20), in_data);
}

TEST(UtilsTest, ErrnoNumberAsStringTest) {
  EXPECT_EQ("No such file or directory", utils::ErrnoNumberAsString(ENOENT));
}

TEST(UtilsTest, IsSymlinkTest) {
  base::ScopedTempDir temp_dir;
  ASSERT_TRUE(temp_dir.CreateUniqueTempDir());
  string temp_file = temp_dir.GetPath().Append("temp-file").value();
  EXPECT_TRUE(utils::WriteFile(temp_file.c_str(), "", 0));
  string temp_symlink = temp_dir.GetPath().Append("temp-symlink").value();
  EXPECT_EQ(0, symlink(temp_file.c_str(), temp_symlink.c_str()));
  EXPECT_FALSE(utils::IsSymlink(temp_dir.GetPath().value().c_str()));
  EXPECT_FALSE(utils::IsSymlink(temp_file.c_str()));
  EXPECT_TRUE(utils::IsSymlink(temp_symlink.c_str()));
  EXPECT_FALSE(utils::IsSymlink("/non/existent/path"));
}

TEST(UtilsTest, SplitPartitionNameTest) {
  string disk;
  int part_num;

  EXPECT_TRUE(utils::SplitPartitionName("/dev/sda3", &disk, &part_num));
  EXPECT_EQ("/dev/sda", disk);
  EXPECT_EQ(3, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/sdp1234", &disk, &part_num));
  EXPECT_EQ("/dev/sdp", disk);
  EXPECT_EQ(1234, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/mmcblk0p3", &disk, &part_num));
  EXPECT_EQ("/dev/mmcblk0", disk);
  EXPECT_EQ(3, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/ubiblock3_2", &disk, &part_num));
  EXPECT_EQ("/dev/ubiblock", disk);
  EXPECT_EQ(3, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/loop10", &disk, &part_num));
  EXPECT_EQ("/dev/loop", disk);
  EXPECT_EQ(10, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/loop28p11", &disk, &part_num));
  EXPECT_EQ("/dev/loop28", disk);
  EXPECT_EQ(11, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/loop10_0", &disk, &part_num));
  EXPECT_EQ("/dev/loop", disk);
  EXPECT_EQ(10, part_num);

  EXPECT_TRUE(utils::SplitPartitionName("/dev/loop28p11_0", &disk, &part_num));
  EXPECT_EQ("/dev/loop28", disk);
  EXPECT_EQ(11, part_num);

  EXPECT_FALSE(utils::SplitPartitionName("/dev/mmcblk0p", &disk, &part_num));
  EXPECT_FALSE(utils::SplitPartitionName("/dev/sda", &disk, &part_num));
  EXPECT_FALSE(utils::SplitPartitionName("/dev/foo/bar", &disk, &part_num));
  EXPECT_FALSE(utils::SplitPartitionName("/", &disk, &part_num));
  EXPECT_FALSE(utils::SplitPartitionName("", &disk, &part_num));
}

TEST(UtilsTest, MakePartitionNameTest) {
  EXPECT_EQ("/dev/sda4", utils::MakePartitionName("/dev/sda", 4));
  EXPECT_EQ("/dev/sda123", utils::MakePartitionName("/dev/sda", 123));
  EXPECT_EQ("/dev/mmcblk2", utils::MakePartitionName("/dev/mmcblk", 2));
  EXPECT_EQ("/dev/mmcblk0p2", utils::MakePartitionName("/dev/mmcblk0", 2));
  EXPECT_EQ("/dev/loop8", utils::MakePartitionName("/dev/loop", 8));
  EXPECT_EQ("/dev/loop12p2", utils::MakePartitionName("/dev/loop12", 2));
  EXPECT_EQ("/dev/ubi5_0", utils::MakePartitionName("/dev/ubiblock", 5));
  EXPECT_EQ("/dev/mtd4", utils::MakePartitionName("/dev/ubiblock", 4));
  EXPECT_EQ("/dev/ubi3_0", utils::MakePartitionName("/dev/ubiblock", 3));
  EXPECT_EQ("/dev/mtd2", utils::MakePartitionName("/dev/ubiblock", 2));
  EXPECT_EQ("/dev/ubi1_0", utils::MakePartitionName("/dev/ubiblock", 1));
}

TEST(UtilsTest, MakePartitionNameForMountTest) {
  EXPECT_EQ("/dev/sda4", utils::MakePartitionNameForMount("/dev/sda4"));
  EXPECT_EQ("/dev/sda123", utils::MakePartitionNameForMount("/dev/sda123"));
  EXPECT_EQ("/dev/mmcblk2", utils::MakePartitionNameForMount("/dev/mmcblk2"));
  EXPECT_EQ("/dev/mmcblk0p2",
            utils::MakePartitionNameForMount("/dev/mmcblk0p2"));
  EXPECT_EQ("/dev/loop0", utils::MakePartitionNameForMount("/dev/loop0"));
  EXPECT_EQ("/dev/loop8", utils::MakePartitionNameForMount("/dev/loop8"));
  EXPECT_EQ("/dev/loop12p2", utils::MakePartitionNameForMount("/dev/loop12p2"));
  EXPECT_EQ("/dev/ubiblock5_0",
            utils::MakePartitionNameForMount("/dev/ubiblock5_0"));
  EXPECT_EQ("/dev/mtd4", utils::MakePartitionNameForMount("/dev/ubi4_0"));
  EXPECT_EQ("/dev/ubiblock3_0",
            utils::MakePartitionNameForMount("/dev/ubiblock3"));
  EXPECT_EQ("/dev/mtd2", utils::MakePartitionNameForMount("/dev/ubi2"));
  EXPECT_EQ("/dev/ubi1_0", utils::MakePartitionNameForMount("/dev/ubiblock1"));
}

TEST(UtilsTest, FuzzIntTest) {
  static const uint32_t kRanges[] = {0, 1, 2, 20};
  for (uint32_t range : kRanges) {
    const int kValue = 50;
    for (int tries = 0; tries < 100; ++tries) {
      uint32_t value = utils::FuzzInt(kValue, range);
      EXPECT_GE(value, kValue - range / 2);
      EXPECT_LE(value, kValue + range - range / 2);
    }
  }
}

namespace {
void GetFileFormatTester(const string& expected,
                         const vector<uint8_t>& contents) {
  test_utils::ScopedTempFile file;
  ASSERT_TRUE(utils::WriteFile(file.path().c_str(),
                               reinterpret_cast<const char*>(contents.data()),
                               contents.size()));
  EXPECT_EQ(expected, utils::GetFileFormat(file.path()));
}
}  // namespace

TEST(UtilsTest, GetFileFormatTest) {
  EXPECT_EQ("File not found.", utils::GetFileFormat("/path/to/nowhere"));
  GetFileFormatTester("data", vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8});
  GetFileFormatTester("ELF", vector<uint8_t>{0x7f, 0x45, 0x4c, 0x46});

  // Real tests from cros_installer on different boards.
  // ELF 32-bit LSB executable, Intel 80386
  GetFileFormatTester(
      "ELF 32-bit little-endian x86",
      vector<uint8_t>{0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
                      0x90, 0x83, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00});

  // ELF 32-bit LSB executable, MIPS
  GetFileFormatTester(
      "ELF 32-bit little-endian mips",
      vector<uint8_t>{0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x03, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00,
                      0xc0, 0x12, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00});

  // ELF 32-bit LSB executable, ARM
  GetFileFormatTester(
      "ELF 32-bit little-endian arm",
      vector<uint8_t>{0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x02, 0x00, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00,
                      0x85, 0x8b, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00});

  // ELF 64-bit LSB executable, x86-64
  GetFileFormatTester(
      "ELF 64-bit little-endian x86-64",
      vector<uint8_t>{0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x02, 0x00, 0x3e, 0x00, 0x01, 0x00, 0x00, 0x00,
                      0xb0, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00});
}

TEST(UtilsTest, FormatTimeDeltaTest) {
  // utils::FormatTimeDelta() is not locale-aware (it's only used for logging
  // which is not localized) so we only need to test the C locale
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromMilliseconds(100)),
            "0.1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(0)), "0s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(1)), "1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(59)), "59s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(60)), "1m0s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(61)), "1m1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(90)), "1m30s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(1205)),
            "20m5s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(3600)),
            "1h0m0s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(3601)),
            "1h0m1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(3661)),
            "1h1m1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(7261)),
            "2h1m1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(86400)),
            "1d0h0m0s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(86401)),
            "1d0h0m1s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(200000)),
            "2d7h33m20s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(200000) +
                                   base::TimeDelta::FromMilliseconds(1)),
            "2d7h33m20.001s");
  EXPECT_EQ(utils::FormatTimeDelta(base::TimeDelta::FromSeconds(-1)), "-1s");
}

TEST(UtilsTest, ConvertToOmahaInstallDate) {
  // The Omaha Epoch starts at Jan 1, 2007 0:00 PST which is a
  // Monday. In Unix time, this point in time is easily obtained via
  // the date(1) command like this:
  //
  //  $ date +"%s" --date="Jan 1, 2007 0:00 PST"
  const time_t omaha_epoch = 1167638400;
  int value;

  // Points in time *on and after* the Omaha epoch should not fail.
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch), &value));
  EXPECT_GE(value, 0);

  // Anything before the Omaha epoch should fail. We test it for two points.
  EXPECT_FALSE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch - 1), &value));
  EXPECT_FALSE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch - 100 * 24 * 3600), &value));

  // Check that we jump from 0 to 7 exactly on the one-week mark, e.g.
  // on Jan 8, 2007 0:00 PST.
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 7 * 24 * 3600 - 1), &value));
  EXPECT_EQ(value, 0);
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 7 * 24 * 3600), &value));
  EXPECT_EQ(value, 7);

  // Check a couple of more values.
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 10 * 24 * 3600), &value));
  EXPECT_EQ(value, 7);
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 20 * 24 * 3600), &value));
  EXPECT_EQ(value, 14);
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 26 * 24 * 3600), &value));
  EXPECT_EQ(value, 21);
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(omaha_epoch + 29 * 24 * 3600), &value));
  EXPECT_EQ(value, 28);

  // The date Jun 4, 2007 0:00 PDT is a Monday and is hence a point
  // where the Omaha InstallDate jumps 7 days. Its unix time is
  // 1180940400. Notably, this is a point in time where Daylight
  // Savings Time (DST) was is in effect (e.g. it's PDT, not PST).
  //
  // Note that as utils::ConvertToOmahaInstallDate() _deliberately_
  // ignores DST (as it's hard to implement in a thread-safe way using
  // glibc, see comments in utils.h) we have to fudge by the DST
  // offset which is one hour. Conveniently, if the function were
  // someday modified to be DST aware, this test would have to be
  // modified as well.
  const time_t dst_time = 1180940400;  // Jun 4, 2007 0:00 PDT.
  const time_t fudge = 3600;
  int value1, value2;
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(dst_time + fudge - 1), &value1));
  EXPECT_TRUE(utils::ConvertToOmahaInstallDate(
      base::Time::FromTimeT(dst_time + fudge), &value2));
  EXPECT_EQ(value1, value2 - 7);
}

TEST(UtilsTest, GetMinorVersion) {
  // Test GetMinorVersion by verifying that it parses the conf file and returns
  // the correct value.
  uint32_t minor_version;

  brillo::KeyValueStore store;
  EXPECT_FALSE(utils::GetMinorVersion(store, &minor_version));

  EXPECT_TRUE(store.LoadFromString("PAYLOAD_MINOR_VERSION=one-two-three\n"));
  EXPECT_FALSE(utils::GetMinorVersion(store, &minor_version));

  EXPECT_TRUE(store.LoadFromString("PAYLOAD_MINOR_VERSION=123\n"));
  EXPECT_TRUE(utils::GetMinorVersion(store, &minor_version));
  EXPECT_EQ(123U, minor_version);
}

static bool BoolMacroTestHelper() {
  int i = 1;
  unsigned int ui = 1;
  bool b = 1;
  std::unique_ptr<char> cptr(new char);

  TEST_AND_RETURN_FALSE(i);
  TEST_AND_RETURN_FALSE(ui);
  TEST_AND_RETURN_FALSE(b);
  TEST_AND_RETURN_FALSE(cptr);

  TEST_AND_RETURN_FALSE_ERRNO(i);
  TEST_AND_RETURN_FALSE_ERRNO(ui);
  TEST_AND_RETURN_FALSE_ERRNO(b);
  TEST_AND_RETURN_FALSE_ERRNO(cptr);

  return true;
}

static void VoidMacroTestHelper(bool* ret) {
  int i = 1;
  unsigned int ui = 1;
  bool b = 1;
  std::unique_ptr<char> cptr(new char);

  *ret = false;

  TEST_AND_RETURN(i);
  TEST_AND_RETURN(ui);
  TEST_AND_RETURN(b);
  TEST_AND_RETURN(cptr);

  TEST_AND_RETURN_ERRNO(i);
  TEST_AND_RETURN_ERRNO(ui);
  TEST_AND_RETURN_ERRNO(b);
  TEST_AND_RETURN_ERRNO(cptr);

  *ret = true;
}

static void ExpectParseRollbackKeyVersion(const string& version,
                                          uint16_t expected_high,
                                          uint16_t expected_low) {
  uint16_t actual_high;
  uint16_t actual_low;
  utils::ParseRollbackKeyVersion(version, &actual_high, &actual_low);
  EXPECT_EQ(expected_high, actual_high);
  EXPECT_EQ(expected_low, actual_low);
}

static void ExpectInvalidParseRollbackKeyVersion(const string& version) {
  ExpectParseRollbackKeyVersion(version,
                                numeric_limits<uint16_t>::max(),
                                numeric_limits<uint16_t>::max());
}

TEST(UtilsTest, TestMacros) {
  bool void_test = false;
  VoidMacroTestHelper(&void_test);
  EXPECT_TRUE(void_test);

  EXPECT_TRUE(BoolMacroTestHelper());
}

TEST(UtilsTest, RunAsRootUnmountFilesystemFailureTest) {
  EXPECT_FALSE(utils::UnmountFilesystem("/path/to/non-existing-dir"));
}

TEST(UtilsTest, RunAsRootUnmountFilesystemBusyFailureTest) {
  test_utils::ScopedTempFile tmp_image("img.XXXXXX");

  EXPECT_TRUE(base::CopyFile(
      test_utils::GetBuildArtifactsPath().Append("gen/disk_ext2_4k.img"),
      base::FilePath(tmp_image.path())));

  base::ScopedTempDir mnt_dir;
  EXPECT_TRUE(mnt_dir.CreateUniqueTempDir());

  string loop_dev;
  test_utils::ScopedLoopbackDeviceBinder loop_binder(
      tmp_image.path(), true, &loop_dev);

  EXPECT_FALSE(utils::IsMountpoint(mnt_dir.GetPath().value()));
  // This is the actual test part. While we hold a file descriptor open for the
  // mounted filesystem, umount should still succeed.
  EXPECT_TRUE(utils::MountFilesystem(
      loop_dev, mnt_dir.GetPath().value(), MS_RDONLY, "ext4", ""));
  // Verify the directory is a mount point now.
  EXPECT_TRUE(utils::IsMountpoint(mnt_dir.GetPath().value()));

  string target_file = mnt_dir.GetPath().Append("empty-file").value();
  int fd = HANDLE_EINTR(open(target_file.c_str(), O_RDONLY));
  EXPECT_GE(fd, 0);
  EXPECT_TRUE(utils::UnmountFilesystem(mnt_dir.GetPath().value()));
  // The filesystem should be already unmounted at this point.
  EXPECT_FALSE(utils::IsMountpoint(mnt_dir.GetPath().value()));
  IGNORE_EINTR(close(fd));
  // The filesystem was already unmounted so this call should fail.
  EXPECT_FALSE(utils::UnmountFilesystem(mnt_dir.GetPath().value()));
}

TEST(UtilsTest, IsMountpointTest) {
  EXPECT_TRUE(utils::IsMountpoint("/"));
  EXPECT_FALSE(utils::IsMountpoint("/path/to/nowhere"));

  base::ScopedTempDir mnt_dir;
  EXPECT_TRUE(mnt_dir.CreateUniqueTempDir());
  EXPECT_FALSE(utils::IsMountpoint(mnt_dir.GetPath().value()));

  test_utils::ScopedTempFile file;
  EXPECT_FALSE(utils::IsMountpoint(file.path()));
}

TEST(UtilsTest, VersionPrefix) {
  EXPECT_EQ(10575, utils::VersionPrefix("10575.39."));
  EXPECT_EQ(10575, utils::VersionPrefix("10575.39"));
  EXPECT_EQ(10575, utils::VersionPrefix("10575.x"));
  EXPECT_EQ(10575, utils::VersionPrefix("10575."));
  EXPECT_EQ(10575, utils::VersionPrefix("10575"));
  EXPECT_EQ(0, utils::VersionPrefix(""));
  EXPECT_EQ(-1, utils::VersionPrefix("x"));
  EXPECT_EQ(-1, utils::VersionPrefix("1x"));
  EXPECT_EQ(-1, utils::VersionPrefix("x.1"));
}

TEST(UtilsTest, ParseDottedVersion) {
  // Valid case.
  ExpectParseRollbackKeyVersion("2.3", 2, 3);
  ExpectParseRollbackKeyVersion("65535.65535", 65535, 65535);

  // Zero is technically allowed but never actually used.
  ExpectParseRollbackKeyVersion("0.0", 0, 0);

  // Invalid cases.
  ExpectInvalidParseRollbackKeyVersion("");
  ExpectInvalidParseRollbackKeyVersion("2");
  ExpectInvalidParseRollbackKeyVersion("2.");
  ExpectInvalidParseRollbackKeyVersion(".2");
  ExpectInvalidParseRollbackKeyVersion("2.2.");
  ExpectInvalidParseRollbackKeyVersion("2.2.3");
  ExpectInvalidParseRollbackKeyVersion(".2.2");
  ExpectInvalidParseRollbackKeyVersion("a.b");
  ExpectInvalidParseRollbackKeyVersion("1.b");
  ExpectInvalidParseRollbackKeyVersion("a.2");
  ExpectInvalidParseRollbackKeyVersion("65536.65536");
  ExpectInvalidParseRollbackKeyVersion("99999.99999");
  ExpectInvalidParseRollbackKeyVersion("99999.1");
  ExpectInvalidParseRollbackKeyVersion("1.99999");
}

}  // namespace chromeos_update_engine
