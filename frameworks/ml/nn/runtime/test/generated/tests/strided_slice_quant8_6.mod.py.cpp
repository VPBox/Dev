// clang-format off
// Generated file (from: strided_slice_quant8_6.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice_quant8_6 {
// Generated strided_slice_quant8_6 test
#include "generated/examples/strided_slice_quant8_6.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice_quant8_6.model.cpp"
} // namespace strided_slice_quant8_6

TEST_F(GeneratedTests, strided_slice_quant8_6) {
    execute(strided_slice_quant8_6::CreateModel,
            strided_slice_quant8_6::is_ignored,
            strided_slice_quant8_6::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, strided_slice_quant8_6, strided_slice_quant8_6::CreateModel)

TEST_F(DynamicOutputShapeTest, strided_slice_quant8_6_dynamic_output_shape) {
    execute(strided_slice_quant8_6::CreateModel_dynamic_output_shape,
            strided_slice_quant8_6::is_ignored_dynamic_output_shape,
            strided_slice_quant8_6::get_examples_dynamic_output_shape());
}

