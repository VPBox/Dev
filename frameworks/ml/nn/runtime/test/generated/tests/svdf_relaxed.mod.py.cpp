// clang-format off
// Generated file (from: svdf_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf_relaxed {
// Generated svdf_relaxed test
#include "generated/examples/svdf_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/svdf_relaxed.model.cpp"
} // namespace svdf_relaxed

TEST_F(GeneratedTests, svdf_relaxed) {
    execute(svdf_relaxed::CreateModel,
            svdf_relaxed::is_ignored,
            svdf_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, svdf_relaxed_dynamic_output_shape) {
    execute(svdf_relaxed::CreateModel_dynamic_output_shape,
            svdf_relaxed::is_ignored_dynamic_output_shape,
            svdf_relaxed::get_examples_dynamic_output_shape());
}

