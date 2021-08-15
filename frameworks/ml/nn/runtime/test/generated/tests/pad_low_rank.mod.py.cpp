// clang-format off
// Generated file (from: pad_low_rank.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_low_rank {
// Generated pad_low_rank test
#include "generated/examples/pad_low_rank.example.cpp"
// Generated model constructor
#include "generated/models/pad_low_rank.model.cpp"
} // namespace pad_low_rank

TEST_F(GeneratedTests, pad_low_rank) {
    execute(pad_low_rank::CreateModel,
            pad_low_rank::is_ignored,
            pad_low_rank::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, pad_low_rank, pad_low_rank::CreateModel)

TEST_F(GeneratedTests, pad_low_rank_float16) {
    execute(pad_low_rank::CreateModel_float16,
            pad_low_rank::is_ignored_float16,
            pad_low_rank::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, pad_low_rank_float16, pad_low_rank::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, pad_low_rank_dynamic_output_shape) {
    execute(pad_low_rank::CreateModel_dynamic_output_shape,
            pad_low_rank::is_ignored_dynamic_output_shape,
            pad_low_rank::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, pad_low_rank_dynamic_output_shape_float16) {
    execute(pad_low_rank::CreateModel_dynamic_output_shape_float16,
            pad_low_rank::is_ignored_dynamic_output_shape_float16,
            pad_low_rank::get_examples_dynamic_output_shape_float16());
}

