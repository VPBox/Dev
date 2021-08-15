// clang-format off
// Generated file (from: transpose.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose {
// Generated transpose test
#include "generated/examples/transpose.example.cpp"
// Generated model constructor
#include "generated/models/transpose.model.cpp"
} // namespace transpose

TEST_F(GeneratedTests, transpose) {
    execute(transpose::CreateModel,
            transpose::is_ignored,
            transpose::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, transpose, transpose::CreateModel)

TEST_F(DynamicOutputShapeTest, transpose_dynamic_output_shape) {
    execute(transpose::CreateModel_dynamic_output_shape,
            transpose::is_ignored_dynamic_output_shape,
            transpose::get_examples_dynamic_output_shape());
}

