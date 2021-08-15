// clang-format off
// Generated file (from: lstm3.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm3 {
// Generated lstm3 test
#include "generated/examples/lstm3.example.cpp"
// Generated model constructor
#include "generated/models/lstm3.model.cpp"
} // namespace lstm3

TEST_F(GeneratedTests, lstm3) {
    execute(lstm3::CreateModel,
            lstm3::is_ignored,
            lstm3::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lstm3, lstm3::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm3_dynamic_output_shape) {
    execute(lstm3::CreateModel_dynamic_output_shape,
            lstm3::is_ignored_dynamic_output_shape,
            lstm3::get_examples_dynamic_output_shape());
}

