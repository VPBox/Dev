// clang-format off
// Generated file (from: relu6_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace relu6_float_2 {
// Generated relu6_float_2 test
#include "generated/examples/relu6_float_2.example.cpp"
// Generated model constructor
#include "generated/models/relu6_float_2.model.cpp"
} // namespace relu6_float_2

TEST_F(GeneratedTests, relu6_float_2) {
    execute(relu6_float_2::CreateModel,
            relu6_float_2::is_ignored,
            relu6_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, relu6_float_2, relu6_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, relu6_float_2_dynamic_output_shape) {
    execute(relu6_float_2::CreateModel_dynamic_output_shape,
            relu6_float_2::is_ignored_dynamic_output_shape,
            relu6_float_2::get_examples_dynamic_output_shape());
}

