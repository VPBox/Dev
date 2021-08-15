// clang-format off
// Generated file (from: tile_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace tile_1 {
// Generated tile_1 test
#include "generated/examples/tile_1.example.cpp"
// Generated model constructor
#include "generated/models/tile_1.model.cpp"
} // namespace tile_1

TEST_F(GeneratedTests, tile_1) {
    execute(tile_1::CreateModel,
            tile_1::is_ignored,
            tile_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, tile_1, tile_1::CreateModel)

TEST_F(GeneratedTests, tile_1_relaxed) {
    execute(tile_1::CreateModel_relaxed,
            tile_1::is_ignored_relaxed,
            tile_1::get_examples_relaxed());
}

TEST_F(GeneratedTests, tile_1_float16) {
    execute(tile_1::CreateModel_float16,
            tile_1::is_ignored_float16,
            tile_1::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, tile_1_float16, tile_1::CreateModel_float16)

TEST_F(GeneratedTests, tile_1_quant8) {
    execute(tile_1::CreateModel_quant8,
            tile_1::is_ignored_quant8,
            tile_1::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, tile_1_quant8, tile_1::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, tile_1_dynamic_output_shape) {
    execute(tile_1::CreateModel_dynamic_output_shape,
            tile_1::is_ignored_dynamic_output_shape,
            tile_1::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, tile_1_dynamic_output_shape_relaxed) {
    execute(tile_1::CreateModel_dynamic_output_shape_relaxed,
            tile_1::is_ignored_dynamic_output_shape_relaxed,
            tile_1::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, tile_1_dynamic_output_shape_float16) {
    execute(tile_1::CreateModel_dynamic_output_shape_float16,
            tile_1::is_ignored_dynamic_output_shape_float16,
            tile_1::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, tile_1_dynamic_output_shape_quant8) {
    execute(tile_1::CreateModel_dynamic_output_shape_quant8,
            tile_1::is_ignored_dynamic_output_shape_quant8,
            tile_1::get_examples_dynamic_output_shape_quant8());
}

