// clang-format off
// Generated file (from: reshape_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reshape_float16 {
// Generated reshape_float16 test
#include "generated/examples/reshape_float16.example.cpp"
// Generated model constructor
#include "generated/models/reshape_float16.model.cpp"
} // namespace reshape_float16

TEST_F(GeneratedTests, reshape_float16) {
    execute(reshape_float16::CreateModel,
            reshape_float16::is_ignored,
            reshape_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, reshape_float16, reshape_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, reshape_float16_dynamic_output_shape) {
    execute(reshape_float16::CreateModel_dynamic_output_shape,
            reshape_float16::is_ignored_dynamic_output_shape,
            reshape_float16::get_examples_dynamic_output_shape());
}

