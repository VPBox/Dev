// clang-format off
// Generated file (from: argmax_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace argmax_1 {
// Generated argmax_1 test
#include "generated/examples/argmax_1.example.cpp"
// Generated model constructor
#include "generated/models/argmax_1.model.cpp"
} // namespace argmax_1

TEST_F(GeneratedTests, argmax_1) {
    execute(argmax_1::CreateModel,
            argmax_1::is_ignored,
            argmax_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_1, argmax_1::CreateModel)

TEST_F(GeneratedTests, argmax_1_relaxed) {
    execute(argmax_1::CreateModel_relaxed,
            argmax_1::is_ignored_relaxed,
            argmax_1::get_examples_relaxed());
}

TEST_F(GeneratedTests, argmax_1_float16) {
    execute(argmax_1::CreateModel_float16,
            argmax_1::is_ignored_float16,
            argmax_1::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_1_float16, argmax_1::CreateModel_float16)

TEST_F(GeneratedTests, argmax_1_int32) {
    execute(argmax_1::CreateModel_int32,
            argmax_1::is_ignored_int32,
            argmax_1::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_1_int32, argmax_1::CreateModel_int32)

TEST_F(GeneratedTests, argmax_1_quant8) {
    execute(argmax_1::CreateModel_quant8,
            argmax_1::is_ignored_quant8,
            argmax_1::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, argmax_1_quant8, argmax_1::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, argmax_1_dynamic_output_shape) {
    execute(argmax_1::CreateModel_dynamic_output_shape,
            argmax_1::is_ignored_dynamic_output_shape,
            argmax_1::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, argmax_1_dynamic_output_shape_relaxed) {
    execute(argmax_1::CreateModel_dynamic_output_shape_relaxed,
            argmax_1::is_ignored_dynamic_output_shape_relaxed,
            argmax_1::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, argmax_1_dynamic_output_shape_float16) {
    execute(argmax_1::CreateModel_dynamic_output_shape_float16,
            argmax_1::is_ignored_dynamic_output_shape_float16,
            argmax_1::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, argmax_1_dynamic_output_shape_int32) {
    execute(argmax_1::CreateModel_dynamic_output_shape_int32,
            argmax_1::is_ignored_dynamic_output_shape_int32,
            argmax_1::get_examples_dynamic_output_shape_int32());
}

TEST_F(DynamicOutputShapeTest, argmax_1_dynamic_output_shape_quant8) {
    execute(argmax_1::CreateModel_dynamic_output_shape_quant8,
            argmax_1::is_ignored_dynamic_output_shape_quant8,
            argmax_1::get_examples_dynamic_output_shape_quant8());
}

