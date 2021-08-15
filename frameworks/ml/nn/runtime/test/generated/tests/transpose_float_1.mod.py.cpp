// clang-format off
// Generated file (from: transpose_float_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_float_1 {
// Generated transpose_float_1 test
#include "generated/examples/transpose_float_1.example.cpp"
// Generated model constructor
#include "generated/models/transpose_float_1.model.cpp"
} // namespace transpose_float_1

TEST_F(GeneratedTests, transpose_float_1) {
    execute(transpose_float_1::CreateModel,
            transpose_float_1::is_ignored,
            transpose_float_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, transpose_float_1, transpose_float_1::CreateModel)

TEST_F(DynamicOutputShapeTest, transpose_float_1_dynamic_output_shape) {
    execute(transpose_float_1::CreateModel_dynamic_output_shape,
            transpose_float_1::is_ignored_dynamic_output_shape,
            transpose_float_1::get_examples_dynamic_output_shape());
}

