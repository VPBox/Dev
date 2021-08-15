// clang-format off
// Generated file (from: local_response_norm_float_4.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace local_response_norm_float_4 {
// Generated local_response_norm_float_4 test
#include "generated/examples/local_response_norm_float_4.example.cpp"
// Generated model constructor
#include "generated/models/local_response_norm_float_4.model.cpp"
} // namespace local_response_norm_float_4

TEST_F(GeneratedTests, local_response_norm_float_4) {
    execute(local_response_norm_float_4::CreateModel,
            local_response_norm_float_4::is_ignored,
            local_response_norm_float_4::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, local_response_norm_float_4, local_response_norm_float_4::CreateModel)

TEST_F(DynamicOutputShapeTest, local_response_norm_float_4_dynamic_output_shape) {
    execute(local_response_norm_float_4::CreateModel_dynamic_output_shape,
            local_response_norm_float_4::is_ignored_dynamic_output_shape,
            local_response_norm_float_4::get_examples_dynamic_output_shape());
}

