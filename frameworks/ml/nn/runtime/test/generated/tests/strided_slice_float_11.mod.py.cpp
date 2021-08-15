// clang-format off
// Generated file (from: strided_slice_float_11.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice_float_11 {
// Generated strided_slice_float_11 test
#include "generated/examples/strided_slice_float_11.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice_float_11.model.cpp"
} // namespace strided_slice_float_11

TEST_F(GeneratedTests, strided_slice_float_11) {
    execute(strided_slice_float_11::CreateModel,
            strided_slice_float_11::is_ignored,
            strided_slice_float_11::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, strided_slice_float_11, strided_slice_float_11::CreateModel)

TEST_F(DynamicOutputShapeTest, strided_slice_float_11_dynamic_output_shape) {
    execute(strided_slice_float_11::CreateModel_dynamic_output_shape,
            strided_slice_float_11::is_ignored_dynamic_output_shape,
            strided_slice_float_11::get_examples_dynamic_output_shape());
}

