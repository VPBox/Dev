// clang-format off
// Generated file (from: svdf_bias_present_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf_bias_present_relaxed {
// Generated svdf_bias_present_relaxed test
#include "generated/examples/svdf_bias_present_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/svdf_bias_present_relaxed.model.cpp"
} // namespace svdf_bias_present_relaxed

TEST_F(GeneratedTests, svdf_bias_present_relaxed) {
    execute(svdf_bias_present_relaxed::CreateModel,
            svdf_bias_present_relaxed::is_ignored,
            svdf_bias_present_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, svdf_bias_present_relaxed_dynamic_output_shape) {
    execute(svdf_bias_present_relaxed::CreateModel_dynamic_output_shape,
            svdf_bias_present_relaxed::is_ignored_dynamic_output_shape,
            svdf_bias_present_relaxed::get_examples_dynamic_output_shape());
}

