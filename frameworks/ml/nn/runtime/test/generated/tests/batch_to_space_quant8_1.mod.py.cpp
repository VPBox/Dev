// clang-format off
// Generated file (from: batch_to_space_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace batch_to_space_quant8_1 {
// Generated batch_to_space_quant8_1 test
#include "generated/examples/batch_to_space_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/batch_to_space_quant8_1.model.cpp"
} // namespace batch_to_space_quant8_1

TEST_F(GeneratedTests, batch_to_space_quant8_1) {
    execute(batch_to_space_quant8_1::CreateModel,
            batch_to_space_quant8_1::is_ignored,
            batch_to_space_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, batch_to_space_quant8_1, batch_to_space_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, batch_to_space_quant8_1_dynamic_output_shape) {
    execute(batch_to_space_quant8_1::CreateModel_dynamic_output_shape,
            batch_to_space_quant8_1::is_ignored_dynamic_output_shape,
            batch_to_space_quant8_1::get_examples_dynamic_output_shape());
}

