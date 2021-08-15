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

#include "update_engine/payload_consumer/filesystem_verifier_action.h"

#include <memory>
#include <string>
#include <utility>

#include <base/bind.h>
#include <base/posix/eintr_wrapper.h>
#include <brillo/message_loops/fake_message_loop.h>
#include <brillo/message_loops/message_loop_utils.h>
#include <brillo/secure_blob.h>
#include <gtest/gtest.h>

#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"

using brillo::MessageLoop;
using std::string;

namespace chromeos_update_engine {

class FilesystemVerifierActionTest : public ::testing::Test {
 protected:
  void SetUp() override { loop_.SetAsCurrent(); }

  void TearDown() override {
    EXPECT_EQ(0, brillo::MessageLoopRunMaxIterations(&loop_, 1));
  }

  // Returns true iff test has completed successfully.
  bool DoTest(bool terminate_early, bool hash_fail);

  void BuildActions(const InstallPlan& install_plan);

  brillo::FakeMessageLoop loop_{nullptr};
  ActionProcessor processor_;
};

class FilesystemVerifierActionTestDelegate : public ActionProcessorDelegate {
 public:
  FilesystemVerifierActionTestDelegate()
      : ran_(false), code_(ErrorCode::kError) {}

  void ProcessingDone(const ActionProcessor* processor, ErrorCode code) {
    MessageLoop::current()->BreakLoop();
  }
  void ProcessingStopped(const ActionProcessor* processor) {
    MessageLoop::current()->BreakLoop();
  }
  void ActionCompleted(ActionProcessor* processor,
                       AbstractAction* action,
                       ErrorCode code) {
    if (action->Type() == FilesystemVerifierAction::StaticType()) {
      ran_ = true;
      code_ = code;
      EXPECT_FALSE(static_cast<FilesystemVerifierAction*>(action)->src_stream_);
    } else if (action->Type() ==
               ObjectCollectorAction<InstallPlan>::StaticType()) {
      auto collector_action =
          static_cast<ObjectCollectorAction<InstallPlan>*>(action);
      install_plan_.reset(new InstallPlan(collector_action->object()));
    }
  }
  bool ran() const { return ran_; }
  ErrorCode code() const { return code_; }

  std::unique_ptr<InstallPlan> install_plan_;

 private:
  bool ran_;
  ErrorCode code_;
};

bool FilesystemVerifierActionTest::DoTest(bool terminate_early,
                                          bool hash_fail) {
  test_utils::ScopedTempFile a_loop_file("a_loop_file.XXXXXX");

  // Make random data for a.
  const size_t kLoopFileSize = 10 * 1024 * 1024 + 512;
  brillo::Blob a_loop_data(kLoopFileSize);
  test_utils::FillWithData(&a_loop_data);

  // Write data to disk
  if (!(test_utils::WriteFileVector(a_loop_file.path(), a_loop_data))) {
    ADD_FAILURE();
    return false;
  }

  // Attach loop devices to the files
  string a_dev;
  test_utils::ScopedLoopbackDeviceBinder a_dev_releaser(
      a_loop_file.path(), false, &a_dev);
  if (!(a_dev_releaser.is_bound())) {
    ADD_FAILURE();
    return false;
  }

  LOG(INFO) << "verifying: " << a_loop_file.path() << " (" << a_dev << ")";

  bool success = true;

  // Set up the action objects
  InstallPlan install_plan;
  install_plan.source_slot = 0;
  install_plan.target_slot = 1;
  InstallPlan::Partition part;
  part.name = "part";
  part.target_size = kLoopFileSize - (hash_fail ? 1 : 0);
  part.target_path = a_dev;
  if (!HashCalculator::RawHashOfData(a_loop_data, &part.target_hash)) {
    ADD_FAILURE();
    success = false;
  }
  part.source_size = kLoopFileSize;
  part.source_path = a_dev;
  if (!HashCalculator::RawHashOfData(a_loop_data, &part.source_hash)) {
    ADD_FAILURE();
    success = false;
  }
  install_plan.partitions = {part};

  BuildActions(install_plan);

  FilesystemVerifierActionTestDelegate delegate;
  processor_.set_delegate(&delegate);

  loop_.PostTask(FROM_HERE,
                 base::Bind(
                     [](ActionProcessor* processor, bool terminate_early) {
                       processor->StartProcessing();
                       if (terminate_early) {
                         processor->StopProcessing();
                       }
                     },
                     base::Unretained(&processor_),
                     terminate_early));
  loop_.Run();

  if (!terminate_early) {
    bool is_delegate_ran = delegate.ran();
    EXPECT_TRUE(is_delegate_ran);
    success = success && is_delegate_ran;
  } else {
    EXPECT_EQ(ErrorCode::kError, delegate.code());
    return (ErrorCode::kError == delegate.code());
  }
  if (hash_fail) {
    ErrorCode expected_exit_code = ErrorCode::kNewRootfsVerificationError;
    EXPECT_EQ(expected_exit_code, delegate.code());
    return (expected_exit_code == delegate.code());
  }
  EXPECT_EQ(ErrorCode::kSuccess, delegate.code());

  // Make sure everything in the out_image is there
  brillo::Blob a_out;
  if (!utils::ReadFile(a_dev, &a_out)) {
    ADD_FAILURE();
    return false;
  }
  const bool is_a_file_reading_eq =
      test_utils::ExpectVectorsEq(a_loop_data, a_out);
  EXPECT_TRUE(is_a_file_reading_eq);
  success = success && is_a_file_reading_eq;

  bool is_install_plan_eq = (*delegate.install_plan_ == install_plan);
  EXPECT_TRUE(is_install_plan_eq);
  success = success && is_install_plan_eq;
  return success;
}

void FilesystemVerifierActionTest::BuildActions(
    const InstallPlan& install_plan) {
  auto feeder_action = std::make_unique<ObjectFeederAction<InstallPlan>>();
  auto verifier_action = std::make_unique<FilesystemVerifierAction>();
  auto collector_action =
      std::make_unique<ObjectCollectorAction<InstallPlan>>();

  feeder_action->set_obj(install_plan);

  BondActions(feeder_action.get(), verifier_action.get());
  BondActions(verifier_action.get(), collector_action.get());

  processor_.EnqueueAction(std::move(feeder_action));
  processor_.EnqueueAction(std::move(verifier_action));
  processor_.EnqueueAction(std::move(collector_action));
}

class FilesystemVerifierActionTest2Delegate : public ActionProcessorDelegate {
 public:
  void ActionCompleted(ActionProcessor* processor,
                       AbstractAction* action,
                       ErrorCode code) {
    if (action->Type() == FilesystemVerifierAction::StaticType()) {
      ran_ = true;
      code_ = code;
    }
  }
  bool ran_;
  ErrorCode code_;
};

TEST_F(FilesystemVerifierActionTest, MissingInputObjectTest) {
  auto copier_action = std::make_unique<FilesystemVerifierAction>();
  auto collector_action =
      std::make_unique<ObjectCollectorAction<InstallPlan>>();

  BondActions(copier_action.get(), collector_action.get());

  processor_.EnqueueAction(std::move(copier_action));
  processor_.EnqueueAction(std::move(collector_action));

  FilesystemVerifierActionTest2Delegate delegate;
  processor_.set_delegate(&delegate);

  processor_.StartProcessing();
  EXPECT_FALSE(processor_.IsRunning());
  EXPECT_TRUE(delegate.ran_);
  EXPECT_EQ(ErrorCode::kError, delegate.code_);
}

TEST_F(FilesystemVerifierActionTest, NonExistentDriveTest) {
  InstallPlan install_plan;
  InstallPlan::Partition part;
  part.name = "nope";
  part.source_path = "/no/such/file";
  part.target_path = "/no/such/file";
  install_plan.partitions = {part};

  BuildActions(install_plan);

  FilesystemVerifierActionTest2Delegate delegate;
  processor_.set_delegate(&delegate);

  processor_.StartProcessing();
  EXPECT_FALSE(processor_.IsRunning());
  EXPECT_TRUE(delegate.ran_);
  EXPECT_EQ(ErrorCode::kFilesystemVerifierError, delegate.code_);
}

TEST_F(FilesystemVerifierActionTest, RunAsRootVerifyHashTest) {
  ASSERT_EQ(0U, getuid());
  EXPECT_TRUE(DoTest(false, false));
}

TEST_F(FilesystemVerifierActionTest, RunAsRootVerifyHashFailTest) {
  ASSERT_EQ(0U, getuid());
  EXPECT_TRUE(DoTest(false, true));
}

TEST_F(FilesystemVerifierActionTest, RunAsRootTerminateEarlyTest) {
  ASSERT_EQ(0U, getuid());
  EXPECT_TRUE(DoTest(true, false));
  // TerminateEarlyTest may leak some null callbacks from the Stream class.
  while (loop_.RunOnce(false)) {
  }
}

#ifdef __ANDROID__
TEST_F(FilesystemVerifierActionTest, RunAsRootWriteVerityTest) {
  test_utils::ScopedTempFile part_file("part_file.XXXXXX");
  constexpr size_t filesystem_size = 200 * 4096;
  constexpr size_t part_size = 256 * 4096;
  brillo::Blob part_data(filesystem_size, 0x1);
  part_data.resize(part_size);
  ASSERT_TRUE(test_utils::WriteFileVector(part_file.path(), part_data));
  string target_path;
  test_utils::ScopedLoopbackDeviceBinder target_device(
      part_file.path(), true, &target_path);

  InstallPlan install_plan;
  InstallPlan::Partition part;
  part.name = "part";
  part.target_path = target_path;
  part.target_size = part_size;
  part.block_size = 4096;
  part.hash_tree_algorithm = "sha1";
  part.hash_tree_data_offset = 0;
  part.hash_tree_data_size = filesystem_size;
  part.hash_tree_offset = filesystem_size;
  part.hash_tree_size = 3 * 4096;
  part.fec_data_offset = 0;
  part.fec_data_size = filesystem_size + part.hash_tree_size;
  part.fec_offset = part.fec_data_size;
  part.fec_size = 2 * 4096;
  part.fec_roots = 2;
  // for i in {1..$((200 * 4096))}; do echo -n -e '\x1' >> part; done
  // avbtool add_hashtree_footer --image part --partition_size $((256 * 4096))
  //     --partition_name part --do_not_append_vbmeta_image
  //     --output_vbmeta_image vbmeta
  // truncate -s $((256 * 4096)) part
  // sha256sum part | xxd -r -p | hexdump -v -e '/1 "0x%02x, "'
  part.target_hash = {0x28, 0xd4, 0x96, 0x75, 0x4c, 0xf5, 0x8a, 0x3e,
                      0x31, 0x85, 0x08, 0x92, 0x85, 0x62, 0xf0, 0x37,
                      0xbc, 0x8d, 0x7e, 0xa4, 0xcb, 0x24, 0x18, 0x7b,
                      0xf3, 0xeb, 0xb5, 0x8d, 0x6f, 0xc8, 0xd8, 0x1a};
  // avbtool info_image --image vbmeta | grep Salt | cut -d':' -f 2 |
  //     xxd -r -p | hexdump -v -e '/1 "0x%02x, "'
  part.hash_tree_salt = {0x9e, 0xcb, 0xf8, 0xd5, 0x0b, 0xb4, 0x43,
                         0x0a, 0x7a, 0x10, 0xad, 0x96, 0xd7, 0x15,
                         0x70, 0xba, 0xed, 0x27, 0xe2, 0xae};
  install_plan.partitions = {part};

  BuildActions(install_plan);

  FilesystemVerifierActionTestDelegate delegate;
  processor_.set_delegate(&delegate);

  loop_.PostTask(
      FROM_HERE,
      base::Bind(
          [](ActionProcessor* processor) { processor->StartProcessing(); },
          base::Unretained(&processor_)));
  loop_.Run();

  EXPECT_FALSE(processor_.IsRunning());
  EXPECT_TRUE(delegate.ran());
  EXPECT_EQ(ErrorCode::kSuccess, delegate.code());
}
#endif  // __ANDROID__

TEST_F(FilesystemVerifierActionTest, RunAsRootSkipWriteVerityTest) {
  test_utils::ScopedTempFile part_file("part_file.XXXXXX");
  constexpr size_t filesystem_size = 200 * 4096;
  constexpr size_t part_size = 256 * 4096;
  brillo::Blob part_data(part_size);
  test_utils::FillWithData(&part_data);
  ASSERT_TRUE(test_utils::WriteFileVector(part_file.path(), part_data));
  string target_path;
  test_utils::ScopedLoopbackDeviceBinder target_device(
      part_file.path(), true, &target_path);

  InstallPlan install_plan;
  install_plan.write_verity = false;
  InstallPlan::Partition part;
  part.name = "part";
  part.target_path = target_path;
  part.target_size = part_size;
  part.block_size = 4096;
  part.hash_tree_data_offset = 0;
  part.hash_tree_data_size = filesystem_size;
  part.hash_tree_offset = filesystem_size;
  part.hash_tree_size = 3 * 4096;
  part.fec_data_offset = 0;
  part.fec_data_size = filesystem_size + part.hash_tree_size;
  part.fec_offset = part.fec_data_size;
  part.fec_size = 2 * 4096;
  EXPECT_TRUE(HashCalculator::RawHashOfData(part_data, &part.target_hash));
  install_plan.partitions = {part};

  BuildActions(install_plan);

  FilesystemVerifierActionTestDelegate delegate;
  processor_.set_delegate(&delegate);

  loop_.PostTask(
      FROM_HERE,
      base::Bind(
          [](ActionProcessor* processor) { processor->StartProcessing(); },
          base::Unretained(&processor_)));
  loop_.Run();

  EXPECT_FALSE(processor_.IsRunning());
  EXPECT_TRUE(delegate.ran());
  EXPECT_EQ(ErrorCode::kSuccess, delegate.code());
}
}  // namespace chromeos_update_engine
