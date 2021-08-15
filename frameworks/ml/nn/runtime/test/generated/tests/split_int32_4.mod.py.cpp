// clang-format off
// Generated file (from: split_int32_4.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace split_int32_4 {
// Generated split_int32_4 test
#include "generated/examples/split_int32_4.example.cpp"
// Generated model constructor
#include "generated/models/split_int32_4.model.cpp"
} // namespace split_int32_4

TEST_F(GeneratedTests, split_int32_4) {
    execute(split_int32_4::CreateModel,
            split_int32_4::is_ignored,
            split_int32_4::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, split_int32_4, split_int32_4::CreateModel)

TEST_F(GeneratedTests, split_int32_4_relaxed) {
    execute(split_int32_4::CreateModel_relaxed,
            split_int32_4::is_ignored_relaxed,
            split_int32_4::get_examples_relaxed());
}

TEST_F(DynamicOutputShapeTest, split_int32_4_dynamic_output_shape) {
    execute(split_int32_4::CreateModel_dynamic_output_shape,
            split_int32_4::is_ignored_dynamic_output_shape,
            split_int32_4::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, split_int32_4_dynamic_output_shape_relaxed) {
    execute(split_int32_4::CreateModel_dynamic_output_shape_relaxed,
            split_int32_4::is_ignored_dynamic_output_shape_relaxed,
            split_int32_4::get_examples_dynamic_output_shape_relaxed());
}

