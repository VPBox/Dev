// clang-format off
// Generated file (from: sub_quantized_different_scales.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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

std::vector<MixedTypedExample>& get_examples_2() {
static std::vector<MixedTypedExample> examples_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_dynamic_output_shape_2;
};

std::vector<MixedTypedExample>& get_examples_3() {
static std::vector<MixedTypedExample> examples_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120}}},
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
return examples_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120}}},
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
return examples_dynamic_output_shape_3;
};

std::vector<MixedTypedExample>& get_examples_4() {
static std::vector<MixedTypedExample> examples_4 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_4;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_4() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_4 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_dynamic_output_shape_4;
};

std::vector<MixedTypedExample>& get_examples_5() {
static std::vector<MixedTypedExample> examples_5 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_5;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_5() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_5 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_dynamic_output_shape_5;
};

std::vector<MixedTypedExample>& get_examples_6() {
static std::vector<MixedTypedExample> examples_6 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 7, 6, 5, 4, 3, 2, 0, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1, 255, 255, 255, 254, 253, 252, 7, 6, 5, 4, 3, 2}}},
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
return examples_6;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_6() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_6 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 7, 6, 5, 4, 3, 2, 0, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1, 255, 255, 255, 254, 253, 252, 7, 6, 5, 4, 3, 2}}},
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
return examples_dynamic_output_shape_6;
};

std::vector<MixedTypedExample>& get_examples_7() {
static std::vector<MixedTypedExample> examples_7 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 220, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220}}},
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
return examples_7;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_7() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_7 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 220, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220}}},
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
return examples_dynamic_output_shape_7;
};

std::vector<MixedTypedExample>& get_examples_8() {
static std::vector<MixedTypedExample> examples_8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 121, 120, 120, 120, 120, 120, 96, 96, 95, 95, 95, 95, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 146, 145, 145, 145, 145, 121, 120, 120, 120, 120, 120}}},
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
return examples_8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 121, 120, 120, 120, 120, 120, 96, 96, 95, 95, 95, 95, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 146, 145, 145, 145, 145, 121, 120, 120, 120, 120, 120}}},
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
return examples_dynamic_output_shape_8;
};

std::vector<MixedTypedExample>& get_examples_9() {
static std::vector<MixedTypedExample> examples_9 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254}}},
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
return examples_9;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_9() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_9 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254}}},
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
return examples_dynamic_output_shape_9;
};

std::vector<MixedTypedExample>& get_examples_10() {
static std::vector<MixedTypedExample> examples_10 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_10;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_10() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_10 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_10;
};

std::vector<MixedTypedExample>& get_examples_11() {
static std::vector<MixedTypedExample> examples_11 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 90, 89, 88, 87, 86, 85, 255, 255, 255, 255, 255, 255, 190, 189, 188, 187, 186, 185, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_11;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_11() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_11 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 90, 89, 88, 87, 86, 85, 255, 255, 255, 255, 255, 255, 190, 189, 188, 187, 186, 185, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_11;
};

std::vector<MixedTypedExample>& get_examples_12() {
static std::vector<MixedTypedExample> examples_12 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145}}},
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
return examples_12;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_12() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_12 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145}}},
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
return examples_dynamic_output_shape_12;
};

std::vector<MixedTypedExample>& get_examples_13() {
static std::vector<MixedTypedExample> examples_13 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_13;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_13() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_13 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_13;
};

std::vector<MixedTypedExample>& get_examples_14() {
static std::vector<MixedTypedExample> examples_14 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_14;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_14() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_14 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_14;
};

std::vector<MixedTypedExample>& get_examples_15() {
static std::vector<MixedTypedExample> examples_15 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_15;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_15() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_15 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_15;
};

std::vector<MixedTypedExample>& get_examples_16() {
static std::vector<MixedTypedExample> examples_16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 240, 238, 238, 236, 236, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 16, 14, 14, 12, 12, 10}}},
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
return examples_16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 240, 238, 238, 236, 236, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 16, 14, 14, 12, 12, 10}}},
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
return examples_dynamic_output_shape_16;
};

std::vector<MixedTypedExample>& get_examples_17() {
static std::vector<MixedTypedExample> examples_17 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 249, 248, 247, 246, 245, 244, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0}}},
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
return examples_17;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_17() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_17 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 249, 248, 247, 246, 245, 244, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0}}},
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
return examples_dynamic_output_shape_17;
};

std::vector<MixedTypedExample>& get_examples_18() {
static std::vector<MixedTypedExample> examples_18 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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
return examples_18;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_18() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_18 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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
return examples_dynamic_output_shape_18;
};

std::vector<MixedTypedExample>& get_examples_19() {
static std::vector<MixedTypedExample> examples_19 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 20, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20}}},
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
return examples_19;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_19() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_19 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 20, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20}}},
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
return examples_dynamic_output_shape_19;
};

std::vector<MixedTypedExample>& get_examples_20() {
static std::vector<MixedTypedExample> examples_20 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 145, 145, 145, 145, 144, 144, 120, 120, 120, 120, 120, 119, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_20;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_20() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_20 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 145, 145, 145, 145, 144, 144, 120, 120, 120, 120, 120, 119, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_dynamic_output_shape_20;
};

std::vector<MixedTypedExample>& get_examples_21() {
static std::vector<MixedTypedExample> examples_21 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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
return examples_21;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_21() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_21 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 250, 249, 248, 247, 246, 245, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0}}},
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
return examples_dynamic_output_shape_21;
};

std::vector<MixedTypedExample>& get_examples_22() {
static std::vector<MixedTypedExample> examples_22 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_22;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_22() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_22 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 251, 250, 249, 248, 247, 246, 1, 0, 0, 0, 0, 0, 252, 251, 250, 249, 248, 247, 2, 1, 0, 0, 0, 0, 253, 252, 251, 250, 249, 248, 3, 2, 1, 0, 0, 0, 254, 253, 252, 251, 250, 249, 4, 3, 2, 1, 0, 0, 255, 254, 253, 252, 251, 250, 5, 4, 3, 2, 1, 0, 255, 255, 254, 253, 252, 251, 6, 5, 4, 3, 2, 1}}},
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
return examples_dynamic_output_shape_22;
};

std::vector<MixedTypedExample>& get_examples_23() {
static std::vector<MixedTypedExample> examples_23 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120}}},
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
return examples_23;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_23() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_23 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 220, 120, 20, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 220, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 20, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120, 20, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 220, 120}}},
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
return examples_dynamic_output_shape_23;
};

std::vector<MixedTypedExample>& get_examples_24() {
static std::vector<MixedTypedExample> examples_24 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_24;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_24() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_24 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 96, 95, 95, 95, 95, 95, 145, 145, 145, 145, 145, 144, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120, 146, 145, 145, 145, 145, 145, 120, 120, 120, 120, 120, 120}}},
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
return examples_dynamic_output_shape_24;
};

std::vector<MixedTypedExample>& get_examples_25() {
static std::vector<MixedTypedExample> examples_25 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 250, 250, 250, 250, 250, 250, 248, 248, 248, 248, 248, 248, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253}}},
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
return examples_25;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_25() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_25 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 250, 250, 250, 250, 250, 250, 248, 248, 248, 248, 248, 248, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253}}},
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
return examples_dynamic_output_shape_25;
};

std::vector<MixedTypedExample>& get_examples_26() {
static std::vector<MixedTypedExample> examples_26 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254}}},
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
return examples_26;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_26() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_26 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 251, 251, 251, 251, 251, 251, 249, 249, 249, 249, 249, 249, 252, 252, 252, 252, 252, 252, 250, 250, 250, 250, 250, 250, 253, 253, 253, 253, 253, 253, 251, 251, 251, 251, 251, 251, 254, 254, 254, 254, 254, 254, 252, 252, 252, 252, 252, 252, 255, 255, 255, 255, 255, 255, 253, 253, 253, 253, 253, 253, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254}}},
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
return examples_dynamic_output_shape_26;
};

std::vector<MixedTypedExample>& get_examples_27() {
static std::vector<MixedTypedExample> examples_27 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {140, 139, 138, 137, 136, 135, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 90, 89, 88, 87, 86, 85, 255, 255, 255, 255, 255, 255, 190, 189, 188, 187, 186, 185, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_27;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_27() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_27 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {140, 139, 138, 137, 136, 135, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 90, 89, 88, 87, 86, 85, 255, 255, 255, 255, 255, 255, 190, 189, 188, 187, 186, 185, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_27;
};

std::vector<MixedTypedExample>& get_examples_28() {
static std::vector<MixedTypedExample> examples_28 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145}}},
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
return examples_28;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_28() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_28 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121, 121, 120, 120, 120, 120, 120, 120, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 145, 146, 146, 146, 146, 146, 146, 145, 145, 145, 145, 145, 145}}},
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
return examples_dynamic_output_shape_28;
};

std::vector<MixedTypedExample>& get_examples_29() {
static std::vector<MixedTypedExample> examples_29 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_29;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_29() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_29 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_29;
};

std::vector<MixedTypedExample>& get_examples_30() {
static std::vector<MixedTypedExample> examples_30 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_30;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_30() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_30 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_30;
};

std::vector<MixedTypedExample>& get_examples_31() {
static std::vector<MixedTypedExample> examples_31 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_31;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_31() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_31 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_31;
};

std::vector<MixedTypedExample>& get_examples_32() {
static std::vector<MixedTypedExample> examples_32 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10}}},
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
return examples_32;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_32() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_32 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10, 255, 255, 255, 255, 255, 255, 15, 14, 13, 12, 11, 10}}},
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
return examples_dynamic_output_shape_32;
};

std::vector<MixedTypedExample>& get_examples_33() {
static std::vector<MixedTypedExample> examples_33 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_33;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_33() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_33 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_33;
};

std::vector<MixedTypedExample>& get_examples_34() {
static std::vector<MixedTypedExample> examples_34 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_34;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_34() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_34 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_34;
};

std::vector<MixedTypedExample>& get_examples_35() {
static std::vector<MixedTypedExample> examples_35 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250, 150, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 251, 151, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252, 152, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 253, 153, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 154, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 155, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_35;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_35() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_35 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250, 150, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 251, 151, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252, 152, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 253, 153, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 154, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 155, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_35;
};

std::vector<MixedTypedExample>& get_examples_36() {
static std::vector<MixedTypedExample> examples_36 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95}}},
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
return examples_36;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_36() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_36 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 119, 119, 95, 95, 95, 95, 94, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95}}},
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
return examples_dynamic_output_shape_36;
};

std::vector<MixedTypedExample>& get_examples_37() {
static std::vector<MixedTypedExample> examples_37 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_37;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_37() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_37 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_37;
};

std::vector<MixedTypedExample>& get_examples_38() {
static std::vector<MixedTypedExample> examples_38 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_38;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_38() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_38 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_38;
};

std::vector<MixedTypedExample>& get_examples_39() {
static std::vector<MixedTypedExample> examples_39 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 250, 150, 50, 0, 0, 0, 0, 0, 0, 0, 0, 255, 251, 151, 51, 0, 0, 0, 0, 0, 0, 0, 0, 255, 252, 152, 52, 0, 0, 0, 0, 0, 0, 0, 0, 255, 253, 153, 53, 0, 0, 0, 0, 0, 0, 0, 0, 255, 254, 154, 54, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 155, 55, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_39;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_39() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_39 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 250, 150, 50, 0, 0, 0, 0, 0, 0, 0, 0, 255, 251, 151, 51, 0, 0, 0, 0, 0, 0, 0, 0, 255, 252, 152, 52, 0, 0, 0, 0, 0, 0, 0, 0, 255, 253, 153, 53, 0, 0, 0, 0, 0, 0, 0, 0, 255, 254, 154, 54, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 155, 55, 0, 0, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_39;
};

std::vector<MixedTypedExample>& get_examples_40() {
static std::vector<MixedTypedExample> examples_40 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95}}},
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
return examples_40;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_40() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_40 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 119, 95, 95, 95, 95, 95, 94, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95, 120, 120, 120, 120, 120, 120, 95, 95, 95, 95, 95, 95}}},
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
return examples_dynamic_output_shape_40;
};

std::vector<MixedTypedExample>& get_examples_41() {
static std::vector<MixedTypedExample> examples_41 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0}}},
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
return examples_41;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_41() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_41 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_41;
};

std::vector<MixedTypedExample>& get_examples_42() {
static std::vector<MixedTypedExample> examples_42 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 3, 1, 1, 1, 1, 1, 1}}},
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
return examples_42;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_42() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_42 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 3, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 3, 1, 1, 1, 1, 1, 1}}},
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
return examples_dynamic_output_shape_42;
};

std::vector<MixedTypedExample>& get_examples_43() {
static std::vector<MixedTypedExample> examples_43 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 119, 118, 117, 116, 115, 0, 0, 0, 0, 0, 0, 121, 120, 119, 118, 117, 116, 0, 0, 0, 0, 0, 0, 122, 121, 120, 119, 118, 117, 0, 0, 0, 0, 0, 0, 123, 122, 121, 120, 119, 118, 0, 0, 0, 0, 0, 0, 124, 123, 122, 121, 120, 119, 0, 0, 0, 0, 0, 0, 125, 124, 123, 122, 121, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 119, 118, 117, 116, 115, 255, 255, 255, 255, 255, 255, 121, 120, 119, 118, 117, 116, 255, 255, 255, 255, 255, 255, 122, 121, 120, 119, 118, 117, 255, 255, 255, 255, 255, 255, 123, 122, 121, 120, 119, 118, 255, 255, 255, 255, 255, 255, 124, 123, 122, 121, 120, 119, 255, 255, 255, 255, 255, 255, 125, 124, 123, 122, 121, 120}}},
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
return examples_43;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_43() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_43 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 119, 118, 117, 116, 115, 0, 0, 0, 0, 0, 0, 121, 120, 119, 118, 117, 116, 0, 0, 0, 0, 0, 0, 122, 121, 120, 119, 118, 117, 0, 0, 0, 0, 0, 0, 123, 122, 121, 120, 119, 118, 0, 0, 0, 0, 0, 0, 124, 123, 122, 121, 120, 119, 0, 0, 0, 0, 0, 0, 125, 124, 123, 122, 121, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 119, 118, 117, 116, 115, 255, 255, 255, 255, 255, 255, 121, 120, 119, 118, 117, 116, 255, 255, 255, 255, 255, 255, 122, 121, 120, 119, 118, 117, 255, 255, 255, 255, 255, 255, 123, 122, 121, 120, 119, 118, 255, 255, 255, 255, 255, 255, 124, 123, 122, 121, 120, 119, 255, 255, 255, 255, 255, 255, 125, 124, 123, 122, 121, 120}}},
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
return examples_dynamic_output_shape_43;
};

std::vector<MixedTypedExample>& get_examples_44() {
static std::vector<MixedTypedExample> examples_44 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120}}},
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
return examples_44;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_44() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_44 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120}}},
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
return examples_dynamic_output_shape_44;
};

std::vector<MixedTypedExample>& get_examples_45() {
static std::vector<MixedTypedExample> examples_45 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_45;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_45() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_45 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_45;
};

std::vector<MixedTypedExample>& get_examples_46() {
static std::vector<MixedTypedExample> examples_46 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_46;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_46() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_46 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_46;
};

std::vector<MixedTypedExample>& get_examples_47() {
static std::vector<MixedTypedExample> examples_47 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_47;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_47() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_47 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_47;
};

std::vector<MixedTypedExample>& get_examples_48() {
static std::vector<MixedTypedExample> examples_48 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0}}},
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
return examples_48;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_48() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_48 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0, 240, 239, 238, 237, 236, 235, 0, 0, 0, 0, 0, 0}}},
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
return examples_dynamic_output_shape_48;
};

std::vector<MixedTypedExample>& get_examples_49() {
static std::vector<MixedTypedExample> examples_49 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_49;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_49() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_49 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_49;
};

std::vector<MixedTypedExample>& get_examples_50() {
static std::vector<MixedTypedExample> examples_50 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_50;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_50() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_50 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_50;
};

std::vector<MixedTypedExample>& get_examples_51() {
static std::vector<MixedTypedExample> examples_51 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_51;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_51() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_51 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_51;
};

std::vector<MixedTypedExample>& get_examples_52() {
static std::vector<MixedTypedExample> examples_52 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 4, 0, 0, 0, 0, 0, 0, 250, 250, 250, 250, 250, 250, 225, 225, 225, 225, 225, 224, 251, 251, 251, 251, 251, 250, 226, 226, 226, 226, 226, 226, 252, 252, 252, 252, 252, 252, 227, 227, 227, 227, 227, 226, 253, 253, 253, 253, 253, 252, 228, 228, 228, 228, 228, 228, 254, 254, 254, 254, 254, 254, 229, 229, 229, 229, 229, 228, 255, 255, 255, 255, 255, 254, 230, 230, 230, 230, 230, 230}}},
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
return examples_52;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_52() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_52 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 2, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 4, 0, 0, 0, 0, 0, 0, 250, 250, 250, 250, 250, 250, 225, 225, 225, 225, 225, 224, 251, 251, 251, 251, 251, 250, 226, 226, 226, 226, 226, 226, 252, 252, 252, 252, 252, 252, 227, 227, 227, 227, 227, 226, 253, 253, 253, 253, 253, 252, 228, 228, 228, 228, 228, 228, 254, 254, 254, 254, 254, 254, 229, 229, 229, 229, 229, 228, 255, 255, 255, 255, 255, 254, 230, 230, 230, 230, 230, 230}}},
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
return examples_dynamic_output_shape_52;
};

std::vector<MixedTypedExample>& get_examples_53() {
static std::vector<MixedTypedExample> examples_53 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_53;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_53() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_53 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_53;
};

std::vector<MixedTypedExample>& get_examples_54() {
static std::vector<MixedTypedExample> examples_54 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_54;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_54() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_54 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_54;
};

std::vector<MixedTypedExample>& get_examples_55() {
static std::vector<MixedTypedExample> examples_55 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_55;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_55() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_55 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_55;
};

std::vector<MixedTypedExample>& get_examples_56() {
static std::vector<MixedTypedExample> examples_56 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 250, 250, 250, 250, 250, 250, 225, 225, 225, 225, 225, 225, 251, 251, 251, 251, 251, 251, 226, 226, 226, 226, 226, 226, 252, 252, 252, 252, 252, 252, 227, 227, 227, 227, 227, 227, 253, 253, 253, 253, 253, 253, 228, 228, 228, 228, 228, 228, 254, 254, 254, 254, 254, 254, 229, 229, 229, 229, 229, 229, 255, 255, 255, 255, 255, 255, 230, 230, 230, 230, 230, 230}}},
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
return examples_56;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_56() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_56 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 250, 250, 250, 250, 250, 250, 225, 225, 225, 225, 225, 225, 251, 251, 251, 251, 251, 251, 226, 226, 226, 226, 226, 226, 252, 252, 252, 252, 252, 252, 227, 227, 227, 227, 227, 227, 253, 253, 253, 253, 253, 253, 228, 228, 228, 228, 228, 228, 254, 254, 254, 254, 254, 254, 229, 229, 229, 229, 229, 229, 255, 255, 255, 255, 255, 255, 230, 230, 230, 230, 230, 230}}},
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
return examples_dynamic_output_shape_56;
};

std::vector<MixedTypedExample>& get_examples_57() {
static std::vector<MixedTypedExample> examples_57 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_57;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_57() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_57 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_57;
};

std::vector<MixedTypedExample>& get_examples_58() {
static std::vector<MixedTypedExample> examples_58 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_58;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_58() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_58 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_58;
};

std::vector<MixedTypedExample>& get_examples_59() {
static std::vector<MixedTypedExample> examples_59 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_59;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_59() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_59 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_59;
};

std::vector<MixedTypedExample>& get_examples_60() {
static std::vector<MixedTypedExample> examples_60 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_60;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_60() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_60 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
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
return examples_dynamic_output_shape_60;
};

std::vector<MixedTypedExample>& get_examples_61() {
static std::vector<MixedTypedExample> examples_61 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 10, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 20, 10, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 30, 20, 10, 0, 0, 0, 255, 255, 255, 255, 255, 255, 40, 30, 20, 10, 0, 0, 255, 255, 255, 255, 255, 255, 50, 40, 30, 20, 10, 0}}},
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
return examples_61;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_61() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_61 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 10, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 20, 10, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 30, 20, 10, 0, 0, 0, 255, 255, 255, 255, 255, 255, 40, 30, 20, 10, 0, 0, 255, 255, 255, 255, 255, 255, 50, 40, 30, 20, 10, 0}}},
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
return examples_dynamic_output_shape_61;
};

std::vector<MixedTypedExample>& get_examples_62() {
static std::vector<MixedTypedExample> examples_62 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 41, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 51, 41, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 11, 1, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 21, 11, 1, 0, 0, 0, 255, 255, 255, 255, 255, 255, 31, 21, 11, 1, 0, 0, 255, 255, 255, 255, 255, 255, 41, 31, 21, 11, 1, 0, 255, 255, 255, 255, 255, 255, 51, 41, 31, 21, 11, 1}}},
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
return examples_62;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_62() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_62 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 41, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 0, 51, 41, 31, 21, 11, 1, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 11, 1, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 21, 11, 1, 0, 0, 0, 255, 255, 255, 255, 255, 255, 31, 21, 11, 1, 0, 0, 255, 255, 255, 255, 255, 255, 41, 31, 21, 11, 1, 0, 255, 255, 255, 255, 255, 255, 51, 41, 31, 21, 11, 1}}},
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
return examples_dynamic_output_shape_62;
};

std::vector<MixedTypedExample>& get_examples_63() {
static std::vector<MixedTypedExample> examples_63 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120}}},
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
return examples_63;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_63() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_63 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 120}}},
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
return examples_dynamic_output_shape_63;
};

std::vector<MixedTypedExample>& get_examples_64() {
static std::vector<MixedTypedExample> examples_64 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 119, 118, 117, 116, 115, 0, 0, 0, 0, 0, 0, 121, 120, 119, 118, 117, 116, 0, 0, 0, 0, 0, 0, 122, 121, 120, 119, 118, 117, 0, 0, 0, 0, 0, 0, 123, 122, 121, 120, 119, 118, 0, 0, 0, 0, 0, 0, 124, 123, 122, 121, 120, 119, 0, 0, 0, 0, 0, 0, 125, 124, 123, 122, 121, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 119, 118, 117, 116, 115, 255, 255, 255, 255, 255, 255, 121, 120, 119, 118, 117, 116, 255, 255, 255, 255, 255, 255, 122, 121, 120, 119, 118, 117, 255, 255, 255, 255, 255, 255, 123, 122, 121, 120, 119, 118, 255, 255, 255, 255, 255, 255, 124, 123, 122, 121, 120, 119, 255, 255, 255, 255, 255, 255, 125, 124, 123, 122, 121, 120}}},
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
return examples_64;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_64() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_64 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {144}}, {1, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}, {1, {0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 250, 251, 252, 253, 254, 255}}},
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
  .operandDimensions = {{0, {144}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 119, 118, 117, 116, 115, 0, 0, 0, 0, 0, 0, 121, 120, 119, 118, 117, 116, 0, 0, 0, 0, 0, 0, 122, 121, 120, 119, 118, 117, 0, 0, 0, 0, 0, 0, 123, 122, 121, 120, 119, 118, 0, 0, 0, 0, 0, 0, 124, 123, 122, 121, 120, 119, 0, 0, 0, 0, 0, 0, 125, 124, 123, 122, 121, 120, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 120, 119, 118, 117, 116, 115, 255, 255, 255, 255, 255, 255, 121, 120, 119, 118, 117, 116, 255, 255, 255, 255, 255, 255, 122, 121, 120, 119, 118, 117, 255, 255, 255, 255, 255, 255, 123, 122, 121, 120, 119, 118, 255, 255, 255, 255, 255, 255, 124, 123, 122, 121, 120, 119, 255, 255, 255, 255, 255, 255, 125, 124, 123, 122, 121, 120}}},
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
return examples_dynamic_output_shape_64;
};

