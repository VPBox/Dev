// clang-format off
// Generated file (from: sub_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sub_v1_2 {
// Generated sub_v1_2 test
#include "generated/examples/sub_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/sub_v1_2.model.cpp"
} // namespace sub_v1_2

TEST_F(GeneratedTests, sub_v1_2_none) {
    execute(sub_v1_2::CreateModel_none,
            sub_v1_2::is_ignored_none,
            sub_v1_2::get_examples_none());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_none, sub_v1_2::CreateModel_none)

TEST_F(GeneratedTests, sub_v1_2_relu) {
    execute(sub_v1_2::CreateModel_relu,
            sub_v1_2::is_ignored_relu,
            sub_v1_2::get_examples_relu());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_relu, sub_v1_2::CreateModel_relu)

TEST_F(GeneratedTests, sub_v1_2_relu1) {
    execute(sub_v1_2::CreateModel_relu1,
            sub_v1_2::is_ignored_relu1,
            sub_v1_2::get_examples_relu1());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_relu1, sub_v1_2::CreateModel_relu1)

TEST_F(GeneratedTests, sub_v1_2_relu6) {
    execute(sub_v1_2::CreateModel_relu6,
            sub_v1_2::is_ignored_relu6,
            sub_v1_2::get_examples_relu6());
}
TEST_AVAILABLE_SINCE(V1_1, sub_v1_2_relu6, sub_v1_2::CreateModel_relu6)

TEST_F(GeneratedTests, sub_v1_2_float16_none) {
    execute(sub_v1_2::CreateModel_float16_none,
            sub_v1_2::is_ignored_float16_none,
            sub_v1_2::get_examples_float16_none());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_float16_none, sub_v1_2::CreateModel_float16_none)

TEST_F(GeneratedTests, sub_v1_2_float16_relu) {
    execute(sub_v1_2::CreateModel_float16_relu,
            sub_v1_2::is_ignored_float16_relu,
            sub_v1_2::get_examples_float16_relu());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_float16_relu, sub_v1_2::CreateModel_float16_relu)

TEST_F(GeneratedTests, sub_v1_2_float16_relu1) {
    execute(sub_v1_2::CreateModel_float16_relu1,
            sub_v1_2::is_ignored_float16_relu1,
            sub_v1_2::get_examples_float16_relu1());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_float16_relu1, sub_v1_2::CreateModel_float16_relu1)

TEST_F(GeneratedTests, sub_v1_2_float16_relu6) {
    execute(sub_v1_2::CreateModel_float16_relu6,
            sub_v1_2::is_ignored_float16_relu6,
            sub_v1_2::get_examples_float16_relu6());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_float16_relu6, sub_v1_2::CreateModel_float16_relu6)

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_none) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_none,
            sub_v1_2::is_ignored_dynamic_output_shape_none,
            sub_v1_2::get_examples_dynamic_output_shape_none());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_relu) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_relu,
            sub_v1_2::is_ignored_dynamic_output_shape_relu,
            sub_v1_2::get_examples_dynamic_output_shape_relu());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_relu1) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_relu1,
            sub_v1_2::is_ignored_dynamic_output_shape_relu1,
            sub_v1_2::get_examples_dynamic_output_shape_relu1());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_relu6) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_relu6,
            sub_v1_2::is_ignored_dynamic_output_shape_relu6,
            sub_v1_2::get_examples_dynamic_output_shape_relu6());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_float16_none) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_float16_none,
            sub_v1_2::is_ignored_dynamic_output_shape_float16_none,
            sub_v1_2::get_examples_dynamic_output_shape_float16_none());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_float16_relu) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_float16_relu,
            sub_v1_2::is_ignored_dynamic_output_shape_float16_relu,
            sub_v1_2::get_examples_dynamic_output_shape_float16_relu());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_float16_relu1) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_float16_relu1,
            sub_v1_2::is_ignored_dynamic_output_shape_float16_relu1,
            sub_v1_2::get_examples_dynamic_output_shape_float16_relu1());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_dynamic_output_shape_float16_relu6) {
    execute(sub_v1_2::CreateModel_dynamic_output_shape_float16_relu6,
            sub_v1_2::is_ignored_dynamic_output_shape_float16_relu6,
            sub_v1_2::get_examples_dynamic_output_shape_float16_relu6());
}

TEST_F(GeneratedTests, sub_v1_2_quant8) {
    execute(sub_v1_2::CreateModel_quant8,
            sub_v1_2::is_ignored_quant8,
            sub_v1_2::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_quant8, sub_v1_2::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, sub_v1_2_quant8_dynamic_output_shape) {
    execute(sub_v1_2::CreateModel_quant8_dynamic_output_shape,
            sub_v1_2::is_ignored_quant8_dynamic_output_shape,
            sub_v1_2::get_examples_quant8_dynamic_output_shape());
}

TEST_F(GeneratedTests, sub_v1_2_zero_sized) {
    execute(sub_v1_2::CreateModel_zero_sized,
            sub_v1_2::is_ignored_zero_sized,
            sub_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_zero_sized, sub_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, sub_v1_2_zero_sized_relaxed) {
    execute(sub_v1_2::CreateModel_zero_sized_relaxed,
            sub_v1_2::is_ignored_zero_sized_relaxed,
            sub_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, sub_v1_2_zero_sized_quant8) {
    execute(sub_v1_2::CreateModel_zero_sized_quant8,
            sub_v1_2::is_ignored_zero_sized_quant8,
            sub_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_zero_sized_quant8, sub_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, sub_v1_2_zero_sized_float16) {
    execute(sub_v1_2::CreateModel_zero_sized_float16,
            sub_v1_2::is_ignored_zero_sized_float16,
            sub_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, sub_v1_2_zero_sized_float16, sub_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, sub_v1_2_zero_sized_dynamic_output_shape) {
    execute(sub_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            sub_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            sub_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(sub_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            sub_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            sub_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(sub_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            sub_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            sub_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, sub_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(sub_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            sub_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            sub_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

