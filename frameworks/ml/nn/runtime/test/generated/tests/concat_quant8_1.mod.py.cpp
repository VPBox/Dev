// clang-format off
// Generated file (from: concat_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace concat_quant8_1 {
// Generated concat_quant8_1 test
#include "generated/examples/concat_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/concat_quant8_1.model.cpp"
} // namespace concat_quant8_1

TEST_F(GeneratedTests, concat_quant8_1) {
    execute(concat_quant8_1::CreateModel,
            concat_quant8_1::is_ignored,
            concat_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, concat_quant8_1, concat_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, concat_quant8_1_dynamic_output_shape) {
    execute(concat_quant8_1::CreateModel_dynamic_output_shape,
            concat_quant8_1::is_ignored_dynamic_output_shape,
            concat_quant8_1::get_examples_dynamic_output_shape());
}

