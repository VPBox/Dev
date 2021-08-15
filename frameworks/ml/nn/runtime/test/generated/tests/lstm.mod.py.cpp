// clang-format off
// Generated file (from: lstm.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace lstm {
// Generated lstm test
#include "generated/examples/lstm.example.cpp"
// Generated model constructor
#include "generated/models/lstm.model.cpp"
} // namespace lstm

TEST_F(GeneratedTests, lstm) {
    execute(lstm::CreateModel,
            lstm::is_ignored,
            lstm::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, lstm, lstm::CreateModel)

TEST_F(DynamicOutputShapeTest, lstm_dynamic_output_shape) {
    execute(lstm::CreateModel_dynamic_output_shape,
            lstm::is_ignored_dynamic_output_shape,
            lstm::get_examples_dynamic_output_shape());
}

