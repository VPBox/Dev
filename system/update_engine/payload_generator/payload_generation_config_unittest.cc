//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/payload_generator/payload_generation_config.h"

#include <utility>

#include <gtest/gtest.h>

namespace chromeos_update_engine {

class PayloadGenerationConfigTest : public ::testing::Test {};

TEST_F(PayloadGenerationConfigTest, SimpleLoadPostInstallConfigTest) {
  ImageConfig image_config;
  image_config.partitions.emplace_back("root");
  brillo::KeyValueStore store;
  EXPECT_TRUE(
      store.LoadFromString("RUN_POSTINSTALL_root=true\n"
                           "POSTINSTALL_PATH_root=postinstall\n"
                           "FILESYSTEM_TYPE_root=ext4\n"
                           "POSTINSTALL_OPTIONAL_root=true"));
  EXPECT_TRUE(image_config.LoadPostInstallConfig(store));
  EXPECT_FALSE(image_config.partitions[0].postinstall.IsEmpty());
  EXPECT_EQ(true, image_config.partitions[0].postinstall.run);
  EXPECT_EQ("postinstall", image_config.partitions[0].postinstall.path);
  EXPECT_EQ("ext4", image_config.partitions[0].postinstall.filesystem_type);
  EXPECT_TRUE(image_config.partitions[0].postinstall.optional);
}

TEST_F(PayloadGenerationConfigTest, LoadPostInstallConfigNameMismatchTest) {
  ImageConfig image_config;
  image_config.partitions.emplace_back("system");
  brillo::KeyValueStore store;
  EXPECT_TRUE(
      store.LoadFromString("RUN_POSTINSTALL_root=true\n"
                           "POSTINSTALL_PATH_root=postinstall\n"
                           "FILESYSTEM_TYPE_root=ext4"));
  EXPECT_FALSE(image_config.LoadPostInstallConfig(store));
  EXPECT_TRUE(image_config.partitions[0].postinstall.IsEmpty());
}

TEST_F(PayloadGenerationConfigTest, LoadDynamicPartitionMetadataTest) {
  ImageConfig image_config;
  brillo::KeyValueStore store;
  ASSERT_TRUE(
      store.LoadFromString("super_partition_groups=group_a group_b\n"
                           "group_a_size=3221225472\n"
                           "group_a_partition_list=system product_services\n"
                           "group_b_size=2147483648\n"
                           "group_b_partition_list=vendor\n"));
  EXPECT_TRUE(image_config.LoadDynamicPartitionMetadata(store));
  ASSERT_NE(nullptr, image_config.dynamic_partition_metadata);

  ASSERT_EQ(2, image_config.dynamic_partition_metadata->groups_size());

  const auto& group_a = image_config.dynamic_partition_metadata->groups(0);
  EXPECT_EQ("group_a", group_a.name());
  EXPECT_EQ(3221225472u, group_a.size());
  ASSERT_EQ(2, group_a.partition_names_size());
  EXPECT_EQ("system", group_a.partition_names(0));
  EXPECT_EQ("product_services", group_a.partition_names(1));

  const auto& group_b = image_config.dynamic_partition_metadata->groups(1);
  EXPECT_EQ("group_b", group_b.name());
  EXPECT_EQ(2147483648u, group_b.size());
  ASSERT_EQ(1, group_b.partition_names_size());
  EXPECT_EQ("vendor", group_b.partition_names(0));
}

TEST_F(PayloadGenerationConfigTest,
       LoadDynamicPartitionMetadataMissingSizeTest) {
  ImageConfig image_config;
  brillo::KeyValueStore store;
  ASSERT_TRUE(
      store.LoadFromString("super_partition_groups=foo\n"
                           "foo_partition_list=baz\n"));
  EXPECT_FALSE(image_config.LoadDynamicPartitionMetadata(store));
  EXPECT_EQ(nullptr, image_config.dynamic_partition_metadata);
}

TEST_F(PayloadGenerationConfigTest, LoadDynamicPartitionMetadataBadSizeTest) {
  ImageConfig image_config;
  brillo::KeyValueStore store;
  ASSERT_TRUE(
      store.LoadFromString("super_partition_groups=foo\n"
                           "foo_size=bar\n"
                           "foo_partition_list=baz\n"));
  EXPECT_FALSE(image_config.LoadDynamicPartitionMetadata(store));
  EXPECT_EQ(nullptr, image_config.dynamic_partition_metadata);
}

TEST_F(PayloadGenerationConfigTest, ValidateDynamicPartitionMetadata) {
  ImageConfig image_config;

  PartitionConfig system("system");
  system.size = 2147483648u;
  PartitionConfig product_services("product_services");
  product_services.size = 1073741824u;

  image_config.partitions.push_back(std::move(system));
  image_config.partitions.push_back(std::move(product_services));

  brillo::KeyValueStore store;
  ASSERT_TRUE(
      store.LoadFromString("super_partition_groups=foo\n"
                           "foo_size=3221225472\n"
                           "foo_partition_list=system product_services\n"));
  EXPECT_TRUE(image_config.LoadDynamicPartitionMetadata(store));
  EXPECT_NE(nullptr, image_config.dynamic_partition_metadata);

  EXPECT_TRUE(image_config.ValidateDynamicPartitionMetadata());
}

TEST_F(PayloadGenerationConfigTest, ValidateDynamicPartitionMetadataTooBig) {
  ImageConfig image_config;

  PartitionConfig system("system");
  system.size = 4294967296u;
  image_config.partitions.push_back(std::move(system));

  brillo::KeyValueStore store;
  ASSERT_TRUE(
      store.LoadFromString("super_partition_groups=foo\n"
                           "foo_size=3221225472\n"
                           "foo_partition_list=system\n"));
  EXPECT_TRUE(image_config.LoadDynamicPartitionMetadata(store));
  EXPECT_NE(nullptr, image_config.dynamic_partition_metadata);

  EXPECT_FALSE(image_config.ValidateDynamicPartitionMetadata());
}
}  // namespace chromeos_update_engine
