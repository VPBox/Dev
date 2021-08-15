// clang-format off
// Generated file (from: unidirectional_sequence_lstm_f16_batch_major.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace unidirectional_sequence_lstm_f16_batch_major {
// Generated unidirectional_sequence_lstm_f16_batch_major test
#include "generated/examples/unidirectional_sequence_lstm_f16_batch_major.example.cpp"
// Generated model constructor
#include "generated/models/unidirectional_sequence_lstm_f16_batch_major.model.cpp"
} // namespace unidirectional_sequence_lstm_f16_batch_major

TEST_F(GeneratedTests, unidirectional_sequence_lstm_f16_batch_major) {
    execute(unidirectional_sequence_lstm_f16_batch_major::CreateModel,
            unidirectional_sequence_lstm_f16_batch_major::is_ignored,
            unidirectional_sequence_lstm_f16_batch_major::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, unidirectional_sequence_lstm_f16_batch_major, unidirectional_sequence_lstm_f16_batch_major::CreateModel)

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_f16_batch_major_dynamic_output_shape) {
    execute(unidirectional_sequence_lstm_f16_batch_major::CreateModel_dynamic_output_shape,
            unidirectional_sequence_lstm_f16_batch_major::is_ignored_dynamic_output_shape,
            unidirectional_sequence_lstm_f16_batch_major::get_examples_dynamic_output_shape());
}

