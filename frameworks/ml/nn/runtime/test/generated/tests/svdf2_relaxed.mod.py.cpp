// clang-format off
// Generated file (from: svdf2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf2_relaxed {
// Generated svdf2_relaxed test
#include "generated/examples/svdf2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/svdf2_relaxed.model.cpp"
} // namespace svdf2_relaxed

TEST_F(GeneratedTests, svdf2_relaxed) {
    execute(svdf2_relaxed::CreateModel,
            svdf2_relaxed::is_ignored,
            svdf2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, svdf2_relaxed_dynamic_output_shape) {
    execute(svdf2_relaxed::CreateModel_dynamic_output_shape,
            svdf2_relaxed::is_ignored_dynamic_output_shape,
            svdf2_relaxed::get_examples_dynamic_output_shape());
}

