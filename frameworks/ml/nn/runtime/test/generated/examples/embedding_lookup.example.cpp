// clang-format off
// Generated file (from: embedding_lookup.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3}}, {1, {3, 2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{1, {0.0f, 0.01f, 0.02f, 0.03f, 0.1f, 0.11f, 0.12000000000000001f, 0.13f, 1.0f, 1.01f, 1.02f, 1.03f, 1.1f, 1.11f, 1.12f, 1.1300000000000001f, 2.0f, 2.01f, 2.02f, 2.03f, 2.1f, 2.11f, 2.12f, 2.13f}}},
  // int -> INT32 map
  .int32Operands = {{0, {1, 0, 2}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.01f, 1.02f, 1.03f, 1.1f, 1.11f, 1.12f, 1.13f, 0.0f, 0.01f, 0.02f, 0.03f, 0.1f, 0.11f, 0.12f, 0.13f, 2.0f, 2.01f, 2.02f, 2.03f, 2.1f, 2.11f, 2.12f, 2.13f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3}}, {1, {3, 2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{1, {0.0f, 0.01f, 0.02f, 0.03f, 0.1f, 0.11f, 0.12000000000000001f, 0.13f, 1.0f, 1.01f, 1.02f, 1.03f, 1.1f, 1.11f, 1.12f, 1.1300000000000001f, 2.0f, 2.01f, 2.02f, 2.03f, 2.1f, 2.11f, 2.12f, 2.13f}}},
  // int -> INT32 map
  .int32Operands = {{0, {1, 0, 2}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.01f, 1.02f, 1.03f, 1.1f, 1.11f, 1.12f, 1.13f, 0.0f, 0.01f, 0.02f, 0.03f, 0.1f, 0.11f, 0.12f, 0.13f, 2.0f, 2.01f, 2.02f, 2.03f, 2.1f, 2.11f, 2.12f, 2.13f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape;
};

