// clang-format off
// Generated file (from: lsh_projection.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lsh_projection {
// Generated lsh_projection test
#include "generated/examples/lsh_projection.example.cpp"
// Generated model constructor
#include "generated/models/lsh_projection.model.cpp"
} // namespace lsh_projection

TEST_F(GeneratedTests, lsh_projection) {
    execute(lsh_projection::CreateModel,
            lsh_projection::is_ignored,
            lsh_projection::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lsh_projection, lsh_projection::CreateModel)

TEST_F(DynamicOutputShapeTest, lsh_projection_dynamic_output_shape) {
    execute(lsh_projection::CreateModel_dynamic_output_shape,
            lsh_projection::is_ignored_dynamic_output_shape,
            lsh_projection::get_examples_dynamic_output_shape());
}

