// clang-format off
// Generated file (from: select_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace select_v1_2 {
// Generated select_v1_2 test
#include "generated/examples/select_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/select_v1_2.model.cpp"
} // namespace select_v1_2

TEST_F(GeneratedTests, select_v1_2_one_dim) {
    execute(select_v1_2::CreateModel,
            select_v1_2::is_ignored,
            select_v1_2::get_examples_one_dim());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_one_dim, select_v1_2::CreateModel)

TEST_F(GeneratedTests, select_v1_2_one_dim_int32) {
    execute(select_v1_2::CreateModel_int32,
            select_v1_2::is_ignored_int32,
            select_v1_2::get_examples_one_dim_int32());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_one_dim_int32, select_v1_2::CreateModel_int32)

TEST_F(GeneratedTests, select_v1_2_one_dim_float16) {
    execute(select_v1_2::CreateModel_float16,
            select_v1_2::is_ignored_float16,
            select_v1_2::get_examples_one_dim_float16());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_one_dim_float16, select_v1_2::CreateModel_float16)

TEST_F(GeneratedTests, select_v1_2_one_dim_relaxed) {
    execute(select_v1_2::CreateModel_relaxed,
            select_v1_2::is_ignored_relaxed,
            select_v1_2::get_examples_one_dim_relaxed());
}

TEST_F(GeneratedTests, select_v1_2_one_dim_quant8) {
    execute(select_v1_2::CreateModel_quant8,
            select_v1_2::is_ignored_quant8,
            select_v1_2::get_examples_one_dim_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_one_dim_quant8, select_v1_2::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, select_v1_2_one_dim_dynamic_output_shape) {
    execute(select_v1_2::CreateModel_dynamic_output_shape,
            select_v1_2::is_ignored_dynamic_output_shape,
            select_v1_2::get_examples_one_dim_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_one_dim_dynamic_output_shape_int32) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_int32,
            select_v1_2::is_ignored_dynamic_output_shape_int32,
            select_v1_2::get_examples_one_dim_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_one_dim_dynamic_output_shape_float16) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_float16,
            select_v1_2::is_ignored_dynamic_output_shape_float16,
            select_v1_2::get_examples_one_dim_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_one_dim_dynamic_output_shape_relaxed) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_relaxed,
            select_v1_2::is_ignored_dynamic_output_shape_relaxed,
            select_v1_2::get_examples_one_dim_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_one_dim_dynamic_output_shape_quant8) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_quant8,
            select_v1_2::is_ignored_dynamic_output_shape_quant8,
            select_v1_2::get_examples_one_dim_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, select_v1_2_two_dim) {
    execute(select_v1_2::CreateModel_2,
            select_v1_2::is_ignored_2,
            select_v1_2::get_examples_two_dim());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_two_dim, select_v1_2::CreateModel_2)

TEST_F(GeneratedTests, select_v1_2_two_dim_int32) {
    execute(select_v1_2::CreateModel_int32_2,
            select_v1_2::is_ignored_int32_2,
            select_v1_2::get_examples_two_dim_int32());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_two_dim_int32, select_v1_2::CreateModel_int32_2)

TEST_F(GeneratedTests, select_v1_2_two_dim_float16) {
    execute(select_v1_2::CreateModel_float16_2,
            select_v1_2::is_ignored_float16_2,
            select_v1_2::get_examples_two_dim_float16());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_two_dim_float16, select_v1_2::CreateModel_float16_2)

TEST_F(GeneratedTests, select_v1_2_two_dim_relaxed) {
    execute(select_v1_2::CreateModel_relaxed_2,
            select_v1_2::is_ignored_relaxed_2,
            select_v1_2::get_examples_two_dim_relaxed());
}

TEST_F(GeneratedTests, select_v1_2_two_dim_quant8) {
    execute(select_v1_2::CreateModel_quant8_2,
            select_v1_2::is_ignored_quant8_2,
            select_v1_2::get_examples_two_dim_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_two_dim_quant8, select_v1_2::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, select_v1_2_two_dim_dynamic_output_shape) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_2,
            select_v1_2::is_ignored_dynamic_output_shape_2,
            select_v1_2::get_examples_two_dim_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_two_dim_dynamic_output_shape_int32) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_int32_2,
            select_v1_2::is_ignored_dynamic_output_shape_int32_2,
            select_v1_2::get_examples_two_dim_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_two_dim_dynamic_output_shape_float16) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_float16_2,
            select_v1_2::is_ignored_dynamic_output_shape_float16_2,
            select_v1_2::get_examples_two_dim_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_two_dim_dynamic_output_shape_relaxed) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_relaxed_2,
            select_v1_2::is_ignored_dynamic_output_shape_relaxed_2,
            select_v1_2::get_examples_two_dim_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_two_dim_dynamic_output_shape_quant8) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_quant8_2,
            select_v1_2::is_ignored_dynamic_output_shape_quant8_2,
            select_v1_2::get_examples_two_dim_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, select_v1_2_five_dim) {
    execute(select_v1_2::CreateModel_3,
            select_v1_2::is_ignored_3,
            select_v1_2::get_examples_five_dim());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_five_dim, select_v1_2::CreateModel_3)

TEST_F(GeneratedTests, select_v1_2_five_dim_int32) {
    execute(select_v1_2::CreateModel_int32_3,
            select_v1_2::is_ignored_int32_3,
            select_v1_2::get_examples_five_dim_int32());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_five_dim_int32, select_v1_2::CreateModel_int32_3)

TEST_F(GeneratedTests, select_v1_2_five_dim_float16) {
    execute(select_v1_2::CreateModel_float16_3,
            select_v1_2::is_ignored_float16_3,
            select_v1_2::get_examples_five_dim_float16());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_five_dim_float16, select_v1_2::CreateModel_float16_3)

TEST_F(GeneratedTests, select_v1_2_five_dim_relaxed) {
    execute(select_v1_2::CreateModel_relaxed_3,
            select_v1_2::is_ignored_relaxed_3,
            select_v1_2::get_examples_five_dim_relaxed());
}

TEST_F(GeneratedTests, select_v1_2_five_dim_quant8) {
    execute(select_v1_2::CreateModel_quant8_3,
            select_v1_2::is_ignored_quant8_3,
            select_v1_2::get_examples_five_dim_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, select_v1_2_five_dim_quant8, select_v1_2::CreateModel_quant8_3)

TEST_F(DynamicOutputShapeTest, select_v1_2_five_dim_dynamic_output_shape) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_3,
            select_v1_2::is_ignored_dynamic_output_shape_3,
            select_v1_2::get_examples_five_dim_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_five_dim_dynamic_output_shape_int32) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_int32_3,
            select_v1_2::is_ignored_dynamic_output_shape_int32_3,
            select_v1_2::get_examples_five_dim_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_five_dim_dynamic_output_shape_float16) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_float16_3,
            select_v1_2::is_ignored_dynamic_output_shape_float16_3,
            select_v1_2::get_examples_five_dim_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_five_dim_dynamic_output_shape_relaxed) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_relaxed_3,
            select_v1_2::is_ignored_dynamic_output_shape_relaxed_3,
            select_v1_2::get_examples_five_dim_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, select_v1_2_five_dim_dynamic_output_shape_quant8) {
    execute(select_v1_2::CreateModel_dynamic_output_shape_quant8_3,
            select_v1_2::is_ignored_dynamic_output_shape_quant8_3,
            select_v1_2::get_examples_five_dim_dynamic_output_shape_quant8());
}

