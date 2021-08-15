// clang-format off
// Generated file (from: sub_v1_2_broadcast.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sub_v1_2_broadcast {
// Generated sub_v1_2_broadcast test
#include "generated/examples/sub_v1_2_broadcast.example.cpp"
// Generated model constructor
#include "generated/models/sub_v1_2_broadcast.model.cpp"
} // namespace sub_v1_2_broadcast

TEST_F(GeneratedTests, sub_v1_2_broadcast_none) {
    execute(sub_v1_2_broadcast::CreateModel_none,
            sub_v1_2_broadcast::is_ignored_none,
            sub_v1_2_broadcast::get_examples_none());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_broadcast_none, sub_v1_2_broadcast::CreateModel_none)

TEST_F(GeneratedTests, sub_v1_2_broadcast_relu) {
    execute(sub_v1_2_broadcast::CreateModel_relu,
            sub_v1_2_broadcast::is_ignored_relu,
            sub_v1_2_broadcast::get_examples_relu());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_broadcast_relu, sub_v1_2_broadcast::CreateModel_relu)

TEST_F(GeneratedTests, sub_v1_2_broadcast_relu1) {
    execute(sub_v1_2_broadcast::CreateModel_relu1,
            sub_v1_2_broadcast::is_ignored_relu1,
            sub_v1_2_broadcast::get_examples_relu1());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_broadcast_relu1, sub_v1_2_broadcast::CreateModel_relu1)

TEST_F(GeneratedTests, sub_v1_2_broadcast_relu6) {
    execute(sub_v1_2_broadcast::CreateModel_relu6,
            sub_v1_2_broadcast::is_ignored_relu6,
            sub_v1_2_broadcast::get_examples_relu6());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_broadcast_relu6, sub_v1_2_broadcast::CreateModel_relu6)

TEST_F(GeneratedTests, sub_v1_2_broadcast_float16_none) {
    execute(sub_v1_2_broadcast::CreateModel_float16_none,
            sub_v1_2_broadcast::is_ignored_float16_none,
            sub_v1_2_broadcast::get_examples_float16_none());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_broadcast_float16_none, sub_v1_2_broadcast::CreateModel_float16_none)

TEST_F(GeneratedTests, sub_v1_2_broadcast_float16_relu) {
    execute(sub_v1_2_broadcast::CreateModel_float16_relu,
            sub_v1_2_broadcast::is_ignored_float16_relu,
            sub_v1_2_broadcast::get_examples_float16_relu());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_broadcast_float16_relu, sub_v1_2_broadcast::CreateModel_float16_relu)

TEST_F(GeneratedTests, sub_v1_2_broadcast_float16_relu1) {
    execute(sub_v1_2_broadcast::CreateModel_float16_relu1,
            sub_v1_2_broadcast::is_ignored_float16_relu1,
            sub_v1_2_broadcast::get_examples_float16_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_broadcast_float16_relu1, sub_v1_2_broadcast::CreateModel_float16_relu1)

TEST_F(GeneratedTests, sub_v1_2_broadcast_float16_relu6) {
    execute(sub_v1_2_broadcast::CreateModel_float16_relu6,
            sub_v1_2_broadcast::is_ignored_float16_relu6,
            sub_v1_2_broadcast::get_examples_float16_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_broadcast_float16_relu6, sub_v1_2_broadcast::CreateModel_float16_relu6)

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_none) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_none,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_none,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_none());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_relu) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_relu,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_relu,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_relu());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_relu1) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_relu1,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_relu1,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_relu1());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_relu6) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_relu6,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_relu6,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_relu6());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_float16_none) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_float16_none,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_float16_none,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_float16_none());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_float16_relu) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_float16_relu,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_float16_relu,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_float16_relu());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_float16_relu1) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_float16_relu1,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_float16_relu1,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_float16_relu1());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_dynamic_output_shape_float16_relu6) {
    execute(sub_v1_2_broadcast::CreateModel_dynamic_output_shape_float16_relu6,
            sub_v1_2_broadcast::is_ignored_dynamic_output_shape_float16_relu6,
            sub_v1_2_broadcast::get_examples_dynamic_output_shape_float16_relu6());
}

TEST_F(GeneratedTests, sub_v1_2_broadcast_quant8) {
    execute(sub_v1_2_broadcast::CreateModel_quant8,
            sub_v1_2_broadcast::is_ignored_quant8,
            sub_v1_2_broadcast::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_broadcast_quant8, sub_v1_2_broadcast::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, sub_v1_2_broadcast_quant8_dynamic_output_shape) {
    execute(sub_v1_2_broadcast::CreateModel_quant8_dynamic_output_shape,
            sub_v1_2_broadcast::is_ignored_quant8_dynamic_output_shape,
            sub_v1_2_broadcast::get_examples_quant8_dynamic_output_shape());
}

