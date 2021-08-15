// clang-format off
// Generated file (from: max_pool_quant8_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace max_pool_quant8_2 {
// Generated max_pool_quant8_2 test
#include "generated/examples/max_pool_quant8_2.example.cpp"
// Generated model constructor
#include "generated/models/max_pool_quant8_2.model.cpp"
} // namespace max_pool_quant8_2

TEST_F(GeneratedTests, max_pool_quant8_2) {
    execute(max_pool_quant8_2::CreateModel,
            max_pool_quant8_2::is_ignored,
            max_pool_quant8_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, max_pool_quant8_2, max_pool_quant8_2::CreateModel)

TEST_F(DynamicOutputShapeTest, max_pool_quant8_2_dynamic_output_shape) {
    execute(max_pool_quant8_2::CreateModel_dynamic_output_shape,
            max_pool_quant8_2::is_ignored_dynamic_output_shape,
            max_pool_quant8_2::get_examples_dynamic_output_shape());
}

