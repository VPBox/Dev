// clang-format off
// Generated file (from: sub_broadcast_float_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace sub_broadcast_float_relaxed {
// Generated sub_broadcast_float_relaxed test
#include "generated/examples/sub_broadcast_float_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/sub_broadcast_float_relaxed.model.cpp"
} // namespace sub_broadcast_float_relaxed

TEST_F(GeneratedTests, sub_broadcast_float_relaxed) {
    execute(sub_broadcast_float_relaxed::CreateModel,
            sub_broadcast_float_relaxed::is_ignored,
            sub_broadcast_float_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, sub_broadcast_float_relaxed_dynamic_output_shape) {
    execute(sub_broadcast_float_relaxed::CreateModel_dynamic_output_shape,
            sub_broadcast_float_relaxed::is_ignored_dynamic_output_shape,
            sub_broadcast_float_relaxed::get_examples_dynamic_output_shape());
}

