// clang-format off
// Generated file (from: transpose_conv2d.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_conv2d {
// Generated transpose_conv2d test
#include "generated/examples/transpose_conv2d.example.cpp"
// Generated model constructor
#include "generated/models/transpose_conv2d.model.cpp"
} // namespace transpose_conv2d

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none) {
    execute(transpose_conv2d::CreateModel_nhwc_none,
            transpose_conv2d::is_ignored_nhwc_none,
            transpose_conv2d::get_examples_nhwc_none());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none, transpose_conv2d::CreateModel_nhwc_none)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_none_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_none_weight_as_input,
            transpose_conv2d::get_examples_nhwc_none_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_weight_as_input, transpose_conv2d::CreateModel_nhwc_none_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_relaxed) {
    execute(transpose_conv2d::CreateModel_nhwc_none_relaxed,
            transpose_conv2d::is_ignored_nhwc_none_relaxed,
            transpose_conv2d::get_examples_nhwc_none_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_none_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_none_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nhwc_none_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_quant8) {
    execute(transpose_conv2d::CreateModel_nhwc_none_quant8,
            transpose_conv2d::is_ignored_nhwc_none_quant8,
            transpose_conv2d::get_examples_nhwc_none_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_quant8, transpose_conv2d::CreateModel_nhwc_none_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_none_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_none_quant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_none_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_quant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_none_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_quant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_none_quant8_2,
            transpose_conv2d::is_ignored_nhwc_none_quant8_2,
            transpose_conv2d::get_examples_nhwc_none_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_quant8_2, transpose_conv2d::CreateModel_nhwc_none_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_none_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_none_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_none_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_none_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nhwc_none_channelQuant8,
            transpose_conv2d::is_ignored_nhwc_none_channelQuant8,
            transpose_conv2d::get_examples_nhwc_none_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_channelQuant8, transpose_conv2d::CreateModel_nhwc_none_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_none_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_none_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_none_channelQuant8_2,
            transpose_conv2d::is_ignored_nhwc_none_channelQuant8_2,
            transpose_conv2d::get_examples_nhwc_none_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_channelQuant8_2, transpose_conv2d::CreateModel_nhwc_none_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_none_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_float16) {
    execute(transpose_conv2d::CreateModel_nhwc_none_float16,
            transpose_conv2d::is_ignored_nhwc_none_float16,
            transpose_conv2d::get_examples_nhwc_none_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_float16, transpose_conv2d::CreateModel_nhwc_none_float16)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_none_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_none_float16_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_none_float16_weight_as_input,
            transpose_conv2d::get_examples_nhwc_none_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_none_float16_weight_as_input, transpose_conv2d::CreateModel_nhwc_none_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu) {
    execute(transpose_conv2d::CreateModel_nhwc_relu,
            transpose_conv2d::is_ignored_nhwc_relu,
            transpose_conv2d::get_examples_nhwc_relu());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu, transpose_conv2d::CreateModel_nhwc_relu)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_relaxed) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_relaxed,
            transpose_conv2d::is_ignored_nhwc_relu_relaxed,
            transpose_conv2d::get_examples_nhwc_relu_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_quant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_quant8,
            transpose_conv2d::is_ignored_nhwc_relu_quant8,
            transpose_conv2d::get_examples_nhwc_relu_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_quant8, transpose_conv2d::CreateModel_nhwc_relu_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu_quant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_quant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_quant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_quant8_2,
            transpose_conv2d::is_ignored_nhwc_relu_quant8_2,
            transpose_conv2d::get_examples_nhwc_relu_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_quant8_2, transpose_conv2d::CreateModel_nhwc_relu_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_channelQuant8,
            transpose_conv2d::is_ignored_nhwc_relu_channelQuant8,
            transpose_conv2d::get_examples_nhwc_relu_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_channelQuant8, transpose_conv2d::CreateModel_nhwc_relu_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_2,
            transpose_conv2d::is_ignored_nhwc_relu_channelQuant8_2,
            transpose_conv2d::get_examples_nhwc_relu_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_channelQuant8_2, transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_float16) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_float16,
            transpose_conv2d::is_ignored_nhwc_relu_float16,
            transpose_conv2d::get_examples_nhwc_relu_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_float16, transpose_conv2d::CreateModel_nhwc_relu_float16)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu_float16_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu_float16_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu_float16_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1,
            transpose_conv2d::is_ignored_nhwc_relu1,
            transpose_conv2d::get_examples_nhwc_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1, transpose_conv2d::CreateModel_nhwc_relu1)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu1_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu1_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_relaxed) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_relaxed,
            transpose_conv2d::is_ignored_nhwc_relu1_relaxed,
            transpose_conv2d::get_examples_nhwc_relu1_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu1_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu1_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_quant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_quant8,
            transpose_conv2d::is_ignored_nhwc_relu1_quant8,
            transpose_conv2d::get_examples_nhwc_relu1_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_quant8, transpose_conv2d::CreateModel_nhwc_relu1_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu1_quant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu1_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_quant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_quant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_quant8_2,
            transpose_conv2d::is_ignored_nhwc_relu1_quant8_2,
            transpose_conv2d::get_examples_nhwc_relu1_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_quant8_2, transpose_conv2d::CreateModel_nhwc_relu1_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu1_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu1_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8,
            transpose_conv2d::is_ignored_nhwc_relu1_channelQuant8,
            transpose_conv2d::get_examples_nhwc_relu1_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_channelQuant8, transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu1_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_2,
            transpose_conv2d::is_ignored_nhwc_relu1_channelQuant8_2,
            transpose_conv2d::get_examples_nhwc_relu1_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_channelQuant8_2, transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu1_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_float16) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_float16,
            transpose_conv2d::is_ignored_nhwc_relu1_float16,
            transpose_conv2d::get_examples_nhwc_relu1_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_float16, transpose_conv2d::CreateModel_nhwc_relu1_float16)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu1_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu1_float16_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu1_float16_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu1_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu1_float16_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu1_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6,
            transpose_conv2d::is_ignored_nhwc_relu6,
            transpose_conv2d::get_examples_nhwc_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6, transpose_conv2d::CreateModel_nhwc_relu6)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu6_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu6_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu6_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_relaxed) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_relaxed,
            transpose_conv2d::is_ignored_nhwc_relu6_relaxed,
            transpose_conv2d::get_examples_nhwc_relu6_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu6_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu6_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_quant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_quant8,
            transpose_conv2d::is_ignored_nhwc_relu6_quant8,
            transpose_conv2d::get_examples_nhwc_relu6_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_quant8, transpose_conv2d::CreateModel_nhwc_relu6_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu6_quant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu6_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_quant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_quant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_quant8_2,
            transpose_conv2d::is_ignored_nhwc_relu6_quant8_2,
            transpose_conv2d::get_examples_nhwc_relu6_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_quant8_2, transpose_conv2d::CreateModel_nhwc_relu6_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu6_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu6_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8,
            transpose_conv2d::is_ignored_nhwc_relu6_channelQuant8,
            transpose_conv2d::get_examples_nhwc_relu6_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_channelQuant8, transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu6_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_2,
            transpose_conv2d::is_ignored_nhwc_relu6_channelQuant8_2,
            transpose_conv2d::get_examples_nhwc_relu6_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_channelQuant8_2, transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relu6_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_float16) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_float16,
            transpose_conv2d::is_ignored_nhwc_relu6_float16,
            transpose_conv2d::get_examples_nhwc_relu6_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_float16, transpose_conv2d::CreateModel_nhwc_relu6_float16)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relu6_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relu6_float16_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relu6_float16_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relu6_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_relu6_float16_weight_as_input, transpose_conv2d::CreateModel_nhwc_relu6_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none) {
    execute(transpose_conv2d::CreateModel_nchw_none,
            transpose_conv2d::is_ignored_nchw_none,
            transpose_conv2d::get_examples_nchw_none());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none, transpose_conv2d::CreateModel_nchw_none)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_none_weight_as_input,
            transpose_conv2d::is_ignored_nchw_none_weight_as_input,
            transpose_conv2d::get_examples_nchw_none_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_weight_as_input, transpose_conv2d::CreateModel_nchw_none_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_relaxed) {
    execute(transpose_conv2d::CreateModel_nchw_none_relaxed,
            transpose_conv2d::is_ignored_nchw_none_relaxed,
            transpose_conv2d::get_examples_nchw_none_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_none_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nchw_none_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nchw_none_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_quant8) {
    execute(transpose_conv2d::CreateModel_nchw_none_quant8,
            transpose_conv2d::is_ignored_nchw_none_quant8,
            transpose_conv2d::get_examples_nchw_none_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_quant8, transpose_conv2d::CreateModel_nchw_none_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_none_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_none_quant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_none_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_quant8_weight_as_input, transpose_conv2d::CreateModel_nchw_none_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_quant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_none_quant8_2,
            transpose_conv2d::is_ignored_nchw_none_quant8_2,
            transpose_conv2d::get_examples_nchw_none_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_quant8_2, transpose_conv2d::CreateModel_nchw_none_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_none_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_none_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_none_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_none_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nchw_none_channelQuant8,
            transpose_conv2d::is_ignored_nchw_none_channelQuant8,
            transpose_conv2d::get_examples_nchw_none_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_channelQuant8, transpose_conv2d::CreateModel_nchw_none_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_none_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_none_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_none_channelQuant8_2,
            transpose_conv2d::is_ignored_nchw_none_channelQuant8_2,
            transpose_conv2d::get_examples_nchw_none_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_channelQuant8_2, transpose_conv2d::CreateModel_nchw_none_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_none_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_float16) {
    execute(transpose_conv2d::CreateModel_nchw_none_float16,
            transpose_conv2d::is_ignored_nchw_none_float16,
            transpose_conv2d::get_examples_nchw_none_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_float16, transpose_conv2d::CreateModel_nchw_none_float16)

TEST_F(GeneratedTests, transpose_conv2d_nchw_none_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_none_float16_weight_as_input,
            transpose_conv2d::is_ignored_nchw_none_float16_weight_as_input,
            transpose_conv2d::get_examples_nchw_none_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_none_float16_weight_as_input, transpose_conv2d::CreateModel_nchw_none_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu) {
    execute(transpose_conv2d::CreateModel_nchw_relu,
            transpose_conv2d::is_ignored_nchw_relu,
            transpose_conv2d::get_examples_nchw_relu());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu, transpose_conv2d::CreateModel_nchw_relu)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_weight_as_input, transpose_conv2d::CreateModel_nchw_relu_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_relaxed) {
    execute(transpose_conv2d::CreateModel_nchw_relu_relaxed,
            transpose_conv2d::is_ignored_nchw_relu_relaxed,
            transpose_conv2d::get_examples_nchw_relu_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_quant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu_quant8,
            transpose_conv2d::is_ignored_nchw_relu_quant8,
            transpose_conv2d::get_examples_nchw_relu_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_quant8, transpose_conv2d::CreateModel_nchw_relu_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu_quant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_quant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_quant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu_quant8_2,
            transpose_conv2d::is_ignored_nchw_relu_quant8_2,
            transpose_conv2d::get_examples_nchw_relu_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_quant8_2, transpose_conv2d::CreateModel_nchw_relu_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu_channelQuant8,
            transpose_conv2d::is_ignored_nchw_relu_channelQuant8,
            transpose_conv2d::get_examples_nchw_relu_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_channelQuant8, transpose_conv2d::CreateModel_nchw_relu_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu_channelQuant8_2,
            transpose_conv2d::is_ignored_nchw_relu_channelQuant8_2,
            transpose_conv2d::get_examples_nchw_relu_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_channelQuant8_2, transpose_conv2d::CreateModel_nchw_relu_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_float16) {
    execute(transpose_conv2d::CreateModel_nchw_relu_float16,
            transpose_conv2d::is_ignored_nchw_relu_float16,
            transpose_conv2d::get_examples_nchw_relu_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_float16, transpose_conv2d::CreateModel_nchw_relu_float16)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu_float16_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu_float16_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu_float16_weight_as_input, transpose_conv2d::CreateModel_nchw_relu_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1) {
    execute(transpose_conv2d::CreateModel_nchw_relu1,
            transpose_conv2d::is_ignored_nchw_relu1,
            transpose_conv2d::get_examples_nchw_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1, transpose_conv2d::CreateModel_nchw_relu1)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu1_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_weight_as_input, transpose_conv2d::CreateModel_nchw_relu1_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_relaxed) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_relaxed,
            transpose_conv2d::is_ignored_nchw_relu1_relaxed,
            transpose_conv2d::get_examples_nchw_relu1_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu1_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu1_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_quant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_quant8,
            transpose_conv2d::is_ignored_nchw_relu1_quant8,
            transpose_conv2d::get_examples_nchw_relu1_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_quant8, transpose_conv2d::CreateModel_nchw_relu1_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu1_quant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu1_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_quant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_quant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_quant8_2,
            transpose_conv2d::is_ignored_nchw_relu1_quant8_2,
            transpose_conv2d::get_examples_nchw_relu1_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_quant8_2, transpose_conv2d::CreateModel_nchw_relu1_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu1_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu1_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_channelQuant8,
            transpose_conv2d::is_ignored_nchw_relu1_channelQuant8,
            transpose_conv2d::get_examples_nchw_relu1_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_channelQuant8, transpose_conv2d::CreateModel_nchw_relu1_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu1_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_2,
            transpose_conv2d::is_ignored_nchw_relu1_channelQuant8_2,
            transpose_conv2d::get_examples_nchw_relu1_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_channelQuant8_2, transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu1_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_float16) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_float16,
            transpose_conv2d::is_ignored_nchw_relu1_float16,
            transpose_conv2d::get_examples_nchw_relu1_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_float16, transpose_conv2d::CreateModel_nchw_relu1_float16)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu1_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu1_float16_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu1_float16_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu1_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu1_float16_weight_as_input, transpose_conv2d::CreateModel_nchw_relu1_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6) {
    execute(transpose_conv2d::CreateModel_nchw_relu6,
            transpose_conv2d::is_ignored_nchw_relu6,
            transpose_conv2d::get_examples_nchw_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6, transpose_conv2d::CreateModel_nchw_relu6)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu6_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu6_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_weight_as_input, transpose_conv2d::CreateModel_nchw_relu6_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_relaxed) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_relaxed,
            transpose_conv2d::is_ignored_nchw_relu6_relaxed,
            transpose_conv2d::get_examples_nchw_relu6_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu6_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu6_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_quant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_quant8,
            transpose_conv2d::is_ignored_nchw_relu6_quant8,
            transpose_conv2d::get_examples_nchw_relu6_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_quant8, transpose_conv2d::CreateModel_nchw_relu6_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu6_quant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu6_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_quant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_quant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_quant8_2,
            transpose_conv2d::is_ignored_nchw_relu6_quant8_2,
            transpose_conv2d::get_examples_nchw_relu6_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_quant8_2, transpose_conv2d::CreateModel_nchw_relu6_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu6_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu6_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_channelQuant8,
            transpose_conv2d::is_ignored_nchw_relu6_channelQuant8,
            transpose_conv2d::get_examples_nchw_relu6_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_channelQuant8, transpose_conv2d::CreateModel_nchw_relu6_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu6_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_2,
            transpose_conv2d::is_ignored_nchw_relu6_channelQuant8_2,
            transpose_conv2d::get_examples_nchw_relu6_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_channelQuant8_2, transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relu6_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_channelQuant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_float16) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_float16,
            transpose_conv2d::is_ignored_nchw_relu6_float16,
            transpose_conv2d::get_examples_nchw_relu6_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_float16, transpose_conv2d::CreateModel_nchw_relu6_float16)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relu6_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relu6_float16_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relu6_float16_weight_as_input,
            transpose_conv2d::get_examples_nchw_relu6_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_relu6_float16_weight_as_input, transpose_conv2d::CreateModel_nchw_relu6_float16_weight_as_input)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_none_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_none_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_none_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_none_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu1_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relu6_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_none_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_none_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_none_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_none_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu1_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu1_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relu6_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relu6_float16_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc) {
    execute(transpose_conv2d::CreateModel_nhwc,
            transpose_conv2d::is_ignored_nhwc,
            transpose_conv2d::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc, transpose_conv2d::CreateModel_nhwc)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_weight_as_input,
            transpose_conv2d::get_examples_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_weight_as_input, transpose_conv2d::CreateModel_nhwc_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed,
            transpose_conv2d::is_ignored_nhwc_relaxed,
            transpose_conv2d::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nhwc_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8,
            transpose_conv2d::is_ignored_nhwc_quant8,
            transpose_conv2d::get_examples_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8, transpose_conv2d::CreateModel_nhwc_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_quant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nhwc_channelQuant8,
            transpose_conv2d::is_ignored_nhwc_channelQuant8,
            transpose_conv2d::get_examples_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_channelQuant8, transpose_conv2d::CreateModel_nhwc_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nhwc_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nhwc_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16) {
    execute(transpose_conv2d::CreateModel_nhwc_float16,
            transpose_conv2d::is_ignored_nhwc_float16,
            transpose_conv2d::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16, transpose_conv2d::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_weight_as_input,
            transpose_conv2d::is_ignored_nhwc_float16_weight_as_input,
            transpose_conv2d::get_examples_nhwc_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_weight_as_input, transpose_conv2d::CreateModel_nhwc_float16_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw) {
    execute(transpose_conv2d::CreateModel_nchw,
            transpose_conv2d::is_ignored_nchw,
            transpose_conv2d::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw, transpose_conv2d::CreateModel_nchw)

TEST_F(GeneratedTests, transpose_conv2d_nchw_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_weight_as_input,
            transpose_conv2d::is_ignored_nchw_weight_as_input,
            transpose_conv2d::get_examples_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_weight_as_input, transpose_conv2d::CreateModel_nchw_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed,
            transpose_conv2d::is_ignored_nchw_relaxed,
            transpose_conv2d::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_nchw_relaxed_weight_as_input,
            transpose_conv2d::get_examples_nchw_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8) {
    execute(transpose_conv2d::CreateModel_nchw_quant8,
            transpose_conv2d::is_ignored_nchw_quant8,
            transpose_conv2d::get_examples_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8, transpose_conv2d::CreateModel_nchw_quant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_quant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_weight_as_input, transpose_conv2d::CreateModel_nchw_quant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_channelQuant8) {
    execute(transpose_conv2d::CreateModel_nchw_channelQuant8,
            transpose_conv2d::is_ignored_nchw_channelQuant8,
            transpose_conv2d::get_examples_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_channelQuant8, transpose_conv2d::CreateModel_nchw_channelQuant8)

TEST_F(GeneratedTests, transpose_conv2d_nchw_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_nchw_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_nchw_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_channelQuant8_weight_as_input, transpose_conv2d::CreateModel_nchw_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16) {
    execute(transpose_conv2d::CreateModel_nchw_float16,
            transpose_conv2d::is_ignored_nchw_float16,
            transpose_conv2d::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16, transpose_conv2d::CreateModel_nchw_float16)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_nchw_float16_weight_as_input,
            transpose_conv2d::is_ignored_nchw_float16_weight_as_input,
            transpose_conv2d::get_examples_nchw_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_weight_as_input, transpose_conv2d::CreateModel_nchw_float16_weight_as_input)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_channelQuant8) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_channelQuant8,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_channelQuant8,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_channelQuant8_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_weight_as_input) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_weight_as_input,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_weight_as_input,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_weight_as_input());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_2) {
    execute(transpose_conv2d::CreateModel_nhwc_2,
            transpose_conv2d::is_ignored_nhwc_2,
            transpose_conv2d::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_2, transpose_conv2d::CreateModel_nhwc_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_2,
            transpose_conv2d::is_ignored_nhwc_relaxed_2,
            transpose_conv2d::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_relaxed_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_relaxed_weight_as_input_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_2) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_2,
            transpose_conv2d::is_ignored_nhwc_quant8_2,
            transpose_conv2d::get_examples_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_2, transpose_conv2d::CreateModel_nhwc_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_2) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_2,
            transpose_conv2d::is_ignored_nhwc_float16_2,
            transpose_conv2d::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_2, transpose_conv2d::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_2,
            transpose_conv2d::is_ignored_nhwc_float16_weight_as_input_2,
            transpose_conv2d::get_examples_nhwc_float16_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_weight_as_input_2, transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_2) {
    execute(transpose_conv2d::CreateModel_nchw_2,
            transpose_conv2d::is_ignored_nchw_2,
            transpose_conv2d::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_2, transpose_conv2d::CreateModel_nchw_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_weight_as_input_2, transpose_conv2d::CreateModel_nchw_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_2) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_2,
            transpose_conv2d::is_ignored_nchw_relaxed_2,
            transpose_conv2d::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_relaxed_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_relaxed_weight_as_input_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_2) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_2,
            transpose_conv2d::is_ignored_nchw_quant8_2,
            transpose_conv2d::get_examples_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_2, transpose_conv2d::CreateModel_nchw_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_weight_as_input_2, transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_2) {
    execute(transpose_conv2d::CreateModel_nchw_float16_2,
            transpose_conv2d::is_ignored_nchw_float16_2,
            transpose_conv2d::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_2, transpose_conv2d::CreateModel_nchw_float16_2)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_nchw_float16_weight_as_input_2,
            transpose_conv2d::is_ignored_nchw_float16_weight_as_input_2,
            transpose_conv2d::get_examples_nchw_float16_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_weight_as_input_2, transpose_conv2d::CreateModel_nchw_float16_weight_as_input_2)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_weight_as_input_2) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_weight_as_input_2,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_weight_as_input_2,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_weight_as_input_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_3) {
    execute(transpose_conv2d::CreateModel_nhwc_3,
            transpose_conv2d::is_ignored_nhwc_3,
            transpose_conv2d::get_examples_nhwc_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_3, transpose_conv2d::CreateModel_nhwc_3)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nhwc_weight_as_input_3,
            transpose_conv2d::is_ignored_nhwc_weight_as_input_3,
            transpose_conv2d::get_examples_nhwc_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_weight_as_input_3, transpose_conv2d::CreateModel_nhwc_weight_as_input_3)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_3) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_3,
            transpose_conv2d::is_ignored_nhwc_relaxed_3,
            transpose_conv2d::get_examples_nhwc_relaxed_3());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_weight_as_input_3,
            transpose_conv2d::is_ignored_nhwc_relaxed_weight_as_input_3,
            transpose_conv2d::get_examples_nhwc_relaxed_weight_as_input_3());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_3) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_3,
            transpose_conv2d::is_ignored_nhwc_quant8_3,
            transpose_conv2d::get_examples_nhwc_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_3, transpose_conv2d::CreateModel_nhwc_quant8_3)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_3,
            transpose_conv2d::is_ignored_nhwc_quant8_weight_as_input_3,
            transpose_conv2d::get_examples_nhwc_quant8_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_weight_as_input_3, transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_3)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_3) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_3,
            transpose_conv2d::is_ignored_nhwc_float16_3,
            transpose_conv2d::get_examples_nhwc_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_3, transpose_conv2d::CreateModel_nhwc_float16_3)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_3,
            transpose_conv2d::is_ignored_nhwc_float16_weight_as_input_3,
            transpose_conv2d::get_examples_nhwc_float16_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_weight_as_input_3, transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_3) {
    execute(transpose_conv2d::CreateModel_nchw_3,
            transpose_conv2d::is_ignored_nchw_3,
            transpose_conv2d::get_examples_nchw_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_3, transpose_conv2d::CreateModel_nchw_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nchw_weight_as_input_3,
            transpose_conv2d::is_ignored_nchw_weight_as_input_3,
            transpose_conv2d::get_examples_nchw_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_weight_as_input_3, transpose_conv2d::CreateModel_nchw_weight_as_input_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_3) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_3,
            transpose_conv2d::is_ignored_nchw_relaxed_3,
            transpose_conv2d::get_examples_nchw_relaxed_3());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_weight_as_input_3,
            transpose_conv2d::is_ignored_nchw_relaxed_weight_as_input_3,
            transpose_conv2d::get_examples_nchw_relaxed_weight_as_input_3());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_3) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_3,
            transpose_conv2d::is_ignored_nchw_quant8_3,
            transpose_conv2d::get_examples_nchw_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_3, transpose_conv2d::CreateModel_nchw_quant8_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_3,
            transpose_conv2d::is_ignored_nchw_quant8_weight_as_input_3,
            transpose_conv2d::get_examples_nchw_quant8_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_weight_as_input_3, transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_3) {
    execute(transpose_conv2d::CreateModel_nchw_float16_3,
            transpose_conv2d::is_ignored_nchw_float16_3,
            transpose_conv2d::get_examples_nchw_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_3, transpose_conv2d::CreateModel_nchw_float16_3)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_nchw_float16_weight_as_input_3,
            transpose_conv2d::is_ignored_nchw_float16_weight_as_input_3,
            transpose_conv2d::get_examples_nchw_float16_weight_as_input_3());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_weight_as_input_3, transpose_conv2d::CreateModel_nchw_float16_weight_as_input_3)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_weight_as_input_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_3());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_weight_as_input_3) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_weight_as_input_3,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_weight_as_input_3,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_weight_as_input_3());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_4) {
    execute(transpose_conv2d::CreateModel_nhwc_4,
            transpose_conv2d::is_ignored_nhwc_4,
            transpose_conv2d::get_examples_nhwc_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_4, transpose_conv2d::CreateModel_nhwc_4)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nhwc_weight_as_input_4,
            transpose_conv2d::is_ignored_nhwc_weight_as_input_4,
            transpose_conv2d::get_examples_nhwc_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_weight_as_input_4, transpose_conv2d::CreateModel_nhwc_weight_as_input_4)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_4) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_4,
            transpose_conv2d::is_ignored_nhwc_relaxed_4,
            transpose_conv2d::get_examples_nhwc_relaxed_4());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_weight_as_input_4,
            transpose_conv2d::is_ignored_nhwc_relaxed_weight_as_input_4,
            transpose_conv2d::get_examples_nhwc_relaxed_weight_as_input_4());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_4) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_4,
            transpose_conv2d::is_ignored_nhwc_quant8_4,
            transpose_conv2d::get_examples_nhwc_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_4, transpose_conv2d::CreateModel_nhwc_quant8_4)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_4,
            transpose_conv2d::is_ignored_nhwc_quant8_weight_as_input_4,
            transpose_conv2d::get_examples_nhwc_quant8_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_weight_as_input_4, transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_4)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_4) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_4,
            transpose_conv2d::is_ignored_nhwc_float16_4,
            transpose_conv2d::get_examples_nhwc_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_4, transpose_conv2d::CreateModel_nhwc_float16_4)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_4,
            transpose_conv2d::is_ignored_nhwc_float16_weight_as_input_4,
            transpose_conv2d::get_examples_nhwc_float16_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_weight_as_input_4, transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_4) {
    execute(transpose_conv2d::CreateModel_nchw_4,
            transpose_conv2d::is_ignored_nchw_4,
            transpose_conv2d::get_examples_nchw_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_4, transpose_conv2d::CreateModel_nchw_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nchw_weight_as_input_4,
            transpose_conv2d::is_ignored_nchw_weight_as_input_4,
            transpose_conv2d::get_examples_nchw_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_weight_as_input_4, transpose_conv2d::CreateModel_nchw_weight_as_input_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_4) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_4,
            transpose_conv2d::is_ignored_nchw_relaxed_4,
            transpose_conv2d::get_examples_nchw_relaxed_4());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_weight_as_input_4,
            transpose_conv2d::is_ignored_nchw_relaxed_weight_as_input_4,
            transpose_conv2d::get_examples_nchw_relaxed_weight_as_input_4());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_4) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_4,
            transpose_conv2d::is_ignored_nchw_quant8_4,
            transpose_conv2d::get_examples_nchw_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_4, transpose_conv2d::CreateModel_nchw_quant8_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_4,
            transpose_conv2d::is_ignored_nchw_quant8_weight_as_input_4,
            transpose_conv2d::get_examples_nchw_quant8_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_weight_as_input_4, transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_4) {
    execute(transpose_conv2d::CreateModel_nchw_float16_4,
            transpose_conv2d::is_ignored_nchw_float16_4,
            transpose_conv2d::get_examples_nchw_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_4, transpose_conv2d::CreateModel_nchw_float16_4)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_nchw_float16_weight_as_input_4,
            transpose_conv2d::is_ignored_nchw_float16_weight_as_input_4,
            transpose_conv2d::get_examples_nchw_float16_weight_as_input_4());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_weight_as_input_4, transpose_conv2d::CreateModel_nchw_float16_weight_as_input_4)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_weight_as_input_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_4());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_weight_as_input_4) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_weight_as_input_4,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_weight_as_input_4,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_weight_as_input_4());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc,
            transpose_conv2d::is_ignored_zero_sized_nhwc,
            transpose_conv2d::get_examples_zero_sized_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc, transpose_conv2d::CreateModel_zero_sized_nhwc)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_relaxed) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_relaxed,
            transpose_conv2d::is_ignored_zero_sized_nhwc_relaxed,
            transpose_conv2d::get_examples_zero_sized_nhwc_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_quant8) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_quant8,
            transpose_conv2d::is_ignored_zero_sized_nhwc_quant8,
            transpose_conv2d::get_examples_zero_sized_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc_quant8, transpose_conv2d::CreateModel_zero_sized_nhwc_quant8)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_float16) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_float16,
            transpose_conv2d::is_ignored_zero_sized_nhwc_float16,
            transpose_conv2d::get_examples_zero_sized_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc_float16, transpose_conv2d::CreateModel_zero_sized_nhwc_float16)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw,
            transpose_conv2d::is_ignored_zero_sized_nchw,
            transpose_conv2d::get_examples_zero_sized_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw, transpose_conv2d::CreateModel_zero_sized_nchw)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_relaxed) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_relaxed,
            transpose_conv2d::is_ignored_zero_sized_nchw_relaxed,
            transpose_conv2d::get_examples_zero_sized_nchw_relaxed());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_quant8) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_quant8,
            transpose_conv2d::is_ignored_zero_sized_nchw_quant8,
            transpose_conv2d::get_examples_zero_sized_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw_quant8, transpose_conv2d::CreateModel_zero_sized_nchw_quant8)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_float16) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_float16,
            transpose_conv2d::is_ignored_zero_sized_nchw_float16,
            transpose_conv2d::get_examples_zero_sized_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw_float16, transpose_conv2d::CreateModel_zero_sized_nchw_float16)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_relaxed) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_quant8) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_float16) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_relaxed) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_quant8) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_float16) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_float16,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_float16,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_2,
            transpose_conv2d::is_ignored_zero_sized_nhwc_2,
            transpose_conv2d::get_examples_zero_sized_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc_2, transpose_conv2d::CreateModel_zero_sized_nhwc_2)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_relaxed_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_relaxed_2,
            transpose_conv2d::is_ignored_zero_sized_nhwc_relaxed_2,
            transpose_conv2d::get_examples_zero_sized_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_quant8_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_quant8_2,
            transpose_conv2d::is_ignored_zero_sized_nhwc_quant8_2,
            transpose_conv2d::get_examples_zero_sized_nhwc_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc_quant8_2, transpose_conv2d::CreateModel_zero_sized_nhwc_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nhwc_float16_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nhwc_float16_2,
            transpose_conv2d::is_ignored_zero_sized_nhwc_float16_2,
            transpose_conv2d::get_examples_zero_sized_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nhwc_float16_2, transpose_conv2d::CreateModel_zero_sized_nhwc_float16_2)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_2,
            transpose_conv2d::is_ignored_zero_sized_nchw_2,
            transpose_conv2d::get_examples_zero_sized_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw_2, transpose_conv2d::CreateModel_zero_sized_nchw_2)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_relaxed_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_relaxed_2,
            transpose_conv2d::is_ignored_zero_sized_nchw_relaxed_2,
            transpose_conv2d::get_examples_zero_sized_nchw_relaxed_2());
}

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_quant8_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_quant8_2,
            transpose_conv2d::is_ignored_zero_sized_nchw_quant8_2,
            transpose_conv2d::get_examples_zero_sized_nchw_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw_quant8_2, transpose_conv2d::CreateModel_zero_sized_nchw_quant8_2)

TEST_F(GeneratedTests, transpose_conv2d_zero_sized_nchw_float16_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_nchw_float16_2,
            transpose_conv2d::is_ignored_zero_sized_nchw_float16_2,
            transpose_conv2d::get_examples_zero_sized_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_zero_sized_nchw_float16_2, transpose_conv2d::CreateModel_zero_sized_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_relaxed_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_quant8_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nhwc_float16_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_relaxed_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_quant8_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_zero_sized_dynamic_output_shape_nchw_float16_2) {
    execute(transpose_conv2d::CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2,
            transpose_conv2d::is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2,
            transpose_conv2d::get_examples_zero_sized_dynamic_output_shape_nchw_float16_2());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_5) {
    execute(transpose_conv2d::CreateModel_nhwc_5,
            transpose_conv2d::is_ignored_nhwc_5,
            transpose_conv2d::get_examples_nhwc_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_5, transpose_conv2d::CreateModel_nhwc_5)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nhwc_weight_as_input_5,
            transpose_conv2d::is_ignored_nhwc_weight_as_input_5,
            transpose_conv2d::get_examples_nhwc_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_weight_as_input_5, transpose_conv2d::CreateModel_nhwc_weight_as_input_5)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_5) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_5,
            transpose_conv2d::is_ignored_nhwc_relaxed_5,
            transpose_conv2d::get_examples_nhwc_relaxed_5());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_relaxed_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nhwc_relaxed_weight_as_input_5,
            transpose_conv2d::is_ignored_nhwc_relaxed_weight_as_input_5,
            transpose_conv2d::get_examples_nhwc_relaxed_weight_as_input_5());
}

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_5) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_5,
            transpose_conv2d::is_ignored_nhwc_quant8_5,
            transpose_conv2d::get_examples_nhwc_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_5, transpose_conv2d::CreateModel_nhwc_quant8_5)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_quant8_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_5,
            transpose_conv2d::is_ignored_nhwc_quant8_weight_as_input_5,
            transpose_conv2d::get_examples_nhwc_quant8_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_quant8_weight_as_input_5, transpose_conv2d::CreateModel_nhwc_quant8_weight_as_input_5)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_5) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_5,
            transpose_conv2d::is_ignored_nhwc_float16_5,
            transpose_conv2d::get_examples_nhwc_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_5, transpose_conv2d::CreateModel_nhwc_float16_5)

TEST_F(GeneratedTests, transpose_conv2d_nhwc_float16_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_5,
            transpose_conv2d::is_ignored_nhwc_float16_weight_as_input_5,
            transpose_conv2d::get_examples_nhwc_float16_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nhwc_float16_weight_as_input_5, transpose_conv2d::CreateModel_nhwc_float16_weight_as_input_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_5) {
    execute(transpose_conv2d::CreateModel_nchw_5,
            transpose_conv2d::is_ignored_nchw_5,
            transpose_conv2d::get_examples_nchw_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_5, transpose_conv2d::CreateModel_nchw_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nchw_weight_as_input_5,
            transpose_conv2d::is_ignored_nchw_weight_as_input_5,
            transpose_conv2d::get_examples_nchw_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_weight_as_input_5, transpose_conv2d::CreateModel_nchw_weight_as_input_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_5) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_5,
            transpose_conv2d::is_ignored_nchw_relaxed_5,
            transpose_conv2d::get_examples_nchw_relaxed_5());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_relaxed_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nchw_relaxed_weight_as_input_5,
            transpose_conv2d::is_ignored_nchw_relaxed_weight_as_input_5,
            transpose_conv2d::get_examples_nchw_relaxed_weight_as_input_5());
}

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_5) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_5,
            transpose_conv2d::is_ignored_nchw_quant8_5,
            transpose_conv2d::get_examples_nchw_quant8_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_5, transpose_conv2d::CreateModel_nchw_quant8_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_quant8_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_5,
            transpose_conv2d::is_ignored_nchw_quant8_weight_as_input_5,
            transpose_conv2d::get_examples_nchw_quant8_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_quant8_weight_as_input_5, transpose_conv2d::CreateModel_nchw_quant8_weight_as_input_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_5) {
    execute(transpose_conv2d::CreateModel_nchw_float16_5,
            transpose_conv2d::is_ignored_nchw_float16_5,
            transpose_conv2d::get_examples_nchw_float16_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_5, transpose_conv2d::CreateModel_nchw_float16_5)

TEST_F(GeneratedTests, transpose_conv2d_nchw_float16_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_nchw_float16_weight_as_input_5,
            transpose_conv2d::is_ignored_nchw_float16_weight_as_input_5,
            transpose_conv2d::get_examples_nchw_float16_weight_as_input_5());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_conv2d_nchw_float16_weight_as_input_5, transpose_conv2d::CreateModel_nchw_float16_weight_as_input_5)

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_relaxed_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_relaxed_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_relaxed_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_relaxed_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_quant8_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_quant8_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_quant8_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_quant8_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nhwc_float16_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nhwc_float16_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nhwc_float16_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nhwc_float16_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_relaxed_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_relaxed_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_relaxed_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_relaxed_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_quant8_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_quant8_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_quant8_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_quant8_weight_as_input_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_5());
}

TEST_F(DynamicOutputShapeTest, transpose_conv2d_dynamic_output_shape_nchw_float16_weight_as_input_5) {
    execute(transpose_conv2d::CreateModel_dynamic_output_shape_nchw_float16_weight_as_input_5,
            transpose_conv2d::is_ignored_dynamic_output_shape_nchw_float16_weight_as_input_5,
            transpose_conv2d::get_examples_dynamic_output_shape_nchw_float16_weight_as_input_5());
}

