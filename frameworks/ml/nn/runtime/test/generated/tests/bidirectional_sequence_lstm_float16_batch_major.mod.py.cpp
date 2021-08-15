// clang-format off
// Generated file (from: bidirectional_sequence_lstm_float16_batch_major.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm_float16_batch_major {
// Generated bidirectional_sequence_lstm_float16_batch_major test
#include "generated/examples/bidirectional_sequence_lstm_float16_batch_major.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm_float16_batch_major.model.cpp"
} // namespace bidirectional_sequence_lstm_float16_batch_major

TEST_F(GeneratedTests, bidirectional_sequence_lstm_float16_batch_major_blackbox) {
    execute(bidirectional_sequence_lstm_float16_batch_major::CreateModel,
            bidirectional_sequence_lstm_float16_batch_major::is_ignored,
            bidirectional_sequence_lstm_float16_batch_major::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_float16_batch_major_blackbox, bidirectional_sequence_lstm_float16_batch_major::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_float16_batch_major_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm_float16_batch_major::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm_float16_batch_major::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm_float16_batch_major::get_examples_blackbox_dynamic_output_shape());
}

