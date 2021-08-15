// clang-format off
// Generated file (from: tile_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace tile_2 {
// Generated tile_2 test
#include "generated/examples/tile_2.example.cpp"
// Generated model constructor
#include "generated/models/tile_2.model.cpp"
} // namespace tile_2

TEST_F(GeneratedTests, tile_2) {
    execute(tile_2::CreateModel,
            tile_2::is_ignored,
            tile_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, tile_2, tile_2::CreateModel)

TEST_F(GeneratedTests, tile_2_relaxed) {
    execute(tile_2::CreateModel_relaxed,
            tile_2::is_ignored_relaxed,
            tile_2::get_examples_relaxed());
}

TEST_F(GeneratedTests, tile_2_float16) {
    execute(tile_2::CreateModel_float16,
            tile_2::is_ignored_float16,
            tile_2::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, tile_2_float16, tile_2::CreateModel_float16)

TEST_F(GeneratedTests, tile_2_quant8) {
    execute(tile_2::CreateModel_quant8,
            tile_2::is_ignored_quant8,
            tile_2::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, tile_2_quant8, tile_2::CreateModel_quant8)

TEST_F(GeneratedTests, tile_2_int32) {
    execute(tile_2::CreateModel_int32,
            tile_2::is_ignored_int32,
            tile_2::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, tile_2_int32, tile_2::CreateModel_int32)

TEST_F(DynamicOutputShapeTest, tile_2_dynamic_output_shape) {
    execute(tile_2::CreateModel_dynamic_output_shape,
            tile_2::is_ignored_dynamic_output_shape,
            tile_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, tile_2_dynamic_output_shape_relaxed) {
    execute(tile_2::CreateModel_dynamic_output_shape_relaxed,
            tile_2::is_ignored_dynamic_output_shape_relaxed,
            tile_2::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, tile_2_dynamic_output_shape_float16) {
    execute(tile_2::CreateModel_dynamic_output_shape_float16,
            tile_2::is_ignored_dynamic_output_shape_float16,
            tile_2::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, tile_2_dynamic_output_shape_quant8) {
    execute(tile_2::CreateModel_dynamic_output_shape_quant8,
            tile_2::is_ignored_dynamic_output_shape_quant8,
            tile_2::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, tile_2_dynamic_output_shape_int32) {
    execute(tile_2::CreateModel_dynamic_output_shape_int32,
            tile_2::is_ignored_dynamic_output_shape_int32,
            tile_2::get_examples_dynamic_output_shape_int32());
}

