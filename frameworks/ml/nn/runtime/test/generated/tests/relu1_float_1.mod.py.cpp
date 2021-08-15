// clang-format off
// Generated file (from: relu1_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu1_float_1 {
// Generated relu1_float_1 test
#include "generated/examples/relu1_float_1.example.cpp"
// Generated model constructor
#include "generated/models/relu1_float_1.model.cpp"
} // namespace relu1_float_1

TEST_F(GeneratedTests, relu1_float_1) {
    execute(relu1_float_1::CreateModel,
            relu1_float_1::is_ignored,
            relu1_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, relu1_float_1, relu1_float_1::CreateModel)

TEST_F(DynamicOutputShapeTest, relu1_float_1_dynamic_output_shape) {
    execute(relu1_float_1::CreateModel_dynamic_output_shape,
            relu1_float_1::is_ignored_dynamic_output_shape,
            relu1_float_1::get_examples_dynamic_output_shape());
}

