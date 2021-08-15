// clang-format off
// Generated file (from: svdf_bias_present.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf_bias_present {
// Generated svdf_bias_present test
#include "generated/examples/svdf_bias_present.example.cpp"
// Generated model constructor
#include "generated/models/svdf_bias_present.model.cpp"
} // namespace svdf_bias_present

TEST_F(GeneratedTests, svdf_bias_present) {
    execute(svdf_bias_present::CreateModel,
            svdf_bias_present::is_ignored,
            svdf_bias_present::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, svdf_bias_present, svdf_bias_present::CreateModel)

TEST_F(DynamicOutputShapeTest, svdf_bias_present_dynamic_output_shape) {
    execute(svdf_bias_present::CreateModel_dynamic_output_shape,
            svdf_bias_present::is_ignored_dynamic_output_shape,
            svdf_bias_present::get_examples_dynamic_output_shape());
}

