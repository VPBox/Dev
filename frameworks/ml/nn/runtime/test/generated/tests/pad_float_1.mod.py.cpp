// clang-format off
// Generated file (from: pad_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_float_1 {
// Generated pad_float_1 test
#include "generated/examples/pad_float_1.example.cpp"
// Generated model constructor
#include "generated/models/pad_float_1.model.cpp"
} // namespace pad_float_1

TEST_F(GeneratedTests, pad_float_1) {
    execute(pad_float_1::CreateModel,
            pad_float_1::is_ignored,
            pad_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, pad_float_1, pad_float_1::CreateModel)

TEST_F(DynamicOutputShapeTest, pad_float_1_dynamic_output_shape) {
    execute(pad_float_1::CreateModel_dynamic_output_shape,
            pad_float_1::is_ignored_dynamic_output_shape,
            pad_float_1::get_examples_dynamic_output_shape());
}

