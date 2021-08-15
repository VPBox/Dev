#include <base/files/file_util.h>
#include <gtest/gtest.h>

#include "AllocationTestHarness.h"

#include "osi/include/config.h"

static const char CONFIG_FILE[] = "/data/local/tmp/config_test.conf";
static const char CONFIG_FILE_CONTENT[] =
    "                                                                                    \n\
first_key=value                                                                      \n\
                                                                                     \n\
# Device ID (DID) configuration                                                      \n\
[DID]                                                                                \n\
                                                                                     \n\
# Record Number: 1, 2 or 3 - maximum of 3 records                                    \n\
recordNumber = 1                                                                     \n\
                                                                                     \n\
# Primary Record - true or false (default)                                           \n\
# There can be only one primary record                                               \n\
primaryRecord = true                                                                 \n\
                                                                                     \n\
# Vendor ID '0xFFFF' indicates no Device ID Service Record is present in the device  \n\
# 0x000F = Broadcom Corporation (default)                                            \n\
#vendorId = 0x000F                                                                   \n\
                                                                                     \n\
# Vendor ID Source                                                                   \n\
# 0x0001 = Bluetooth SIG assigned Device ID Vendor ID value (default)                \n\
# 0x0002 = USB Implementer's Forum assigned Device ID Vendor ID value                \n\
#vendorIdSource = 0x0001                                                             \n\
                                                                                     \n\
# Product ID & Product Version                                                       \n\
# Per spec DID v1.3 0xJJMN for version is interpreted as JJ.M.N                      \n\
# JJ: major version number, M: minor version number, N: sub-minor version number     \n\
# For example: 1200, v14.3.6                                                         \n\
productId = 0x1200                                                                   \n\
version = 0x1111                                                                     \n\
                                                                                     \n\
# Optional attributes                                                                \n\
#clientExecutableURL =                                                               \n\
#serviceDescription =                                                                \n\
#documentationURL =                                                                  \n\
                                                                                     \n\
# Additional optional DID records. Bluedroid supports up to 3 records.               \n\
[DID]                                                                                \n\
[DID]                                                                                \n\
version = 0x1436                                                                     \n\
                                                                                     \n\
HiSyncId = 18446744073709551615                                                      \n\
HiSyncId2 = 15001900                                                                 \n\
";

class ConfigTest : public AllocationTestHarness {
 protected:
  virtual void SetUp() {
    AllocationTestHarness::SetUp();
    FILE* fp = fopen(CONFIG_FILE, "wt");
    fwrite(CONFIG_FILE_CONTENT, 1, sizeof(CONFIG_FILE_CONTENT), fp);
    fclose(fp);
  }
};

TEST_F(ConfigTest, config_new_empty) {
  std::unique_ptr<config_t> config = config_new_empty();
  EXPECT_TRUE(config.get() != NULL);
}

TEST_F(ConfigTest, config_new_no_file) {
  std::unique_ptr<config_t> config = config_new("/meow");
  EXPECT_TRUE(config.get() == NULL);
}

TEST_F(ConfigTest, config_new) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config.get() != NULL);
}

TEST_F(ConfigTest, config_new_clone) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  std::unique_ptr<config_t> clone = config_new_clone(*config);

  config_set_string(clone.get(), CONFIG_DEFAULT_SECTION, "first_key",
                    "not_value");

  std::string one = std::string("one");
  EXPECT_STRNE(
      config_get_string(*config, CONFIG_DEFAULT_SECTION, "first_key", &one)
          ->c_str(),
      config_get_string(*clone, CONFIG_DEFAULT_SECTION, "first_key", &one)
          ->c_str());
}

TEST_F(ConfigTest, config_has_section) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config_has_section(*config, "DID"));
}

TEST_F(ConfigTest, config_has_key_in_default_section) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config_has_key(*config, CONFIG_DEFAULT_SECTION, "first_key"));
  EXPECT_STREQ(
      config_get_string(*config, CONFIG_DEFAULT_SECTION, "first_key", nullptr)
          ->c_str(),
      "value");
}

TEST_F(ConfigTest, config_has_keys) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config_has_key(*config, "DID", "recordNumber"));
  EXPECT_TRUE(config_has_key(*config, "DID", "primaryRecord"));
  EXPECT_TRUE(config_has_key(*config, "DID", "productId"));
  EXPECT_TRUE(config_has_key(*config, "DID", "version"));
}

TEST_F(ConfigTest, config_no_bad_keys) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_FALSE(config_has_key(*config, "DID_BAD", "primaryRecord"));
  EXPECT_FALSE(config_has_key(*config, "DID", "primaryRecord_BAD"));
  EXPECT_FALSE(
      config_has_key(*config, CONFIG_DEFAULT_SECTION, "primaryRecord"));
}

TEST_F(ConfigTest, config_get_int_version) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_int(*config, "DID", "version", 0), 0x1436);
}

TEST_F(ConfigTest, config_get_int_default) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_int(*config, "DID", "primaryRecord", 123), 123);
}

TEST_F(ConfigTest, config_get_uint64) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_uint64(*config, "DID", "HiSyncId", 0),
            0xFFFFFFFFFFFFFFFF);
  EXPECT_EQ(config_get_uint64(*config, "DID", "HiSyncId2", 0),
            uint64_t(15001900));
}

TEST_F(ConfigTest, config_get_uint64_default) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_uint64(*config, "DID", "primaryRecord", 123),
            uint64_t(123));
}

TEST_F(ConfigTest, config_remove_section) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config_remove_section(config.get(), "DID"));
  EXPECT_FALSE(config_has_section(*config, "DID"));
  EXPECT_FALSE(config_has_key(*config, "DID", "productId"));
}

TEST_F(ConfigTest, config_remove_section_missing) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_FALSE(config_remove_section(config.get(), "not a section"));
}

TEST_F(ConfigTest, config_remove_key) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_int(*config, "DID", "productId", 999), 0x1200);
  EXPECT_TRUE(config_remove_key(config.get(), "DID", "productId"));
  EXPECT_FALSE(config_has_key(*config, "DID", "productId"));
}

TEST_F(ConfigTest, config_remove_key_missing) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_EQ(config_get_int(*config, "DID", "productId", 999), 0x1200);
  EXPECT_TRUE(config_remove_key(config.get(), "DID", "productId"));
  EXPECT_EQ(config_get_int(*config, "DID", "productId", 999), 999);
}

TEST_F(ConfigTest, config_save_basic) {
  std::unique_ptr<config_t> config = config_new(CONFIG_FILE);
  EXPECT_TRUE(config_save(*config, CONFIG_FILE));
}

TEST_F(ConfigTest, checksum_read) {
  std::string filename = "/data/misc/bluedroid/test.checksum";
  std::string checksum = "0x1234";
  base::FilePath file_path(filename);

  EXPECT_EQ(base::WriteFile(file_path, checksum.data(), checksum.size()),
            (int)checksum.size());

  EXPECT_EQ(checksum_read(filename.c_str()), checksum.c_str());
}

TEST_F(ConfigTest, checksum_save) {
  std::string filename = "/data/misc/bluedroid/test.checksum";
  std::string checksum = "0x1234";
  base::FilePath file_path(filename);

  EXPECT_TRUE(checksum_save(checksum, filename));

  EXPECT_TRUE(base::PathExists(file_path));
}
