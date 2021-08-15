// clang-format off
// Generated file (from: softmax_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace softmax_float_1 {
// Generated softmax_float_1 test
#include "generated/examples/softmax_float_1.example.cpp"
// Generated model constructor
#include "generated/models/softmax_float_1.model.cpp"
} // namespace softmax_float_1

TEST_F(GeneratedTests, softmax_float_1) {
    execute(softmax_float_1::CreateModel,
            softmax_float_1::is_ignored,
            softmax_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_float_1, softmax_float_1::CreateModel)

TEST_F(DynamicOutputShapeTest, softmax_float_1_dynamic_output_shape) {
    execute(softmax_float_1::CreateModel_dynamic_output_shape,
            softmax_float_1::is_ignored_dynamic_output_shape,
            softmax_float_1::get_examples_dynamic_output_shape());
}

