// clang-format off
// Generated file (from: prelu.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace prelu {
// Generated prelu test
#include "generated/examples/prelu.example.cpp"
// Generated model constructor
#include "generated/models/prelu.model.cpp"
} // namespace prelu

TEST_F(GeneratedTests, prelu) {
    execute(prelu::CreateModel,
            prelu::is_ignored,
            prelu::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, prelu, prelu::CreateModel)

TEST_F(GeneratedTests, prelu_relaxed) {
    execute(prelu::CreateModel_relaxed,
            prelu::is_ignored_relaxed,
            prelu::get_examples_relaxed());
}

TEST_F(GeneratedTests, prelu_quant8) {
    execute(prelu::CreateModel_quant8,
            prelu::is_ignored_quant8,
            prelu::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_quant8, prelu::CreateModel_quant8)

TEST_F(GeneratedTests, prelu_quant8_2) {
    execute(prelu::CreateModel_quant8_2,
            prelu::is_ignored_quant8_2,
            prelu::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_quant8_2, prelu::CreateModel_quant8_2)

TEST_F(GeneratedTests, prelu_quant8_3) {
    execute(prelu::CreateModel_quant8_3,
            prelu::is_ignored_quant8_3,
            prelu::get_examples_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_quant8_3, prelu::CreateModel_quant8_3)

TEST_F(GeneratedTests, prelu_quant8_4) {
    execute(prelu::CreateModel_quant8_4,
            prelu::is_ignored_quant8_4,
            prelu::get_examples_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_quant8_4, prelu::CreateModel_quant8_4)

TEST_F(GeneratedTests, prelu_float16) {
    execute(prelu::CreateModel_float16,
            prelu::is_ignored_float16,
            prelu::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_float16, prelu::CreateModel_float16)

TEST_F(GeneratedTests, prelu_weight_as_input) {
    execute(prelu::CreateModel_weight_as_input,
            prelu::is_ignored_weight_as_input,
            prelu::get_examples_weight_as_input());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input, prelu::CreateModel_weight_as_input)

TEST_F(GeneratedTests, prelu_weight_as_input_relaxed) {
    execute(prelu::CreateModel_weight_as_input_relaxed,
            prelu::is_ignored_weight_as_input_relaxed,
            prelu::get_examples_weight_as_input_relaxed());
}

TEST_F(GeneratedTests, prelu_weight_as_input_quant8) {
    execute(prelu::CreateModel_weight_as_input_quant8,
            prelu::is_ignored_weight_as_input_quant8,
            prelu::get_examples_weight_as_input_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input_quant8, prelu::CreateModel_weight_as_input_quant8)

TEST_F(GeneratedTests, prelu_weight_as_input_quant8_2) {
    execute(prelu::CreateModel_weight_as_input_quant8_2,
            prelu::is_ignored_weight_as_input_quant8_2,
            prelu::get_examples_weight_as_input_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input_quant8_2, prelu::CreateModel_weight_as_input_quant8_2)

TEST_F(GeneratedTests, prelu_weight_as_input_quant8_3) {
    execute(prelu::CreateModel_weight_as_input_quant8_3,
            prelu::is_ignored_weight_as_input_quant8_3,
            prelu::get_examples_weight_as_input_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input_quant8_3, prelu::CreateModel_weight_as_input_quant8_3)

TEST_F(GeneratedTests, prelu_weight_as_input_quant8_4) {
    execute(prelu::CreateModel_weight_as_input_quant8_4,
            prelu::is_ignored_weight_as_input_quant8_4,
            prelu::get_examples_weight_as_input_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input_quant8_4, prelu::CreateModel_weight_as_input_quant8_4)

TEST_F(GeneratedTests, prelu_weight_as_input_float16) {
    execute(prelu::CreateModel_weight_as_input_float16,
            prelu::is_ignored_weight_as_input_float16,
            prelu::get_examples_weight_as_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, prelu_weight_as_input_float16, prelu::CreateModel_weight_as_input_float16)

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape) {
    execute(prelu::CreateModel_dynamic_output_shape,
            prelu::is_ignored_dynamic_output_shape,
            prelu::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_relaxed) {
    execute(prelu::CreateModel_dynamic_output_shape_relaxed,
            prelu::is_ignored_dynamic_output_shape_relaxed,
            prelu::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_quant8) {
    execute(prelu::CreateModel_dynamic_output_shape_quant8,
            prelu::is_ignored_dynamic_output_shape_quant8,
            prelu::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_quant8_2) {
    execute(prelu::CreateModel_dynamic_output_shape_quant8_2,
            prelu::is_ignored_dynamic_output_shape_quant8_2,
            prelu::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_quant8_3) {
    execute(prelu::CreateModel_dynamic_output_shape_quant8_3,
            prelu::is_ignored_dynamic_output_shape_quant8_3,
            prelu::get_examples_dynamic_output_shape_quant8_3());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_quant8_4) {
    execute(prelu::CreateModel_dynamic_output_shape_quant8_4,
            prelu::is_ignored_dynamic_output_shape_quant8_4,
            prelu::get_examples_dynamic_output_shape_quant8_4());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_float16) {
    execute(prelu::CreateModel_dynamic_output_shape_float16,
            prelu::is_ignored_dynamic_output_shape_float16,
            prelu::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input,
            prelu::is_ignored_dynamic_output_shape_weight_as_input,
            prelu::get_examples_dynamic_output_shape_weight_as_input());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_relaxed) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_relaxed,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_relaxed,
            prelu::get_examples_dynamic_output_shape_weight_as_input_relaxed());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_quant8) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_quant8,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_quant8,
            prelu::get_examples_dynamic_output_shape_weight_as_input_quant8());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_quant8_2) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_quant8_2,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_quant8_2,
            prelu::get_examples_dynamic_output_shape_weight_as_input_quant8_2());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_quant8_3) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_quant8_3,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_quant8_3,
            prelu::get_examples_dynamic_output_shape_weight_as_input_quant8_3());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_quant8_4) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_quant8_4,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_quant8_4,
            prelu::get_examples_dynamic_output_shape_weight_as_input_quant8_4());
}

TEST_F(DynamicOutputShapeTest, prelu_dynamic_output_shape_weight_as_input_float16) {
    execute(prelu::CreateModel_dynamic_output_shape_weight_as_input_float16,
            prelu::is_ignored_dynamic_output_shape_weight_as_input_float16,
            prelu::get_examples_dynamic_output_shape_weight_as_input_float16());
}

