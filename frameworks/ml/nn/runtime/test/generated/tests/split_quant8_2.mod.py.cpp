// clang-format off
// Generated file (from: split_quant8_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace split_quant8_2 {
// Generated split_quant8_2 test
#include "generated/examples/split_quant8_2.example.cpp"
// Generated model constructor
#include "generated/models/split_quant8_2.model.cpp"
} // namespace split_quant8_2

TEST_F(GeneratedTests, split_quant8_2) {
    execute(split_quant8_2::CreateModel,
            split_quant8_2::is_ignored,
            split_quant8_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, split_quant8_2, split_quant8_2::CreateModel)

TEST_F(GeneratedTests, split_quant8_2_relaxed) {
    execute(split_quant8_2::CreateModel_relaxed,
            split_quant8_2::is_ignored_relaxed,
            split_quant8_2::get_examples_relaxed());
}

TEST_F(DynamicOutputShapeTest, split_quant8_2_dynamic_output_shape) {
    execute(split_quant8_2::CreateModel_dynamic_output_shape,
            split_quant8_2::is_ignored_dynamic_output_shape,
            split_quant8_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, split_quant8_2_dynamic_output_shape_relaxed) {
    execute(split_quant8_2::CreateModel_dynamic_output_shape_relaxed,
            split_quant8_2::is_ignored_dynamic_output_shape_relaxed,
            split_quant8_2::get_examples_dynamic_output_shape_relaxed());
}

