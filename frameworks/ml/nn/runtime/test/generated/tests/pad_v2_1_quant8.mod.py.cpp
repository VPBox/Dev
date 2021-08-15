// clang-format off
// Generated file (from: pad_v2_1_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_v2_1_quant8 {
// Generated pad_v2_1_quant8 test
#include "generated/examples/pad_v2_1_quant8.example.cpp"
// Generated model constructor
#include "generated/models/pad_v2_1_quant8.model.cpp"
} // namespace pad_v2_1_quant8

TEST_F(GeneratedTests, pad_v2_1_quant8) {
    execute(pad_v2_1_quant8::CreateModel,
            pad_v2_1_quant8::is_ignored,
            pad_v2_1_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, pad_v2_1_quant8, pad_v2_1_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, pad_v2_1_quant8_dynamic_output_shape) {
    execute(pad_v2_1_quant8::CreateModel_dynamic_output_shape,
            pad_v2_1_quant8::is_ignored_dynamic_output_shape,
            pad_v2_1_quant8::get_examples_dynamic_output_shape());
}

