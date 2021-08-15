// clang-format off
// Generated file (from: mean.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace mean {
// Generated mean test
#include "generated/examples/mean.example.cpp"
// Generated model constructor
#include "generated/models/mean.model.cpp"
} // namespace mean

TEST_F(GeneratedTests, mean) {
    execute(mean::CreateModel,
            mean::is_ignored,
            mean::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, mean, mean::CreateModel)

TEST_F(DynamicOutputShapeTest, mean_dynamic_output_shape) {
    execute(mean::CreateModel_dynamic_output_shape,
            mean::is_ignored_dynamic_output_shape,
            mean::get_examples_dynamic_output_shape());
}

