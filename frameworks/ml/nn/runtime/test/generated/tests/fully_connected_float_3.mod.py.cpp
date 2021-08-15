// clang-format off
// Generated file (from: fully_connected_float_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_float_3 {
// Generated fully_connected_float_3 test
#include "generated/examples/fully_connected_float_3.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_float_3.model.cpp"
} // namespace fully_connected_float_3

TEST_F(GeneratedTests, fully_connected_float_3) {
    execute(fully_connected_float_3::CreateModel,
            fully_connected_float_3::is_ignored,
            fully_connected_float_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, fully_connected_float_3, fully_connected_float_3::CreateModel)

TEST_F(DynamicOutputShapeTest, fully_connected_float_3_dynamic_output_shape) {
    execute(fully_connected_float_3::CreateModel_dynamic_output_shape,
            fully_connected_float_3::is_ignored_dynamic_output_shape,
            fully_connected_float_3::get_examples_dynamic_output_shape());
}

