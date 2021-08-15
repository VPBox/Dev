// clang-format off
// Generated file (from: lstm_state.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm_state {
// Generated lstm_state test
#include "generated/examples/lstm_state.example.cpp"
// Generated model constructor
#include "generated/models/lstm_state.model.cpp"
} // namespace lstm_state

TEST_F(GeneratedTests, lstm_state) {
    execute(lstm_state::CreateModel,
            lstm_state::is_ignored,
            lstm_state::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lstm_state, lstm_state::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm_state_dynamic_output_shape) {
    execute(lstm_state::CreateModel_dynamic_output_shape,
            lstm_state::is_ignored_dynamic_output_shape,
            lstm_state::get_examples_dynamic_output_shape());
}

