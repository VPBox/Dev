// clang-format off
// Generated file (from: div.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace div {
// Generated div test
#include "generated/examples/div.example.cpp"
// Generated model constructor
#include "generated/models/div.model.cpp"
} // namespace div

TEST_F(GeneratedTests, div) {
    execute(div::CreateModel,
            div::is_ignored,
            div::get_examples());
}
TEST_AVAILABLE_SINCE(V1_1, div, div::CreateModel)

TEST_F(DynamicOutputShapeTest, div_dynamic_output_shape) {
    execute(div::CreateModel_dynamic_output_shape,
            div::is_ignored_dynamic_output_shape,
            div::get_examples_dynamic_output_shape());
}

