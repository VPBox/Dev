// clang-format off
// Generated file (from: grouped_conv2d.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace grouped_conv2d {
// Generated grouped_conv2d test
#include "generated/examples/grouped_conv2d.example.cpp"
// Generated model constructor
#include "generated/models/grouped_conv2d.model.cpp"
} // namespace grouped_conv2d

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none) {
    execute(grouped_conv2d::CreateModel_nhwc_none,
            grouped_conv2d::is_ignored_nhwc_none,
            grouped_conv2d::get_examples_nhwc_none());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none, grouped_conv2d::CreateModel_nhwc_none)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_none_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_none_weight_as_input,
            grouped_conv2d::get_examples_nhwc_none_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_weight_as_input, grouped_conv2d::CreateModel_nhwc_none_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_relaxed) {
    execute(grouped_conv2d::CreateModel_nhwc_none_relaxed,
            grouped_conv2d::is_ignored_nhwc_none_relaxed,
            grouped_conv2d::get_examples_nhwc_none_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_none_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_none_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nhwc_none_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_quant8) {
    execute(grouped_conv2d::CreateModel_nhwc_none_quant8,
            grouped_conv2d::is_ignored_nhwc_none_quant8,
            grouped_conv2d::get_examples_nhwc_none_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_quant8, grouped_conv2d::CreateModel_nhwc_none_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_none_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_none_quant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_none_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_quant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_none_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_quant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_none_quant8_2,
            grouped_conv2d::is_ignored_nhwc_none_quant8_2,
            grouped_conv2d::get_examples_nhwc_none_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_quant8_2, grouped_conv2d::CreateModel_nhwc_none_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_none_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_none_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_none_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_none_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nhwc_none_channelQuant8,
            grouped_conv2d::is_ignored_nhwc_none_channelQuant8,
            grouped_conv2d::get_examples_nhwc_none_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_channelQuant8, grouped_conv2d::CreateModel_nhwc_none_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_none_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_none_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_none_channelQuant8_2,
            grouped_conv2d::is_ignored_nhwc_none_channelQuant8_2,
            grouped_conv2d::get_examples_nhwc_none_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_channelQuant8_2, grouped_conv2d::CreateModel_nhwc_none_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_none_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_none_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_float16) {
    execute(grouped_conv2d::CreateModel_nhwc_none_float16,
            grouped_conv2d::is_ignored_nhwc_none_float16,
            grouped_conv2d::get_examples_nhwc_none_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_float16, grouped_conv2d::CreateModel_nhwc_none_float16)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_none_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_none_float16_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_none_float16_weight_as_input,
            grouped_conv2d::get_examples_nhwc_none_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_none_float16_weight_as_input, grouped_conv2d::CreateModel_nhwc_none_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu) {
    execute(grouped_conv2d::CreateModel_nhwc_relu,
            grouped_conv2d::is_ignored_nhwc_relu,
            grouped_conv2d::get_examples_nhwc_relu());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu, grouped_conv2d::CreateModel_nhwc_relu)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_relaxed) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_relaxed,
            grouped_conv2d::is_ignored_nhwc_relu_relaxed,
            grouped_conv2d::get_examples_nhwc_relu_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_quant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_quant8,
            grouped_conv2d::is_ignored_nhwc_relu_quant8,
            grouped_conv2d::get_examples_nhwc_relu_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_quant8, grouped_conv2d::CreateModel_nhwc_relu_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu_quant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_quant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_quant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_quant8_2,
            grouped_conv2d::is_ignored_nhwc_relu_quant8_2,
            grouped_conv2d::get_examples_nhwc_relu_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_quant8_2, grouped_conv2d::CreateModel_nhwc_relu_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_channelQuant8,
            grouped_conv2d::is_ignored_nhwc_relu_channelQuant8,
            grouped_conv2d::get_examples_nhwc_relu_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_channelQuant8, grouped_conv2d::CreateModel_nhwc_relu_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_2,
            grouped_conv2d::is_ignored_nhwc_relu_channelQuant8_2,
            grouped_conv2d::get_examples_nhwc_relu_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_channelQuant8_2, grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_float16) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_float16,
            grouped_conv2d::is_ignored_nhwc_relu_float16,
            grouped_conv2d::get_examples_nhwc_relu_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_float16, grouped_conv2d::CreateModel_nhwc_relu_float16)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu_float16_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu_float16_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu_float16_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1,
            grouped_conv2d::is_ignored_nhwc_relu1,
            grouped_conv2d::get_examples_nhwc_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1, grouped_conv2d::CreateModel_nhwc_relu1)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu1_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu1_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_relaxed) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_relaxed,
            grouped_conv2d::is_ignored_nhwc_relu1_relaxed,
            grouped_conv2d::get_examples_nhwc_relu1_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu1_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu1_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_quant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_quant8,
            grouped_conv2d::is_ignored_nhwc_relu1_quant8,
            grouped_conv2d::get_examples_nhwc_relu1_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_quant8, grouped_conv2d::CreateModel_nhwc_relu1_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu1_quant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu1_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_quant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_quant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_quant8_2,
            grouped_conv2d::is_ignored_nhwc_relu1_quant8_2,
            grouped_conv2d::get_examples_nhwc_relu1_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_quant8_2, grouped_conv2d::CreateModel_nhwc_relu1_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu1_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu1_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu1_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8,
            grouped_conv2d::is_ignored_nhwc_relu1_channelQuant8,
            grouped_conv2d::get_examples_nhwc_relu1_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_channelQuant8, grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu1_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_2,
            grouped_conv2d::is_ignored_nhwc_relu1_channelQuant8_2,
            grouped_conv2d::get_examples_nhwc_relu1_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_channelQuant8_2, grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu1_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu1_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_float16) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_float16,
            grouped_conv2d::is_ignored_nhwc_relu1_float16,
            grouped_conv2d::get_examples_nhwc_relu1_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_float16, grouped_conv2d::CreateModel_nhwc_relu1_float16)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu1_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu1_float16_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu1_float16_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu1_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu1_float16_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu1_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6,
            grouped_conv2d::is_ignored_nhwc_relu6,
            grouped_conv2d::get_examples_nhwc_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6, grouped_conv2d::CreateModel_nhwc_relu6)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu6_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu6_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu6_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_relaxed) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_relaxed,
            grouped_conv2d::is_ignored_nhwc_relu6_relaxed,
            grouped_conv2d::get_examples_nhwc_relu6_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu6_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu6_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_quant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_quant8,
            grouped_conv2d::is_ignored_nhwc_relu6_quant8,
            grouped_conv2d::get_examples_nhwc_relu6_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_quant8, grouped_conv2d::CreateModel_nhwc_relu6_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu6_quant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu6_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_quant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_quant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_quant8_2,
            grouped_conv2d::is_ignored_nhwc_relu6_quant8_2,
            grouped_conv2d::get_examples_nhwc_relu6_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_quant8_2, grouped_conv2d::CreateModel_nhwc_relu6_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu6_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu6_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu6_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8,
            grouped_conv2d::is_ignored_nhwc_relu6_channelQuant8,
            grouped_conv2d::get_examples_nhwc_relu6_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_channelQuant8, grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu6_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_2,
            grouped_conv2d::is_ignored_nhwc_relu6_channelQuant8_2,
            grouped_conv2d::get_examples_nhwc_relu6_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_channelQuant8_2, grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nhwc_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nhwc_relu6_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nhwc_relu6_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_float16) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_float16,
            grouped_conv2d::is_ignored_nhwc_relu6_float16,
            grouped_conv2d::get_examples_nhwc_relu6_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_float16, grouped_conv2d::CreateModel_nhwc_relu6_float16)

TEST_F(GeneratedTests, grouped_conv2d_nhwc_relu6_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nhwc_relu6_float16_weight_as_input,
            grouped_conv2d::is_ignored_nhwc_relu6_float16_weight_as_input,
            grouped_conv2d::get_examples_nhwc_relu6_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nhwc_relu6_float16_weight_as_input, grouped_conv2d::CreateModel_nhwc_relu6_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none) {
    execute(grouped_conv2d::CreateModel_nchw_none,
            grouped_conv2d::is_ignored_nchw_none,
            grouped_conv2d::get_examples_nchw_none());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none, grouped_conv2d::CreateModel_nchw_none)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_none_weight_as_input,
            grouped_conv2d::is_ignored_nchw_none_weight_as_input,
            grouped_conv2d::get_examples_nchw_none_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_weight_as_input, grouped_conv2d::CreateModel_nchw_none_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_relaxed) {
    execute(grouped_conv2d::CreateModel_nchw_none_relaxed,
            grouped_conv2d::is_ignored_nchw_none_relaxed,
            grouped_conv2d::get_examples_nchw_none_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_none_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nchw_none_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nchw_none_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_quant8) {
    execute(grouped_conv2d::CreateModel_nchw_none_quant8,
            grouped_conv2d::is_ignored_nchw_none_quant8,
            grouped_conv2d::get_examples_nchw_none_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_quant8, grouped_conv2d::CreateModel_nchw_none_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_none_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_none_quant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_none_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_quant8_weight_as_input, grouped_conv2d::CreateModel_nchw_none_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_quant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_none_quant8_2,
            grouped_conv2d::is_ignored_nchw_none_quant8_2,
            grouped_conv2d::get_examples_nchw_none_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_quant8_2, grouped_conv2d::CreateModel_nchw_none_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_none_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_none_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_none_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_none_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nchw_none_channelQuant8,
            grouped_conv2d::is_ignored_nchw_none_channelQuant8,
            grouped_conv2d::get_examples_nchw_none_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_channelQuant8, grouped_conv2d::CreateModel_nchw_none_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_none_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_none_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_none_channelQuant8_2,
            grouped_conv2d::is_ignored_nchw_none_channelQuant8_2,
            grouped_conv2d::get_examples_nchw_none_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_channelQuant8_2, grouped_conv2d::CreateModel_nchw_none_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_none_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_none_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_float16) {
    execute(grouped_conv2d::CreateModel_nchw_none_float16,
            grouped_conv2d::is_ignored_nchw_none_float16,
            grouped_conv2d::get_examples_nchw_none_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_float16, grouped_conv2d::CreateModel_nchw_none_float16)

TEST_F(GeneratedTests, grouped_conv2d_nchw_none_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_none_float16_weight_as_input,
            grouped_conv2d::is_ignored_nchw_none_float16_weight_as_input,
            grouped_conv2d::get_examples_nchw_none_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_none_float16_weight_as_input, grouped_conv2d::CreateModel_nchw_none_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu) {
    execute(grouped_conv2d::CreateModel_nchw_relu,
            grouped_conv2d::is_ignored_nchw_relu,
            grouped_conv2d::get_examples_nchw_relu());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu, grouped_conv2d::CreateModel_nchw_relu)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_weight_as_input, grouped_conv2d::CreateModel_nchw_relu_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_relaxed) {
    execute(grouped_conv2d::CreateModel_nchw_relu_relaxed,
            grouped_conv2d::is_ignored_nchw_relu_relaxed,
            grouped_conv2d::get_examples_nchw_relu_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_quant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu_quant8,
            grouped_conv2d::is_ignored_nchw_relu_quant8,
            grouped_conv2d::get_examples_nchw_relu_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_quant8, grouped_conv2d::CreateModel_nchw_relu_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu_quant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_quant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_quant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu_quant8_2,
            grouped_conv2d::is_ignored_nchw_relu_quant8_2,
            grouped_conv2d::get_examples_nchw_relu_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_quant8_2, grouped_conv2d::CreateModel_nchw_relu_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu_channelQuant8,
            grouped_conv2d::is_ignored_nchw_relu_channelQuant8,
            grouped_conv2d::get_examples_nchw_relu_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_channelQuant8, grouped_conv2d::CreateModel_nchw_relu_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu_channelQuant8_2,
            grouped_conv2d::is_ignored_nchw_relu_channelQuant8_2,
            grouped_conv2d::get_examples_nchw_relu_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_channelQuant8_2, grouped_conv2d::CreateModel_nchw_relu_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_float16) {
    execute(grouped_conv2d::CreateModel_nchw_relu_float16,
            grouped_conv2d::is_ignored_nchw_relu_float16,
            grouped_conv2d::get_examples_nchw_relu_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_float16, grouped_conv2d::CreateModel_nchw_relu_float16)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu_float16_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu_float16_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu_float16_weight_as_input, grouped_conv2d::CreateModel_nchw_relu_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1) {
    execute(grouped_conv2d::CreateModel_nchw_relu1,
            grouped_conv2d::is_ignored_nchw_relu1,
            grouped_conv2d::get_examples_nchw_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1, grouped_conv2d::CreateModel_nchw_relu1)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu1_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu1_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_weight_as_input, grouped_conv2d::CreateModel_nchw_relu1_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_relaxed) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_relaxed,
            grouped_conv2d::is_ignored_nchw_relu1_relaxed,
            grouped_conv2d::get_examples_nchw_relu1_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu1_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu1_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_quant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_quant8,
            grouped_conv2d::is_ignored_nchw_relu1_quant8,
            grouped_conv2d::get_examples_nchw_relu1_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_quant8, grouped_conv2d::CreateModel_nchw_relu1_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu1_quant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu1_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_quant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_quant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_quant8_2,
            grouped_conv2d::is_ignored_nchw_relu1_quant8_2,
            grouped_conv2d::get_examples_nchw_relu1_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_quant8_2, grouped_conv2d::CreateModel_nchw_relu1_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu1_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu1_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu1_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_channelQuant8,
            grouped_conv2d::is_ignored_nchw_relu1_channelQuant8,
            grouped_conv2d::get_examples_nchw_relu1_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_channelQuant8, grouped_conv2d::CreateModel_nchw_relu1_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu1_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_2,
            grouped_conv2d::is_ignored_nchw_relu1_channelQuant8_2,
            grouped_conv2d::get_examples_nchw_relu1_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_channelQuant8_2, grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu1_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu1_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_float16) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_float16,
            grouped_conv2d::is_ignored_nchw_relu1_float16,
            grouped_conv2d::get_examples_nchw_relu1_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_float16, grouped_conv2d::CreateModel_nchw_relu1_float16)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu1_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu1_float16_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu1_float16_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu1_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu1_float16_weight_as_input, grouped_conv2d::CreateModel_nchw_relu1_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6) {
    execute(grouped_conv2d::CreateModel_nchw_relu6,
            grouped_conv2d::is_ignored_nchw_relu6,
            grouped_conv2d::get_examples_nchw_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6, grouped_conv2d::CreateModel_nchw_relu6)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu6_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu6_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_weight_as_input, grouped_conv2d::CreateModel_nchw_relu6_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_relaxed) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_relaxed,
            grouped_conv2d::is_ignored_nchw_relu6_relaxed,
            grouped_conv2d::get_examples_nchw_relu6_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu6_relaxed_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu6_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_quant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_quant8,
            grouped_conv2d::is_ignored_nchw_relu6_quant8,
            grouped_conv2d::get_examples_nchw_relu6_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_quant8, grouped_conv2d::CreateModel_nchw_relu6_quant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu6_quant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu6_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_quant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_quant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_quant8_2,
            grouped_conv2d::is_ignored_nchw_relu6_quant8_2,
            grouped_conv2d::get_examples_nchw_relu6_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_quant8_2, grouped_conv2d::CreateModel_nchw_relu6_quant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu6_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu6_quant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_quant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu6_quant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_channelQuant8) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_channelQuant8,
            grouped_conv2d::is_ignored_nchw_relu6_channelQuant8,
            grouped_conv2d::get_examples_nchw_relu6_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_channelQuant8, grouped_conv2d::CreateModel_nchw_relu6_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu6_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_2,
            grouped_conv2d::is_ignored_nchw_relu6_channelQuant8_2,
            grouped_conv2d::get_examples_nchw_relu6_channelQuant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_channelQuant8_2, grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_nchw_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_nchw_relu6_channelQuant8_weight_as_input_2());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_channelQuant8_weight_as_input_2, grouped_conv2d::CreateModel_nchw_relu6_channelQuant8_weight_as_input_2)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_float16) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_float16,
            grouped_conv2d::is_ignored_nchw_relu6_float16,
            grouped_conv2d::get_examples_nchw_relu6_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_float16, grouped_conv2d::CreateModel_nchw_relu6_float16)

TEST_F(GeneratedTests, grouped_conv2d_nchw_relu6_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_nchw_relu6_float16_weight_as_input,
            grouped_conv2d::is_ignored_nchw_relu6_float16_weight_as_input,
            grouped_conv2d::get_examples_nchw_relu6_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_nchw_relu6_float16_weight_as_input, grouped_conv2d::CreateModel_nchw_relu6_float16_weight_as_input)

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_none_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_none_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_none_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_none_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu1_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu1_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu1_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu1_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nhwc_relu6_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nhwc_relu6_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nhwc_relu6_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nhwc_relu6_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_none_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_none_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_none_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_none_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu1_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu1_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu1_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu1_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_relaxed) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_relaxed,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_relaxed,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_quant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_quant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_float16) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_float16,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_float16,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_dynamic_output_shape_nchw_relu6_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_dynamic_output_shape_nchw_relu6_float16_weight_as_input,
            grouped_conv2d::is_ignored_dynamic_output_shape_nchw_relu6_float16_weight_as_input,
            grouped_conv2d::get_examples_dynamic_output_shape_nchw_relu6_float16_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc) {
    execute(grouped_conv2d::CreateModel_large_nhwc,
            grouped_conv2d::is_ignored_large_nhwc,
            grouped_conv2d::get_examples_large_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc, grouped_conv2d::CreateModel_large_nhwc)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nhwc_weight_as_input,
            grouped_conv2d::is_ignored_large_nhwc_weight_as_input,
            grouped_conv2d::get_examples_large_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_weight_as_input, grouped_conv2d::CreateModel_large_nhwc_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_relaxed) {
    execute(grouped_conv2d::CreateModel_large_nhwc_relaxed,
            grouped_conv2d::is_ignored_large_nhwc_relaxed,
            grouped_conv2d::get_examples_large_nhwc_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nhwc_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_large_nhwc_relaxed_weight_as_input,
            grouped_conv2d::get_examples_large_nhwc_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_quant8) {
    execute(grouped_conv2d::CreateModel_large_nhwc_quant8,
            grouped_conv2d::is_ignored_large_nhwc_quant8,
            grouped_conv2d::get_examples_large_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_quant8, grouped_conv2d::CreateModel_large_nhwc_quant8)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nhwc_quant8_weight_as_input,
            grouped_conv2d::is_ignored_large_nhwc_quant8_weight_as_input,
            grouped_conv2d::get_examples_large_nhwc_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_quant8_weight_as_input, grouped_conv2d::CreateModel_large_nhwc_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_channelQuant8) {
    execute(grouped_conv2d::CreateModel_large_nhwc_channelQuant8,
            grouped_conv2d::is_ignored_large_nhwc_channelQuant8,
            grouped_conv2d::get_examples_large_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_channelQuant8, grouped_conv2d::CreateModel_large_nhwc_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_large_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_large_nhwc_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_large_nhwc_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_float16) {
    execute(grouped_conv2d::CreateModel_large_nhwc_float16,
            grouped_conv2d::is_ignored_large_nhwc_float16,
            grouped_conv2d::get_examples_large_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_float16, grouped_conv2d::CreateModel_large_nhwc_float16)

TEST_F(GeneratedTests, grouped_conv2d_large_nhwc_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nhwc_float16_weight_as_input,
            grouped_conv2d::is_ignored_large_nhwc_float16_weight_as_input,
            grouped_conv2d::get_examples_large_nhwc_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nhwc_float16_weight_as_input, grouped_conv2d::CreateModel_large_nhwc_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw) {
    execute(grouped_conv2d::CreateModel_large_nchw,
            grouped_conv2d::is_ignored_large_nchw,
            grouped_conv2d::get_examples_large_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw, grouped_conv2d::CreateModel_large_nchw)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nchw_weight_as_input,
            grouped_conv2d::is_ignored_large_nchw_weight_as_input,
            grouped_conv2d::get_examples_large_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_weight_as_input, grouped_conv2d::CreateModel_large_nchw_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_relaxed) {
    execute(grouped_conv2d::CreateModel_large_nchw_relaxed,
            grouped_conv2d::is_ignored_large_nchw_relaxed,
            grouped_conv2d::get_examples_large_nchw_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nchw_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_large_nchw_relaxed_weight_as_input,
            grouped_conv2d::get_examples_large_nchw_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_quant8) {
    execute(grouped_conv2d::CreateModel_large_nchw_quant8,
            grouped_conv2d::is_ignored_large_nchw_quant8,
            grouped_conv2d::get_examples_large_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_quant8, grouped_conv2d::CreateModel_large_nchw_quant8)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nchw_quant8_weight_as_input,
            grouped_conv2d::is_ignored_large_nchw_quant8_weight_as_input,
            grouped_conv2d::get_examples_large_nchw_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_quant8_weight_as_input, grouped_conv2d::CreateModel_large_nchw_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_channelQuant8) {
    execute(grouped_conv2d::CreateModel_large_nchw_channelQuant8,
            grouped_conv2d::is_ignored_large_nchw_channelQuant8,
            grouped_conv2d::get_examples_large_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_channelQuant8, grouped_conv2d::CreateModel_large_nchw_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_large_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_large_nchw_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_large_nchw_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_float16) {
    execute(grouped_conv2d::CreateModel_large_nchw_float16,
            grouped_conv2d::is_ignored_large_nchw_float16,
            grouped_conv2d::get_examples_large_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_float16, grouped_conv2d::CreateModel_large_nchw_float16)

TEST_F(GeneratedTests, grouped_conv2d_large_nchw_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_nchw_float16_weight_as_input,
            grouped_conv2d::is_ignored_large_nchw_float16_weight_as_input,
            grouped_conv2d::get_examples_large_nchw_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_large_nchw_float16_weight_as_input, grouped_conv2d::CreateModel_large_nchw_float16_weight_as_input)

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_relaxed) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_relaxed,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_relaxed,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_quant8) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_quant8,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_quant8,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_quant8_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_quant8_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_channelQuant8) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_float16) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_float16,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_float16,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nhwc_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nhwc_float16_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nhwc_float16_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nhwc_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_relaxed) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_relaxed,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_relaxed,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_relaxed_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_quant8) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_quant8,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_quant8,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_quant8_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_quant8_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_channelQuant8) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_channelQuant8,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_channelQuant8,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_float16) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_float16,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_float16,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_large_dynamic_output_shape_nchw_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_large_dynamic_output_shape_nchw_float16_weight_as_input,
            grouped_conv2d::is_ignored_large_dynamic_output_shape_nchw_float16_weight_as_input,
            grouped_conv2d::get_examples_large_dynamic_output_shape_nchw_float16_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc) {
    execute(grouped_conv2d::CreateModel_channel_nhwc,
            grouped_conv2d::is_ignored_channel_nhwc,
            grouped_conv2d::get_examples_channel_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc, grouped_conv2d::CreateModel_channel_nhwc)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_weight_as_input,
            grouped_conv2d::is_ignored_channel_nhwc_weight_as_input,
            grouped_conv2d::get_examples_channel_nhwc_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_weight_as_input, grouped_conv2d::CreateModel_channel_nhwc_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_relaxed) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_relaxed,
            grouped_conv2d::is_ignored_channel_nhwc_relaxed,
            grouped_conv2d::get_examples_channel_nhwc_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_channel_nhwc_relaxed_weight_as_input,
            grouped_conv2d::get_examples_channel_nhwc_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_quant8) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_quant8,
            grouped_conv2d::is_ignored_channel_nhwc_quant8,
            grouped_conv2d::get_examples_channel_nhwc_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_quant8, grouped_conv2d::CreateModel_channel_nhwc_quant8)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_quant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_nhwc_quant8_weight_as_input,
            grouped_conv2d::get_examples_channel_nhwc_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_quant8_weight_as_input, grouped_conv2d::CreateModel_channel_nhwc_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_channelQuant8) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_channelQuant8,
            grouped_conv2d::is_ignored_channel_nhwc_channelQuant8,
            grouped_conv2d::get_examples_channel_nhwc_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_channelQuant8, grouped_conv2d::CreateModel_channel_nhwc_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_channel_nhwc_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_channel_nhwc_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_float16) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_float16,
            grouped_conv2d::is_ignored_channel_nhwc_float16,
            grouped_conv2d::get_examples_channel_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_float16, grouped_conv2d::CreateModel_channel_nhwc_float16)

TEST_F(GeneratedTests, grouped_conv2d_channel_nhwc_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nhwc_float16_weight_as_input,
            grouped_conv2d::is_ignored_channel_nhwc_float16_weight_as_input,
            grouped_conv2d::get_examples_channel_nhwc_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nhwc_float16_weight_as_input, grouped_conv2d::CreateModel_channel_nhwc_float16_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw) {
    execute(grouped_conv2d::CreateModel_channel_nchw,
            grouped_conv2d::is_ignored_channel_nchw,
            grouped_conv2d::get_examples_channel_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw, grouped_conv2d::CreateModel_channel_nchw)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nchw_weight_as_input,
            grouped_conv2d::is_ignored_channel_nchw_weight_as_input,
            grouped_conv2d::get_examples_channel_nchw_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_weight_as_input, grouped_conv2d::CreateModel_channel_nchw_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_relaxed) {
    execute(grouped_conv2d::CreateModel_channel_nchw_relaxed,
            grouped_conv2d::is_ignored_channel_nchw_relaxed,
            grouped_conv2d::get_examples_channel_nchw_relaxed());
}

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nchw_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_channel_nchw_relaxed_weight_as_input,
            grouped_conv2d::get_examples_channel_nchw_relaxed_weight_as_input());
}

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_quant8) {
    execute(grouped_conv2d::CreateModel_channel_nchw_quant8,
            grouped_conv2d::is_ignored_channel_nchw_quant8,
            grouped_conv2d::get_examples_channel_nchw_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_quant8, grouped_conv2d::CreateModel_channel_nchw_quant8)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nchw_quant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_nchw_quant8_weight_as_input,
            grouped_conv2d::get_examples_channel_nchw_quant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_quant8_weight_as_input, grouped_conv2d::CreateModel_channel_nchw_quant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_channelQuant8) {
    execute(grouped_conv2d::CreateModel_channel_nchw_channelQuant8,
            grouped_conv2d::is_ignored_channel_nchw_channelQuant8,
            grouped_conv2d::get_examples_channel_nchw_channelQuant8());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_channelQuant8, grouped_conv2d::CreateModel_channel_nchw_channelQuant8)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_channel_nchw_channelQuant8_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_channelQuant8_weight_as_input, grouped_conv2d::CreateModel_channel_nchw_channelQuant8_weight_as_input)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_float16) {
    execute(grouped_conv2d::CreateModel_channel_nchw_float16,
            grouped_conv2d::is_ignored_channel_nchw_float16,
            grouped_conv2d::get_examples_channel_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_float16, grouped_conv2d::CreateModel_channel_nchw_float16)

TEST_F(GeneratedTests, grouped_conv2d_channel_nchw_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_nchw_float16_weight_as_input,
            grouped_conv2d::is_ignored_channel_nchw_float16_weight_as_input,
            grouped_conv2d::get_examples_channel_nchw_float16_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, grouped_conv2d_channel_nchw_float16_weight_as_input, grouped_conv2d::CreateModel_channel_nchw_float16_weight_as_input)

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_relaxed) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_relaxed,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_relaxed,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_quant8) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_quant8,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_quant8,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_quant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_quant8_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_channelQuant8) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_float16) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_float16,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_float16,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nhwc_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nhwc_float16_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nhwc_float16_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nhwc_float16_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_relaxed) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_relaxed,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_relaxed,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_relaxed_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_relaxed_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_relaxed_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_relaxed_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_quant8) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_quant8,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_quant8,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_quant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_quant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_quant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_quant8_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_quant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_channelQuant8) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_channelQuant8,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_channelQuant8,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_channelQuant8());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_float16) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_float16,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_float16,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_float16());
}

TEST_F(DynamicOutputShapeTest, grouped_conv2d_channel_dynamic_output_shape_nchw_float16_weight_as_input) {
    execute(grouped_conv2d::CreateModel_channel_dynamic_output_shape_nchw_float16_weight_as_input,
            grouped_conv2d::is_ignored_channel_dynamic_output_shape_nchw_float16_weight_as_input,
            grouped_conv2d::get_examples_channel_dynamic_output_shape_nchw_float16_weight_as_input());
}

