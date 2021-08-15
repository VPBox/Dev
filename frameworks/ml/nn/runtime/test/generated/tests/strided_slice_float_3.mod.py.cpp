// clang-format off
// Generated file (from: strided_slice_float_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice_float_3 {
// Generated strided_slice_float_3 test
#include "generated/examples/strided_slice_float_3.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice_float_3.model.cpp"
} // namespace strided_slice_float_3

TEST_F(GeneratedTests, strided_slice_float_3) {
    execute(strided_slice_float_3::CreateModel,
            strided_slice_float_3::is_ignored,
            strided_slice_float_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, strided_slice_float_3, strided_slice_float_3::CreateModel)

TEST_F(DynamicOutputShapeTest, strided_slice_float_3_dynamic_output_shape) {
    execute(strided_slice_float_3::CreateModel_dynamic_output_shape,
            strided_slice_float_3::is_ignored_dynamic_output_shape,
            strided_slice_float_3::get_examples_dynamic_output_shape());
}

