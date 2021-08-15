// clang-format off
// Generated file (from: box_with_nms_limit_linear.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f, 0.39705884f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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

std::vector<MixedTypedExample>& get_examples_relaxed() {
static std::vector<MixedTypedExample> examples_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f, 0.39705884f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
return examples_relaxed;
};

std::vector<MixedTypedExample>& get_examples_float16() {
static std::vector<MixedTypedExample> examples_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.5f, 0.8999999761581421f, 0.800000011920929f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.949999988079071f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f, 0.949999988079071f, 0.8999999761581421f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.800000011920929f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
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
return examples_float16;
};

std::vector<MixedTypedExample>& get_examples_quant8() {
static std::vector<MixedTypedExample> examples_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {90, 95, 75, 80, 70, 85, 60, 90, 95, 90, 65, 90, 80, 85, 80, 60, 60, 20, 60, 80, 40, 90, 55, 60, 90, 75, 70, 80, 70, 85, 90, 95, 75, 80, 85, 80, 60, 90, 95, 60, 60, 20, 50, 90, 80, 90, 75, 70, 90, 65, 90, 90, 55, 60, 60, 80, 40}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 8, 80, 80, 0, 0, 80, 80, 0, 0, 80, 80, 16, 16, 88, 88, 8, 8, 88, 88, 8, 8, 88, 88, 24, 24, 96, 96, 16, 16, 96, 96, 16, 16, 96, 96, 32, 32, 104, 104, 24, 24, 104, 104, 24, 24, 104, 104, 40, 40, 112, 112, 32, 32, 112, 112, 32, 32, 112, 112, 48, 48, 120, 120, 40, 40, 120, 120, 40, 40, 120, 120, 56, 56, 128, 128, 48, 48, 128, 128, 48, 48, 128, 128, 64, 64, 136, 136, 56, 56, 136, 136, 56, 56, 136, 136, 72, 72, 144, 144, 64, 64, 144, 144, 64, 64, 144, 144, 16, 16, 88, 88, 16, 16, 96, 96, 16, 16, 96, 96, 8, 8, 80, 80, 8, 8, 88, 88, 8, 8, 88, 88, 40, 40, 112, 112, 40, 40, 120, 120, 40, 40, 120, 120, 24, 24, 96, 96, 24, 24, 104, 104, 24, 24, 104, 104, 48, 48, 120, 120, 48, 48, 128, 128, 48, 48, 128, 128, 0, 0, 8, 8, 0, 0, 16, 16, 0, 0, 16, 16, 72, 72, 144, 144, 72, 72, 152, 152, 72, 72, 152, 152, 32, 32, 104, 104, 32, 32, 112, 112, 32, 32, 112, 112, 64, 64, 136, 136, 64, 64, 144, 144, 64, 64, 144, 144, 56, 56, 128, 128, 56, 56, 136, 136, 56, 56, 136, 136}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {95, 85, 75, 95, 70, 42, 40, 95, 90, 85, 75, 95, 80, 70, 42, 40}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {0, 0, 80, 80, 32, 32, 112, 112, 64, 64, 144, 144, 16, 16, 96, 96, 64, 64, 144, 144, 32, 32, 112, 112, 0, 0, 80, 80, 8, 8, 88, 88, 0, 0, 16, 16, 40, 40, 120, 120, 72, 72, 152, 152, 24, 24, 104, 104, 0, 0, 16, 16, 72, 72, 152, 152, 40, 40, 120, 120, 8, 8, 88, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f, 0.39705884f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f, 0.39705884f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
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
return examples_dynamic_output_shape_relaxed;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.5f, 0.8999999761581421f, 0.800000011920929f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
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
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.949999988079071f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f, 0.949999988079071f, 0.8999999761581421f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.800000011920929f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f, 1.0f, 1.0f, 11.0f, 11.0f}}},
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
return examples_dynamic_output_shape_float16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {90, 95, 75, 80, 70, 85, 60, 90, 95, 90, 65, 90, 80, 85, 80, 60, 60, 20, 60, 80, 40, 90, 55, 60, 90, 75, 70, 80, 70, 85, 90, 95, 75, 80, 85, 80, 60, 90, 95, 60, 60, 20, 50, 90, 80, 90, 75, 70, 90, 65, 90, 90, 55, 60, 60, 80, 40}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 8, 80, 80, 0, 0, 80, 80, 0, 0, 80, 80, 16, 16, 88, 88, 8, 8, 88, 88, 8, 8, 88, 88, 24, 24, 96, 96, 16, 16, 96, 96, 16, 16, 96, 96, 32, 32, 104, 104, 24, 24, 104, 104, 24, 24, 104, 104, 40, 40, 112, 112, 32, 32, 112, 112, 32, 32, 112, 112, 48, 48, 120, 120, 40, 40, 120, 120, 40, 40, 120, 120, 56, 56, 128, 128, 48, 48, 128, 128, 48, 48, 128, 128, 64, 64, 136, 136, 56, 56, 136, 136, 56, 56, 136, 136, 72, 72, 144, 144, 64, 64, 144, 144, 64, 64, 144, 144, 16, 16, 88, 88, 16, 16, 96, 96, 16, 16, 96, 96, 8, 8, 80, 80, 8, 8, 88, 88, 8, 8, 88, 88, 40, 40, 112, 112, 40, 40, 120, 120, 40, 40, 120, 120, 24, 24, 96, 96, 24, 24, 104, 104, 24, 24, 104, 104, 48, 48, 120, 120, 48, 48, 128, 128, 48, 48, 128, 128, 0, 0, 8, 8, 0, 0, 16, 16, 0, 0, 16, 16, 72, 72, 144, 144, 72, 72, 152, 152, 72, 72, 152, 152, 32, 32, 104, 104, 32, 32, 112, 112, 32, 32, 112, 112, 64, 64, 136, 136, 64, 64, 144, 144, 64, 64, 144, 144, 56, 56, 128, 128, 56, 56, 136, 136, 56, 56, 136, 136}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {16}}, {1, {16, 4}}, {2, {16}}, {3, {16}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, {3, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {95, 85, 75, 95, 70, 42, 40, 95, 90, 85, 75, 95, 80, 70, 42, 40}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {0, 0, 80, 80, 32, 32, 112, 112, 64, 64, 144, 144, 16, 16, 96, 96, 64, 64, 144, 144, 32, 32, 112, 112, 0, 0, 80, 80, 8, 8, 88, 88, 0, 0, 16, 16, 40, 40, 120, 120, 72, 72, 152, 152, 24, 24, 104, 104, 0, 0, 16, 16, 72, 72, 152, 152, 40, 40, 120, 120, 8, 8, 88, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8;
};

std::vector<MixedTypedExample>& get_examples_2() {
static std::vector<MixedTypedExample> examples_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
return examples_2;
};

std::vector<MixedTypedExample>& get_examples_relaxed_2() {
static std::vector<MixedTypedExample> examples_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
return examples_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_float16_2() {
static std::vector<MixedTypedExample> examples_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.5f, 0.8999999761581421f, 0.800000011920929f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.949999988079071f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f, 0.949999988079071f, 0.8999999761581421f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.800000011920929f, 0.699999988079071f, 0.4235294461250305f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
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
return examples_float16_2;
};

std::vector<MixedTypedExample>& get_examples_quant8_2() {
static std::vector<MixedTypedExample> examples_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {218, 223, 203, 208, 198, 213, 188, 218, 223, 218, 193, 218, 208, 213, 208, 188, 188, 148, 188, 208, 168, 218, 183, 188, 218, 203, 198, 208, 198, 213, 218, 223, 203, 208, 213, 208, 188, 218, 223, 188, 188, 148, 178, 218, 208, 218, 203, 198, 218, 193, 218, 218, 183, 188, 188, 208, 168}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 8, 80, 80, 0, 0, 80, 80, 0, 0, 80, 80, 16, 16, 88, 88, 8, 8, 88, 88, 8, 8, 88, 88, 24, 24, 96, 96, 16, 16, 96, 96, 16, 16, 96, 96, 32, 32, 104, 104, 24, 24, 104, 104, 24, 24, 104, 104, 40, 40, 112, 112, 32, 32, 112, 112, 32, 32, 112, 112, 48, 48, 120, 120, 40, 40, 120, 120, 40, 40, 120, 120, 56, 56, 128, 128, 48, 48, 128, 128, 48, 48, 128, 128, 64, 64, 136, 136, 56, 56, 136, 136, 56, 56, 136, 136, 72, 72, 144, 144, 64, 64, 144, 144, 64, 64, 144, 144, 16, 16, 88, 88, 16, 16, 96, 96, 16, 16, 96, 96, 8, 8, 80, 80, 8, 8, 88, 88, 8, 8, 88, 88, 40, 40, 112, 112, 40, 40, 120, 120, 40, 40, 120, 120, 24, 24, 96, 96, 24, 24, 104, 104, 24, 24, 104, 104, 48, 48, 120, 120, 48, 48, 128, 128, 48, 48, 128, 128, 0, 0, 8, 8, 0, 0, 16, 16, 0, 0, 16, 16, 72, 72, 144, 144, 72, 72, 152, 152, 72, 72, 152, 152, 32, 32, 104, 104, 32, 32, 112, 112, 32, 32, 112, 112, 64, 64, 136, 136, 64, 64, 144, 144, 64, 64, 144, 144, 56, 56, 128, 128, 56, 56, 136, 136, 56, 56, 136, 136}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {223, 213, 203, 223, 198, 170, 168, 223, 218, 213, 203, 223, 208, 198, 170}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {0, 0, 80, 80, 32, 32, 112, 112, 64, 64, 144, 144, 16, 16, 96, 96, 64, 64, 144, 144, 32, 32, 112, 112, 0, 0, 80, 80, 8, 8, 88, 88, 0, 0, 16, 16, 40, 40, 120, 120, 72, 72, 152, 152, 24, 24, 104, 104, 0, 0, 16, 16, 72, 72, 152, 152, 40, 40, 120, 120}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
return examples_dynamic_output_shape_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.9f, 0.95f, 0.75f, 0.8f, 0.7f, 0.85f, 0.6f, 0.9f, 0.95f, 0.9f, 0.65f, 0.9f, 0.8f, 0.85f, 0.8f, 0.6f, 0.6f, 0.2f, 0.6f, 0.8f, 0.4f, 0.9f, 0.55f, 0.6f, 0.9f, 0.75f, 0.7f, 0.8f, 0.7f, 0.85f, 0.9f, 0.95f, 0.75f, 0.8f, 0.85f, 0.8f, 0.6f, 0.9f, 0.95f, 0.6f, 0.6f, 0.2f, 0.5f, 0.9f, 0.8f, 0.9f, 0.75f, 0.7f, 0.9f, 0.65f, 0.9f, 0.9f, 0.55f, 0.6f, 0.6f, 0.8f, 0.4f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.95f, 0.85f, 0.75f, 0.95f, 0.7f, 0.42352945f, 0.39705884f, 0.95f, 0.9f, 0.85f, 0.75f, 0.95f, 0.8f, 0.7f, 0.42352945f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
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
return examples_dynamic_output_shape_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.800000011920929f, 0.699999988079071f, 0.8500000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.75f, 0.800000011920929f, 0.8500000238418579f, 0.800000011920929f, 0.6000000238418579f, 0.8999999761581421f, 0.949999988079071f, 0.6000000238418579f, 0.6000000238418579f, 0.20000000298023224f, 0.5f, 0.8999999761581421f, 0.800000011920929f, 0.8999999761581421f, 0.75f, 0.699999988079071f, 0.8999999761581421f, 0.6499999761581421f, 0.8999999761581421f, 0.8999999761581421f, 0.550000011920929f, 0.6000000238418579f, 0.6000000238418579f, 0.800000011920929f, 0.4000000059604645f}}, {1, {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f, 2.0f, 2.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 3.0f, 3.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 4.0f, 4.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 5.0f, 5.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 6.0f, 6.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 7.0f, 7.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 8.0f, 8.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f, 9.0f, 9.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 11.0f, 11.0f, 2.0f, 2.0f, 12.0f, 12.0f, 2.0f, 2.0f, 12.0f, 12.0f, 1.0f, 1.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 1.0f, 1.0f, 11.0f, 11.0f, 5.0f, 5.0f, 14.0f, 14.0f, 5.0f, 5.0f, 15.0f, 15.0f, 5.0f, 5.0f, 15.0f, 15.0f, 3.0f, 3.0f, 12.0f, 12.0f, 3.0f, 3.0f, 13.0f, 13.0f, 3.0f, 3.0f, 13.0f, 13.0f, 6.0f, 6.0f, 15.0f, 15.0f, 6.0f, 6.0f, 16.0f, 16.0f, 6.0f, 6.0f, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 18.0f, 18.0f, 9.0f, 9.0f, 19.0f, 19.0f, 9.0f, 9.0f, 19.0f, 19.0f, 4.0f, 4.0f, 13.0f, 13.0f, 4.0f, 4.0f, 14.0f, 14.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 17.0f, 17.0f, 8.0f, 8.0f, 18.0f, 18.0f, 8.0f, 8.0f, 18.0f, 18.0f, 7.0f, 7.0f, 16.0f, 16.0f, 7.0f, 7.0f, 17.0f, 17.0f, 7.0f, 7.0f, 17.0f, 17.0f}}},
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
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.949999988079071f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.699999988079071f, 0.4235294461250305f, 0.3970588445663452f, 0.949999988079071f, 0.8999999761581421f, 0.8500000238418579f, 0.75f, 0.949999988079071f, 0.800000011920929f, 0.699999988079071f, 0.4235294461250305f}}, {1, {0.0f, 0.0f, 10.0f, 10.0f, 4.0f, 4.0f, 14.0f, 14.0f, 8.0f, 8.0f, 18.0f, 18.0f, 2.0f, 2.0f, 12.0f, 12.0f, 8.0f, 8.0f, 18.0f, 18.0f, 4.0f, 4.0f, 14.0f, 14.0f, 0.0f, 0.0f, 10.0f, 10.0f, 1.0f, 1.0f, 11.0f, 11.0f, 0.0f, 0.0f, 2.0f, 2.0f, 5.0f, 5.0f, 15.0f, 15.0f, 9.0f, 9.0f, 19.0f, 19.0f, 3.0f, 3.0f, 13.0f, 13.0f, 0.0f, 0.0f, 2.0f, 2.0f, 9.0f, 9.0f, 19.0f, 19.0f, 5.0f, 5.0f, 15.0f, 15.0f}}},
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
return examples_dynamic_output_shape_float16_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {19, 3}}, {1, {19, 12}}, {2, {19}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {218, 223, 203, 208, 198, 213, 188, 218, 223, 218, 193, 218, 208, 213, 208, 188, 188, 148, 188, 208, 168, 218, 183, 188, 218, 203, 198, 208, 198, 213, 218, 223, 203, 208, 213, 208, 188, 218, 223, 188, 188, 148, 178, 218, 208, 218, 203, 198, 218, 193, 218, 218, 183, 188, 188, 208, 168}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 8, 80, 80, 0, 0, 80, 80, 0, 0, 80, 80, 16, 16, 88, 88, 8, 8, 88, 88, 8, 8, 88, 88, 24, 24, 96, 96, 16, 16, 96, 96, 16, 16, 96, 96, 32, 32, 104, 104, 24, 24, 104, 104, 24, 24, 104, 104, 40, 40, 112, 112, 32, 32, 112, 112, 32, 32, 112, 112, 48, 48, 120, 120, 40, 40, 120, 120, 40, 40, 120, 120, 56, 56, 128, 128, 48, 48, 128, 128, 48, 48, 128, 128, 64, 64, 136, 136, 56, 56, 136, 136, 56, 56, 136, 136, 72, 72, 144, 144, 64, 64, 144, 144, 64, 64, 144, 144, 16, 16, 88, 88, 16, 16, 96, 96, 16, 16, 96, 96, 8, 8, 80, 80, 8, 8, 88, 88, 8, 8, 88, 88, 40, 40, 112, 112, 40, 40, 120, 120, 40, 40, 120, 120, 24, 24, 96, 96, 24, 24, 104, 104, 24, 24, 104, 104, 48, 48, 120, 120, 48, 48, 128, 128, 48, 48, 128, 128, 0, 0, 8, 8, 0, 0, 16, 16, 0, 0, 16, 16, 72, 72, 144, 144, 72, 72, 152, 152, 72, 72, 152, 152, 32, 32, 104, 104, 32, 32, 112, 112, 32, 32, 112, 112, 64, 64, 136, 136, 64, 64, 144, 144, 64, 64, 144, 144, 56, 56, 128, 128, 56, 56, 136, 136, 56, 56, 136, 136}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {15}}, {1, {15, 4}}, {2, {15}}, {3, {15}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2}}, {3, {1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {223, 213, 203, 223, 198, 170, 168, 223, 218, 213, 203, 223, 208, 198, 170}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {0, 0, 80, 80, 32, 32, 112, 112, 64, 64, 144, 144, 16, 16, 96, 96, 64, 64, 144, 144, 32, 32, 112, 112, 0, 0, 80, 80, 8, 8, 88, 88, 0, 0, 16, 16, 40, 40, 120, 120, 72, 72, 152, 152, 24, 24, 104, 104, 0, 0, 16, 16, 72, 72, 152, 152, 40, 40, 120, 120}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_2;
};

