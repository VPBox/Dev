// clang-format off
// Generated file (from: unidirectional_sequence_lstm_batch_major_peephole_projection_bias.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace unidirectional_sequence_lstm_batch_major_peephole_projection_bias {
// Generated unidirectional_sequence_lstm_batch_major_peephole_projection_bias test
#include "generated/examples/unidirectional_sequence_lstm_batch_major_peephole_projection_bias.example.cpp"
// Generated model constructor
#include "generated/models/unidirectional_sequence_lstm_batch_major_peephole_projection_bias.model.cpp"
} // namespace unidirectional_sequence_lstm_batch_major_peephole_projection_bias

TEST_F(GeneratedTests, unidirectional_sequence_lstm_batch_major_peephole_projection_bias) {
    execute(unidirectional_sequence_lstm_batch_major_peephole_projection_bias::CreateModel,
            unidirectional_sequence_lstm_batch_major_peephole_projection_bias::is_ignored,
            unidirectional_sequence_lstm_batch_major_peephole_projection_bias::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, unidirectional_sequence_lstm_batch_major_peephole_projection_bias, unidirectional_sequence_lstm_batch_major_peephole_projection_bias::CreateModel)

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_batch_major_peephole_projection_bias_dynamic_output_shape) {
    execute(unidirectional_sequence_lstm_batch_major_peephole_projection_bias::CreateModel_dynamic_output_shape,
            unidirectional_sequence_lstm_batch_major_peephole_projection_bias::is_ignored_dynamic_output_shape,
            unidirectional_sequence_lstm_batch_major_peephole_projection_bias::get_examples_dynamic_output_shape());
}

