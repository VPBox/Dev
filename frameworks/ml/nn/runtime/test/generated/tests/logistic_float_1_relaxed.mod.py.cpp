// clang-format off
// Generated file (from: logistic_float_1_relaxed.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace logistic_float_1_relaxed {
// Generated logistic_float_1_relaxed test
#include "generated/examples/logistic_float_1_relaxed.example.cpp"
// Generated model constructor
#include "generated/models/logistic_float_1_relaxed.model.cpp"
} // namespace logistic_float_1_relaxed

TEST_F(GeneratedTests, logistic_float_1_relaxed) {
    execute(logistic_float_1_relaxed::CreateModel,
            logistic_float_1_relaxed::is_ignored,
            logistic_float_1_relaxed::get_examples());
}

TEST_F(DynamicOutputShapeTest, logistic_float_1_relaxed_dynamic_output_shape) {
    execute(logistic_float_1_relaxed::CreateModel_dynamic_output_shape,
            logistic_float_1_relaxed::is_ignored_dynamic_output_shape,
            logistic_float_1_relaxed::get_examples_dynamic_output_shape());
}

