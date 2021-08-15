// clang-format off
// Generated file (from: mobilenet_quantized.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mobilenet_quantized {
// Generated mobilenet_quantized test
#include "generated/examples/mobilenet_quantized.example.cpp"
// Generated model constructor
#include "generated/models/mobilenet_quantized.model.cpp"
} // namespace mobilenet_quantized

TEST_F(GeneratedTests, mobilenet_quantized) {
    execute(mobilenet_quantized::CreateModel,
            mobilenet_quantized::is_ignored,
            mobilenet_quantized::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, mobilenet_quantized, mobilenet_quantized::CreateModel)

TEST_F(DynamicOutputShapeTest, mobilenet_quantized_dynamic_output_shape) {
    execute(mobilenet_quantized::CreateModel_dynamic_output_shape,
            mobilenet_quantized::is_ignored_dynamic_output_shape,
            mobilenet_quantized::get_examples_dynamic_output_shape());
}

