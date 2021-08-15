// clang-format off
// Generated file (from: reduce_min.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reduce_min {
// Generated reduce_min test
#include "generated/examples/reduce_min.example.cpp"
// Generated model constructor
#include "generated/models/reduce_min.model.cpp"
} // namespace reduce_min

TEST_F(GeneratedTests, reduce_min) {
    execute(reduce_min::CreateModel,
            reduce_min::is_ignored,
            reduce_min::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min, reduce_min::CreateModel)

TEST_F(GeneratedTests, reduce_min_relaxed) {
    execute(reduce_min::CreateModel_relaxed,
            reduce_min::is_ignored_relaxed,
            reduce_min::get_examples_relaxed());
}

TEST_F(GeneratedTests, reduce_min_float16) {
    execute(reduce_min::CreateModel_float16,
            reduce_min::is_ignored_float16,
            reduce_min::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_float16, reduce_min::CreateModel_float16)

TEST_F(GeneratedTests, reduce_min_quant8) {
    execute(reduce_min::CreateModel_quant8,
            reduce_min::is_ignored_quant8,
            reduce_min::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_quant8, reduce_min::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape) {
    execute(reduce_min::CreateModel_dynamic_output_shape,
            reduce_min::is_ignored_dynamic_output_shape,
            reduce_min::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_relaxed) {
    execute(reduce_min::CreateModel_dynamic_output_shape_relaxed,
            reduce_min::is_ignored_dynamic_output_shape_relaxed,
            reduce_min::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_float16) {
    execute(reduce_min::CreateModel_dynamic_output_shape_float16,
            reduce_min::is_ignored_dynamic_output_shape_float16,
            reduce_min::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_quant8) {
    execute(reduce_min::CreateModel_dynamic_output_shape_quant8,
            reduce_min::is_ignored_dynamic_output_shape_quant8,
            reduce_min::get_examples_dynamic_output_shape_quant8());
}

TEST_F(GeneratedTests, reduce_min_2) {
    execute(reduce_min::CreateModel_2,
            reduce_min::is_ignored_2,
            reduce_min::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_2, reduce_min::CreateModel_2)

TEST_F(GeneratedTests, reduce_min_relaxed_2) {
    execute(reduce_min::CreateModel_relaxed_2,
            reduce_min::is_ignored_relaxed_2,
            reduce_min::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, reduce_min_float16_2) {
    execute(reduce_min::CreateModel_float16_2,
            reduce_min::is_ignored_float16_2,
            reduce_min::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_float16_2, reduce_min::CreateModel_float16_2)

TEST_F(GeneratedTests, reduce_min_quant8_2) {
    execute(reduce_min::CreateModel_quant8_2,
            reduce_min::is_ignored_quant8_2,
            reduce_min::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_quant8_2, reduce_min::CreateModel_quant8_2)

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_2) {
    execute(reduce_min::CreateModel_dynamic_output_shape_2,
            reduce_min::is_ignored_dynamic_output_shape_2,
            reduce_min::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_relaxed_2) {
    execute(reduce_min::CreateModel_dynamic_output_shape_relaxed_2,
            reduce_min::is_ignored_dynamic_output_shape_relaxed_2,
            reduce_min::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_float16_2) {
    execute(reduce_min::CreateModel_dynamic_output_shape_float16_2,
            reduce_min::is_ignored_dynamic_output_shape_float16_2,
            reduce_min::get_examples_dynamic_output_shape_float16_2());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_quant8_2) {
    execute(reduce_min::CreateModel_dynamic_output_shape_quant8_2,
            reduce_min::is_ignored_dynamic_output_shape_quant8_2,
            reduce_min::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(GeneratedTests, reduce_min_3) {
    execute(reduce_min::CreateModel_3,
            reduce_min::is_ignored_3,
            reduce_min::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_3, reduce_min::CreateModel_3)

TEST_F(GeneratedTests, reduce_min_relaxed_3) {
    execute(reduce_min::CreateModel_relaxed_3,
            reduce_min::is_ignored_relaxed_3,
            reduce_min::get_examples_relaxed_3());
}

TEST_F(GeneratedTests, reduce_min_float16_3) {
    execute(reduce_min::CreateModel_float16_3,
            reduce_min::is_ignored_float16_3,
            reduce_min::get_examples_float16_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_float16_3, reduce_min::CreateModel_float16_3)

TEST_F(GeneratedTests, reduce_min_quant8_3) {
    execute(reduce_min::CreateModel_quant8_3,
            reduce_min::is_ignored_quant8_3,
            reduce_min::get_examples_quant8_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_quant8_3, reduce_min::CreateModel_quant8_3)

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_3) {
    execute(reduce_min::CreateModel_dynamic_output_shape_3,
            reduce_min::is_ignored_dynamic_output_shape_3,
            reduce_min::get_examples_dynamic_output_shape_3());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_relaxed_3) {
    execute(reduce_min::CreateModel_dynamic_output_shape_relaxed_3,
            reduce_min::is_ignored_dynamic_output_shape_relaxed_3,
            reduce_min::get_examples_dynamic_output_shape_relaxed_3());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_float16_3) {
    execute(reduce_min::CreateModel_dynamic_output_shape_float16_3,
            reduce_min::is_ignored_dynamic_output_shape_float16_3,
            reduce_min::get_examples_dynamic_output_shape_float16_3());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_quant8_3) {
    execute(reduce_min::CreateModel_dynamic_output_shape_quant8_3,
            reduce_min::is_ignored_dynamic_output_shape_quant8_3,
            reduce_min::get_examples_dynamic_output_shape_quant8_3());
}

TEST_F(GeneratedTests, reduce_min_4) {
    execute(reduce_min::CreateModel_4,
            reduce_min::is_ignored_4,
            reduce_min::get_examples_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_4, reduce_min::CreateModel_4)

TEST_F(GeneratedTests, reduce_min_relaxed_4) {
    execute(reduce_min::CreateModel_relaxed_4,
            reduce_min::is_ignored_relaxed_4,
            reduce_min::get_examples_relaxed_4());
}

TEST_F(GeneratedTests, reduce_min_float16_4) {
    execute(reduce_min::CreateModel_float16_4,
            reduce_min::is_ignored_float16_4,
            reduce_min::get_examples_float16_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_float16_4, reduce_min::CreateModel_float16_4)

TEST_F(GeneratedTests, reduce_min_quant8_4) {
    execute(reduce_min::CreateModel_quant8_4,
            reduce_min::is_ignored_quant8_4,
            reduce_min::get_examples_quant8_4());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_min_quant8_4, reduce_min::CreateModel_quant8_4)

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_4) {
    execute(reduce_min::CreateModel_dynamic_output_shape_4,
            reduce_min::is_ignored_dynamic_output_shape_4,
            reduce_min::get_examples_dynamic_output_shape_4());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_relaxed_4) {
    execute(reduce_min::CreateModel_dynamic_output_shape_relaxed_4,
            reduce_min::is_ignored_dynamic_output_shape_relaxed_4,
            reduce_min::get_examples_dynamic_output_shape_relaxed_4());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_float16_4) {
    execute(reduce_min::CreateModel_dynamic_output_shape_float16_4,
            reduce_min::is_ignored_dynamic_output_shape_float16_4,
            reduce_min::get_examples_dynamic_output_shape_float16_4());
}

TEST_F(DynamicOutputShapeTest, reduce_min_dynamic_output_shape_quant8_4) {
    execute(reduce_min::CreateModel_dynamic_output_shape_quant8_4,
            reduce_min::is_ignored_dynamic_output_shape_quant8_4,
            reduce_min::get_examples_dynamic_output_shape_quant8_4());
}

