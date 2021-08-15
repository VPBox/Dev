/*
 * Copyright 2019 The Android Open Source Project
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

#include <compositionengine/LayerCreationArgs.h>
#include <compositionengine/impl/Layer.h>
#include <compositionengine/mock/CompositionEngine.h>
#include <compositionengine/mock/LayerFE.h>

namespace android::compositionengine {
namespace {

using testing::StrictMock;

class LayerTest : public testing::Test {
public:
    ~LayerTest() override = default;

    StrictMock<mock::CompositionEngine> mCompositionEngine;
    sp<LayerFE> mLayerFE = new StrictMock<mock::LayerFE>();
    impl::Layer mLayer{mCompositionEngine, LayerCreationArgs{mLayerFE}};
};

/* ------------------------------------------------------------------------
 * Basic construction
 */

TEST_F(LayerTest, canInstantiateLayer) {}

} // namespace
} // namespace android::compositionengine
