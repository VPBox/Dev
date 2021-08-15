// clang-format off
// Generated file (from: argmax_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace argmax_3 {
// Generated argmax_3 test
#include "generated/examples/argmax_3.example.cpp"
// Generated model constructor
#include "generated/models/argmax_3.model.cpp"
} // namespace argmax_3

TEST_F(GeneratedTests, argmax_3) {
    execute(argmax_3::CreateModel,
            argmax_3::is_ignored,
            argmax_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_3, argmax_3::CreateModel)

TEST_F(GeneratedTests, argmax_3_relaxed) {
    execute(argmax_3::CreateModel_relaxed,
            argmax_3::is_ignored_relaxed,
            argmax_3::get_examples_relaxed());
}

TEST_F(GeneratedTests, argmax_3_float16) {
    execute(argmax_3::CreateModel_float16,
            argmax_3::is_ignored_float16,
            argmax_3::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_3_float16, argmax_3::CreateModel_float16)

TEST_F(GeneratedTests, argmax_3_int32) {
    execute(argmax_3::CreateModel_int32,
            argmax_3::is_ignored_int32,
            argmax_3::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_3_int32, argmax_3::CreateModel_int32)

TEST_F(GeneratedTests, argmax_3_quant8) {
    execute(argmax_3::CreateModel_quant8,
            argmax_3::is_ignored_quant8,
            argmax_3::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_3_quant8, argmax_3::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, argmax_3_dynamic_output_shape) {
    execute(argmax_3::CreateModel_dynamic_output_shape,
            argmax_3::is_ignored_dynamic_output_shape,
            argmax_3::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, argmax_3_dynamic_output_shape_relaxed) {
    execute(argmax_3::CreateModel_dynamic_output_shape_relaxed,
            argmax_3::is_ignored_dynamic_output_shape_relaxed,
            argmax_3::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, argmax_3_dynamic_output_shape_float16) {
    execute(argmax_3::CreateModel_dynamic_output_shape_float16,
            argmax_3::is_ignored_dynamic_output_shape_float16,
            argmax_3::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, argmax_3_dynamic_output_shape_int32) {
    execute(argmax_3::CreateModel_dynamic_output_shape_int32,
            argmax_3::is_ignored_dynamic_output_shape_int32,
            argmax_3::get_examples_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, argmax_3_dynamic_output_shape_quant8) {
    execute(argmax_3::CreateModel_dynamic_output_shape_quant8,
            argmax_3::is_ignored_dynamic_output_shape_quant8,
            argmax_3::get_examples_dynamic_output_shape_quant8());
}

