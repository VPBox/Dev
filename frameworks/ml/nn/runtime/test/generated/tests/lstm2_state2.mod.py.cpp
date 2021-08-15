// clang-format off
// Generated file (from: lstm2_state2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm2_state2 {
// Generated lstm2_state2 test
#include "generated/examples/lstm2_state2.example.cpp"
// Generated model constructor
#include "generated/models/lstm2_state2.model.cpp"
} // namespace lstm2_state2

TEST_F(GeneratedTests, lstm2_state2) {
    execute(lstm2_state2::CreateModel,
            lstm2_state2::is_ignored,
            lstm2_state2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lstm2_state2, lstm2_state2::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm2_state2_dynamic_output_shape) {
    execute(lstm2_state2::CreateModel_dynamic_output_shape,
            lstm2_state2::is_ignored_dynamic_output_shape,
            lstm2_state2::get_examples_dynamic_output_shape());
}

