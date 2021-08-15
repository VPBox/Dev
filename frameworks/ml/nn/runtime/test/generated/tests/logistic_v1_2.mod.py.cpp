// clang-format off
// Generated file (from: logistic_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace logistic_v1_2 {
// Generated logistic_v1_2 test
#include "generated/examples/logistic_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/logistic_v1_2.model.cpp"
} // namespace logistic_v1_2

TEST_F(GeneratedTests, logistic_v1_2) {
    execute(logistic_v1_2::CreateModel,
            logistic_v1_2::is_ignored,
            logistic_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, logistic_v1_2, logistic_v1_2::CreateModel)

TEST_F(DynamicOutputShapeTest, logistic_v1_2_dynamic_output_shape) {
    execute(logistic_v1_2::CreateModel_dynamic_output_shape,
            logistic_v1_2::is_ignored_dynamic_output_shape,
            logistic_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, logistic_v1_2_2) {
    execute(logistic_v1_2::CreateModel_2,
            logistic_v1_2::is_ignored_2,
            logistic_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, logistic_v1_2_2, logistic_v1_2::CreateModel_2)

TEST_F(DynamicOutputShapeTest, logistic_v1_2_dynamic_output_shape_2) {
    execute(logistic_v1_2::CreateModel_dynamic_output_shape_2,
            logistic_v1_2::is_ignored_dynamic_output_shape_2,
            logistic_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, logistic_v1_2_zero_sized) {
    execute(logistic_v1_2::CreateModel_zero_sized,
            logistic_v1_2::is_ignored_zero_sized,
            logistic_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, logistic_v1_2_zero_sized, logistic_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, logistic_v1_2_zero_sized_relaxed) {
    execute(logistic_v1_2::CreateModel_zero_sized_relaxed,
            logistic_v1_2::is_ignored_zero_sized_relaxed,
            logistic_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, logistic_v1_2_zero_sized_quant8) {
    execute(logistic_v1_2::CreateModel_zero_sized_quant8,
            logistic_v1_2::is_ignored_zero_sized_quant8,
            logistic_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, logistic_v1_2_zero_sized_quant8, logistic_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, logistic_v1_2_zero_sized_float16) {
    execute(logistic_v1_2::CreateModel_zero_sized_float16,
            logistic_v1_2::is_ignored_zero_sized_float16,
            logistic_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, logistic_v1_2_zero_sized_float16, logistic_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, logistic_v1_2_zero_sized_dynamic_output_shape) {
    execute(logistic_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            logistic_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            logistic_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, logistic_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(logistic_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            logistic_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            logistic_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, logistic_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(logistic_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            logistic_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            logistic_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, logistic_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(logistic_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            logistic_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            logistic_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

