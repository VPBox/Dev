// clang-format off
// Generated file (from: bidirectional_sequence_lstm_merge_outputs.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm_merge_outputs {
// Generated bidirectional_sequence_lstm_merge_outputs test
#include "generated/examples/bidirectional_sequence_lstm_merge_outputs.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm_merge_outputs.model.cpp"
} // namespace bidirectional_sequence_lstm_merge_outputs

TEST_F(GeneratedTests, bidirectional_sequence_lstm_merge_outputs_blackbox) {
    execute(bidirectional_sequence_lstm_merge_outputs::CreateModel,
            bidirectional_sequence_lstm_merge_outputs::is_ignored,
            bidirectional_sequence_lstm_merge_outputs::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_merge_outputs_blackbox, bidirectional_sequence_lstm_merge_outputs::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_merge_outputs_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm_merge_outputs::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm_merge_outputs::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm_merge_outputs::get_examples_blackbox_dynamic_output_shape());
}

