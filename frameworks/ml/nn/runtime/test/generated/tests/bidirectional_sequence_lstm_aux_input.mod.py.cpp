// clang-format off
// Generated file (from: bidirectional_sequence_lstm_aux_input.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm_aux_input {
// Generated bidirectional_sequence_lstm_aux_input test
#include "generated/examples/bidirectional_sequence_lstm_aux_input.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm_aux_input.model.cpp"
} // namespace bidirectional_sequence_lstm_aux_input

TEST_F(GeneratedTests, bidirectional_sequence_lstm_aux_input_blackbox) {
    execute(bidirectional_sequence_lstm_aux_input::CreateModel,
            bidirectional_sequence_lstm_aux_input::is_ignored,
            bidirectional_sequence_lstm_aux_input::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_aux_input_blackbox, bidirectional_sequence_lstm_aux_input::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_aux_input_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm_aux_input::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm_aux_input::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm_aux_input::get_examples_blackbox_dynamic_output_shape());
}

