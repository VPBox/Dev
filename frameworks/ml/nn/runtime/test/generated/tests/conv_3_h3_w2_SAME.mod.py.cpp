// clang-format off
// Generated file (from: conv_3_h3_w2_SAME.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace conv_3_h3_w2_SAME {
// Generated conv_3_h3_w2_SAME test
#include "generated/examples/conv_3_h3_w2_SAME.example.cpp"
// Generated model constructor
#include "generated/models/conv_3_h3_w2_SAME.model.cpp"
} // namespace conv_3_h3_w2_SAME

TEST_F(GeneratedTests, conv_3_h3_w2_SAME) {
    execute(conv_3_h3_w2_SAME::CreateModel,
            conv_3_h3_w2_SAME::is_ignored,
            conv_3_h3_w2_SAME::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, conv_3_h3_w2_SAME, conv_3_h3_w2_SAME::CreateModel)

TEST_F(DynamicOutputShapeTest, conv_3_h3_w2_SAME_dynamic_output_shape) {
    execute(conv_3_h3_w2_SAME::CreateModel_dynamic_output_shape,
            conv_3_h3_w2_SAME::is_ignored_dynamic_output_shape,
            conv_3_h3_w2_SAME::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, conv_3_h3_w2_SAME_2) {
    execute(conv_3_h3_w2_SAME::CreateModel_2,
            conv_3_h3_w2_SAME::is_ignored_2,
            conv_3_h3_w2_SAME::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_0, conv_3_h3_w2_SAME_2, conv_3_h3_w2_SAME::CreateModel_2)

TEST_F(DynamicOutputShapeTest, conv_3_h3_w2_SAME_dynamic_output_shape_2) {
    execute(conv_3_h3_w2_SAME::CreateModel_dynamic_output_shape_2,
            conv_3_h3_w2_SAME::is_ignored_dynamic_output_shape_2,
            conv_3_h3_w2_SAME::get_examples_dynamic_output_shape_2());
}

