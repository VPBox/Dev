// clang-format off
// Generated file (from: svdf_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf_float16 {
// Generated svdf_float16 test
#include "generated/examples/svdf_float16.example.cpp"
// Generated model constructor
#include "generated/models/svdf_float16.model.cpp"
} // namespace svdf_float16

TEST_F(GeneratedTests, svdf_float16) {
    execute(svdf_float16::CreateModel,
            svdf_float16::is_ignored,
            svdf_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, svdf_float16, svdf_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, svdf_float16_dynamic_output_shape) {
    execute(svdf_float16::CreateModel_dynamic_output_shape,
            svdf_float16::is_ignored_dynamic_output_shape,
            svdf_float16::get_examples_dynamic_output_shape());
}

