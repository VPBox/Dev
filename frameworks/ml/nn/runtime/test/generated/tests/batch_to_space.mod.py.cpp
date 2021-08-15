// clang-format off
// Generated file (from: batch_to_space.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace batch_to_space {
// Generated batch_to_space test
#include "generated/examples/batch_to_space.example.cpp"
// Generated model constructor
#include "generated/models/batch_to_space.model.cpp"
} // namespace batch_to_space

TEST_F(GeneratedTests, batch_to_space) {
    execute(batch_to_space::CreateModel,
            batch_to_space::is_ignored,
            batch_to_space::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, batch_to_space, batch_to_space::CreateModel)

TEST_F(DynamicOutputShapeTest, batch_to_space_dynamic_output_shape) {
    execute(batch_to_space::CreateModel_dynamic_output_shape,
            batch_to_space::is_ignored_dynamic_output_shape,
            batch_to_space::get_examples_dynamic_output_shape());
}

