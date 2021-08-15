// clang-format off
// Generated file (from: bbox_graph.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace bbox_graph {
// Generated bbox_graph test
#include "generated/examples/bbox_graph.example.cpp"
// Generated model constructor
#include "generated/models/bbox_graph.model.cpp"
} // namespace bbox_graph

TEST_F(GeneratedTests, bbox_graph_zero_sized) {
    execute(bbox_graph::CreateModel_zero_sized,
            bbox_graph::is_ignored_zero_sized,
            bbox_graph::get_examples_zero_sized());
}
TEST_AVAILABLE_SINCE(V1_2, bbox_graph_zero_sized, bbox_graph::CreateModel_zero_sized)

TEST_F(GeneratedTests, bbox_graph_zero_sized_relaxed) {
    execute(bbox_graph::CreateModel_zero_sized_relaxed,
            bbox_graph::is_ignored_zero_sized_relaxed,
            bbox_graph::get_examples_zero_sized_relaxed());
}

TEST_F(GeneratedTests, bbox_graph_zero_sized_float16) {
    execute(bbox_graph::CreateModel_zero_sized_float16,
            bbox_graph::is_ignored_zero_sized_float16,
            bbox_graph::get_examples_zero_sized_float16());
}
TEST_AVAILABLE_SINCE(V1_2, bbox_graph_zero_sized_float16, bbox_graph::CreateModel_zero_sized_float16)

TEST_F(GeneratedTests, bbox_graph_zero_sized_quant8) {
    execute(bbox_graph::CreateModel_zero_sized_quant8,
            bbox_graph::is_ignored_zero_sized_quant8,
            bbox_graph::get_examples_zero_sized_quant8());
}
TEST_AVAILABLE_SINCE(V1_2, bbox_graph_zero_sized_quant8, bbox_graph::CreateModel_zero_sized_quant8)

TEST_F(DynamicOutputShapeTest, bbox_graph_zero_sized_dynamic_output_shape) {
    execute(bbox_graph::CreateModel_zero_sized_dynamic_output_shape,
            bbox_graph::is_ignored_zero_sized_dynamic_output_shape,
            bbox_graph::get_examples_zero_sized_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, bbox_graph_zero_sized_dynamic_output_shape_relaxed) {
    execute(bbox_graph::CreateModel_zero_sized_dynamic_output_shape_relaxed,
            bbox_graph::is_ignored_zero_sized_dynamic_output_shape_relaxed,
            bbox_graph::get_examples_zero_sized_dynamic_output_shape_relaxed());
}

TEST_F(DynamicOutputShapeTest, bbox_graph_zero_sized_dynamic_output_shape_float16) {
    execute(bbox_graph::CreateModel_zero_sized_dynamic_output_shape_float16,
            bbox_graph::is_ignored_zero_sized_dynamic_output_shape_float16,
            bbox_graph::get_examples_zero_sized_dynamic_output_shape_float16());
}

TEST_F(DynamicOutputShapeTest, bbox_graph_zero_sized_dynamic_output_shape_quant8) {
    execute(bbox_graph::CreateModel_zero_sized_dynamic_output_shape_quant8,
            bbox_graph::is_ignored_zero_sized_dynamic_output_shape_quant8,
            bbox_graph::get_examples_zero_sized_dynamic_output_shape_quant8());
}

