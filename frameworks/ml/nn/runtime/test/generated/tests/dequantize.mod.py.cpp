// clang-format off
// Generated file (from: dequantize.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace dequantize {
// Generated dequantize test
#include "generated/examples/dequantize.example.cpp"
// Generated model constructor
#include "generated/models/dequantize.model.cpp"
} // namespace dequantize

TEST_F(GeneratedTests, dequantize) {
    execute(dequantize::CreateModel,
            dequantize::is_ignored,
            dequantize::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, dequantize, dequantize::CreateModel)

TEST_F(DynamicOutputShapeTest, dequantize_dynamic_output_shape) {
    execute(dequantize::CreateModel_dynamic_output_shape,
            dequantize::is_ignored_dynamic_output_shape,
            dequantize::get_examples_dynamic_output_shape());
}

