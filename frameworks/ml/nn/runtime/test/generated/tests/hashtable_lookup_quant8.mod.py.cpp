// clang-format off
// Generated file (from: hashtable_lookup_quant8.mod.py). Do not edit
#include "../../TestGenerated.h"

namespace hashtable_lookup_quant8 {
// Generated hashtable_lookup_quant8 test
#include "generated/examples/hashtable_lookup_quant8.example.cpp"
// Generated model constructor
#include "generated/models/hashtable_lookup_quant8.model.cpp"
} // namespace hashtable_lookup_quant8

TEST_F(GeneratedTests, hashtable_lookup_quant8) {
    execute(hashtable_lookup_quant8::CreateModel,
            hashtable_lookup_quant8::is_ignored,
            hashtable_lookup_quant8::get_examples());
}
TEST_AVAILABLE_SINCE(V1_0, hashtable_lookup_quant8, hashtable_lookup_quant8::CreateModel)

TEST_F(DynamicOutputShapeTest, hashtable_lookup_quant8_dynamic_output_shape) {
    execute(hashtable_lookup_quant8::CreateModel_dynamic_output_shape,
            hashtable_lookup_quant8::is_ignored_dynamic_output_shape,
            hashtable_lookup_quant8::get_examples_dynamic_output_shape());
}

