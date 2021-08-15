// clang-format off
// Generated file (from: dequantize_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace dequantize_relaxed {
// Generated dequantize_relaxed test
#include "generated/examples/dequantize_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/dequantize_relaxed.model.cpp"
} // namespace dequantize_relaxed

TEST_F(GeneratedTests, dequantize_relaxed) {
    execute(dequantize_relaxed::CreateModel,
            dequantize_relaxed::is_ignored,
            dequantize_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, dequantize_relaxed_dynamic_output_shape) {
    execute(dequantize_relaxed::CreateModel_dynamic_output_shape,
            dequantize_relaxed::is_ignored_dynamic_output_shape,
            dequantize_relaxed::get_examples_dynamic_output_shape());
}

