// clang-format off
// Generated file (from: l2_normalization.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace l2_normalization {
// Generated l2_normalization test
#include "generated/examples/l2_normalization.example.cpp"
// Generated model constructor
#include "generated/models/l2_normalization.model.cpp"
} // namespace l2_normalization

TEST_F(GeneratedTests, l2_normalization) {
    execute(l2_normalization::CreateModel,
            l2_normalization::is_ignored,
            l2_normalization::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, l2_normalization, l2_normalization::CreateModel)

TEST_F(DynamicOutputShapeTest, l2_normalization_dynamic_output_shape) {
    execute(l2_normalization::CreateModel_dynamic_output_shape,
            l2_normalization::is_ignored_dynamic_output_shape,
            l2_normalization::get_examples_dynamic_output_shape());
}

