// clang-format off
// Generated file (from: reduce_all.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace reduce_all {
// Generated reduce_all test
#include "generated/examples/reduce_all.example.cpp"
// Generated model constructor
#include "generated/models/reduce_all.model.cpp"
} // namespace reduce_all

TEST_F(GeneratedTests, reduce_all) {
    execute(reduce_all::CreateModel,
            reduce_all::is_ignored,
            reduce_all::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_all, reduce_all::CreateModel)

TEST_F(DynamicOutputShapeTest, reduce_all_dynamic_output_shape) {
    execute(reduce_all::CreateModel_dynamic_output_shape,
            reduce_all::is_ignored_dynamic_output_shape,
            reduce_all::get_examples_dynamic_output_shape());
}

TEST_F(GeneratedTests, reduce_all_2) {
    execute(reduce_all::CreateModel_2,
            reduce_all::is_ignored_2,
            reduce_all::get_examples_2());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_all_2, reduce_all::CreateModel_2)

TEST_F(DynamicOutputShapeTest, reduce_all_dynamic_output_shape_2) {
    execute(reduce_all::CreateModel_dynamic_output_shape_2,
            reduce_all::is_ignored_dynamic_output_shape_2,
            reduce_all::get_examples_dynamic_output_shape_2());
}

TEST_F(GeneratedTests, reduce_all_3) {
    execute(reduce_all::CreateModel_3,
            reduce_all::is_ignored_3,
            reduce_all::get_examples_3());
}
TEST_AVAILABLE_SINCE(V1_2, reduce_all_3, reduce_all::CreateModel_3)

TEST_F(DynamicOutputShapeTest, reduce_all_dynamic_output_shape_3) {
    execute(reduce_all::CreateModel_dynamic_output_shape_3,
            reduce_all::is_ignored_dynamic_output_shape_3,
            reduce_all::get_examples_dynamic_output_shape_3());
}

