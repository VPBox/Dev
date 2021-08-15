// clang-format off
// Generated file (from: depth_to_space_float_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depth_to_space_float_3 {
// Generated depth_to_space_float_3 test
#include "generated/examples/depth_to_space_float_3.example.cpp"
// Generated model constructor
#include "generated/models/depth_to_space_float_3.model.cpp"
} // namespace depth_to_space_float_3

TEST_F(GeneratedTests, depth_to_space_float_3) {
    execute(depth_to_space_float_3::CreateModel,
            depth_to_space_float_3::is_ignored,
            depth_to_space_float_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depth_to_space_float_3, depth_to_space_float_3::CreateModel)

TEST_F(DynamicOutputShapeTest, depth_to_space_float_3_dynamic_output_shape) {
    execute(depth_to_space_float_3::CreateModel_dynamic_output_shape,
            depth_to_space_float_3::is_ignored_dynamic_output_shape,
            depth_to_space_float_3::get_examples_dynamic_output_shape());
}

