// clang-format off
// Generated file (from: random_multinomial.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace random_multinomial {
// Generated random_multinomial test
#include "generated/examples/random_multinomial.example.cpp"
// Generated model constructor
#include "generated/models/random_multinomial.model.cpp"
} // namespace random_multinomial

TEST_F(GeneratedTests, random_multinomial) {
    execute(random_multinomial::CreateModel,
            random_multinomial::is_ignored,
            random_multinomial::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, random_multinomial, random_multinomial::CreateModel)

TEST_F(DynamicOutputShapeTest, random_multinomial_dynamic_output_shape) {
    execute(random_multinomial::CreateModel_dynamic_output_shape,
            random_multinomial::is_ignored_dynamic_output_shape,
            random_multinomial::get_examples_dynamic_output_shape());
}

