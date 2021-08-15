// clang-format off
// Generated file (from: sub_broadcast_float.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sub_broadcast_float {
// Generated sub_broadcast_float test
#include "generated/examples/sub_broadcast_float.example.cpp"
// Generated model constructor
#include "generated/models/sub_broadcast_float.model.cpp"
} // namespace sub_broadcast_float

TEST_F(GeneratedTests, sub_broadcast_float) {
    execute(sub_broadcast_float::CreateModel,
            sub_broadcast_float::is_ignored,
            sub_broadcast_float::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, sub_broadcast_float, sub_broadcast_float::CreateModel)

TEST_F(DynamicOutputShapeTest, sub_broadcast_float_dynamic_output_shape) {
    execute(sub_broadcast_float::CreateModel_dynamic_output_shape,
            sub_broadcast_float::is_ignored_dynamic_output_shape,
            sub_broadcast_float::get_examples_dynamic_output_shape());
}

