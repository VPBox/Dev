// clang-format off
// Generated file (from: mean_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mean_float16 {
// Generated mean_float16 test
#include "generated/examples/mean_float16.example.cpp"
// Generated model constructor
#include "generated/models/mean_float16.model.cpp"
} // namespace mean_float16

TEST_F(GeneratedTests, mean_float16) {
    execute(mean_float16::CreateModel,
            mean_float16::is_ignored,
            mean_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, mean_float16, mean_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, mean_float16_dynamic_output_shape) {
    execute(mean_float16::CreateModel_dynamic_output_shape,
            mean_float16::is_ignored_dynamic_output_shape,
            mean_float16::get_examples_dynamic_output_shape());
}

