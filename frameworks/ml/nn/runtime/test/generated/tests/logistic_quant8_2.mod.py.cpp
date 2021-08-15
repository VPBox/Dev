// clang-format off
// Generated file (from: logistic_quant8_2.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace logistic_quant8_2 {
// Generated logistic_quant8_2 test
#include "generated/examples/logistic_quant8_2.example.cpp"
// Generated model constructor
#include "generated/models/logistic_quant8_2.model.cpp"
} // namespace logistic_quant8_2

TEST_F(GeneratedTests, logistic_quant8_2) {
    execute(logistic_quant8_2::CreateModel,
            logistic_quant8_2::is_ignored,
            logistic_quant8_2::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, logistic_quant8_2, logistic_quant8_2::CreateModel)

TEST_F(DynamicOutputShapeTest, logistic_quant8_2_dynamic_output_shape) {
    execute(logistic_quant8_2::CreateModel_dynamic_output_shape,
            logistic_quant8_2::is_ignored_dynamic_output_shape,
            logistic_quant8_2::get_examples_dynamic_output_shape());
}

