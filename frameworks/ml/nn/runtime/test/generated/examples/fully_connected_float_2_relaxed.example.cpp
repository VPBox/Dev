// clang-format off
// Generated file (from: fully_connected_float_2_relaxed.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.503691f, 0.196961f, 0.521017f, 0.554248f, 0.288678f, 0.792476f, 0.561653f, 0.46223f, 0.650736f, 0.163132f, 0.029658f, 0.411544f, 0.470539f, 0.57239f, 0.538755f, 0.21203f}}},
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
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0732134f, 0.0f, 0.0f, 0.0f, 0.280859f, 0.0f, 0.128927f, 0.0f, 0.0777251f, 0.0f, 0.270268f, 0.271435f, 0.0173503f, 0.335465f, 0.235562f, 0.0f, 0.0745866f, 0.0f, 0.051611f, 0.0f, 0.253876f, 0.0f, 0.0814873f, 0.0f, 0.104104f, 0.0f, 0.248529f, 0.264194f, 0.0f, 0.302973f, 0.166252f}}},
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
  .operandDimensions = {{0, {2, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.503691f, 0.196961f, 0.521017f, 0.554248f, 0.288678f, 0.792476f, 0.561653f, 0.46223f, 0.650736f, 0.163132f, 0.029658f, 0.411544f, 0.470539f, 0.57239f, 0.538755f, 0.21203f}}},
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
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0732134f, 0.0f, 0.0f, 0.0f, 0.280859f, 0.0f, 0.128927f, 0.0f, 0.0777251f, 0.0f, 0.270268f, 0.271435f, 0.0173503f, 0.335465f, 0.235562f, 0.0f, 0.0745866f, 0.0f, 0.051611f, 0.0f, 0.253876f, 0.0f, 0.0814873f, 0.0f, 0.104104f, 0.0f, 0.248529f, 0.264194f, 0.0f, 0.302973f, 0.166252f}}},
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

