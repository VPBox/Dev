// clang-format off
// Generated file (from: bidirectional_sequence_lstm_cifg_peephole.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_lstm_cifg_peephole {
// Generated bidirectional_sequence_lstm_cifg_peephole test
#include "generated/examples/bidirectional_sequence_lstm_cifg_peephole.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_lstm_cifg_peephole.model.cpp"
} // namespace bidirectional_sequence_lstm_cifg_peephole

TEST_F(GeneratedTests, bidirectional_sequence_lstm_cifg_peephole_blackbox) {
    execute(bidirectional_sequence_lstm_cifg_peephole::CreateModel,
            bidirectional_sequence_lstm_cifg_peephole::is_ignored,
            bidirectional_sequence_lstm_cifg_peephole::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_lstm_cifg_peephole_blackbox, bidirectional_sequence_lstm_cifg_peephole::CreateModel)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_lstm_cifg_peephole_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_lstm_cifg_peephole::CreateModel_dynamic_output_shape,
            bidirectional_sequence_lstm_cifg_peephole::is_ignored_dynamic_output_shape,
            bidirectional_sequence_lstm_cifg_peephole::get_examples_blackbox_dynamic_output_shape());
}

