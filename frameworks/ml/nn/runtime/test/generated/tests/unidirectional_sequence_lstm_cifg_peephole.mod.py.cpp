// clang-format off
// Generated file (from: unidirectional_sequence_lstm_cifg_peephole.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace unidirectional_sequence_lstm_cifg_peephole {
// Generated unidirectional_sequence_lstm_cifg_peephole test
#include "generated/examples/unidirectional_sequence_lstm_cifg_peephole.example.cpp"
// Generated model constructor
#include "generated/models/unidirectional_sequence_lstm_cifg_peephole.model.cpp"
} // namespace unidirectional_sequence_lstm_cifg_peephole

TEST_F(GeneratedTests, unidirectional_sequence_lstm_cifg_peephole) {
    execute(unidirectional_sequence_lstm_cifg_peephole::CreateModel,
            unidirectional_sequence_lstm_cifg_peephole::is_ignored,
            unidirectional_sequence_lstm_cifg_peephole::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, unidirectional_sequence_lstm_cifg_peephole, unidirectional_sequence_lstm_cifg_peephole::CreateModel)

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_cifg_peephole_dynamic_output_shape) {
    execute(unidirectional_sequence_lstm_cifg_peephole::CreateModel_dynamic_output_shape,
            unidirectional_sequence_lstm_cifg_peephole::is_ignored_dynamic_output_shape,
            unidirectional_sequence_lstm_cifg_peephole::get_examples_dynamic_output_shape());
}

