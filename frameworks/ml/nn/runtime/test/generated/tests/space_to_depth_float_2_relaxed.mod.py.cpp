// clang-format off
// Generated file (from: space_to_depth_float_2_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_depth_float_2_relaxed {
// Generated space_to_depth_float_2_relaxed test
#include "generated/examples/space_to_depth_float_2_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/space_to_depth_float_2_relaxed.model.cpp"
} // namespace space_to_depth_float_2_relaxed

TEST_F(GeneratedTests, space_to_depth_float_2_relaxed) {
    execute(space_to_depth_float_2_relaxed::CreateModel,
            space_to_depth_float_2_relaxed::is_ignored,
            space_to_depth_float_2_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, space_to_depth_float_2_relaxed_dynamic_output_shape) {
    execute(space_to_depth_float_2_relaxed::CreateModel_dynamic_output_shape,
            space_to_depth_float_2_relaxed::is_ignored_dynamic_output_shape,
            space_to_depth_float_2_relaxed::get_examples_dynamic_output_shape());
}

