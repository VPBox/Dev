// clang-format off
// Generated file (from: squeeze.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace squeeze {
// Generated squeeze test
#include "generated/examples/squeeze.example.cpp"
// Generated model constructor
#include "generated/models/squeeze.model.cpp"
} // namespace squeeze

TEST_F(GeneratedTests, squeeze) {
    execute(squeeze::CreateModel,
            squeeze::is_ignored,
            squeeze::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, squeeze, squeeze::CreateModel)

TEST_F(DynamicOutputShapeTest, squeeze_dynamic_output_shape) {
    execute(squeeze::CreateModel_dynamic_output_shape,
            squeeze::is_ignored_dynamic_output_shape,
            squeeze::get_examples_dynamic_output_shape());
}

