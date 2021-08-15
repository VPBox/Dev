// clang-format off
// Generated file (from: unidirectional_sequence_lstm_layer_norm_cifg_peephole.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace unidirectional_sequence_lstm_layer_norm_cifg_peephole {
// Generated unidirectional_sequence_lstm_layer_norm_cifg_peephole test
#include "generated/examples/unidirectional_sequence_lstm_layer_norm_cifg_peephole.example.cpp"
// Generated model constructor
#include "generated/models/unidirectional_sequence_lstm_layer_norm_cifg_peephole.model.cpp"
} // namespace unidirectional_sequence_lstm_layer_norm_cifg_peephole

TEST_F(GeneratedTests, unidirectional_sequence_lstm_layer_norm_cifg_peephole) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, unidirectional_sequence_lstm_layer_norm_cifg_peephole, unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel)

TEST_F(GeneratedTests, unidirectional_sequence_lstm_layer_norm_cifg_peephole_relaxed) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_relaxed,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored_relaxed,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples_relaxed());
}

TEST_F(GeneratedTests, unidirectional_sequence_lstm_layer_norm_cifg_peephole_float16) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_float16,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored_float16,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, unidirectional_sequence_lstm_layer_norm_cifg_peephole_float16, unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_layer_norm_cifg_peephole_dynamic_output_shape) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_dynamic_output_shape,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored_dynamic_output_shape,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_layer_norm_cifg_peephole_dynamic_output_shape_relaxed) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_dynamic_output_shape_relaxed,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored_dynamic_output_shape_relaxed,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, unidirectional_sequence_lstm_layer_norm_cifg_peephole_dynamic_output_shape_float16) {
    execute(unidirectional_sequence_lstm_layer_norm_cifg_peephole::CreateModel_dynamic_output_shape_float16,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::is_ignored_dynamic_output_shape_float16,
            unidirectional_sequence_lstm_layer_norm_cifg_peephole::get_examples_dynamic_output_shape_float16());
}

