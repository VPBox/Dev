// clang-format off
// Generated file (from: max_pool_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace max_pool_float_1_relaxed {
// Generated max_pool_float_1_relaxed test
#include "generated/examples/max_pool_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/max_pool_float_1_relaxed.model.cpp"
} // namespace max_pool_float_1_relaxed

TEST_F(GeneratedTests, max_pool_float_1_relaxed) {
    execute(max_pool_float_1_relaxed::CreateModel,
            max_pool_float_1_relaxed::is_ignored,
            max_pool_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, max_pool_float_1_relaxed_dynamic_output_shape) {
    execute(max_pool_float_1_relaxed::CreateModel_dynamic_output_shape,
            max_pool_float_1_relaxed::is_ignored_dynamic_output_shape,
            max_pool_float_1_relaxed::get_examples_dynamic_output_shape());
}

