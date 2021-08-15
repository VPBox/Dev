// clang-format off
// Generated file (from: lstm3_state_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm3_state_float16 {
// Generated lstm3_state_float16 test
#include "generated/examples/lstm3_state_float16.example.cpp"
// Generated model constructor
#include "generated/models/lstm3_state_float16.model.cpp"
} // namespace lstm3_state_float16

TEST_F(GeneratedTests, lstm3_state_float16) {
    execute(lstm3_state_float16::CreateModel,
            lstm3_state_float16::is_ignored,
            lstm3_state_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, lstm3_state_float16, lstm3_state_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm3_state_float16_dynamic_output_shape) {
    execute(lstm3_state_float16::CreateModel_dynamic_output_shape,
            lstm3_state_float16::is_ignored_dynamic_output_shape,
            lstm3_state_float16::get_examples_dynamic_output_shape());
}

