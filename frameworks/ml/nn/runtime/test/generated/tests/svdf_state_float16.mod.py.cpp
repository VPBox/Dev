// clang-format off
// Generated file (from: svdf_state_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf_state_float16 {
// Generated svdf_state_float16 test
#include "generated/examples/svdf_state_float16.example.cpp"
// Generated model constructor
#include "generated/models/svdf_state_float16.model.cpp"
} // namespace svdf_state_float16

TEST_F(GeneratedTests, svdf_state_float16) {
    execute(svdf_state_float16::CreateModel,
            svdf_state_float16::is_ignored,
            svdf_state_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, svdf_state_float16, svdf_state_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, svdf_state_float16_dynamic_output_shape) {
    execute(svdf_state_float16::CreateModel_dynamic_output_shape,
            svdf_state_float16::is_ignored_dynamic_output_shape,
            svdf_state_float16::get_examples_dynamic_output_shape());
}

