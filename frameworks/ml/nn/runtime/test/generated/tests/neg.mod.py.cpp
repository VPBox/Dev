// clang-format off
// Generated file (from: neg.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace neg {
// Generated neg test
#include "generated/examples/neg.example.cpp"
// Generated model constructor
#include "generated/models/neg.model.cpp"
} // namespace neg

TEST_F(GeneratedTests, neg) {
    execute(neg::CreateModel,
            neg::is_ignored,
            neg::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, neg, neg::CreateModel)

TEST_F(GeneratedTests, neg_relaxed) {
    execute(neg::CreateModel_relaxed,
            neg::is_ignored_relaxed,
            neg::get_examples_relaxed());
}

TEST_F(GeneratedTests, neg_float16) {
    execute(neg::CreateModel_float16,
            neg::is_ignored_float16,
            neg::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, neg_float16, neg::CreateModel_float16)

TEST_F(GeneratedTests, neg_int32) {
    execute(neg::CreateModel_int32,
            neg::is_ignored_int32,
            neg::get_examples_int32());
}
TEST_AVAILABLE_SINCE(V1_2, neg_int32, neg::CreateModel_int32)

TEST_F(DynamicOutputShapeTest, neg_dynamic_output_shape) {
    execute(neg::CreateModel_dynamic_output_shape,
            neg::is_ignored_dynamic_output_shape,
            neg::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, neg_dynamic_output_shape_relaxed) {
    execute(neg::CreateModel_dynamic_output_shape_relaxed,
            neg::is_ignored_dynamic_output_shape_relaxed,
            neg::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, neg_dynamic_output_shape_float16) {
    execute(neg::CreateModel_dynamic_output_shape_float16,
            neg::is_ignored_dynamic_output_shape_float16,
            neg::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, neg_dynamic_output_shape_int32) {
    execute(neg::CreateModel_dynamic_output_shape_int32,
            neg::is_ignored_dynamic_output_shape_int32,
            neg::get_examples_dynamic_output_shape_int32());
}

