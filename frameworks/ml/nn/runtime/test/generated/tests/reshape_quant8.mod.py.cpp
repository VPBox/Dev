// clang-format off
// Generated file (from: reshape_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reshape_quant8 {
// Generated reshape_quant8 test
#include "generated/examples/reshape_quant8.example.cpp"
// Generated model constructor
#include "generated/models/reshape_quant8.model.cpp"
} // namespace reshape_quant8

TEST_F(GeneratedTests, reshape_quant8) {
    execute(reshape_quant8::CreateModel,
            reshape_quant8::is_ignored,
            reshape_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, reshape_quant8, reshape_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, reshape_quant8_dynamic_output_shape) {
    execute(reshape_quant8::CreateModel_dynamic_output_shape,
            reshape_quant8::is_ignored_dynamic_output_shape,
            reshape_quant8::get_examples_dynamic_output_shape());
}

