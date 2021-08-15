// clang-format off
// Generated file (from: resize_bilinear_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace resize_bilinear_2 {
// Generated resize_bilinear_2 test
#include "generated/examples/resize_bilinear_2.example.cpp"
// Generated model constructor
#include "generated/models/resize_bilinear_2.model.cpp"
} // namespace resize_bilinear_2

TEST_F(GeneratedTests, resize_bilinear_2) {
    execute(resize_bilinear_2::CreateModel,
            resize_bilinear_2::is_ignored,
            resize_bilinear_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, resize_bilinear_2, resize_bilinear_2::CreateModel)

TEST_F(DynamicOutputShapeTest, resize_bilinear_2_dynamic_output_shape) {
    execute(resize_bilinear_2::CreateModel_dynamic_output_shape,
            resize_bilinear_2::is_ignored_dynamic_output_shape,
            resize_bilinear_2::get_examples_dynamic_output_shape());
}

