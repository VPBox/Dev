// clang-format off
// Generated file (from: l2_normalization_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_normalization_relaxed {
// Generated l2_normalization_relaxed test
#include "generated/examples/l2_normalization_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/l2_normalization_relaxed.model.cpp"
} // namespace l2_normalization_relaxed

TEST_F(GeneratedTests, l2_normalization_relaxed) {
    execute(l2_normalization_relaxed::CreateModel,
            l2_normalization_relaxed::is_ignored,
            l2_normalization_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, l2_normalization_relaxed_dynamic_output_shape) {
    execute(l2_normalization_relaxed::CreateModel_dynamic_output_shape,
            l2_normalization_relaxed::is_ignored_dynamic_output_shape,
            l2_normalization_relaxed::get_examples_dynamic_output_shape());
}

