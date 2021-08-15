// clang-format off
// Generated file (from: svdf.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf {
// Generated svdf test
#include "generated/examples/svdf.example.cpp"
// Generated model constructor
#include "generated/models/svdf.model.cpp"
} // namespace svdf

TEST_F(GeneratedTests, svdf) {
    execute(svdf::CreateModel,
            svdf::is_ignored,
            svdf::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, svdf, svdf::CreateModel)

TEST_F(DynamicOutputShapeTest, svdf_dynamic_output_shape) {
    execute(svdf::CreateModel_dynamic_output_shape,
            svdf::is_ignored_dynamic_output_shape,
            svdf::get_examples_dynamic_output_shape());
}

