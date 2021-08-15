// clang-format off
// Generated file (from: fully_connected_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_float_2 {
// Generated fully_connected_float_2 test
#include "generated/examples/fully_connected_float_2.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_float_2.model.cpp"
} // namespace fully_connected_float_2

TEST_F(GeneratedTests, fully_connected_float_2) {
    execute(fully_connected_float_2::CreateModel,
            fully_connected_float_2::is_ignored,
            fully_connected_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, fully_connected_float_2, fully_connected_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, fully_connected_float_2_dynamic_output_shape) {
    execute(fully_connected_float_2::CreateModel_dynamic_output_shape,
            fully_connected_float_2::is_ignored_dynamic_output_shape,
            fully_connected_float_2::get_examples_dynamic_output_shape());
}

