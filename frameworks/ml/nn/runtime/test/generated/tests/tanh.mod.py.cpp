// clang-format off
// Generated file (from: tanh.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace tanh {
// Generated tanh test
#include "generated/examples/tanh.example.cpp"
// Generated model constructor
#include "generated/models/tanh.model.cpp"
} // namespace tanh

TEST_F(GeneratedTests, tanh) {
    execute(tanh::CreateModel,
            tanh::is_ignored,
            tanh::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, tanh, tanh::CreateModel)

TEST_F(DynamicOutputShapeTest, tanh_dynamic_output_shape) {
    execute(tanh::CreateModel_dynamic_output_shape,
            tanh::is_ignored_dynamic_output_shape,
            tanh::get_examples_dynamic_output_shape());
}

