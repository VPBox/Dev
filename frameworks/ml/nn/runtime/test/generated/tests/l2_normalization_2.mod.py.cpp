// clang-format off
// Generated file (from: l2_normalization_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_normalization_2 {
// Generated l2_normalization_2 test
#include "generated/examples/l2_normalization_2.example.cpp"
// Generated model constructor
#include "generated/models/l2_normalization_2.model.cpp"
} // namespace l2_normalization_2

TEST_F(GeneratedTests, l2_normalization_2) {
    execute(l2_normalization_2::CreateModel,
            l2_normalization_2::is_ignored,
            l2_normalization_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, l2_normalization_2, l2_normalization_2::CreateModel)

TEST_F(DynamicOutputShapeTest, l2_normalization_2_dynamic_output_shape) {
    execute(l2_normalization_2::CreateModel_dynamic_output_shape,
            l2_normalization_2::is_ignored_dynamic_output_shape,
            l2_normalization_2::get_examples_dynamic_output_shape());
}

