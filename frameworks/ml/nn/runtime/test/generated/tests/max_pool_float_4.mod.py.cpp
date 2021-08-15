// clang-format off
// Generated file (from: max_pool_float_4.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace max_pool_float_4 {
// Generated max_pool_float_4 test
#include "generated/examples/max_pool_float_4.example.cpp"
// Generated model constructor
#include "generated/models/max_pool_float_4.model.cpp"
} // namespace max_pool_float_4

TEST_F(GeneratedTests, max_pool_float_4) {
    execute(max_pool_float_4::CreateModel,
            max_pool_float_4::is_ignored,
            max_pool_float_4::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, max_pool_float_4, max_pool_float_4::CreateModel)

TEST_F(DynamicOutputShapeTest, max_pool_float_4_dynamic_output_shape) {
    execute(max_pool_float_4::CreateModel_dynamic_output_shape,
            max_pool_float_4::is_ignored_dynamic_output_shape,
            max_pool_float_4::get_examples_dynamic_output_shape());
}

