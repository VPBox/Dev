// clang-format off
// Generated file (from: space_to_depth_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace space_to_depth_quant8_1 {
// Generated space_to_depth_quant8_1 test
#include "generated/examples/space_to_depth_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/space_to_depth_quant8_1.model.cpp"
} // namespace space_to_depth_quant8_1

TEST_F(GeneratedTests, space_to_depth_quant8_1) {
    execute(space_to_depth_quant8_1::CreateModel,
            space_to_depth_quant8_1::is_ignored,
            space_to_depth_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, space_to_depth_quant8_1, space_to_depth_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, space_to_depth_quant8_1_dynamic_output_shape) {
    execute(space_to_depth_quant8_1::CreateModel_dynamic_output_shape,
            space_to_depth_quant8_1::is_ignored_dynamic_output_shape,
            space_to_depth_quant8_1::get_examples_dynamic_output_shape());
}

