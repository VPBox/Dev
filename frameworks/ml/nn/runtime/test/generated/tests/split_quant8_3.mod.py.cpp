// clang-format off
// Generated file (from: split_quant8_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace split_quant8_3 {
// Generated split_quant8_3 test
#include "generated/examples/split_quant8_3.example.cpp"
// Generated model constructor
#include "generated/models/split_quant8_3.model.cpp"
} // namespace split_quant8_3

TEST_F(GeneratedTests, split_quant8_3) {
    execute(split_quant8_3::CreateModel,
            split_quant8_3::is_ignored,
            split_quant8_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, split_quant8_3, split_quant8_3::CreateModel)

TEST_F(DynamicOutputShapeTest, split_quant8_3_dynamic_output_shape) {
    execute(split_quant8_3::CreateModel_dynamic_output_shape,
            split_quant8_3::is_ignored_dynamic_output_shape,
            split_quant8_3::get_examples_dynamic_output_shape());
}

