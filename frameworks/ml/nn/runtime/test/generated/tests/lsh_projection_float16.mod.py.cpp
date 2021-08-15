// clang-format off
// Generated file (from: lsh_projection_float16.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lsh_projection_float16 {
// Generated lsh_projection_float16 test
#include "generated/examples/lsh_projection_float16.example.cpp"
// Generated model constructor
#include "generated/models/lsh_projection_float16.model.cpp"
} // namespace lsh_projection_float16

TEST_F(GeneratedTests, lsh_projection_float16) {
    execute(lsh_projection_float16::CreateModel,
            lsh_projection_float16::is_ignored,
            lsh_projection_float16::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, lsh_projection_float16, lsh_projection_float16::CreateModel)

TEST_F(GeneratedTests, lsh_projection_float16_float16) {
    execute(lsh_projection_float16::CreateModel_float16,
            lsh_projection_float16::is_ignored_float16,
            lsh_projection_float16::get_examples_float16());
}
TEST_AVAILABLE_SINCE(V1_2, lsh_projection_float16_float16, lsh_projection_float16::CreateModel_float16)

TEST_F(DynamicOutputShapeTest, lsh_projection_float16_dynamic_output_shape) {
    execute(lsh_projection_float16::CreateModel_dynamic_output_shape,
            lsh_projection_float16::is_ignored_dynamic_output_shape,
            lsh_projection_float16::get_examples_dynamic_output_shape());
}

TEST_F(DynamicOutputShapeTest, lsh_projection_float16_dynamic_output_shape_float16) {
    execute(lsh_projection_float16::CreateModel_dynamic_output_shape_float16,
            lsh_projection_float16::is_ignored_dynamic_output_shape_float16,
            lsh_projection_float16::get_examples_dynamic_output_shape_float16());
}

