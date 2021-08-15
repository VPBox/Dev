// clang-format off
// Generated file (from: add_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace add_quant8 {
// Generated add_quant8 test
#include "generated/examples/add_quant8.example.cpp"
// Generated model constructor
#include "generated/models/add_quant8.model.cpp"
} // namespace add_quant8

TEST_F(GeneratedTests, add_quant8) {
    execute(add_quant8::CreateModel,
            add_quant8::is_ignored,
            add_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, add_quant8, add_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, add_quant8_dynamic_output_shape) {
    execute(add_quant8::CreateModel_dynamic_output_shape,
            add_quant8::is_ignored_dynamic_output_shape,
            add_quant8::get_examples_dynamic_output_shape());
}

