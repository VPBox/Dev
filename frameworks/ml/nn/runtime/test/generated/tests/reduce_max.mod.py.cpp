// clang-format off
// Generated file (from: reduce_max.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reduce_max {
// Generated reduce_max test
#include "generated/examples/reduce_max.example.cpp"
// Generated model constructor
#include "generated/models/reduce_max.model.cpp"
} // namespace reduce_max

TEST_F(GeneratedTests, reduce_max) {
    execute(reduce_max::CreateModel,
            reduce_max::is_ignored,
            reduce_max::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max, reduce_max::CreateModel)

TEST_F(GeneratedTests, reduce_max_relaxed) {
    execute(reduce_max::CreateModel_relaxed,
            reduce_max::is_ignored_relaxed,
            reduce_max::get_examples_relaxed());
}

TEST_F(GeneratedTests, reduce_max_float16) {
    execute(reduce_max::CreateModel_float16,
            reduce_max::is_ignored_float16,
            reduce_max::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_float16, reduce_max::CreateModel_float16)

TEST_F(GeneratedTests, reduce_max_quant8) {
    execute(reduce_max::CreateModel_quant8,
            reduce_max::is_ignored_quant8,
            reduce_max::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_quant8, reduce_max::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape) {
    execute(reduce_max::CreateModel_dynamic_output_shape,
            reduce_max::is_ignored_dynamic_output_shape,
            reduce_max::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_relaxed) {
    execute(reduce_max::CreateModel_dynamic_output_shape_relaxed,
            reduce_max::is_ignored_dynamic_output_shape_relaxed,
            reduce_max::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_float16) {
    execute(reduce_max::CreateModel_dynamic_output_shape_float16,
            reduce_max::is_ignored_dynamic_output_shape_float16,
            reduce_max::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_quant8) {
    execute(reduce_max::CreateModel_dynamic_output_shape_quant8,
            reduce_max::is_ignored_dynamic_output_shape_quant8,
            reduce_max::get_examples_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, reduce_max_2) {
    execute(reduce_max::CreateModel_2,
            reduce_max::is_ignored_2,
            reduce_max::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_2, reduce_max::CreateModel_2)

TEST_F(GeneratedTests, reduce_max_relaxed_2) {
    execute(reduce_max::CreateModel_relaxed_2,
            reduce_max::is_ignored_relaxed_2,
            reduce_max::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, reduce_max_float16_2) {
    execute(reduce_max::CreateModel_float16_2,
            reduce_max::is_ignored_float16_2,
            reduce_max::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_float16_2, reduce_max::CreateModel_float16_2)

TEST_F(GeneratedTests, reduce_max_quant8_2) {
    execute(reduce_max::CreateModel_quant8_2,
            reduce_max::is_ignored_quant8_2,
            reduce_max::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_quant8_2, reduce_max::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_2) {
    execute(reduce_max::CreateModel_dynamic_output_shape_2,
            reduce_max::is_ignored_dynamic_output_shape_2,
            reduce_max::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_relaxed_2) {
    execute(reduce_max::CreateModel_dynamic_output_shape_relaxed_2,
            reduce_max::is_ignored_dynamic_output_shape_relaxed_2,
            reduce_max::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_float16_2) {
    execute(reduce_max::CreateModel_dynamic_output_shape_float16_2,
            reduce_max::is_ignored_dynamic_output_shape_float16_2,
            reduce_max::get_examples_dynamic_output_shape_float16_2());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_quant8_2) {
    execute(reduce_max::CreateModel_dynamic_output_shape_quant8_2,
            reduce_max::is_ignored_dynamic_output_shape_quant8_2,
            reduce_max::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(GeneratedTests, reduce_max_3) {
    execute(reduce_max::CreateModel_3,
            reduce_max::is_ignored_3,
            reduce_max::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_3, reduce_max::CreateModel_3)

TEST_F(GeneratedTests, reduce_max_relaxed_3) {
    execute(reduce_max::CreateModel_relaxed_3,
            reduce_max::is_ignored_relaxed_3,
            reduce_max::get_examples_relaxed_3());
}

TEST_F(GeneratedTests, reduce_max_float16_3) {
    execute(reduce_max::CreateModel_float16_3,
            reduce_max::is_ignored_float16_3,
            reduce_max::get_examples_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_float16_3, reduce_max::CreateModel_float16_3)

TEST_F(GeneratedTests, reduce_max_quant8_3) {
    execute(reduce_max::CreateModel_quant8_3,
            reduce_max::is_ignored_quant8_3,
            reduce_max::get_examples_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_quant8_3, reduce_max::CreateModel_quant8_3)

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_3) {
    execute(reduce_max::CreateModel_dynamic_output_shape_3,
            reduce_max::is_ignored_dynamic_output_shape_3,
            reduce_max::get_examples_dynamic_output_shape_3());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_relaxed_3) {
    execute(reduce_max::CreateModel_dynamic_output_shape_relaxed_3,
            reduce_max::is_ignored_dynamic_output_shape_relaxed_3,
            reduce_max::get_examples_dynamic_output_shape_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_float16_3) {
    execute(reduce_max::CreateModel_dynamic_output_shape_float16_3,
            reduce_max::is_ignored_dynamic_output_shape_float16_3,
            reduce_max::get_examples_dynamic_output_shape_float16_3());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_quant8_3) {
    execute(reduce_max::CreateModel_dynamic_output_shape_quant8_3,
            reduce_max::is_ignored_dynamic_output_shape_quant8_3,
            reduce_max::get_examples_dynamic_output_shape_quant8_3());
}

TEST_F(GeneratedTests, reduce_max_4) {
    execute(reduce_max::CreateModel_4,
            reduce_max::is_ignored_4,
            reduce_max::get_examples_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_4, reduce_max::CreateModel_4)

TEST_F(GeneratedTests, reduce_max_relaxed_4) {
    execute(reduce_max::CreateModel_relaxed_4,
            reduce_max::is_ignored_relaxed_4,
            reduce_max::get_examples_relaxed_4());
}

TEST_F(GeneratedTests, reduce_max_float16_4) {
    execute(reduce_max::CreateModel_float16_4,
            reduce_max::is_ignored_float16_4,
            reduce_max::get_examples_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_float16_4, reduce_max::CreateModel_float16_4)

TEST_F(GeneratedTests, reduce_max_quant8_4) {
    execute(reduce_max::CreateModel_quant8_4,
            reduce_max::is_ignored_quant8_4,
            reduce_max::get_examples_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_max_quant8_4, reduce_max::CreateModel_quant8_4)

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_4) {
    execute(reduce_max::CreateModel_dynamic_output_shape_4,
            reduce_max::is_ignored_dynamic_output_shape_4,
            reduce_max::get_examples_dynamic_output_shape_4());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_relaxed_4) {
    execute(reduce_max::CreateModel_dynamic_output_shape_relaxed_4,
            reduce_max::is_ignored_dynamic_output_shape_relaxed_4,
            reduce_max::get_examples_dynamic_output_shape_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_float16_4) {
    execute(reduce_max::CreateModel_dynamic_output_shape_float16_4,
            reduce_max::is_ignored_dynamic_output_shape_float16_4,
            reduce_max::get_examples_dynamic_output_shape_float16_4());
}

TEST_F(DynamicOutputShapeTest, reduce_max_dynamic_output_shape_quant8_4) {
    execute(reduce_max::CreateModel_dynamic_output_shape_quant8_4,
            reduce_max::is_ignored_dynamic_output_shape_quant8_4,
            reduce_max::get_examples_dynamic_output_shape_quant8_4());
}

