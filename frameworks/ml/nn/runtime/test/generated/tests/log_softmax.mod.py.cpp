// clang-format off
// Generated file (from: log_softmax.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace log_softmax {
// Generated log_softmax test
#include "generated/examples/log_softmax.example.cpp"
// Generated model constructor
#include "generated/models/log_softmax.model.cpp"
} // namespace log_softmax

TEST_F(GeneratedTests, log_softmax) {
    execute(log_softmax::CreateModel,
            log_softmax::is_ignored,
            log_softmax::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax, log_softmax::CreateModel)

TEST_F(GeneratedTests, log_softmax_relaxed) {
    execute(log_softmax::CreateModel_relaxed,
            log_softmax::is_ignored_relaxed,
            log_softmax::get_examples_relaxed());
}

TEST_F(GeneratedTests, log_softmax_float16) {
    execute(log_softmax::CreateModel_float16,
            log_softmax::is_ignored_float16,
            log_softmax::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_float16, log_softmax::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape) {
    execute(log_softmax::CreateModel_dynamic_output_shape,
            log_softmax::is_ignored_dynamic_output_shape,
            log_softmax::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_relaxed) {
    execute(log_softmax::CreateModel_dynamic_output_shape_relaxed,
            log_softmax::is_ignored_dynamic_output_shape_relaxed,
            log_softmax::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_float16) {
    execute(log_softmax::CreateModel_dynamic_output_shape_float16,
            log_softmax::is_ignored_dynamic_output_shape_float16,
            log_softmax::get_examples_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, log_softmax_2) {
    execute(log_softmax::CreateModel_2,
            log_softmax::is_ignored_2,
            log_softmax::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_2, log_softmax::CreateModel_2)

TEST_F(GeneratedTests, log_softmax_relaxed_2) {
    execute(log_softmax::CreateModel_relaxed_2,
            log_softmax::is_ignored_relaxed_2,
            log_softmax::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, log_softmax_float16_2) {
    execute(log_softmax::CreateModel_float16_2,
            log_softmax::is_ignored_float16_2,
            log_softmax::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_float16_2, log_softmax::CreateModel_float16_2)

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_2) {
    execute(log_softmax::CreateModel_dynamic_output_shape_2,
            log_softmax::is_ignored_dynamic_output_shape_2,
            log_softmax::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_relaxed_2) {
    execute(log_softmax::CreateModel_dynamic_output_shape_relaxed_2,
            log_softmax::is_ignored_dynamic_output_shape_relaxed_2,
            log_softmax::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_float16_2) {
    execute(log_softmax::CreateModel_dynamic_output_shape_float16_2,
            log_softmax::is_ignored_dynamic_output_shape_float16_2,
            log_softmax::get_examples_dynamic_output_shape_float16_2());
}

TEST_F(GeneratedTests, log_softmax_3) {
    execute(log_softmax::CreateModel_3,
            log_softmax::is_ignored_3,
            log_softmax::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_3, log_softmax::CreateModel_3)

TEST_F(GeneratedTests, log_softmax_relaxed_3) {
    execute(log_softmax::CreateModel_relaxed_3,
            log_softmax::is_ignored_relaxed_3,
            log_softmax::get_examples_relaxed_3());
}

TEST_F(GeneratedTests, log_softmax_float16_3) {
    execute(log_softmax::CreateModel_float16_3,
            log_softmax::is_ignored_float16_3,
            log_softmax::get_examples_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_float16_3, log_softmax::CreateModel_float16_3)

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_3) {
    execute(log_softmax::CreateModel_dynamic_output_shape_3,
            log_softmax::is_ignored_dynamic_output_shape_3,
            log_softmax::get_examples_dynamic_output_shape_3());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_relaxed_3) {
    execute(log_softmax::CreateModel_dynamic_output_shape_relaxed_3,
            log_softmax::is_ignored_dynamic_output_shape_relaxed_3,
            log_softmax::get_examples_dynamic_output_shape_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_float16_3) {
    execute(log_softmax::CreateModel_dynamic_output_shape_float16_3,
            log_softmax::is_ignored_dynamic_output_shape_float16_3,
            log_softmax::get_examples_dynamic_output_shape_float16_3());
}

TEST_F(GeneratedTests, log_softmax_4) {
    execute(log_softmax::CreateModel_4,
            log_softmax::is_ignored_4,
            log_softmax::get_examples_4());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_4, log_softmax::CreateModel_4)

TEST_F(GeneratedTests, log_softmax_relaxed_4) {
    execute(log_softmax::CreateModel_relaxed_4,
            log_softmax::is_ignored_relaxed_4,
            log_softmax::get_examples_relaxed_4());
}

TEST_F(GeneratedTests, log_softmax_float16_4) {
    execute(log_softmax::CreateModel_float16_4,
            log_softmax::is_ignored_float16_4,
            log_softmax::get_examples_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, log_softmax_float16_4, log_softmax::CreateModel_float16_4)

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_4) {
    execute(log_softmax::CreateModel_dynamic_output_shape_4,
            log_softmax::is_ignored_dynamic_output_shape_4,
            log_softmax::get_examples_dynamic_output_shape_4());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_relaxed_4) {
    execute(log_softmax::CreateModel_dynamic_output_shape_relaxed_4,
            log_softmax::is_ignored_dynamic_output_shape_relaxed_4,
            log_softmax::get_examples_dynamic_output_shape_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, log_softmax_dynamic_output_shape_float16_4) {
    execute(log_softmax::CreateModel_dynamic_output_shape_float16_4,
            log_softmax::is_ignored_dynamic_output_shape_float16_4,
            log_softmax::get_examples_dynamic_output_shape_float16_4());
}

