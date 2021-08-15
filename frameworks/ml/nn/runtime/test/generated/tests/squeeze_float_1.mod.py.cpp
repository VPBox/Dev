// clang-format off
// Generated file (from: squeeze_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace squeeze_float_1 {
// Generated squeeze_float_1 test
#include "generated/examples/squeeze_float_1.example.cpp"
// Generated model constructor
#include "generated/models/squeeze_float_1.model.cpp"
} // namespace squeeze_float_1

TEST_F(GeneratedTests, squeeze_float_1) {
    execute(squeeze_float_1::CreateModel,
            squeeze_float_1::is_ignored,
            squeeze_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, squeeze_float_1, squeeze_float_1::CreateModel)

TEST_F(DynamicOutputShapeTest, squeeze_float_1_dynamic_output_shape) {
    execute(squeeze_float_1::CreateModel_dynamic_output_shape,
            squeeze_float_1::is_ignored_dynamic_output_shape,
            squeeze_float_1::get_examples_dynamic_output_shape());
}

