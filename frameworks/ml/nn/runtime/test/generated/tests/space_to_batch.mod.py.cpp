// clang-format off
// Generated file (from: space_to_batch.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_batch {
// Generated space_to_batch test
#include "generated/examples/space_to_batch.example.cpp"
// Generated model constructor
#include "generated/models/space_to_batch.model.cpp"
} // namespace space_to_batch

TEST_F(GeneratedTests, space_to_batch) {
    execute(space_to_batch::CreateModel,
            space_to_batch::is_ignored,
            space_to_batch::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, space_to_batch, space_to_batch::CreateModel)

TEST_F(DynamicOutputShapeTest, space_to_batch_dynamic_output_shape) {
    execute(space_to_batch::CreateModel_dynamic_output_shape,
            space_to_batch::is_ignored_dynamic_output_shape,
            space_to_batch::get_examples_dynamic_output_shape());
}

