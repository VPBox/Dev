// clang-format off
// Generated file (from: bidirectional_sequence_lstm_norm_fw_output.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm_norm_fw_output {
// Generated bidirectional_sequence_lstm_norm_fw_output test
#include "generated/examples/bidirectional_sequence_lstm_norm_fw_output.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm_norm_fw_output.model.cpp"
} // namespace bidirectional_sequence_lstm_norm_fw_output

TEST_F(GeneratedTests, bidirectional_sequence_lstm_norm_fw_output_blackbox) {
    execute(bidirectional_sequence_lstm_norm_fw_output::CreateModel,
            bidirectional_sequence_lstm_norm_fw_output::is_ignored,
            bidirectional_sequence_lstm_norm_fw_output::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_norm_fw_output_blackbox, bidirectional_sequence_lstm_norm_fw_output::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_norm_fw_output_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm_norm_fw_output::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm_norm_fw_output::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm_norm_fw_output::get_examples_blackbox_dynamic_output_shape());
}

