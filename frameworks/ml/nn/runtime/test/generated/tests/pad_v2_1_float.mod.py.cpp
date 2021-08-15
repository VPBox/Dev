// clang-format off
// Generated file (from: pad_v2_1_float.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_v2_1_float {
// Generated pad_v2_1_float test
#include "generated/examples/pad_v2_1_float.example.cpp"
// Generated model constructor
#include "generated/models/pad_v2_1_float.model.cpp"
} // namespace pad_v2_1_float

TEST_F(GeneratedTests, pad_v2_1_float) {
    execute(pad_v2_1_float::CreateModel,
            pad_v2_1_float::is_ignored,
            pad_v2_1_float::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, pad_v2_1_float, pad_v2_1_float::CreateModel)

TEST_F(GeneratedTests, pad_v2_1_float_float16) {
    execute(pad_v2_1_float::CreateModel_float16,
            pad_v2_1_float::is_ignored_float16,
            pad_v2_1_float::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, pad_v2_1_float_float16, pad_v2_1_float::CreateModel_float16)

TEST_F(GeneratedTests, pad_v2_1_float_relaxed) {
    execute(pad_v2_1_float::CreateModel_relaxed,
            pad_v2_1_float::is_ignored_relaxed,
            pad_v2_1_float::get_examples_relaxed());
}

TEST_F(DynamicOutputShapeTest, pad_v2_1_float_dynamic_output_shape) {
    execute(pad_v2_1_float::CreateModel_dynamic_output_shape,
            pad_v2_1_float::is_ignored_dynamic_output_shape,
            pad_v2_1_float::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, pad_v2_1_float_dynamic_output_shape_float16) {
    execute(pad_v2_1_float::CreateModel_dynamic_output_shape_float16,
            pad_v2_1_float::is_ignored_dynamic_output_shape_float16,
            pad_v2_1_float::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, pad_v2_1_float_dynamic_output_shape_relaxed) {
    execute(pad_v2_1_float::CreateModel_dynamic_output_shape_relaxed,
            pad_v2_1_float::is_ignored_dynamic_output_shape_relaxed,
            pad_v2_1_float::get_examples_dynamic_output_shape_relaxed());
}

