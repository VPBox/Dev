// clang-format off
// Generated file (from: concat_float_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace concat_float_3 {
// Generated concat_float_3 test
#include "generated/examples/concat_float_3.example.cpp"
// Generated model constructor
#include "generated/models/concat_float_3.model.cpp"
} // namespace concat_float_3

TEST_F(GeneratedTests, concat_float_3) {
    execute(concat_float_3::CreateModel,
            concat_float_3::is_ignored,
            concat_float_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, concat_float_3, concat_float_3::CreateModel)

TEST_F(DynamicOutputShapeTest, concat_float_3_dynamic_output_shape) {
    execute(concat_float_3::CreateModel_dynamic_output_shape,
            concat_float_3::is_ignored_dynamic_output_shape,
            concat_float_3::get_examples_dynamic_output_shape());
}

