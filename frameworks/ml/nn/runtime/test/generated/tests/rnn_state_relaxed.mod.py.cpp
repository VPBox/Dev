// clang-format off
// Generated file (from: rnn_state_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace rnn_state_relaxed {
// Generated rnn_state_relaxed test
#include "generated/examples/rnn_state_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/rnn_state_relaxed.model.cpp"
} // namespace rnn_state_relaxed

TEST_F(GeneratedTests, rnn_state_relaxed) {
    execute(rnn_state_relaxed::CreateModel,
            rnn_state_relaxed::is_ignored,
            rnn_state_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, rnn_state_relaxed_dynamic_output_shape) {
    execute(rnn_state_relaxed::CreateModel_dynamic_output_shape,
            rnn_state_relaxed::is_ignored_dynamic_output_shape,
            rnn_state_relaxed::get_examples_dynamic_output_shape());
}

