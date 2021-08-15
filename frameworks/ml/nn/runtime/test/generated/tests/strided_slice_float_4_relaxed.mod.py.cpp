// clang-format off
// Generated file (from: strided_slice_float_4_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace strided_slice_float_4_relaxed {
// Generated strided_slice_float_4_relaxed test
#include "generated/examples/strided_slice_float_4_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/strided_slice_float_4_relaxed.model.cpp"
} // namespace strided_slice_float_4_relaxed

TEST_F(GeneratedTests, strided_slice_float_4_relaxed) {
    execute(strided_slice_float_4_relaxed::CreateModel,
            strided_slice_float_4_relaxed::is_ignored,
            strided_slice_float_4_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, strided_slice_float_4_relaxed_dynamic_output_shape) {
    execute(strided_slice_float_4_relaxed::CreateModel_dynamic_output_shape,
            strided_slice_float_4_relaxed::is_ignored_dynamic_output_shape,
            strided_slice_float_4_relaxed::get_examples_dynamic_output_shape());
}

