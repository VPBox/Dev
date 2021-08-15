// clang-format off
// Generated file (from: local_response_norm_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace local_response_norm_float_2 {
// Generated local_response_norm_float_2 test
#include "generated/examples/local_response_norm_float_2.example.cpp"
// Generated model constructor
#include "generated/models/local_response_norm_float_2.model.cpp"
} // namespace local_response_norm_float_2

TEST_F(GeneratedTests, local_response_norm_float_2) {
    execute(local_response_norm_float_2::CreateModel,
            local_response_norm_float_2::is_ignored,
            local_response_norm_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, local_response_norm_float_2, local_response_norm_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, local_response_norm_float_2_dynamic_output_shape) {
    execute(local_response_norm_float_2::CreateModel_dynamic_output_shape,
            local_response_norm_float_2::is_ignored_dynamic_output_shape,
            local_response_norm_float_2::get_examples_dynamic_output_shape());
}

