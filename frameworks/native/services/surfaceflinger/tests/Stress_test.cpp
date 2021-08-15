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

#include <gtest/gtest.h>

#include <gui/SurfaceComposerClient.h>

#include <utils/String8.h>

#include <thread>
#include <functional>
#include <layerproto/LayerProtoParser.h>

namespace android {

TEST(SurfaceFlingerStress, create_and_destroy) {
    auto do_stress = []() {
        sp<SurfaceComposerClient> client = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, client->initCheck());
        for (int j = 0; j < 1000; j++) {
            auto surf = client->createSurface(String8("t"), 100, 100,
                    PIXEL_FORMAT_RGBA_8888, 0);
            ASSERT_TRUE(surf != nullptr);
            surf.clear();
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread(do_stress));
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

surfaceflinger::LayersProto generateLayerProto() {
    surfaceflinger::LayersProto layersProto;
    std::array<surfaceflinger::LayerProto*, 10> layers = {};
    for (size_t i = 0; i < layers.size(); ++i) {
        layers[i] = layersProto.add_layers();
        layers[i]->set_id(i);
    }

    layers[0]->add_children(1);
    layers[1]->set_parent(0);
    layers[0]->add_children(2);
    layers[2]->set_parent(0);
    layers[0]->add_children(3);
    layers[3]->set_parent(0);
    layers[2]->add_children(4);
    layers[4]->set_parent(2);
    layers[3]->add_children(5);
    layers[5]->set_parent(3);
    layers[5]->add_children(6);
    layers[6]->set_parent(5);
    layers[5]->add_children(7);
    layers[7]->set_parent(5);
    layers[6]->add_children(8);
    layers[8]->set_parent(6);

    layers[4]->set_z_order_relative_of(3);
    layers[3]->add_relatives(4);
    layers[8]->set_z_order_relative_of(9);
    layers[9]->add_relatives(8);
    layers[3]->set_z_order_relative_of(1);
    layers[1]->add_relatives(3);

/* ----------------------------
 *       - 0 -      - 9 -
 *      /  |  \
 *     1   2   3(1)
 *         |    |
 *         4(3) 5
 *             / \
 *            6   7
 *            |
 *            8(9)
 * -------------------------- */

    return layersProto;
}

TEST(LayerProtoStress, mem_info) {
    std::string cmd = "dumpsys meminfo ";
    cmd += std::to_string(getpid());
    system(cmd.c_str());
    for (int i = 0; i < 100000; i++) {
        surfaceflinger::LayersProto layersProto = generateLayerProto();
        auto layerTree = surfaceflinger::LayerProtoParser::generateLayerTree(layersProto);
        surfaceflinger::LayerProtoParser::layerTreeToString(layerTree);
    }
    system(cmd.c_str());
}

}
