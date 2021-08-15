// clang-format off
// Generated file (from: strided_slice.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice {
// Generated strided_slice test
#include "generated/examples/strided_slice.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice.model.cpp"
} // namespace strided_slice

TEST_F(GeneratedTests, strided_slice) {
    execute(strided_slice::CreateModel,
            strided_slice::is_ignored,
            strided_slice::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, strided_slice, strided_slice::CreateModel)

TEST_F(DynamicOutputShapeTest, strided_slice_dynamic_output_shape) {
    execute(strided_slice::CreateModel_dynamic_output_shape,
            strided_slice::is_ignored_dynamic_output_shape,
            strided_slice::get_examples_dynamic_output_shape());
}

