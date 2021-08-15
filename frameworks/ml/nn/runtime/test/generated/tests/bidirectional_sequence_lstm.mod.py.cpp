// clang-format off
// Generated file (from: bidirectional_sequence_lstm.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm {
// Generated bidirectional_sequence_lstm test
#include "generated/examples/bidirectional_sequence_lstm.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm.model.cpp"
} // namespace bidirectional_sequence_lstm

TEST_F(GeneratedTests, bidirectional_sequence_lstm_blackbox) {
    execute(bidirectional_sequence_lstm::CreateModel,
            bidirectional_sequence_lstm::is_ignored,
            bidirectional_sequence_lstm::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_blackbox, bidirectional_sequence_lstm::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm::get_examples_blackbox_dynamic_output_shape());
}

