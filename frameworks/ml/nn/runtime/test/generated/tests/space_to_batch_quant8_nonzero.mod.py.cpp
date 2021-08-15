// clang-format off
// Generated file (from: space_to_batch_quant8_nonzero.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_batch_quant8_nonzero {
// Generated space_to_batch_quant8_nonzero test
#include "generated/examples/space_to_batch_quant8_nonzero.example.cpp"
// Generated model constructor
#include "generated/models/space_to_batch_quant8_nonzero.model.cpp"
} // namespace space_to_batch_quant8_nonzero

TEST_F(GeneratedTests, space_to_batch_quant8_nonzero) {
    execute(space_to_batch_quant8_nonzero::CreateModel,
            space_to_batch_quant8_nonzero::is_ignored,
            space_to_batch_quant8_nonzero::get_examples());
}
TEST_AVAILABLE_SINCE(V1_2, space_to_batch_quant8_nonzero, space_to_batch_quant8_nonzero::CreateModel)

TEST_F(DynamicOutputShapeTest, space_to_batch_quant8_nonzero_dynamic_output_shape) {
    execute(space_to_batch_quant8_nonzero::CreateModel_dynamic_output_shape,
            space_to_batch_quant8_nonzero::is_ignored_dynamic_output_shape,
            space_to_batch_quant8_nonzero::get_examples_dynamic_output_shape());
}

