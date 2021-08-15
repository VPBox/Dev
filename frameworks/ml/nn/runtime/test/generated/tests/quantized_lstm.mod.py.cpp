// clang-format off
// Generated file (from: quantized_lstm.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace quantized_lstm {
// Generated quantized_lstm test
#include "generated/examples/quantized_lstm.example.cpp"
// Generated model constructor
#include "generated/models/quantized_lstm.model.cpp"
} // namespace quantized_lstm

TEST_F(GeneratedTests, quantized_lstm) {
    execute(quantized_lstm::CreateModel,
            quantized_lstm::is_ignored,
            quantized_lstm::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, quantized_lstm, quantized_lstm::CreateModel)

TEST_F(GeneratedTests, quantized_lstm_relaxed) {
    execute(quantized_lstm::CreateModel_relaxed,
            quantized_lstm::is_ignored_relaxed,
            quantized_lstm::get_examples_relaxed());
}

TEST_F(DynamicOutputShapeTest, quantized_lstm_dynamic_output_shape) {
    execute(quantized_lstm::CreateModel_dynamic_output_shape,
            quantized_lstm::is_ignored_dynamic_output_shape,
            quantized_lstm::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, quantized_lstm_dynamic_output_shape_relaxed) {
    execute(quantized_lstm::CreateModel_dynamic_output_shape_relaxed,
            quantized_lstm::is_ignored_dynamic_output_shape_relaxed,
            quantized_lstm::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(GeneratedTests, quantized_lstm_constant_weights) {
    execute(quantized_lstm::CreateModel_2,
            quantized_lstm::is_ignored_2,
            quantized_lstm::get_examples_constant_weights());
}
TEST_AVAILABLE_SINCE(V1_2, quantized_lstm_constant_weights, quantized_lstm::CreateModel_2)

TEST_F(GeneratedTests, quantized_lstm_constant_weights_relaxed) {
    execute(quantized_lstm::CreateModel_relaxed_2,
            quantized_lstm::is_ignored_relaxed_2,
            quantized_lstm::get_examples_constant_weights_relaxed());
}

TEST_F(DynamicOutputShapeTest, quantized_lstm_constant_weights_dynamic_output_shape) {
    execute(quantized_lstm::CreateModel_dynamic_output_shape_2,
            quantized_lstm::is_ignored_dynamic_output_shape_2,
            quantized_lstm::get_examples_constant_weights_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, quantized_lstm_constant_weights_dynamic_output_shape_relaxed) {
    execute(quantized_lstm::CreateModel_dynamic_output_shape_relaxed_2,
            quantized_lstm::is_ignored_dynamic_output_shape_relaxed_2,
            quantized_lstm::get_examples_constant_weights_dynamic_output_shape_relaxed());
}

