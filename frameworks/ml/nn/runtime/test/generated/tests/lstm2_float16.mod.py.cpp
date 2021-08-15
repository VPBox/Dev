// clang-format off
// Generated file (from: lstm2_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm2_float16 {
// Generated lstm2_float16 test
#include "generated/examples/lstm2_float16.example.cpp"
// Generated model constructor
#include "generated/models/lstm2_float16.model.cpp"
} // namespace lstm2_float16

TEST_F(GeneratedTests, lstm2_float16) {
    execute(lstm2_float16::CreateModel,
            lstm2_float16::is_ignored,
            lstm2_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, lstm2_float16, lstm2_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm2_float16_dynamic_output_shape) {
    execute(lstm2_float16::CreateModel_dynamic_output_shape,
            lstm2_float16::is_ignored_dynamic_output_shape,
            lstm2_float16::get_examples_dynamic_output_shape());
}

