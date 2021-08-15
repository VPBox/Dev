// clang-format off
// Generated file (from: tile_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace tile_3 {
// Generated tile_3 test
#include "generated/examples/tile_3.example.cpp"
// Generated model constructor
#include "generated/models/tile_3.model.cpp"
} // namespace tile_3

TEST_F(GeneratedTests, tile_3) {
    execute(tile_3::CreateModel,
            tile_3::is_ignored,
            tile_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, tile_3, tile_3::CreateModel)

TEST_F(GeneratedTests, tile_3_relaxed) {
    execute(tile_3::CreateModel_relaxed,
            tile_3::is_ignored_relaxed,
            tile_3::get_examples_relaxed());
}

TEST_F(GeneratedTests, tile_3_float16) {
    execute(tile_3::CreateModel_float16,
            tile_3::is_ignored_float16,
            tile_3::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, tile_3_float16, tile_3::CreateModel_float16)

TEST_F(GeneratedTests, tile_3_quant8) {
    execute(tile_3::CreateModel_quant8,
            tile_3::is_ignored_quant8,
            tile_3::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, tile_3_quant8, tile_3::CreateModel_quant8)

TEST_F(GeneratedTests, tile_3_int32) {
    execute(tile_3::CreateModel_int32,
            tile_3::is_ignored_int32,
            tile_3::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, tile_3_int32, tile_3::CreateModel_int32)

TEST_F(DynamicOutputShapeTest, tile_3_dynamic_output_shape) {
    execute(tile_3::CreateModel_dynamic_output_shape,
            tile_3::is_ignored_dynamic_output_shape,
            tile_3::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, tile_3_dynamic_output_shape_relaxed) {
    execute(tile_3::CreateModel_dynamic_output_shape_relaxed,
            tile_3::is_ignored_dynamic_output_shape_relaxed,
            tile_3::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, tile_3_dynamic_output_shape_float16) {
    execute(tile_3::CreateModel_dynamic_output_shape_float16,
            tile_3::is_ignored_dynamic_output_shape_float16,
            tile_3::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, tile_3_dynamic_output_shape_quant8) {
    execute(tile_3::CreateModel_dynamic_output_shape_quant8,
            tile_3::is_ignored_dynamic_output_shape_quant8,
            tile_3::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, tile_3_dynamic_output_shape_int32) {
    execute(tile_3::CreateModel_dynamic_output_shape_int32,
            tile_3::is_ignored_dynamic_output_shape_int32,
            tile_3::get_examples_dynamic_output_shape_int32());
}

