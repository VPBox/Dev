// clang-format off
// Generated file (from: svdf2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace svdf2 {
// Generated svdf2 test
#include "generated/examples/svdf2.example.cpp"
// Generated model constructor
#include "generated/models/svdf2.model.cpp"
} // namespace svdf2

TEST_F(GeneratedTests, svdf2) {
    execute(svdf2::CreateModel,
            svdf2::is_ignored,
            svdf2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, svdf2, svdf2::CreateModel)

TEST_F(DynamicOutputShapeTest, svdf2_dynamic_output_shape) {
    execute(svdf2::CreateModel_dynamic_output_shape,
            svdf2::is_ignored_dynamic_output_shape,
            svdf2::get_examples_dynamic_output_shape());
}

