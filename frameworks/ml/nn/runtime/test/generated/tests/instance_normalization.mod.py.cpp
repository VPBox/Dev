// clang-format off
// Generated file (from: instance_normalization.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace instance_normalization {
// Generated instance_normalization test
#include "generated/examples/instance_normalization.example.cpp"
// Generated model constructor
#include "generated/models/instance_normalization.model.cpp"
} // namespace instance_normalization

TEST_F(GeneratedTests, instance_normalization_nhwc) {
    execute(instance_normalization::CreateModel_nhwc,
            instance_normalization::is_ignored_nhwc,
            instance_normalization::get_examples_nhwc());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nhwc, instance_normalization::CreateModel_nhwc)

TEST_F(GeneratedTests, instance_normalization_nhwc_relaxed) {
    execute(instance_normalization::CreateModel_nhwc_relaxed,
            instance_normalization::is_ignored_nhwc_relaxed,
            instance_normalization::get_examples_nhwc_relaxed());
}

TEST_F(GeneratedTests, instance_normalization_nhwc_float16) {
    execute(instance_normalization::CreateModel_nhwc_float16,
            instance_normalization::is_ignored_nhwc_float16,
            instance_normalization::get_examples_nhwc_float16());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nhwc_float16, instance_normalization::CreateModel_nhwc_float16)

TEST_F(GeneratedTests, instance_normalization_nchw) {
    execute(instance_normalization::CreateModel_nchw,
            instance_normalization::is_ignored_nchw,
            instance_normalization::get_examples_nchw());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nchw, instance_normalization::CreateModel_nchw)

TEST_F(GeneratedTests, instance_normalization_nchw_relaxed) {
    execute(instance_normalization::CreateModel_nchw_relaxed,
            instance_normalization::is_ignored_nchw_relaxed,
            instance_normalization::get_examples_nchw_relaxed());
}

TEST_F(GeneratedTests, instance_normalization_nchw_float16) {
    execute(instance_normalization::CreateModel_nchw_float16,
            instance_normalization::is_ignored_nchw_float16,
            instance_normalization::get_examples_nchw_float16());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nchw_float16, instance_normalization::CreateModel_nchw_float16)

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc,
            instance_normalization::get_examples_dynamic_output_shape_nhwc());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc_relaxed) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc_relaxed,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc_relaxed,
            instance_normalization::get_examples_dynamic_output_shape_nhwc_relaxed());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc_float16) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc_float16,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc_float16,
            instance_normalization::get_examples_dynamic_output_shape_nhwc_float16());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw,
            instance_normalization::is_ignored_dynamic_output_shape_nchw,
            instance_normalization::get_examples_dynamic_output_shape_nchw());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw_relaxed) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw_relaxed,
            instance_normalization::is_ignored_dynamic_output_shape_nchw_relaxed,
            instance_normalization::get_examples_dynamic_output_shape_nchw_relaxed());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw_float16) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw_float16,
            instance_normalization::is_ignored_dynamic_output_shape_nchw_float16,
            instance_normalization::get_examples_dynamic_output_shape_nchw_float16());
}

TEST_F(GeneratedTests, instance_normalization_nhwc_2) {
    execute(instance_normalization::CreateModel_nhwc_2,
            instance_normalization::is_ignored_nhwc_2,
            instance_normalization::get_examples_nhwc_2());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nhwc_2, instance_normalization::CreateModel_nhwc_2)

TEST_F(GeneratedTests, instance_normalization_nhwc_relaxed_2) {
    execute(instance_normalization::CreateModel_nhwc_relaxed_2,
            instance_normalization::is_ignored_nhwc_relaxed_2,
            instance_normalization::get_examples_nhwc_relaxed_2());
}

TEST_F(GeneratedTests, instance_normalization_nhwc_float16_2) {
    execute(instance_normalization::CreateModel_nhwc_float16_2,
            instance_normalization::is_ignored_nhwc_float16_2,
            instance_normalization::get_examples_nhwc_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nhwc_float16_2, instance_normalization::CreateModel_nhwc_float16_2)

TEST_F(GeneratedTests, instance_normalization_nchw_2) {
    execute(instance_normalization::CreateModel_nchw_2,
            instance_normalization::is_ignored_nchw_2,
            instance_normalization::get_examples_nchw_2());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nchw_2, instance_normalization::CreateModel_nchw_2)

TEST_F(GeneratedTests, instance_normalization_nchw_relaxed_2) {
    execute(instance_normalization::CreateModel_nchw_relaxed_2,
            instance_normalization::is_ignored_nchw_relaxed_2,
            instance_normalization::get_examples_nchw_relaxed_2());
}

TEST_F(GeneratedTests, instance_normalization_nchw_float16_2) {
    execute(instance_normalization::CreateModel_nchw_float16_2,
            instance_normalization::is_ignored_nchw_float16_2,
            instance_normalization::get_examples_nchw_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, instance_normalization_nchw_float16_2, instance_normalization::CreateModel_nchw_float16_2)

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc_2,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc_2,
            instance_normalization::get_examples_dynamic_output_shape_nhwc_2());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc_relaxed_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc_relaxed_2,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc_relaxed_2,
            instance_normalization::get_examples_dynamic_output_shape_nhwc_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nhwc_float16_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nhwc_float16_2,
            instance_normalization::is_ignored_dynamic_output_shape_nhwc_float16_2,
            instance_normalization::get_examples_dynamic_output_shape_nhwc_float16_2());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw_2,
            instance_normalization::is_ignored_dynamic_output_shape_nchw_2,
            instance_normalization::get_examples_dynamic_output_shape_nchw_2());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw_relaxed_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw_relaxed_2,
            instance_normalization::is_ignored_dynamic_output_shape_nchw_relaxed_2,
            instance_normalization::get_examples_dynamic_output_shape_nchw_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, instance_normalization_dynamic_output_shape_nchw_float16_2) {
    execute(instance_normalization::CreateModel_dynamic_output_shape_nchw_float16_2,
            instance_normalization::is_ignored_dynamic_output_shape_nchw_float16_2,
            instance_normalization::get_examples_dynamic_output_shape_nchw_float16_2());
}

