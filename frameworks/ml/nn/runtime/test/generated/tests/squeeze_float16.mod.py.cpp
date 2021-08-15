// clang-format off
// Generated file (from: squeeze_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace squeeze_float16 {
// Generated squeeze_float16 test
#include "generated/examples/squeeze_float16.example.cpp"
// Generated model constructor
#include "generated/models/squeeze_float16.model.cpp"
} // namespace squeeze_float16

TEST_F(GeneratedTests, squeeze_float16) {
    execute(squeeze_float16::CreateModel,
            squeeze_float16::is_ignored,
            squeeze_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, squeeze_float16, squeeze_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, squeeze_float16_dynamic_output_shape) {
    execute(squeeze_float16::CreateModel_dynamic_output_shape,
            squeeze_float16::is_ignored_dynamic_output_shape,
            squeeze_float16::get_examples_dynamic_output_shape());
}

