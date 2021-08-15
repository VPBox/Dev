// clang-format off
// Generated file (from: l2_pool_float.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_pool_float {
// Generated l2_pool_float test
#include "generated/examples/l2_pool_float.example.cpp"
// Generated model constructor
#include "generated/models/l2_pool_float.model.cpp"
} // namespace l2_pool_float

TEST_F(GeneratedTests, l2_pool_float) {
    execute(l2_pool_float::CreateModel,
            l2_pool_float::is_ignored,
            l2_pool_float::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, l2_pool_float, l2_pool_float::CreateModel)

TEST_F(DynamicOutputShapeTest, l2_pool_float_dynamic_output_shape) {
    execute(l2_pool_float::CreateModel_dynamic_output_shape,
            l2_pool_float::is_ignored_dynamic_output_shape,
            l2_pool_float::get_examples_dynamic_output_shape());
}

