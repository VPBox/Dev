// clang-format off
// Generated file (from: logistic_float_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace logistic_float_2 {
// Generated logistic_float_2 test
#include "generated/examples/logistic_float_2.example.cpp"
// Generated model constructor
#include "generated/models/logistic_float_2.model.cpp"
} // namespace logistic_float_2

TEST_F(GeneratedTests, logistic_float_2) {
    execute(logistic_float_2::CreateModel,
            logistic_float_2::is_ignored,
            logistic_float_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, logistic_float_2, logistic_float_2::CreateModel)

TEST_F(DynamicOutputShapeTest, logistic_float_2_dynamic_output_shape) {
    execute(logistic_float_2::CreateModel_dynamic_output_shape,
            logistic_float_2::is_ignored_dynamic_output_shape,
            logistic_float_2::get_examples_dynamic_output_shape());
}

