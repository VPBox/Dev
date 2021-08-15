// clang-format off
// Generated file (from: fully_connected_float.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace fully_connected_float {
// Generated fully_connected_float test
#include "generated/examples/fully_connected_float.example.cpp"
// Generated model constructor
#include "generated/models/fully_connected_float.model.cpp"
} // namespace fully_connected_float

TEST_F(GeneratedTests, fully_connected_float) {
    execute(fully_connected_float::CreateModel,
            fully_connected_float::is_ignored,
            fully_connected_float::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, fully_connected_float, fully_connected_float::CreateModel)

TEST_F(DynamicOutputShapeTest, fully_connected_float_dynamic_output_shape) {
    execute(fully_connected_float::CreateModel_dynamic_output_shape,
            fully_connected_float::is_ignored_dynamic_output_shape,
            fully_connected_float::get_examples_dynamic_output_shape());
}

