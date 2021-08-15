// clang-format off
// Generated file (from: softmax_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace softmax_float_2 {
// Generated softmax_float_2 test
#include "generated/examples/softmax_float_2.example.cpp"
// Generated model constructor
#include "generated/models/softmax_float_2.model.cpp"
} // namespace softmax_float_2

TEST_F(GeneratedTests, softmax_float_2) {
    execute(softmax_float_2::CreateModel,
            softmax_float_2::is_ignored,
            softmax_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_float_2, softmax_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, softmax_float_2_dynamic_output_shape) {
    execute(softmax_float_2::CreateModel_dynamic_output_shape,
            softmax_float_2::is_ignored_dynamic_output_shape,
            softmax_float_2::get_examples_dynamic_output_shape());
}

