// clang-format off
// Generated file (from: reshape.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reshape {
// Generated reshape test
#include "generated/examples/reshape.example.cpp"
// Generated model constructor
#include "generated/models/reshape.model.cpp"
} // namespace reshape

TEST_F(GeneratedTests, reshape) {
    execute(reshape::CreateModel,
            reshape::is_ignored,
            reshape::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, reshape, reshape::CreateModel)

TEST_F(DynamicOutputShapeTest, reshape_dynamic_output_shape) {
    execute(reshape::CreateModel_dynamic_output_shape,
            reshape::is_ignored_dynamic_output_shape,
            reshape::get_examples_dynamic_output_shape());
}

