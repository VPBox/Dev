// clang-format off
// Generated file (from: transpose_conv2d_large.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_conv2d_large {
// Generated transpose_conv2d_large test
#include "generated/examples/transpose_conv2d_large.example.cpp"
// Generated model constructor
#include "generated/models/transpose_conv2d_large.model.cpp"
} // namespace transpose_conv2d_large

TEST_F(GeneratedTests, transpose_conv2d_large_quant8) {
    execute(transpose_conv2d_large::CreateModel_quant8,
            transpose_conv2d_large::is_ignored_quant8,
            transpose_conv2d_large::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_large_quant8, transpose_conv2d_large::CreateModel_quant8)

TEST_F(GeneratedTests, transpose_conv2d_large_channelQuant8) {
    execute(transpose_conv2d_large::CreateModel_channelQuant8,
            transpose_conv2d_large::is_ignored_channelQuant8,
            transpose_conv2d_large::get_examples_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_large_channelQuant8, transpose_conv2d_large::CreateModel_channelQuant8)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_large_dynamic_output_shape_quant8) {
    execute(transpose_conv2d_large::CreateModel_dynamic_output_shape_quant8,
            transpose_conv2d_large::is_ignored_dynamic_output_shape_quant8,
            transpose_conv2d_large::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_large_dynamic_output_shape_channelQuant8) {
    execute(transpose_conv2d_large::CreateModel_dynamic_output_shape_channelQuant8,
            transpose_conv2d_large::is_ignored_dynamic_output_shape_channelQuant8,
            transpose_conv2d_large::get_examples_dynamic_output_shape_channelQuant8());
}

