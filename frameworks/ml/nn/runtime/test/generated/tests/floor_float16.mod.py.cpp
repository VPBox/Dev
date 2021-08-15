// clang-format off
// Generated file (from: floor_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace floor_float16 {
// Generated floor_float16 test
#include "generated/examples/floor_float16.example.cpp"
// Generated model constructor
#include "generated/models/floor_float16.model.cpp"
} // namespace floor_float16

TEST_F(GeneratedTests, floor_float16) {
    execute(floor_float16::CreateModel,
            floor_float16::is_ignored,
            floor_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, floor_float16, floor_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, floor_float16_dynamic_output_shape) {
    execute(floor_float16::CreateModel_dynamic_output_shape,
            floor_float16::is_ignored_dynamic_output_shape,
            floor_float16::get_examples_dynamic_output_shape());
}

