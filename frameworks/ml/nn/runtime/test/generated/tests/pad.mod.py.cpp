// clang-format off
// Generated file (from: pad.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad {
// Generated pad test
#include "generated/examples/pad.example.cpp"
// Generated model constructor
#include "generated/models/pad.model.cpp"
} // namespace pad

TEST_F(GeneratedTests, pad) {
    execute(pad::CreateModel,
            pad::is_ignored,
            pad::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, pad, pad::CreateModel)

TEST_F(DynamicOutputShapeTest, pad_dynamic_output_shape) {
    execute(pad::CreateModel_dynamic_output_shape,
            pad::is_ignored_dynamic_output_shape,
            pad::get_examples_dynamic_output_shape());
}

