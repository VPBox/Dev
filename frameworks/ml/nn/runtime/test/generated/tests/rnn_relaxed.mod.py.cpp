// clang-format off
// Generated file (from: rnn_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace rnn_relaxed {
// Generated rnn_relaxed test
#include "generated/examples/rnn_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/rnn_relaxed.model.cpp"
} // namespace rnn_relaxed

TEST_F(GeneratedTests, rnn_relaxed) {
    execute(rnn_relaxed::CreateModel,
            rnn_relaxed::is_ignored,
            rnn_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, rnn_relaxed_dynamic_output_shape) {
    execute(rnn_relaxed::CreateModel_dynamic_output_shape,
            rnn_relaxed::is_ignored_dynamic_output_shape,
            rnn_relaxed::get_examples_dynamic_output_shape());
}

