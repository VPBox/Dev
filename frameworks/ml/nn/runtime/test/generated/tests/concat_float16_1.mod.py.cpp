// clang-format off
// Generated file (from: concat_float16_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace concat_float16_1 {
// Generated concat_float16_1 test
#include "generated/examples/concat_float16_1.example.cpp"
// Generated model constructor
#include "generated/models/concat_float16_1.model.cpp"
} // namespace concat_float16_1

TEST_F(GeneratedTests, concat_float16_1) {
    execute(concat_float16_1::CreateModel,
            concat_float16_1::is_ignored,
            concat_float16_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, concat_float16_1, concat_float16_1::CreateModel)

TEST_F(DynamicOutputShapeTest, concat_float16_1_dynamic_output_shape) {
    execute(concat_float16_1::CreateModel_dynamic_output_shape,
            concat_float16_1::is_ignored_dynamic_output_shape,
            concat_float16_1::get_examples_dynamic_output_shape());
}

