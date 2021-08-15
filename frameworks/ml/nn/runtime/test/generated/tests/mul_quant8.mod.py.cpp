// clang-format off
// Generated file (from: mul_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mul_quant8 {
// Generated mul_quant8 test
#include "generated/examples/mul_quant8.example.cpp"
// Generated model constructor
#include "generated/models/mul_quant8.model.cpp"
} // namespace mul_quant8

TEST_F(GeneratedTests, mul_quant8) {
    execute(mul_quant8::CreateModel,
            mul_quant8::is_ignored,
            mul_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, mul_quant8, mul_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, mul_quant8_dynamic_output_shape) {
    execute(mul_quant8::CreateModel_dynamic_output_shape,
            mul_quant8::is_ignored_dynamic_output_shape,
            mul_quant8::get_examples_dynamic_output_shape());
}

