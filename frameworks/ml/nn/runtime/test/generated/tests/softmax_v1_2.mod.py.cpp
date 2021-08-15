// clang-format off
// Generated file (from: softmax_v1_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace softmax_v1_2 {
// Generated softmax_v1_2 test
#include "generated/examples/softmax_v1_2.example.cpp"
// Generated model constructor
#include "generated/models/softmax_v1_2.model.cpp"
} // namespace softmax_v1_2

TEST_F(GeneratedTests, softmax_v1_2) {
    execute(softmax_v1_2::CreateModel,
            softmax_v1_2::is_ignored,
            softmax_v1_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_v1_2, softmax_v1_2::CreateModel)

TEST_F(GeneratedTests, softmax_v1_2_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_dim1_axis0,
            softmax_v1_2::is_ignored_dim1_axis0,
            softmax_v1_2::get_examples_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_dim1_axis0, softmax_v1_2::CreateModel_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_dim3_axis2,
            softmax_v1_2::is_ignored_dim3_axis2,
            softmax_v1_2::get_examples_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_dim3_axis2, softmax_v1_2::CreateModel_dim3_axis2)

TEST_F(GeneratedTests, softmax_v1_2_relaxed) {
    execute(softmax_v1_2::CreateModel_relaxed,
            softmax_v1_2::is_ignored_relaxed,
            softmax_v1_2::get_examples_relaxed());
}

TEST_F(GeneratedTests, softmax_v1_2_relaxed_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_relaxed_dim1_axis0,
            softmax_v1_2::is_ignored_relaxed_dim1_axis0,
            softmax_v1_2::get_examples_relaxed_dim1_axis0());
}

TEST_F(GeneratedTests, softmax_v1_2_relaxed_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_relaxed_dim3_axis2,
            softmax_v1_2::is_ignored_relaxed_dim3_axis2,
            softmax_v1_2::get_examples_relaxed_dim3_axis2());
}

TEST_F(GeneratedTests, softmax_v1_2_float16) {
    execute(softmax_v1_2::CreateModel_float16,
            softmax_v1_2::is_ignored_float16,
            softmax_v1_2::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16, softmax_v1_2::CreateModel_float16)

TEST_F(GeneratedTests, softmax_v1_2_float16_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_float16_dim1_axis0,
            softmax_v1_2::is_ignored_float16_dim1_axis0,
            softmax_v1_2::get_examples_float16_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16_dim1_axis0, softmax_v1_2::CreateModel_float16_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_float16_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_float16_dim3_axis2,
            softmax_v1_2::is_ignored_float16_dim3_axis2,
            softmax_v1_2::get_examples_float16_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16_dim3_axis2, softmax_v1_2::CreateModel_float16_dim3_axis2)

TEST_F(GeneratedTests, softmax_v1_2_quant8) {
    execute(softmax_v1_2::CreateModel_quant8,
            softmax_v1_2::is_ignored_quant8,
            softmax_v1_2::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_v1_2_quant8, softmax_v1_2::CreateModel_quant8)

TEST_F(GeneratedTests, softmax_v1_2_quant8_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_quant8_dim1_axis0,
            softmax_v1_2::is_ignored_quant8_dim1_axis0,
            softmax_v1_2::get_examples_quant8_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_quant8_dim1_axis0, softmax_v1_2::CreateModel_quant8_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_quant8_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_quant8_dim3_axis2,
            softmax_v1_2::is_ignored_quant8_dim3_axis2,
            softmax_v1_2::get_examples_quant8_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_quant8_dim3_axis2, softmax_v1_2::CreateModel_quant8_dim3_axis2)

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape,
            softmax_v1_2::is_ignored_dynamic_output_shape,
            softmax_v1_2::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_dim1_axis0,
            softmax_v1_2::is_ignored_dynamic_output_shape_dim1_axis0,
            softmax_v1_2::get_examples_dynamic_output_shape_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_dim3_axis2,
            softmax_v1_2::is_ignored_dynamic_output_shape_dim3_axis2,
            softmax_v1_2::get_examples_dynamic_output_shape_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed_dim1_axis0,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed_dim1_axis0,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed_dim3_axis2,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed_dim3_axis2,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16,
            softmax_v1_2::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16_dim1_axis0,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16_dim1_axis0,
            softmax_v1_2::get_examples_dynamic_output_shape_float16_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16_dim3_axis2,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16_dim3_axis2,
            softmax_v1_2::get_examples_dynamic_output_shape_float16_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8_dim1_axis0,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8_dim1_axis0,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8_dim3_axis2,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8_dim3_axis2,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8_dim3_axis2());
}

TEST_F(GeneratedTests, softmax_v1_2_2) {
    execute(softmax_v1_2::CreateModel_2,
            softmax_v1_2::is_ignored_2,
            softmax_v1_2::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_v1_2_2, softmax_v1_2::CreateModel_2)

TEST_F(GeneratedTests, softmax_v1_2_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_dim1_axis0_2,
            softmax_v1_2::is_ignored_dim1_axis0_2,
            softmax_v1_2::get_examples_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_dim1_axis0_2, softmax_v1_2::CreateModel_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_dim3_axis2_2,
            softmax_v1_2::is_ignored_dim3_axis2_2,
            softmax_v1_2::get_examples_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_dim3_axis2_2, softmax_v1_2::CreateModel_dim3_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_relaxed_2) {
    execute(softmax_v1_2::CreateModel_relaxed_2,
            softmax_v1_2::is_ignored_relaxed_2,
            softmax_v1_2::get_examples_relaxed_2());
}

TEST_F(GeneratedTests, softmax_v1_2_relaxed_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_relaxed_dim1_axis0_2,
            softmax_v1_2::is_ignored_relaxed_dim1_axis0_2,
            softmax_v1_2::get_examples_relaxed_dim1_axis0_2());
}

TEST_F(GeneratedTests, softmax_v1_2_relaxed_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_relaxed_dim3_axis2_2,
            softmax_v1_2::is_ignored_relaxed_dim3_axis2_2,
            softmax_v1_2::get_examples_relaxed_dim3_axis2_2());
}

TEST_F(GeneratedTests, softmax_v1_2_float16_2) {
    execute(softmax_v1_2::CreateModel_float16_2,
            softmax_v1_2::is_ignored_float16_2,
            softmax_v1_2::get_examples_float16_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16_2, softmax_v1_2::CreateModel_float16_2)

TEST_F(GeneratedTests, softmax_v1_2_float16_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_float16_dim1_axis0_2,
            softmax_v1_2::is_ignored_float16_dim1_axis0_2,
            softmax_v1_2::get_examples_float16_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16_dim1_axis0_2, softmax_v1_2::CreateModel_float16_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_float16_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_float16_dim3_axis2_2,
            softmax_v1_2::is_ignored_float16_dim3_axis2_2,
            softmax_v1_2::get_examples_float16_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_float16_dim3_axis2_2, softmax_v1_2::CreateModel_float16_dim3_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_quant8_2) {
    execute(softmax_v1_2::CreateModel_quant8_2,
            softmax_v1_2::is_ignored_quant8_2,
            softmax_v1_2::get_examples_quant8_2());
}
TEST_AVAILABLE_SINCE(V1_0, softmax_v1_2_quant8_2, softmax_v1_2::CreateModel_quant8_2)

TEST_F(GeneratedTests, softmax_v1_2_quant8_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_quant8_dim1_axis0_2,
            softmax_v1_2::is_ignored_quant8_dim1_axis0_2,
            softmax_v1_2::get_examples_quant8_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_quant8_dim1_axis0_2, softmax_v1_2::CreateModel_quant8_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_quant8_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_quant8_dim3_axis2_2,
            softmax_v1_2::is_ignored_quant8_dim3_axis2_2,
            softmax_v1_2::get_examples_quant8_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_quant8_dim3_axis2_2, softmax_v1_2::CreateModel_quant8_dim3_axis2_2)

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_2,
            softmax_v1_2::get_examples_dynamic_output_shape_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_dim1_axis0_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_dim1_axis0_2,
            softmax_v1_2::get_examples_dynamic_output_shape_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_dim3_axis2_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_dim3_axis2_2,
            softmax_v1_2::get_examples_dynamic_output_shape_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed_2,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed_dim1_axis0_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed_dim1_axis0_2,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_relaxed_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_relaxed_dim3_axis2_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_relaxed_dim3_axis2_2,
            softmax_v1_2::get_examples_dynamic_output_shape_relaxed_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16_2,
            softmax_v1_2::get_examples_dynamic_output_shape_float16_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16_dim1_axis0_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16_dim1_axis0_2,
            softmax_v1_2::get_examples_dynamic_output_shape_float16_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_float16_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_float16_dim3_axis2_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_float16_dim3_axis2_2,
            softmax_v1_2::get_examples_dynamic_output_shape_float16_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8_2,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8_dim1_axis0_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8_dim1_axis0_2,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_dynamic_output_shape_quant8_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_dynamic_output_shape_quant8_dim3_axis2_2,
            softmax_v1_2::is_ignored_dynamic_output_shape_quant8_dim3_axis2_2,
            softmax_v1_2::get_examples_dynamic_output_shape_quant8_dim3_axis2_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis0,
            softmax_v1_2::is_ignored_axis_dim4_axis0,
            softmax_v1_2::get_examples_axis_dim4_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis0, softmax_v1_2::CreateModel_axis_dim4_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_dim4_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis0_neg, softmax_v1_2::CreateModel_axis_dim4_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis1,
            softmax_v1_2::is_ignored_axis_dim4_axis1,
            softmax_v1_2::get_examples_axis_dim4_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis1, softmax_v1_2::CreateModel_axis_dim4_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_dim4_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis1_neg, softmax_v1_2::CreateModel_axis_dim4_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis2,
            softmax_v1_2::is_ignored_axis_dim4_axis2,
            softmax_v1_2::get_examples_axis_dim4_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis2, softmax_v1_2::CreateModel_axis_dim4_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_dim4_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis2_neg, softmax_v1_2::CreateModel_axis_dim4_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis3,
            softmax_v1_2::is_ignored_axis_dim4_axis3,
            softmax_v1_2::get_examples_axis_dim4_axis3());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis3, softmax_v1_2::CreateModel_axis_dim4_axis3)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_dim4_axis3_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis3_neg, softmax_v1_2::CreateModel_axis_dim4_axis3_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis0,
            softmax_v1_2::is_ignored_axis_dim3_axis0,
            softmax_v1_2::get_examples_axis_dim3_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis0, softmax_v1_2::CreateModel_axis_dim3_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_dim3_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis0_neg, softmax_v1_2::CreateModel_axis_dim3_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis1,
            softmax_v1_2::is_ignored_axis_dim3_axis1,
            softmax_v1_2::get_examples_axis_dim3_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis1, softmax_v1_2::CreateModel_axis_dim3_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_dim3_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis1_neg, softmax_v1_2::CreateModel_axis_dim3_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis2,
            softmax_v1_2::is_ignored_axis_dim3_axis2,
            softmax_v1_2::get_examples_axis_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis2, softmax_v1_2::CreateModel_axis_dim3_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_dim3_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis2_neg, softmax_v1_2::CreateModel_axis_dim3_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis0,
            softmax_v1_2::is_ignored_axis_dim2_axis0,
            softmax_v1_2::get_examples_axis_dim2_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis0, softmax_v1_2::CreateModel_axis_dim2_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_dim2_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis0_neg, softmax_v1_2::CreateModel_axis_dim2_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis1,
            softmax_v1_2::is_ignored_axis_dim2_axis1,
            softmax_v1_2::get_examples_axis_dim2_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis1, softmax_v1_2::CreateModel_axis_dim2_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_dim2_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis1_neg, softmax_v1_2::CreateModel_axis_dim2_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dim1_axis0,
            softmax_v1_2::is_ignored_axis_dim1_axis0,
            softmax_v1_2::get_examples_axis_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim1_axis0, softmax_v1_2::CreateModel_axis_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_dim1_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim1_axis0_neg, softmax_v1_2::CreateModel_axis_dim1_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis0,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis0,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis0());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis0_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis1,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis1,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis1());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis1_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis2_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis3,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis3,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis3());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis3_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis0,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis0,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis0());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis0_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis1,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis1,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis1());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis1_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis2_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis0,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis0,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis0());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis0_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis1,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis1,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis1());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis1_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim1_axis0,
            softmax_v1_2::is_ignored_axis_relaxed_dim1_axis0,
            softmax_v1_2::get_examples_axis_relaxed_dim1_axis0());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_relaxed_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_relaxed_dim1_axis0_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis0,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis0,
            softmax_v1_2::get_examples_axis_float16_dim4_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis0, softmax_v1_2::CreateModel_axis_float16_dim4_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_float16_dim4_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis0_neg, softmax_v1_2::CreateModel_axis_float16_dim4_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis1,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis1,
            softmax_v1_2::get_examples_axis_float16_dim4_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis1, softmax_v1_2::CreateModel_axis_float16_dim4_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_float16_dim4_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis1_neg, softmax_v1_2::CreateModel_axis_float16_dim4_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis2, softmax_v1_2::CreateModel_axis_float16_dim4_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_float16_dim4_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis2_neg, softmax_v1_2::CreateModel_axis_float16_dim4_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis3,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis3,
            softmax_v1_2::get_examples_axis_float16_dim4_axis3());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis3, softmax_v1_2::CreateModel_axis_float16_dim4_axis3)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_float16_dim4_axis3_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis3_neg, softmax_v1_2::CreateModel_axis_float16_dim4_axis3_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis0,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis0,
            softmax_v1_2::get_examples_axis_float16_dim3_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis0, softmax_v1_2::CreateModel_axis_float16_dim3_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_float16_dim3_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis0_neg, softmax_v1_2::CreateModel_axis_float16_dim3_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis1,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis1,
            softmax_v1_2::get_examples_axis_float16_dim3_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis1, softmax_v1_2::CreateModel_axis_float16_dim3_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_float16_dim3_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis1_neg, softmax_v1_2::CreateModel_axis_float16_dim3_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis2, softmax_v1_2::CreateModel_axis_float16_dim3_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_float16_dim3_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis2_neg, softmax_v1_2::CreateModel_axis_float16_dim3_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis0,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis0,
            softmax_v1_2::get_examples_axis_float16_dim2_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis0, softmax_v1_2::CreateModel_axis_float16_dim2_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_float16_dim2_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis0_neg, softmax_v1_2::CreateModel_axis_float16_dim2_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis1,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis1,
            softmax_v1_2::get_examples_axis_float16_dim2_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis1, softmax_v1_2::CreateModel_axis_float16_dim2_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_float16_dim2_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis1_neg, softmax_v1_2::CreateModel_axis_float16_dim2_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim1_axis0,
            softmax_v1_2::is_ignored_axis_float16_dim1_axis0,
            softmax_v1_2::get_examples_axis_float16_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim1_axis0, softmax_v1_2::CreateModel_axis_float16_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_float16_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_float16_dim1_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim1_axis0_neg, softmax_v1_2::CreateModel_axis_float16_dim1_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis0,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis0,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis0, softmax_v1_2::CreateModel_axis_quant8_dim4_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis0_neg, softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis1,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis1,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis1, softmax_v1_2::CreateModel_axis_quant8_dim4_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis1_neg, softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis2_neg, softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis3,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis3,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis3());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis3, softmax_v1_2::CreateModel_axis_quant8_dim4_axis3)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis3_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis3_neg, softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis0,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis0,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis0, softmax_v1_2::CreateModel_axis_quant8_dim3_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis0_neg, softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis1,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis1,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis1, softmax_v1_2::CreateModel_axis_quant8_dim3_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis1_neg, softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis2_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis2_neg, softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis0,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis0,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis0, softmax_v1_2::CreateModel_axis_quant8_dim2_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis0_neg, softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis1,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis1,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis1());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis1, softmax_v1_2::CreateModel_axis_quant8_dim2_axis1)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis1_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis1_neg, softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_neg)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim1_axis0,
            softmax_v1_2::is_ignored_axis_quant8_dim1_axis0,
            softmax_v1_2::get_examples_axis_quant8_dim1_axis0());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim1_axis0, softmax_v1_2::CreateModel_axis_quant8_dim1_axis0)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_quant8_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_quant8_dim1_axis0_neg());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim1_axis0_neg, softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_neg)

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis3,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis3,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis3());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis3_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim1_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim1_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim1_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis3,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis3,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis3());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis3_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim1_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim1_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim1_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis3) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis3());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis3_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis2_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis1) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis1());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis1_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim1_axis0) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim1_axis0());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim1_axis0_neg) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_neg,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_neg,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_dim4_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis0_2, softmax_v1_2::CreateModel_axis_dim4_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dim4_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis0_neg_2, softmax_v1_2::CreateModel_axis_dim4_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_dim4_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis1_2, softmax_v1_2::CreateModel_axis_dim4_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dim4_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis1_neg_2, softmax_v1_2::CreateModel_axis_dim4_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_dim4_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis2_2, softmax_v1_2::CreateModel_axis_dim4_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dim4_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis2_neg_2, softmax_v1_2::CreateModel_axis_dim4_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_dim4_axis3_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis3_2, softmax_v1_2::CreateModel_axis_dim4_axis3_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_dim4_axis3_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim4_axis3_neg_2, softmax_v1_2::CreateModel_axis_dim4_axis3_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_dim3_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis0_2, softmax_v1_2::CreateModel_axis_dim3_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dim3_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis0_neg_2, softmax_v1_2::CreateModel_axis_dim3_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_dim3_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis1_2, softmax_v1_2::CreateModel_axis_dim3_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dim3_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis1_neg_2, softmax_v1_2::CreateModel_axis_dim3_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis2_2, softmax_v1_2::CreateModel_axis_dim3_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dim3_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim3_axis2_neg_2, softmax_v1_2::CreateModel_axis_dim3_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_dim2_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis0_2, softmax_v1_2::CreateModel_axis_dim2_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dim2_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis0_neg_2, softmax_v1_2::CreateModel_axis_dim2_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_dim2_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis1_2, softmax_v1_2::CreateModel_axis_dim2_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dim2_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim2_axis1_neg_2, softmax_v1_2::CreateModel_axis_dim2_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim1_axis0_2, softmax_v1_2::CreateModel_axis_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dim1_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_dim1_axis0_neg_2, softmax_v1_2::CreateModel_axis_dim1_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis0_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis0_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis1_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis1_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis2_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis2_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis3_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim4_axis3_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis0_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis0_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis1_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis1_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis2_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim3_axis2_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis0_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis0_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis1_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim2_axis1_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_relaxed_dim1_axis0_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_relaxed_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_relaxed_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_relaxed_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_relaxed_dim1_axis0_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis0_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis0_neg_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis1_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis1_neg_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis2_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis2_neg_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis3_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis3_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis3_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim4_axis3_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim4_axis3_neg_2, softmax_v1_2::CreateModel_axis_float16_dim4_axis3_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis0_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis0_neg_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis1_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis1_neg_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis2_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim3_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim3_axis2_neg_2, softmax_v1_2::CreateModel_axis_float16_dim3_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_float16_dim2_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis0_2, softmax_v1_2::CreateModel_axis_float16_dim2_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim2_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis0_neg_2, softmax_v1_2::CreateModel_axis_float16_dim2_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_float16_dim2_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis1_2, softmax_v1_2::CreateModel_axis_float16_dim2_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim2_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim2_axis1_neg_2, softmax_v1_2::CreateModel_axis_float16_dim2_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_float16_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_float16_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim1_axis0_2, softmax_v1_2::CreateModel_axis_float16_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_float16_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_float16_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_float16_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_float16_dim1_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_float16_dim1_axis0_neg_2, softmax_v1_2::CreateModel_axis_float16_dim1_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis0_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis0_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis1_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis1_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis2_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis2_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis3_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis3_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim4_axis3_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim4_axis3_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim4_axis3_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis0_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis0_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis1_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis1_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis2_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis2_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim3_axis2_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim3_axis2_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim3_axis2_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis0_2, softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis0_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim2_axis0_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis1_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis1_2, softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim2_axis1_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim2_axis1_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim2_axis1_neg_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_quant8_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_quant8_dim1_axis0_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim1_axis0_2, softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_2)

TEST_F(GeneratedTests, softmax_v1_2_axis_quant8_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_quant8_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_quant8_dim1_axis0_neg_2());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_axis_quant8_dim1_axis0_neg_2, softmax_v1_2::CreateModel_axis_quant8_dim1_axis0_neg_2)

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis3_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim4_axis3_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim3_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim2_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_dim1_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis3_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim4_axis3_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim3_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim2_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_float16_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_float16_dim1_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis3_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis2_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis1_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim1_axis0_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_2());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2) {
    execute(softmax_v1_2::CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2,
            softmax_v1_2::is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2,
            softmax_v1_2::get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2());
}

TEST_F(GeneratedTests, softmax_v1_2_zero_sized) {
    execute(softmax_v1_2::CreateModel_zero_sized,
            softmax_v1_2::is_ignored_zero_sized,
            softmax_v1_2::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_zero_sized, softmax_v1_2::CreateModel_zero_sized)

TEST_F(GeneratedTests, softmax_v1_2_zero_sized_relaxed) {
    execute(softmax_v1_2::CreateModel_zero_sized_relaxed,
            softmax_v1_2::is_ignored_zero_sized_relaxed,
            softmax_v1_2::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, softmax_v1_2_zero_sized_quant8) {
    execute(softmax_v1_2::CreateModel_zero_sized_quant8,
            softmax_v1_2::is_ignored_zero_sized_quant8,
            softmax_v1_2::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_zero_sized_quant8, softmax_v1_2::CreateModel_zero_sized_quant8)

TEST_F(GeneratedTests, softmax_v1_2_zero_sized_float16) {
    execute(softmax_v1_2::CreateModel_zero_sized_float16,
            softmax_v1_2::is_ignored_zero_sized_float16,
            softmax_v1_2::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, softmax_v1_2_zero_sized_float16, softmax_v1_2::CreateModel_zero_sized_float16)

TEST_F(DynamicOutputShapeTest, softmax_v1_2_zero_sized_dynamic_output_shape) {
    execute(softmax_v1_2::CreateModel_zero_sized_dynamic_output_shape,
            softmax_v1_2::is_ignored_zero_sized_dynamic_output_shape,
            softmax_v1_2::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_zero_sized_dynamic_output_shape_relaxed) {
    execute(softmax_v1_2::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            softmax_v1_2::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            softmax_v1_2::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_zero_sized_dynamic_output_shape_quant8) {
    execute(softmax_v1_2::CreateModel_zero_sized_dynamic_output_shape_quant8,
            softmax_v1_2::is_ignored_zero_sized_dynamic_output_shape_quant8,
            softmax_v1_2::get_examples_zero_sized_dynamic_output_shape_quant8());
}

TEST_F(DynamicOutputShapeTest, softmax_v1_2_zero_sized_dynamic_output_shape_float16) {
    execute(softmax_v1_2::CreateModel_zero_sized_dynamic_output_shape_float16,
            softmax_v1_2::is_ignored_zero_sized_dynamic_output_shape_float16,
            softmax_v1_2::get_examples_zero_sized_dynamic_output_shape_float16());
}

