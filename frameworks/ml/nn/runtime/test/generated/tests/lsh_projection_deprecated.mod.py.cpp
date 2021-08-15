// clang-format off
// Generated file (from: lsh_projection_deprecated.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lsh_projection_deprecated {
// Generated lsh_projection_deprecated test
#include "generated/examples/lsh_projection_deprecated.example.cpp"
// Generated model constructor
#include "generated/models/lsh_projection_deprecated.model.cpp"
} // namespace lsh_projection_deprecated

TEST_F(GeneratedTests, lsh_projection_deprecated) {
    execute(lsh_projection_deprecated::CreateModel,
            lsh_projection_deprecated::is_ignored,
            lsh_projection_deprecated::get_examples());
}

TEST_F(DynamicOutputShapeTest, lsh_projection_deprecated_dynamic_output_shape) {
    execute(lsh_projection_deprecated::CreateModel_dynamic_output_shape,
            lsh_projection_deprecated::is_ignored_dynamic_output_shape,
            lsh_projection_deprecated::get_examples_dynamic_output_shape());
}

