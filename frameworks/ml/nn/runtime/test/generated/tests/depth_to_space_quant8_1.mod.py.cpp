// clang-format off
// Generated file (from: depth_to_space_quant8_1.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace depth_to_space_quant8_1 {
// Generated depth_to_space_quant8_1 test
#include "generated/examples/depth_to_space_quant8_1.example.cpp"
// Generated model constructor
#include "generated/models/depth_to_space_quant8_1.model.cpp"
} // namespace depth_to_space_quant8_1

TEST_F(GeneratedTests, depth_to_space_quant8_1) {
    execute(depth_to_space_quant8_1::CreateModel,
            depth_to_space_quant8_1::is_ignored,
            depth_to_space_quant8_1::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, depth_to_space_quant8_1, depth_to_space_quant8_1::CreateModel)

TEST_F(DynamicOutputShapeTest, depth_to_space_quant8_1_dynamic_output_shape) {
    execute(depth_to_space_quant8_1::CreateModel_dynamic_output_shape,
            depth_to_space_quant8_1::is_ignored_dynamic_output_shape,
            depth_to_space_quant8_1::get_examples_dynamic_output_shape());
}

