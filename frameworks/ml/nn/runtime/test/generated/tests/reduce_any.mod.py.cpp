// clang-format off
// Generated file (from: reduce_any.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reduce_any {
// Generated reduce_any test
#include "generated/examples/reduce_any.example.cpp"
// Generated model constructor
#include "generated/models/reduce_any.model.cpp"
} // namespace reduce_any

TEST_F(GeneratedTests, reduce_any) {
    execute(reduce_any::CreateModel,
            reduce_any::is_ignored,
            reduce_any::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_any, reduce_any::CreateModel)

TEST_F(DynamicOutputShapeTest, reduce_any_dynamic_output_shape) {
    execute(reduce_any::CreateModel_dynamic_output_shape,
            reduce_any::is_ignored_dynamic_output_shape,
            reduce_any::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, reduce_any_2) {
    execute(reduce_any::CreateModel_2,
            reduce_any::is_ignored_2,
            reduce_any::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_any_2, reduce_any::CreateModel_2)

TEST_F(DynamicOutputShapeTest, reduce_any_dynamic_output_shape_2) {
    execute(reduce_any::CreateModel_dynamic_output_shape_2,
            reduce_any::is_ignored_dynamic_output_shape_2,
            reduce_any::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, reduce_any_3) {
    execute(reduce_any::CreateModel_3,
            reduce_any::is_ignored_3,
            reduce_any::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_any_3, reduce_any::CreateModel_3)

TEST_F(DynamicOutputShapeTest, reduce_any_dynamic_output_shape_3) {
    execute(reduce_any::CreateModel_dynamic_output_shape_3,
            reduce_any::is_ignored_dynamic_output_shape_3,
            reduce_any::get_examples_dynamic_output_shape_3());
}

