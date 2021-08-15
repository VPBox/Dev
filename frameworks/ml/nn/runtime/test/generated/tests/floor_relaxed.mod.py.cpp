// clang-format off
// Generated file (from: floor_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace floor_relaxed {
// Generated floor_relaxed test
#include "generated/examples/floor_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/floor_relaxed.model.cpp"
} // namespace floor_relaxed

TEST_F(GeneratedTests, floor_relaxed) {
    execute(floor_relaxed::CreateModel,
            floor_relaxed::is_ignored,
            floor_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, floor_relaxed_dynamic_output_shape) {
    execute(floor_relaxed::CreateModel_dynamic_output_shape,
            floor_relaxed::is_ignored_dynamic_output_shape,
            floor_relaxed::get_examples_dynamic_output_shape());
}

