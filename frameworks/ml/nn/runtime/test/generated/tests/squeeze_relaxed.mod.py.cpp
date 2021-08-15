// clang-format off
// Generated file (from: squeeze_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace squeeze_relaxed {
// Generated squeeze_relaxed test
#include "generated/examples/squeeze_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/squeeze_relaxed.model.cpp"
} // namespace squeeze_relaxed

TEST_F(GeneratedTests, squeeze_relaxed) {
    execute(squeeze_relaxed::CreateModel,
            squeeze_relaxed::is_ignored,
            squeeze_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, squeeze_relaxed_dynamic_output_shape) {
    execute(squeeze_relaxed::CreateModel_dynamic_output_shape,
            squeeze_relaxed::is_ignored_dynamic_output_shape,
            squeeze_relaxed::get_examples_dynamic_output_shape());
}

