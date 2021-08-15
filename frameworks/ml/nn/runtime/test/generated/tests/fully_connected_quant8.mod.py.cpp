// clang-format off
// Generated file (from: fully_connected_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_quant8 {
// Generated fully_connected_quant8 test
#include "generated/examples/fully_connected_quant8.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_quant8.model.cpp"
} // namespace fully_connected_quant8

TEST_F(GeneratedTests, fully_connected_quant8) {
    execute(fully_connected_quant8::CreateModel,
            fully_connected_quant8::is_ignored,
            fully_connected_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, fully_connected_quant8, fully_connected_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, fully_connected_quant8_dynamic_output_shape) {
    execute(fully_connected_quant8::CreateModel_dynamic_output_shape,
            fully_connected_quant8::is_ignored_dynamic_output_shape,
            fully_connected_quant8::get_examples_dynamic_output_shape());
}

