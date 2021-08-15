/*
 * Copyright 2018 The Android Open Source Project
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

#include <compositionengine/impl/CompositionEngine.h>
#include <gtest/gtest.h>
#include <renderengine/mock/RenderEngine.h>

#include "MockHWComposer.h"

namespace android::compositionengine {
namespace {

using ::testing::StrictMock;

class CompositionEngineTest : public testing::Test {
public:
    ~CompositionEngineTest() override;
    mock::HWComposer* mHwc = new StrictMock<mock::HWComposer>();
    renderengine::mock::RenderEngine* mRenderEngine =
            new StrictMock<renderengine::mock::RenderEngine>();
    impl::CompositionEngine mEngine;
};

CompositionEngineTest::~CompositionEngineTest() = default;

TEST_F(CompositionEngineTest, canInstantiateCompositionEngine) {
    auto engine = impl::createCompositionEngine();
    EXPECT_TRUE(engine.get() != nullptr);
}

TEST_F(CompositionEngineTest, canSetHWComposer) {
    mEngine.setHwComposer(std::unique_ptr<android::HWComposer>(mHwc));

    EXPECT_EQ(mHwc, &mEngine.getHwComposer());
}

TEST_F(CompositionEngineTest, canSetRenderEngine) {
    mEngine.setRenderEngine(std::unique_ptr<renderengine::RenderEngine>(mRenderEngine));

    EXPECT_EQ(mRenderEngine, &mEngine.getRenderEngine());
}

} // namespace
} // namespace android::compositionengine
