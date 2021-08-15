// clang-format off
// Generated file (from: transpose_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace transpose_relaxed {
// Generated transpose_relaxed test
#include "generated/examples/transpose_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/transpose_relaxed.model.cpp"
} // namespace transpose_relaxed

TEST_F(GeneratedTests, transpose_relaxed) {
    execute(transpose_relaxed::CreateModel,
            transpose_relaxed::is_ignored,
            transpose_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, transpose_relaxed_dynamic_output_shape) {
    execute(transpose_relaxed::CreateModel_dynamic_output_shape,
            transpose_relaxed::is_ignored_dynamic_output_shape,
            transpose_relaxed::get_examples_dynamic_output_shape());
}

