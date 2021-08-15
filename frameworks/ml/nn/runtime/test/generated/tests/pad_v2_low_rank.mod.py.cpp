// clang-format off
// Generated file (from: pad_v2_low_rank.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_v2_low_rank {
// Generated pad_v2_low_rank test
#include "generated/examples/pad_v2_low_rank.example.cpp"
// Generated model constructor
#include "generated/models/pad_v2_low_rank.model.cpp"
} // namespace pad_v2_low_rank

TEST_F(GeneratedTests, pad_v2_low_rank) {
    execute(pad_v2_low_rank::CreateModel,
            pad_v2_low_rank::is_ignored,
            pad_v2_low_rank::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, pad_v2_low_rank, pad_v2_low_rank::CreateModel)

TEST_F(GeneratedTests, pad_v2_low_rank_float16) {
    execute(pad_v2_low_rank::CreateModel_float16,
            pad_v2_low_rank::is_ignored_float16,
            pad_v2_low_rank::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, pad_v2_low_rank_float16, pad_v2_low_rank::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, pad_v2_low_rank_dynamic_output_shape) {
    execute(pad_v2_low_rank::CreateModel_dynamic_output_shape,
            pad_v2_low_rank::is_ignored_dynamic_output_shape,
            pad_v2_low_rank::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, pad_v2_low_rank_dynamic_output_shape_float16) {
    execute(pad_v2_low_rank::CreateModel_dynamic_output_shape_float16,
            pad_v2_low_rank::is_ignored_dynamic_output_shape_float16,
            pad_v2_low_rank::get_examples_dynamic_output_shape_float16());
}

