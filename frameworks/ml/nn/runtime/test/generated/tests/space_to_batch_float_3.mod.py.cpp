// clang-format off
// Generated file (from: space_to_batch_float_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_batch_float_3 {
// Generated space_to_batch_float_3 test
#include "generated/examples/space_to_batch_float_3.example.cpp"
// Generated model constructor
#include "generated/models/space_to_batch_float_3.model.cpp"
} // namespace space_to_batch_float_3

TEST_F(GeneratedTests, space_to_batch_float_3) {
    execute(space_to_batch_float_3::CreateModel,
            space_to_batch_float_3::is_ignored,
            space_to_batch_float_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, space_to_batch_float_3, space_to_batch_float_3::CreateModel)

TEST_F(DynamicOutputShapeTest, space_to_batch_float_3_dynamic_output_shape) {
    execute(space_to_batch_float_3::CreateModel_dynamic_output_shape,
            space_to_batch_float_3::is_ignored_dynamic_output_shape,
            space_to_batch_float_3::get_examples_dynamic_output_shape());
}

