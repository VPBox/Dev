// clang-format off
// Generated file (from: lstm_state2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm_state2 {
// Generated lstm_state2 test
#include "generated/examples/lstm_state2.example.cpp"
// Generated model constructor
#include "generated/models/lstm_state2.model.cpp"
} // namespace lstm_state2

TEST_F(GeneratedTests, lstm_state2) {
    execute(lstm_state2::CreateModel,
            lstm_state2::is_ignored,
            lstm_state2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lstm_state2, lstm_state2::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm_state2_dynamic_output_shape) {
    execute(lstm_state2::CreateModel_dynamic_output_shape,
            lstm_state2::is_ignored_dynamic_output_shape,
            lstm_state2::get_examples_dynamic_output_shape());
}

