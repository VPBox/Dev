// clang-format off
// Generated file (from: conv_3_h3_w2_VALID.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_3_h3_w2_VALID {
// Generated conv_3_h3_w2_VALID test
#include "generated/examples/conv_3_h3_w2_VALID.example.cpp"
// Generated model constructor
#include "generated/models/conv_3_h3_w2_VALID.model.cpp"
} // namespace conv_3_h3_w2_VALID

TEST_F(GeneratedTests, conv_3_h3_w2_VALID) {
    execute(conv_3_h3_w2_VALID::CreateModel,
            conv_3_h3_w2_VALID::is_ignored,
            conv_3_h3_w2_VALID::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_3_h3_w2_VALID, conv_3_h3_w2_VALID::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_3_h3_w2_VALID_dynamic_output_shape) {
    execute(conv_3_h3_w2_VALID::CreateModel_dynamic_output_shape,
            conv_3_h3_w2_VALID::is_ignored_dynamic_output_shape,
            conv_3_h3_w2_VALID::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, conv_3_h3_w2_VALID_2) {
    execute(conv_3_h3_w2_VALID::CreateModel_2,
            conv_3_h3_w2_VALID::is_ignored_2,
            conv_3_h3_w2_VALID::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_0, conv_3_h3_w2_VALID_2, conv_3_h3_w2_VALID::CreateModel_2)

TEST_F(DynamicOutputShapeTest, conv_3_h3_w2_VALID_dynamic_output_shape_2) {
    execute(conv_3_h3_w2_VALID::CreateModel_dynamic_output_shape_2,
            conv_3_h3_w2_VALID::is_ignored_dynamic_output_shape_2,
            conv_3_h3_w2_VALID::get_examples_dynamic_output_shape_2());
}

