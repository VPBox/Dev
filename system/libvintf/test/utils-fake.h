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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gmock/gmock.h>

#include <vintf/ObjectFactory.h>
#include <vintf/PropertyFetcher.h>
#include "utils.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Return;

namespace android {
namespace vintf {
namespace details {

class MockFileSystem : public FileSystem {
   public:
    MockFileSystem() {}

    MOCK_CONST_METHOD2(fetch, status_t(const std::string& path, std::string& fetched));
    MOCK_CONST_METHOD3(listFiles,
                       status_t(const std::string&, std::vector<std::string>*, std::string*));

    status_t fetch(const std::string& path, std::string* fetched, std::string*) const override {
        // Call the mocked function
        return fetch(path, *fetched);
    }
   private:
    FileSystemImpl mImpl;
};

class MockRuntimeInfo : public RuntimeInfo {
   public:
    MockRuntimeInfo() {
        ON_CALL(*this, fetchAllInformation(_))
            .WillByDefault(Invoke(this, &MockRuntimeInfo::doFetch));
    }
    MOCK_METHOD1(fetchAllInformation, status_t(RuntimeInfo::FetchFlags));
    status_t doFetch(RuntimeInfo::FetchFlags flags);
    void failNextFetch() { failNextFetch_ = true; }

   private:
    bool failNextFetch_ = false;
};
class MockRuntimeInfoFactory : public ObjectFactory<RuntimeInfo> {
   public:
    MockRuntimeInfoFactory(const std::shared_ptr<MockRuntimeInfo>& info) { object_ = info; }
    std::shared_ptr<RuntimeInfo> make_shared() const override { return object_; }
    std::shared_ptr<MockRuntimeInfo> getInfo() const { return object_; }

   private:
    std::shared_ptr<MockRuntimeInfo> object_;
};

class MockPropertyFetcher : public PropertyFetcher {
   public:
    MockPropertyFetcher() = default;
    MOCK_CONST_METHOD2(getProperty, std::string(const std::string&, const std::string&));
    MOCK_CONST_METHOD2(getBoolProperty, bool(const std::string&, bool));
    MOCK_CONST_METHOD3(getUintProperty, uint64_t(const std::string&, uint64_t, uint64_t));
};

}  // namespace details
}  // namespace vintf
}  // namespace android
