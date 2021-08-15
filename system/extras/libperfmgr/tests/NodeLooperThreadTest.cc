/*
 * Copyright (C) 2017 The Android Open Source Project
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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <thread>

#include <android-base/file.h>

#include <gtest/gtest.h>

#include "perfmgr/FileNode.h"
#include "perfmgr/NodeLooperThread.h"

namespace android {
namespace perfmgr {

using namespace std::chrono_literals;

constexpr auto kSLEEP_TOLERANCE_MS = 50ms;

class NodeLooperThreadTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
        std::unique_ptr<TemporaryFile> tf = std::make_unique<TemporaryFile>();
        nodes_.emplace_back(new FileNode(
            "n0", tf->path, {{"n0_value0"}, {"n0_value1"}, {"n0_value2"}}, 2,
            false));
        files_.emplace_back(std::move(tf));
        tf = std::make_unique<TemporaryFile>();
        nodes_.emplace_back(new FileNode(
            "n1", tf->path, {{"n1_value0"}, {"n1_value1"}, {"n1_value2"}}, 2,
            true));
        files_.emplace_back(std::move(tf));
    }

    virtual void TearDown() {
        nodes_.clear();
        files_.clear();
    }
    std::vector<std::unique_ptr<Node>> nodes_;
    std::vector<std::unique_ptr<TemporaryFile>> files_;
};

static inline void _VerifyPathValue(const std::string& path,
                                    const std::string& value) {
    std::string s;
    EXPECT_TRUE(android::base::ReadFileToString(path, &s)) << strerror(errno);
    EXPECT_EQ(value, s);
}

// Test default value init
TEST_F(NodeLooperThreadTest, InitRunTest) {
    sp<NodeLooperThread> th = new NodeLooperThread(std::move(nodes_));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    EXPECT_TRUE(th->isRunning());
    _VerifyPathValue(files_[0]->path, "");
    _VerifyPathValue(files_[1]->path, "n1_value2");
    th->Stop();
    EXPECT_FALSE(th->isRunning());
}

// Test add request
TEST_F(NodeLooperThreadTest, AddRequest) {
    sp<NodeLooperThread> th = new NodeLooperThread(std::move(nodes_));
    EXPECT_TRUE(th->isRunning());
    // Dummy LAUNCH boost actions:
    // Node0, value0, 200ms
    // Node1, value1, 400ms
    std::vector<NodeAction> actions{{0, 0, 200ms}, {1, 1, 400ms}};
    EXPECT_TRUE(th->Request(actions, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    std::this_thread::sleep_for(200ms);
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    std::this_thread::sleep_for(200ms);
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value2");
    th->Stop();
    EXPECT_FALSE(th->isRunning());
}

// Test request to override expire time
TEST_F(NodeLooperThreadTest, AddRequestOverride) {
    sp<NodeLooperThread> th = new NodeLooperThread(std::move(nodes_));
    EXPECT_TRUE(th->isRunning());
    // Dummy LAUNCH boost actions:
    // Node0, value0, 200ms
    // Node1, value1, 500ms
    std::vector<NodeAction> actions{{0, 0, 200ms}, {1, 1, 500ms}};
    EXPECT_TRUE(th->Request(actions, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    // Dummy LAUNCH boost actions:
    // Node0, value0, 300ms will extend
    // Node1, value1, 100ms will not extend
    actions = std::vector<NodeAction>{{0, 0, 300ms}, {1, 1, 100ms}};
    EXPECT_TRUE(th->Request(actions, "LAUNCH"));
    std::this_thread::sleep_for(200ms);
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    std::this_thread::sleep_for(150ms);
    // Node0 value0 expired
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    std::this_thread::sleep_for(150ms);
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value2");
    th->Stop();
    EXPECT_FALSE(th->isRunning());
}

// Test cancel request
TEST_F(NodeLooperThreadTest, CancelRequest) {
    sp<NodeLooperThread> th = new NodeLooperThread(std::move(nodes_));
    EXPECT_TRUE(th->isRunning());
    // Dummy LAUNCH boost actions:
    // Node0, value0, forever
    // Node1, value1, forever
    std::vector<NodeAction> actions{{0, 0, 0ms}, {1, 1, 0ms}};
    EXPECT_TRUE(th->Request(actions, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    EXPECT_TRUE(th->Cancel(actions, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value2");
    th->Stop();
    EXPECT_FALSE(th->isRunning());
}

// Test multiple request
TEST_F(NodeLooperThreadTest, MultipleRequest) {
    sp<NodeLooperThread> th = new NodeLooperThread(std::move(nodes_));
    EXPECT_TRUE(th->isRunning());
    // Dummy LAUNCH boost actions:
    // Node0, value1, 800ms
    // Node1, value1, forever
    std::vector<NodeAction> actions_interaction{{0, 1, 800ms}, {1, 1, 0ms}};
    EXPECT_TRUE(th->Request(actions_interaction, "INTERACTION"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value1");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    // Dummy LAUNCH boost actions:
    // Node0, value0, forever
    // Node1, value0, 400ms
    std::vector<NodeAction> actions_launch{{0, 0, 0ms}, {1, 0, 400ms}};
    EXPECT_TRUE(th->Request(actions_launch, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value0");
    std::this_thread::sleep_for(400ms);
    // "LAUNCH" node1 expired
    _VerifyPathValue(files_[0]->path, "n0_value0");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    EXPECT_TRUE(th->Cancel(actions_launch, "LAUNCH"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    // "LAUNCH" canceled
    _VerifyPathValue(files_[0]->path, "n0_value1");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    std::this_thread::sleep_for(400ms);
    // "INTERACTION" node0 expired
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value1");
    EXPECT_TRUE(th->Cancel(actions_interaction, "INTERACTION"));
    std::this_thread::sleep_for(kSLEEP_TOLERANCE_MS);
    // "INTERACTION" canceled
    _VerifyPathValue(files_[0]->path, "n0_value2");
    _VerifyPathValue(files_[1]->path, "n1_value2");
    th->Stop();
    EXPECT_FALSE(th->isRunning());
}

}  // namespace perfmgr
}  // namespace android
