// clang-format off
// Generated file (from: pad_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_float16 {
// Generated pad_float16 test
#include "generated/examples/pad_float16.example.cpp"
// Generated model constructor
#include "generated/models/pad_float16.model.cpp"
} // namespace pad_float16

TEST_F(GeneratedTests, pad_float16) {
    execute(pad_float16::CreateModel,
            pad_float16::is_ignored,
            pad_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, pad_float16, pad_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, pad_float16_dynamic_output_shape) {
    execute(pad_float16::CreateModel_dynamic_output_shape,
            pad_float16::is_ignored_dynamic_output_shape,
            pad_float16::get_examples_dynamic_output_shape());
}

