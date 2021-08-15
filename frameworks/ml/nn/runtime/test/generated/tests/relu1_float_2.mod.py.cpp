// clang-format off
// Generated file (from: relu1_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu1_float_2 {
// Generated relu1_float_2 test
#include "generated/examples/relu1_float_2.example.cpp"
// Generated model constructor
#include "generated/models/relu1_float_2.model.cpp"
} // namespace relu1_float_2

TEST_F(GeneratedTests, relu1_float_2) {
    execute(relu1_float_2::CreateModel,
            relu1_float_2::is_ignored,
            relu1_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, relu1_float_2, relu1_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, relu1_float_2_dynamic_output_shape) {
    execute(relu1_float_2::CreateModel_dynamic_output_shape,
            relu1_float_2::is_ignored_dynamic_output_shape,
            relu1_float_2::get_examples_dynamic_output_shape());
}

