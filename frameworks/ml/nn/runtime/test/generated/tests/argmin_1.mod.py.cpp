// clang-format off
// Generated file (from: argmin_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace argmin_1 {
// Generated argmin_1 test
#include "generated/examples/argmin_1.example.cpp"
// Generated model constructor
#include "generated/models/argmin_1.model.cpp"
} // namespace argmin_1

TEST_F(GeneratedTests, argmin_1) {
    execute(argmin_1::CreateModel,
            argmin_1::is_ignored,
            argmin_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_1, argmin_1::CreateModel)

TEST_F(GeneratedTests, argmin_1_relaxed) {
    execute(argmin_1::CreateModel_relaxed,
            argmin_1::is_ignored_relaxed,
            argmin_1::get_examples_relaxed());
}

TEST_F(GeneratedTests, argmin_1_float16) {
    execute(argmin_1::CreateModel_float16,
            argmin_1::is_ignored_float16,
            argmin_1::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_1_float16, argmin_1::CreateModel_float16)

TEST_F(GeneratedTests, argmin_1_int32) {
    execute(argmin_1::CreateModel_int32,
            argmin_1::is_ignored_int32,
            argmin_1::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_1_int32, argmin_1::CreateModel_int32)

TEST_F(GeneratedTests, argmin_1_quant8) {
    execute(argmin_1::CreateModel_quant8,
            argmin_1::is_ignored_quant8,
            argmin_1::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, argmin_1_quant8, argmin_1::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, argmin_1_dynamic_output_shape) {
    execute(argmin_1::CreateModel_dynamic_output_shape,
            argmin_1::is_ignored_dynamic_output_shape,
            argmin_1::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, argmin_1_dynamic_output_shape_relaxed) {
    execute(argmin_1::CreateModel_dynamic_output_shape_relaxed,
            argmin_1::is_ignored_dynamic_output_shape_relaxed,
            argmin_1::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, argmin_1_dynamic_output_shape_float16) {
    execute(argmin_1::CreateModel_dynamic_output_shape_float16,
            argmin_1::is_ignored_dynamic_output_shape_float16,
            argmin_1::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, argmin_1_dynamic_output_shape_int32) {
    execute(argmin_1::CreateModel_dynamic_output_shape_int32,
            argmin_1::is_ignored_dynamic_output_shape_int32,
            argmin_1::get_examples_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, argmin_1_dynamic_output_shape_quant8) {
    execute(argmin_1::CreateModel_dynamic_output_shape_quant8,
            argmin_1::is_ignored_dynamic_output_shape_quant8,
            argmin_1::get_examples_dynamic_output_shape_quant8());
}

