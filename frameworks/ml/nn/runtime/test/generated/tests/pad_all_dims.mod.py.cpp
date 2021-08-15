// clang-format off
// Generated file (from: pad_all_dims.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_all_dims {
// Generated pad_all_dims test
#include "generated/examples/pad_all_dims.example.cpp"
// Generated model constructor
#include "generated/models/pad_all_dims.model.cpp"
} // namespace pad_all_dims

TEST_F(GeneratedTests, pad_all_dims) {
    execute(pad_all_dims::CreateModel,
            pad_all_dims::is_ignored,
            pad_all_dims::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, pad_all_dims, pad_all_dims::CreateModel)

TEST_F(GeneratedTests, pad_all_dims_float16) {
    execute(pad_all_dims::CreateModel_float16,
            pad_all_dims::is_ignored_float16,
            pad_all_dims::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, pad_all_dims_float16, pad_all_dims::CreateModel_float16)

TEST_F(GeneratedTests, pad_all_dims_relaxed) {
    execute(pad_all_dims::CreateModel_relaxed,
            pad_all_dims::is_ignored_relaxed,
            pad_all_dims::get_examples_relaxed());
}

TEST_F(GeneratedTests, pad_all_dims_quant8) {
    execute(pad_all_dims::CreateModel_quant8,
            pad_all_dims::is_ignored_quant8,
            pad_all_dims::get_examples_quant8());
}
TEST_AVAILABLE_SINCE(V1_1, pad_all_dims_quant8, pad_all_dims::CreateModel_quant8)

TEST_F(DynamicOutputShapeTest, pad_all_dims_dynamic_output_shape) {
    execute(pad_all_dims::CreateModel_dynamic_output_shape,
            pad_all_dims::is_ignored_dynamic_output_shape,
            pad_all_dims::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, pad_all_dims_dynamic_output_shape_float16) {
    execute(pad_all_dims::CreateModel_dynamic_output_shape_float16,
            pad_all_dims::is_ignored_dynamic_output_shape_float16,
            pad_all_dims::get_examples_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, pad_all_dims_dynamic_output_shape_relaxed) {
    execute(pad_all_dims::CreateModel_dynamic_output_shape_relaxed,
            pad_all_dims::is_ignored_dynamic_output_shape_relaxed,
            pad_all_dims::get_examples_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, pad_all_dims_dynamic_output_shape_quant8) {
    execute(pad_all_dims::CreateModel_dynamic_output_shape_quant8,
            pad_all_dims::is_ignored_dynamic_output_shape_quant8,
            pad_all_dims::get_examples_dynamic_output_shape_quant8());
}

