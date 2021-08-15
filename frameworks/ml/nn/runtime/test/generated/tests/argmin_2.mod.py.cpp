// clang-format off
// Generated file (from: argmin_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace argmin_2 {
// Generated argmin_2 test
#include "generated/examples/argmin_2.example.cpp"
// Generated model constructor
#include "generated/models/argmin_2.model.cpp"
} // namespace argmin_2

TEST_F(GeneratedTests, argmin_2) {
    execute(argmin_2::CreateModel,
            argmin_2::is_ignored,
            argmin_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_2, argmin_2::CreateModel)

TEST_F(GeneratedTests, argmin_2_relaxed) {
    execute(argmin_2::CreateModel_relaxed,
            argmin_2::is_ignored_relaxed,
            argmin_2::get_examples_relaxed());
}

TEST_F(GeneratedTests, argmin_2_float16) {
    execute(argmin_2::CreateModel_float16,
            argmin_2::is_ignored_float16,
            argmin_2::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_2_float16, argmin_2::CreateModel_float16)

TEST_F(GeneratedTests, argmin_2_int32) {
    execute(argmin_2::CreateModel_int32,
            argmin_2::is_ignored_int32,
            argmin_2::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_2_int32, argmin_2::CreateModel_int32)

TEST_F(GeneratedTests, argmin_2_quant8) {
    execute(argmin_2::CreateModel_quant8,
            argmin_2::is_ignored_quant8,
            argmin_2::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_2_quant8, argmin_2::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, argmin_2_dynamic_output_shape) {
    execute(argmin_2::CreateModel_dynamic_output_shape,
            argmin_2::is_ignored_dynamic_output_shape,
            argmin_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, argmin_2_dynamic_output_shape_relaxed) {
    execute(argmin_2::CreateModel_dynamic_output_shape_relaxed,
            argmin_2::is_ignored_dynamic_output_shape_relaxed,
            argmin_2::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, argmin_2_dynamic_output_shape_float16) {
    execute(argmin_2::CreateModel_dynamic_output_shape_float16,
            argmin_2::is_ignored_dynamic_output_shape_float16,
            argmin_2::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, argmin_2_dynamic_output_shape_int32) {
    execute(argmin_2::CreateModel_dynamic_output_shape_int32,
            argmin_2::is_ignored_dynamic_output_shape_int32,
            argmin_2::get_examples_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, argmin_2_dynamic_output_shape_quant8) {
    execute(argmin_2::CreateModel_dynamic_output_shape_quant8,
            argmin_2::is_ignored_dynamic_output_shape_quant8,
            argmin_2::get_examples_dynamic_output_shape_quant8());
}

