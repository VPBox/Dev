// clang-format off
// Generated file (from: pad_quant8_nonzero.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace pad_quant8_nonzero {
// Generated pad_quant8_nonzero test
#include "generated/examples/pad_quant8_nonzero.example.cpp"
// Generated model constructor
#include "generated/models/pad_quant8_nonzero.model.cpp"
} // namespace pad_quant8_nonzero

TEST_F(GeneratedTests, pad_quant8_nonzero) {
    execute(pad_quant8_nonzero::CreateModel,
            pad_quant8_nonzero::is_ignored,
            pad_quant8_nonzero::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, pad_quant8_nonzero, pad_quant8_nonzero::CreateModel)

TEST_F(DynamicOutputShapeTest, pad_quant8_nonzero_dynamic_output_shape) {
    execute(pad_quant8_nonzero::CreateModel_dynamic_output_shape,
            pad_quant8_nonzero::is_ignored_dynamic_output_shape,
            pad_quant8_nonzero::get_examples_dynamic_output_shape());
}

