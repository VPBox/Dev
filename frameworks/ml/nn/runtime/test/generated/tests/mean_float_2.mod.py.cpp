// clang-format off
// Generated file (from: mean_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mean_float_2 {
// Generated mean_float_2 test
#include "generated/examples/mean_float_2.example.cpp"
// Generated model constructor
#include "generated/models/mean_float_2.model.cpp"
} // namespace mean_float_2

TEST_F(GeneratedTests, mean_float_2) {
    execute(mean_float_2::CreateModel,
            mean_float_2::is_ignored,
            mean_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, mean_float_2, mean_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, mean_float_2_dynamic_output_shape) {
    execute(mean_float_2::CreateModel_dynamic_output_shape,
            mean_float_2::is_ignored_dynamic_output_shape,
            mean_float_2::get_examples_dynamic_output_shape());
}

