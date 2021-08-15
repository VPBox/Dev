// clang-format off
// Generated file (from: relu6_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu6_quant8_1 {
// Generated relu6_quant8_1 test
#include "generated/examples/relu6_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/relu6_quant8_1.model.cpp"
} // namespace relu6_quant8_1

TEST_F(GeneratedTests, relu6_quant8_1) {
    execute(relu6_quant8_1::CreateModel,
            relu6_quant8_1::is_ignored,
            relu6_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, relu6_quant8_1, relu6_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, relu6_quant8_1_dynamic_output_shape) {
    execute(relu6_quant8_1::CreateModel_dynamic_output_shape,
            relu6_quant8_1::is_ignored_dynamic_output_shape,
            relu6_quant8_1::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, relu6_quant8_1_2) {
    execute(relu6_quant8_1::CreateModel_2,
            relu6_quant8_1::is_ignored_2,
            relu6_quant8_1::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_0, relu6_quant8_1_2, relu6_quant8_1::CreateModel_2)

TEST_F(DynamicOutputShapeTest, relu6_quant8_1_dynamic_output_shape_2) {
    execute(relu6_quant8_1::CreateModel_dynamic_output_shape_2,
            relu6_quant8_1::is_ignored_dynamic_output_shape_2,
            relu6_quant8_1::get_examples_dynamic_output_shape_2());
}

