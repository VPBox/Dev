// clang-format off
// Generated file (from: concat_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace concat_float_2 {
// Generated concat_float_2 test
#include "generated/examples/concat_float_2.example.cpp"
// Generated model constructor
#include "generated/models/concat_float_2.model.cpp"
} // namespace concat_float_2

TEST_F(GeneratedTests, concat_float_2) {
    execute(concat_float_2::CreateModel,
            concat_float_2::is_ignored,
            concat_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, concat_float_2, concat_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, concat_float_2_dynamic_output_shape) {
    execute(concat_float_2::CreateModel_dynamic_output_shape,
            concat_float_2::is_ignored_dynamic_output_shape,
            concat_float_2::get_examples_dynamic_output_shape());
}

