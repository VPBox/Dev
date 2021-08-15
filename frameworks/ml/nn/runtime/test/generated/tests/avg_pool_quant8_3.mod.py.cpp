// clang-format off
// Generated file (from: avg_pool_quant8_3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace avg_pool_quant8_3 {
// Generated avg_pool_quant8_3 test
#include "generated/examples/avg_pool_quant8_3.example.cpp"
// Generated model constructor
#include "generated/models/avg_pool_quant8_3.model.cpp"
} // namespace avg_pool_quant8_3

TEST_F(GeneratedTests, avg_pool_quant8_3) {
    execute(avg_pool_quant8_3::CreateModel,
            avg_pool_quant8_3::is_ignored,
            avg_pool_quant8_3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, avg_pool_quant8_3, avg_pool_quant8_3::CreateModel)

TEST_F(DynamicOutputShapeTest, avg_pool_quant8_3_dynamic_output_shape) {
    execute(avg_pool_quant8_3::CreateModel_dynamic_output_shape,
            avg_pool_quant8_3::is_ignored_dynamic_output_shape,
            avg_pool_quant8_3::get_examples_dynamic_output_shape());
}

