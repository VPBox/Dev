// clang-format off
// Generated file (from: l2_pool_float_large.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_pool_float_large {
// Generated l2_pool_float_large test
#include "generated/examples/l2_pool_float_large.example.cpp"
// Generated model constructor
#include "generated/models/l2_pool_float_large.model.cpp"
} // namespace l2_pool_float_large

TEST_F(GeneratedTests, l2_pool_float_large) {
    execute(l2_pool_float_large::CreateModel,
            l2_pool_float_large::is_ignored,
            l2_pool_float_large::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, l2_pool_float_large, l2_pool_float_large::CreateModel)

TEST_F(DynamicOutputShapeTest, l2_pool_float_large_dynamic_output_shape) {
    execute(l2_pool_float_large::CreateModel_dynamic_output_shape,
            l2_pool_float_large::is_ignored_dynamic_output_shape,
            l2_pool_float_large::get_examples_dynamic_output_shape());
}

