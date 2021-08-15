// clang-format off
// Generated file (from: transpose_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_float16 {
// Generated transpose_float16 test
#include "generated/examples/transpose_float16.example.cpp"
// Generated model constructor
#include "generated/models/transpose_float16.model.cpp"
} // namespace transpose_float16

TEST_F(GeneratedTests, transpose_float16) {
    execute(transpose_float16::CreateModel,
            transpose_float16::is_ignored,
            transpose_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, transpose_float16, transpose_float16::CreateModel)

TEST_F(DynamicOutputShapeTest, transpose_float16_dynamic_output_shape) {
    execute(transpose_float16::CreateModel_dynamic_output_shape,
            transpose_float16::is_ignored_dynamic_output_shape,
            transpose_float16::get_examples_dynamic_output_shape());
}

