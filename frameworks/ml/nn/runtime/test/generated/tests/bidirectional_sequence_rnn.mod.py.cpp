// clang-format off
// Generated file (from: bidirectional_sequence_rnn.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bidirectional_sequence_rnn {
// Generated bidirectional_sequence_rnn test
#include "generated/examples/bidirectional_sequence_rnn.example.cpp"
// Generated model constructor
#include "generated/models/bidirectional_sequence_rnn.model.cpp"
} // namespace bidirectional_sequence_rnn

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox) {
    execute(bidirectional_sequence_rnn::CreateModel,
            bidirectional_sequence_rnn::is_ignored,
            bidirectional_sequence_rnn::get_examples_blackbox());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox, bidirectional_sequence_rnn::CreateModel)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed,
            bidirectional_sequence_rnn::is_ignored_relaxed,
            bidirectional_sequence_rnn::get_examples_blackbox_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16,
            bidirectional_sequence_rnn::is_ignored_float16,
            bidirectional_sequence_rnn::get_examples_blackbox_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_float16, bidirectional_sequence_rnn::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape,
            bidirectional_sequence_rnn::get_examples_blackbox_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed,
            bidirectional_sequence_rnn::get_examples_blackbox_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16,
            bidirectional_sequence_rnn::get_examples_blackbox_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major) {
    execute(bidirectional_sequence_rnn::CreateModel_2,
            bidirectional_sequence_rnn::is_ignored_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_time_major, bidirectional_sequence_rnn::CreateModel_2)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed_2,
            bidirectional_sequence_rnn::is_ignored_relaxed_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16_2,
            bidirectional_sequence_rnn::is_ignored_float16_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_time_major_float16, bidirectional_sequence_rnn::CreateModel_float16_2)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_2,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed_2,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16_2,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16_2,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs) {
    execute(bidirectional_sequence_rnn::CreateModel_3,
            bidirectional_sequence_rnn::is_ignored_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs, bidirectional_sequence_rnn::CreateModel_3)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed_3,
            bidirectional_sequence_rnn::is_ignored_relaxed_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16_3,
            bidirectional_sequence_rnn::is_ignored_float16_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_float16, bidirectional_sequence_rnn::CreateModel_float16_3)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_3,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed_3,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_time_major_merge_outputs_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16_3,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16_3,
            bidirectional_sequence_rnn::get_examples_blackbox_time_major_merge_outputs_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_reversed_inputs) {
    execute(bidirectional_sequence_rnn::CreateModel_4,
            bidirectional_sequence_rnn::is_ignored_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_reversed_inputs, bidirectional_sequence_rnn::CreateModel_4)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_reversed_inputs_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed_4,
            bidirectional_sequence_rnn::is_ignored_relaxed_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_reversed_inputs_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16_4,
            bidirectional_sequence_rnn::is_ignored_float16_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_reversed_inputs_float16, bidirectional_sequence_rnn::CreateModel_float16_4)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_reversed_inputs_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_4,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_reversed_inputs_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed_4,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_reversed_inputs_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16_4,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16_4,
            bidirectional_sequence_rnn::get_examples_blackbox_reversed_inputs_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_aux_input) {
    execute(bidirectional_sequence_rnn::CreateModel_5,
            bidirectional_sequence_rnn::is_ignored_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_aux_input, bidirectional_sequence_rnn::CreateModel_5)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_aux_input_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed_5,
            bidirectional_sequence_rnn::is_ignored_relaxed_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_aux_input_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16_5,
            bidirectional_sequence_rnn::is_ignored_float16_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_aux_input_float16, bidirectional_sequence_rnn::CreateModel_float16_5)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_aux_input_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_5,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_aux_input_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed_5,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_aux_input_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16_5,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16_5,
            bidirectional_sequence_rnn::get_examples_blackbox_aux_input_dynamic_output_shape_float16());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_regular_and_aux_input) {
    execute(bidirectional_sequence_rnn::CreateModel_6,
            bidirectional_sequence_rnn::is_ignored_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_regular_and_aux_input, bidirectional_sequence_rnn::CreateModel_6)

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_relaxed_6,
            bidirectional_sequence_rnn::is_ignored_relaxed_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input_relaxed());
}

TEST_F(GeneratedTests, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_float16_6,
            bidirectional_sequence_rnn::is_ignored_float16_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_float16, bidirectional_sequence_rnn::CreateModel_float16_6)

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_dynamic_output_shape) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_6,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_dynamic_output_shape_relaxed) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_relaxed_6,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_relaxed_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bidirectional_sequence_rnn_blackbox_regular_and_aux_input_dynamic_output_shape_float16) {
    execute(bidirectional_sequence_rnn::CreateModel_dynamic_output_shape_float16_6,
            bidirectional_sequence_rnn::is_ignored_dynamic_output_shape_float16_6,
            bidirectional_sequence_rnn::get_examples_blackbox_regular_and_aux_input_dynamic_output_shape_float16());
}

