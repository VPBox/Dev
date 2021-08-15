// clang-format off
// Generated file (from: resize_nearest_neighbor.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace resize_nearest_neighbor {
// Generated resize_nearest_neighbor test
#include "generated/examples/resize_nearest_neighbor.example.cpp"
// Generated model constructor
#include "generated/models/resize_nearest_neighbor.model.cpp"
} // namespace resize_nearest_neighbor

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc,
            resize_nearest_neighbor::is_ignored_shape_nhwc,
            resize_nearest_neighbor::get_examples_shape_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc, resize_nearest_neighbor::CreateModel_shape_nhwc)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16, resize_nearest_neighbor::CreateModel_shape_nhwc_float16)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw,
            resize_nearest_neighbor::is_ignored_shape_nchw,
            resize_nearest_neighbor::get_examples_shape_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw, resize_nearest_neighbor::CreateModel_shape_nchw)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8, resize_nearest_neighbor::CreateModel_shape_nchw_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16,
            resize_nearest_neighbor::get_examples_shape_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16, resize_nearest_neighbor::CreateModel_shape_nchw_float16)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc,
            resize_nearest_neighbor::is_ignored_scale_nhwc,
            resize_nearest_neighbor::get_examples_scale_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc, resize_nearest_neighbor::CreateModel_scale_nhwc)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16, resize_nearest_neighbor::CreateModel_scale_nhwc_float16)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw,
            resize_nearest_neighbor::is_ignored_scale_nchw,
            resize_nearest_neighbor::get_examples_scale_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw, resize_nearest_neighbor::CreateModel_scale_nchw)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8, resize_nearest_neighbor::CreateModel_scale_nchw_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16,
            resize_nearest_neighbor::get_examples_scale_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16, resize_nearest_neighbor::CreateModel_scale_nchw_float16)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_2,
            resize_nearest_neighbor::is_ignored_shape_nhwc_2,
            resize_nearest_neighbor::get_examples_shape_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_2, resize_nearest_neighbor::CreateModel_shape_nhwc_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_2, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_2, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_2,
            resize_nearest_neighbor::is_ignored_shape_nchw_2,
            resize_nearest_neighbor::get_examples_shape_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_2, resize_nearest_neighbor::CreateModel_shape_nchw_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_2, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_2,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_2, resize_nearest_neighbor::CreateModel_shape_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_2,
            resize_nearest_neighbor::is_ignored_scale_nhwc_2,
            resize_nearest_neighbor::get_examples_scale_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_2, resize_nearest_neighbor::CreateModel_scale_nhwc_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_2, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_2, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_2,
            resize_nearest_neighbor::is_ignored_scale_nchw_2,
            resize_nearest_neighbor::get_examples_scale_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_2, resize_nearest_neighbor::CreateModel_scale_nchw_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_2, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_2,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_2, resize_nearest_neighbor::CreateModel_scale_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_3,
            resize_nearest_neighbor::is_ignored_shape_nhwc_3,
            resize_nearest_neighbor::get_examples_shape_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_3, resize_nearest_neighbor::CreateModel_shape_nhwc_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_3,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_3,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_3, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_3,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_3,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_3, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_3,
            resize_nearest_neighbor::is_ignored_shape_nchw_3,
            resize_nearest_neighbor::get_examples_shape_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_3, resize_nearest_neighbor::CreateModel_shape_nchw_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_3,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_3,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_3,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_3,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_3, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_3,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_3,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_3, resize_nearest_neighbor::CreateModel_shape_nchw_float16_3)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_3,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_3,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_3,
            resize_nearest_neighbor::is_ignored_scale_nhwc_3,
            resize_nearest_neighbor::get_examples_scale_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_3, resize_nearest_neighbor::CreateModel_scale_nhwc_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_3,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_3,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_3,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_3,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_3, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_3,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_3,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_3, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_3,
            resize_nearest_neighbor::is_ignored_scale_nchw_3,
            resize_nearest_neighbor::get_examples_scale_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_3, resize_nearest_neighbor::CreateModel_scale_nchw_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_3,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_3,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_3,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_3,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_3, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_3)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_3,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_3,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_3, resize_nearest_neighbor::CreateModel_scale_nchw_float16_3)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_3) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_3,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_3,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_3());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_4,
            resize_nearest_neighbor::is_ignored_shape_nhwc_4,
            resize_nearest_neighbor::get_examples_shape_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_4, resize_nearest_neighbor::CreateModel_shape_nhwc_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_4,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_4,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_4, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_4,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_4,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_4, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_4,
            resize_nearest_neighbor::is_ignored_shape_nchw_4,
            resize_nearest_neighbor::get_examples_shape_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_4, resize_nearest_neighbor::CreateModel_shape_nchw_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_4,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_4,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_4,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_4,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_4, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_4,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_4,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_4, resize_nearest_neighbor::CreateModel_shape_nchw_float16_4)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_4,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_4,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_4,
            resize_nearest_neighbor::is_ignored_scale_nhwc_4,
            resize_nearest_neighbor::get_examples_scale_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_4, resize_nearest_neighbor::CreateModel_scale_nhwc_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_4,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_4,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_4,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_4,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_4, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_4,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_4,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_4, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_4,
            resize_nearest_neighbor::is_ignored_scale_nchw_4,
            resize_nearest_neighbor::get_examples_scale_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_4, resize_nearest_neighbor::CreateModel_scale_nchw_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_4,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_4,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_4,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_4,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_4, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_4)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_4,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_4,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_4, resize_nearest_neighbor::CreateModel_scale_nchw_float16_4)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_4) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_4,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_4,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_4());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_5,
            resize_nearest_neighbor::is_ignored_shape_nhwc_5,
            resize_nearest_neighbor::get_examples_shape_nhwc_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_5, resize_nearest_neighbor::CreateModel_shape_nhwc_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_5,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_5,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_5, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_5,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_5,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_5, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_5,
            resize_nearest_neighbor::is_ignored_shape_nchw_5,
            resize_nearest_neighbor::get_examples_shape_nchw_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_5, resize_nearest_neighbor::CreateModel_shape_nchw_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_5,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_5,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_5,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_5,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_5, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_5,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_5,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_5, resize_nearest_neighbor::CreateModel_shape_nchw_float16_5)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_5,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_5,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_5,
            resize_nearest_neighbor::is_ignored_scale_nhwc_5,
            resize_nearest_neighbor::get_examples_scale_nhwc_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_5, resize_nearest_neighbor::CreateModel_scale_nhwc_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_5,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_5,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_5,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_5,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_5, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_5,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_5,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_5, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_5,
            resize_nearest_neighbor::is_ignored_scale_nchw_5,
            resize_nearest_neighbor::get_examples_scale_nchw_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_5, resize_nearest_neighbor::CreateModel_scale_nchw_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_5,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_5,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_5,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_5,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_5, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_5)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_5,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_5,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_5, resize_nearest_neighbor::CreateModel_scale_nchw_float16_5)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_5());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_5) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_5,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_5,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_5());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_6,
            resize_nearest_neighbor::is_ignored_shape_nhwc_6,
            resize_nearest_neighbor::get_examples_shape_nhwc_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_6, resize_nearest_neighbor::CreateModel_shape_nhwc_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_6,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_6,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_6, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_6,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_6,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_6, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_6,
            resize_nearest_neighbor::is_ignored_shape_nchw_6,
            resize_nearest_neighbor::get_examples_shape_nchw_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_6, resize_nearest_neighbor::CreateModel_shape_nchw_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_6,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_6,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_6,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_6,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_6, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_6,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_6,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_6, resize_nearest_neighbor::CreateModel_shape_nchw_float16_6)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_6,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_6,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_6,
            resize_nearest_neighbor::is_ignored_scale_nhwc_6,
            resize_nearest_neighbor::get_examples_scale_nhwc_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_6, resize_nearest_neighbor::CreateModel_scale_nhwc_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_6,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_6,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_6,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_6,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_6, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_6,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_6,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_6, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_6,
            resize_nearest_neighbor::is_ignored_scale_nchw_6,
            resize_nearest_neighbor::get_examples_scale_nchw_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_6, resize_nearest_neighbor::CreateModel_scale_nchw_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_6,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_6,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_6,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_6,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_6, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_6)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_6,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_6,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_6());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_6, resize_nearest_neighbor::CreateModel_scale_nchw_float16_6)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_6());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_6) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_6,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_6,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_6());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_7,
            resize_nearest_neighbor::is_ignored_shape_nhwc_7,
            resize_nearest_neighbor::get_examples_shape_nhwc_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_7, resize_nearest_neighbor::CreateModel_shape_nhwc_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_7,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_7,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_7, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_7,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_7,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_7, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_7,
            resize_nearest_neighbor::is_ignored_shape_nchw_7,
            resize_nearest_neighbor::get_examples_shape_nchw_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_7, resize_nearest_neighbor::CreateModel_shape_nchw_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_7,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_7,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_7,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_7,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_7, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_7,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_7,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_7, resize_nearest_neighbor::CreateModel_shape_nchw_float16_7)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_7,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_7,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_7,
            resize_nearest_neighbor::is_ignored_scale_nhwc_7,
            resize_nearest_neighbor::get_examples_scale_nhwc_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_7, resize_nearest_neighbor::CreateModel_scale_nhwc_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_7,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_7,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_7,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_7,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_7, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_7,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_7,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_7, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_7,
            resize_nearest_neighbor::is_ignored_scale_nchw_7,
            resize_nearest_neighbor::get_examples_scale_nchw_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_7, resize_nearest_neighbor::CreateModel_scale_nchw_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_7,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_7,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_7,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_7,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_7, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_7)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_7,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_7,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_7());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_7, resize_nearest_neighbor::CreateModel_scale_nchw_float16_7)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_7());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_7) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_7,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_7,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_7());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_8,
            resize_nearest_neighbor::is_ignored_shape_nhwc_8,
            resize_nearest_neighbor::get_examples_shape_nhwc_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_8, resize_nearest_neighbor::CreateModel_shape_nhwc_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::is_ignored_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::get_examples_shape_nhwc_relaxed_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_8,
            resize_nearest_neighbor::is_ignored_shape_nhwc_quant8_8,
            resize_nearest_neighbor::get_examples_shape_nhwc_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_quant8_8, resize_nearest_neighbor::CreateModel_shape_nhwc_quant8_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nhwc_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nhwc_float16_8,
            resize_nearest_neighbor::is_ignored_shape_nhwc_float16_8,
            resize_nearest_neighbor::get_examples_shape_nhwc_float16_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nhwc_float16_8, resize_nearest_neighbor::CreateModel_shape_nhwc_float16_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_8,
            resize_nearest_neighbor::is_ignored_shape_nchw_8,
            resize_nearest_neighbor::get_examples_shape_nchw_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_8, resize_nearest_neighbor::CreateModel_shape_nchw_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_relaxed_8,
            resize_nearest_neighbor::is_ignored_shape_nchw_relaxed_8,
            resize_nearest_neighbor::get_examples_shape_nchw_relaxed_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_quant8_8,
            resize_nearest_neighbor::is_ignored_shape_nchw_quant8_8,
            resize_nearest_neighbor::get_examples_shape_nchw_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_quant8_8, resize_nearest_neighbor::CreateModel_shape_nchw_quant8_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_shape_nchw_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_nchw_float16_8,
            resize_nearest_neighbor::is_ignored_shape_nchw_float16_8,
            resize_nearest_neighbor::get_examples_shape_nchw_float16_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_shape_nchw_float16_8, resize_nearest_neighbor::CreateModel_shape_nchw_float16_8)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_relaxed_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_quant8_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_quant8_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_quant8_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nhwc_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nhwc_float16_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nhwc_float16_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nhwc_float16_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_relaxed_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_relaxed_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_relaxed_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_quant8_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_quant8_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_quant8_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_shape_dynamic_output_shape_nchw_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_shape_dynamic_output_shape_nchw_float16_8,
            resize_nearest_neighbor::is_ignored_shape_dynamic_output_shape_nchw_float16_8,
            resize_nearest_neighbor::get_examples_shape_dynamic_output_shape_nchw_float16_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_8,
            resize_nearest_neighbor::is_ignored_scale_nhwc_8,
            resize_nearest_neighbor::get_examples_scale_nhwc_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_8, resize_nearest_neighbor::CreateModel_scale_nhwc_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_relaxed_8,
            resize_nearest_neighbor::is_ignored_scale_nhwc_relaxed_8,
            resize_nearest_neighbor::get_examples_scale_nhwc_relaxed_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_8,
            resize_nearest_neighbor::is_ignored_scale_nhwc_quant8_8,
            resize_nearest_neighbor::get_examples_scale_nhwc_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_quant8_8, resize_nearest_neighbor::CreateModel_scale_nhwc_quant8_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nhwc_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nhwc_float16_8,
            resize_nearest_neighbor::is_ignored_scale_nhwc_float16_8,
            resize_nearest_neighbor::get_examples_scale_nhwc_float16_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nhwc_float16_8, resize_nearest_neighbor::CreateModel_scale_nhwc_float16_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_8,
            resize_nearest_neighbor::is_ignored_scale_nchw_8,
            resize_nearest_neighbor::get_examples_scale_nchw_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_8, resize_nearest_neighbor::CreateModel_scale_nchw_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_relaxed_8,
            resize_nearest_neighbor::is_ignored_scale_nchw_relaxed_8,
            resize_nearest_neighbor::get_examples_scale_nchw_relaxed_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_quant8_8,
            resize_nearest_neighbor::is_ignored_scale_nchw_quant8_8,
            resize_nearest_neighbor::get_examples_scale_nchw_quant8_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_quant8_8, resize_nearest_neighbor::CreateModel_scale_nchw_quant8_8)

TEST_F(GeneratedTests, resize_nearest_neighbor_scale_nchw_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_nchw_float16_8,
            resize_nearest_neighbor::is_ignored_scale_nchw_float16_8,
            resize_nearest_neighbor::get_examples_scale_nchw_float16_8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_scale_nchw_float16_8, resize_nearest_neighbor::CreateModel_scale_nchw_float16_8)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_relaxed_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_relaxed_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_quant8_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_quant8_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_quant8_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nhwc_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nhwc_float16_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nhwc_float16_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nhwc_float16_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_relaxed_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_relaxed_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_relaxed_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_relaxed_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_quant8_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_quant8_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_quant8_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_quant8_8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_scale_dynamic_output_shape_nchw_float16_8) {
    execute(resize_nearest_neighbor::CreateModel_scale_dynamic_output_shape_nchw_float16_8,
            resize_nearest_neighbor::is_ignored_scale_dynamic_output_shape_nchw_float16_8,
            resize_nearest_neighbor::get_examples_scale_dynamic_output_shape_nchw_float16_8());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc, resize_nearest_neighbor::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_quant8,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc_quant8, resize_nearest_neighbor::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_float16,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_float16,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc_float16, resize_nearest_neighbor::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw,
            resize_nearest_neighbor::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw, resize_nearest_neighbor::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_relaxed,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_quant8,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_quant8,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw_quant8, resize_nearest_neighbor::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_float16,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_float16,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw_float16, resize_nearest_neighbor::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_2,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc_2, resize_nearest_neighbor::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc_quant8_2, resize_nearest_neighbor::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nhwc_float16_2, resize_nearest_neighbor::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_2,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw_2, resize_nearest_neighbor::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw_quant8_2, resize_nearest_neighbor::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, resize_nearest_neighbor_zero_sized_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_zero_sized_nchw_float16_2,
            resize_nearest_neighbor::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, resize_nearest_neighbor_zero_sized_nchw_float16_2, resize_nearest_neighbor::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, resize_nearest_neighbor_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(resize_nearest_neighbor::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            resize_nearest_neighbor::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

