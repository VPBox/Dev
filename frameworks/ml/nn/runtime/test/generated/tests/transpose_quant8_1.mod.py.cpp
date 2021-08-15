// clang-format off
// Generated file (from: transpose_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_quant8_1 {
// Generated transpose_quant8_1 test
#include "generated/examples/transpose_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/transpose_quant8_1.model.cpp"
} // namespace transpose_quant8_1

TEST_F(GeneratedTests, transpose_quant8_1) {
    execute(transpose_quant8_1::CreateModel,
            transpose_quant8_1::is_ignored,
            transpose_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, transpose_quant8_1, transpose_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, transpose_quant8_1_dynamic_output_shape) {
    execute(transpose_quant8_1::CreateModel_dynamic_output_shape,
            transpose_quant8_1::is_ignored_dynamic_output_shape,
            transpose_quant8_1::get_examples_dynamic_output_shape());
}

