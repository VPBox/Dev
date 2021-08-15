// clang-format off
// Generated file (from: squeeze_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace squeeze_float_1_relaxed {
// Generated squeeze_float_1_relaxed test
#include "generated/examples/squeeze_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/squeeze_float_1_relaxed.model.cpp"
} // namespace squeeze_float_1_relaxed

TEST_F(GeneratedTests, squeeze_float_1_relaxed) {
    execute(squeeze_float_1_relaxed::CreateModel,
            squeeze_float_1_relaxed::is_ignored,
            squeeze_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, squeeze_float_1_relaxed_dynamic_output_shape) {
    execute(squeeze_float_1_relaxed::CreateModel_dynamic_output_shape,
            squeeze_float_1_relaxed::is_ignored_dynamic_output_shape,
            squeeze_float_1_relaxed::get_examples_dynamic_output_shape());
}

