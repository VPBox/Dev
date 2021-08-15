// clang-format off
// Generated file (from: resize_bilinear_2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace resize_bilinear_2_relaxed {
// Generated resize_bilinear_2_relaxed test
#include "generated/examples/resize_bilinear_2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/resize_bilinear_2_relaxed.model.cpp"
} // namespace resize_bilinear_2_relaxed

TEST_F(GeneratedTests, resize_bilinear_2_relaxed) {
    execute(resize_bilinear_2_relaxed::CreateModel,
            resize_bilinear_2_relaxed::is_ignored,
            resize_bilinear_2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, resize_bilinear_2_relaxed_dynamic_output_shape) {
    execute(resize_bilinear_2_relaxed::CreateModel_dynamic_output_shape,
            resize_bilinear_2_relaxed::is_ignored_dynamic_output_shape,
            resize_bilinear_2_relaxed::get_examples_dynamic_output_shape());
}

