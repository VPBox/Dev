// clang-format off
// Generated file (from: conv2d_v1_2.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples_nhwc() {
static std::vector<MixedTypedExample> examples_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc;
};

std::vector<MixedTypedExample>& get_examples_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_nhwc_float16() {
static std::vector<MixedTypedExample> examples_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}, {1, {2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 2, 2, 2}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_nchw() {
static std::vector<MixedTypedExample> examples_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw;
};

std::vector<MixedTypedExample>& get_examples_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_nchw_quant8() {
static std::vector<MixedTypedExample> examples_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_nchw_float16() {
static std::vector<MixedTypedExample> examples_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}, {1, {2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 2, 2, 2}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}, {1, {2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 2, 2, 2}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 3, 1}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}, {1, {2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {2, 2, 2, 2, 1, 2, 2, 2, 2}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 2, 2, 2}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {7, 7, 7, 7}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 3}}, {1, {1, 2, 2, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f}}, {1, {0.25f, 0.25f, 0.25f, 0.25f}}, {2, {0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.875f, 0.875f, 0.875f, 0.875f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_nhwc_2() {
static std::vector<MixedTypedExample> examples_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_relaxed_2() {
static std::vector<MixedTypedExample> examples_nhwc_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_nhwc_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_float16_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_2() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_weight_as_input_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_relaxed_2() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_weight_as_input_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_quant8_2() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}, {1, {129, 135, 141, 131, 137, 143, 133, 139, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_weight_as_input_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 4, 10, 16, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nhwc_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_nhwc_weight_as_input_float16_2() {
static std::vector<MixedTypedExample> examples_nhwc_weight_as_input_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nhwc_weight_as_input_float16_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_2() {
static std::vector<MixedTypedExample> examples_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_relaxed_2() {
static std::vector<MixedTypedExample> examples_nchw_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_nchw_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_float16_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_2() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_weight_as_input_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_relaxed_2() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_weight_as_input_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_quant8_2() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}, {1, {129, 135, 141, 131, 137, 143, 133, 139, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_weight_as_input_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 4, 10, 16, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_nchw_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_weight_as_input_float16_2() {
static std::vector<MixedTypedExample> examples_nchw_weight_as_input_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_nchw_weight_as_input_float16_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_float16_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}, {1, {129, 135, 141, 131, 137, 143, 133, 139, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_weight_as_input_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 4, 10, 16, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_weight_as_input_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_weight_as_input_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 4, 1}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 3, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nhwc_weight_as_input_float16_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_float16_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}, {1, {129, 135, 141, 131, 137, 143, 133, 139, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_weight_as_input_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {-800}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 4, 10, 16, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {50, 50, 50, 50, 85, 162, 207, 50, 50, 84, 111, 50}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_weight_as_input_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_weight_as_input_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 3, 4}}, {1, {1, 3, 3, 1}}, {2, {1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {-200.0f}}},
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
  .operandDimensions = {{0, {1, 1, 3, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 112.0f, 157.0f, 0.0f, 0.0f, 34.0f, 61.0f, 0.0f}}},
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
return examples_dynamic_output_shape_nchw_weight_as_input_float16_2;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc() {
static std::vector<MixedTypedExample> examples_channel_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_channel_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_channel_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_float16() {
static std::vector<MixedTypedExample> examples_channel_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_channel_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_channel_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_channel_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}, {1, {1, 2, 3, 4, 5, 6, 7, 8, 9}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {1, 2, 3, 5, 6, 8, 12, 13, 15}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_channel_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw() {
static std::vector<MixedTypedExample> examples_channel_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_channel_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_quant8() {
static std::vector<MixedTypedExample> examples_channel_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_float16() {
static std::vector<MixedTypedExample> examples_channel_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_channel_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_channel_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_channel_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}, {1, {1, 2, 3, 4, 5, 6, 7, 8, 9}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {1, 2, 3, 5, 6, 8, 12, 13, 15}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_channel_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}, {1, {1, 2, 3, 4, 5, 6, 7, 8, 9}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {1, 2, 3, 5, 6, 8, 12, 13, 15}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 1, 1, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_quant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}, {1, {1, 2, 3, 4, 5, 6, 7, 8, 9}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {10, 10, 10}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {1, 2, 3, 5, 6, 8, 12, 13, 15}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {30, 75, 120}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_channel_dynamic_output_shape_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_channel_dynamic_output_shape_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 1, 1}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {5.0f, 5.0f, 5.0f}}, {1, {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {15.0f, 37.5f, 60.0f}}},
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
return examples_channel_dynamic_output_shape_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc() {
static std::vector<MixedTypedExample> examples_large_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_large_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_large_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_nhwc_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 163, 169, 193, 208, 223, 229, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_float16() {
static std::vector<MixedTypedExample> examples_large_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}, {1, {130, 136, 142, 132, 138, 144, 134, 140, 146}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 3, 6, 9}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 163, 169, 193, 208, 223, 229, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nhwc_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_large_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_large_nchw() {
static std::vector<MixedTypedExample> examples_large_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_large_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_quant8() {
static std::vector<MixedTypedExample> examples_large_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_nchw_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 131, 134, 137, 140, 143, 129, 132, 135, 138, 141, 144, 130, 133, 136, 139, 142, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 193, 229, 255, 255, 255, 163, 208, 253, 255, 255, 255, 169, 223, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_float16() {
static std::vector<MixedTypedExample> examples_large_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}, {1, {130, 136, 142, 132, 138, 144, 134, 140, 146}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 131, 134, 137, 140, 143, 129, 132, 135, 138, 141, 144, 130, 133, 136, 139, 142, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 3, 6, 9}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 193, 229, 255, 255, 255, 163, 208, 253, 255, 255, 255, 169, 223, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_nchw_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_large_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 163, 169, 193, 208, 223, 229, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}, {1, {130, 136, 142, 132, 138, 144, 134, 140, 146}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 18, 21, 33, 40, 48, 51, 63, 75, 69, 86, 102, 87, 108, 129, 105, 130, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 3, 6, 9}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 163, 169, 193, 208, 223, 229, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nhwc_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nhwc_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 2, 3, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 2, 3, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 36.0f, 42.0f, 66.0f, 81.0f, 96.0f, 102.0f, 126.0f, 150.0f, 138.0f, 171.0f, 204.0f, 174.0f, 216.0f, 258.0f, 210.0f, 261.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nhwc_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_quant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 131, 134, 137, 140, 143, 129, 132, 135, 138, 141, 144, 130, 133, 136, 139, 142, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 193, 229, 255, 255, 255, 163, 208, 253, 255, 255, 255, 169, 223, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input_quant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}, {1, {130, 136, 142, 132, 138, 144, 134, 140, 146}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_weight_as_input_quant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {130, 136, 142, 148, 154, 160, 132, 138, 144, 150, 156, 162, 134, 140, 146, 152, 158, 164}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 6, 12, 18}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {15, 33, 51, 69, 87, 105, 18, 40, 63, 86, 108, 130, 21, 48, 75, 102, 129, 156}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 0, 0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 131, 134, 137, 140, 143, 129, 132, 135, 138, 141, 144, 130, 133, 136, 139, 142, 145}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {{1, {2, 8, 14, 2, 5, 8, 3, 6, 9}}},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {157, 193, 229, 255, 255, 255, 163, 208, 253, 255, 255, 255, 169, 223, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2;
};

std::vector<MixedTypedExample>& get_examples_large_dynamic_output_shape_nchw_weight_as_input_float16() {
static std::vector<MixedTypedExample> examples_large_dynamic_output_shape_nchw_weight_as_input_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 2, 3}}, {1, {3, 1, 1, 3}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 4.0f, 7.0f, 10.0f, 13.0f, 16.0f, 2.0f, 5.0f, 8.0f, 11.0f, 14.0f, 17.0f, 3.0f, 6.0f, 9.0f, 12.0f, 15.0f, 18.0f}}, {1, {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f}}, {2, {0.0f, 0.0f, 0.0f}}},
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
  .operandDimensions = {{0, {1, 3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {30.0f, 66.0f, 102.0f, 138.0f, 174.0f, 210.0f, 36.0f, 81.0f, 126.0f, 171.0f, 216.0f, 261.0f, 42.0f, 96.0f, 150.0f, 204.0f, 258.0f, 312.0f}}},
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
return examples_large_dynamic_output_shape_nchw_weight_as_input_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nhwc() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_nhwc;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nhwc_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.8528399467468262f, -0.03936560079455376f, -0.1273529976606369f, 1.431149959564209f, -0.302293986082077f, -1.0401999950408936f, 0.6550229787826538f, -0.5876139998435974f, 1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -0.34635698795318604f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1045060008764267f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, 1.420259952545166f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, -0.34343498945236206f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, -1.467170000076294f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f, 1.2374099493026733f, -0.5274019837379456f, -0.3995400071144104f, -0.01286229956895113f, 1.364400029182434f, 0.9857550263404846f, -0.7181180119514465f, -0.10080000013113022f, 1.2432700395584106f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.0006142780184745789f, -1.2122100591659546f, 0.4438610076904297f, 0.10211700201034546f, -2.527139902114868f, 1.4748899936676025f, 0.1734739989042282f, -0.23757700622081757f, 1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, -1.6270999908447266f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.13376200199127197f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 1.2237199544906616f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, -0.8389049768447876f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 0.8918229937553406f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f, 0.16622799634933472f, 0.43455401062965393f, -2.5752899646759033f, -0.9586619734764099f, -2.2397799491882324f, 2.667759895324707f, 0.5426009893417358f, 1.7610700130462646f, -1.081339955329895f}}},
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
return examples_1_H3_W2_SAME_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nchw() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_nchw;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_nchw_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.8528399467468262f, -0.03936560079455376f, -0.1273529976606369f, 1.431149959564209f, -0.302293986082077f, -1.0401999950408936f, 0.6550229787826538f, -0.5876139998435974f, 1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -0.34635698795318604f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1045060008764267f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, 1.420259952545166f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, -0.34343498945236206f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, -1.467170000076294f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f, 1.2374099493026733f, -0.5274019837379456f, -0.3995400071144104f, -0.01286229956895113f, 1.364400029182434f, 0.9857550263404846f, -0.7181180119514465f, -0.10080000013113022f, 1.2432700395584106f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.0006142780184745789f, -1.2122100591659546f, 0.4438610076904297f, 0.10211700201034546f, -2.527139902114868f, 1.4748899936676025f, 0.1734739989042282f, -0.23757700622081757f, 1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, -1.6270999908447266f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.13376200199127197f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 1.2237199544906616f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, -0.8389049768447876f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 0.8918229937553406f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f, 0.16622799634933472f, 0.43455401062965393f, -2.5752899646759033f, -0.9586619734764099f, -2.2397799491882324f, 2.667759895324707f, 0.5426009893417358f, 1.7610700130462646f, -1.081339955329895f}}},
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
return examples_1_H3_W2_SAME_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.8528399467468262f, -0.03936560079455376f, -0.1273529976606369f, 1.431149959564209f, -0.302293986082077f, -1.0401999950408936f, 0.6550229787826538f, -0.5876139998435974f, 1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -0.34635698795318604f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1045060008764267f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, 1.420259952545166f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, -0.34343498945236206f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, -1.467170000076294f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f, 1.2374099493026733f, -0.5274019837379456f, -0.3995400071144104f, -0.01286229956895113f, 1.364400029182434f, 0.9857550263404846f, -0.7181180119514465f, -0.10080000013113022f, 1.2432700395584106f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.0006142780184745789f, -1.2122100591659546f, 0.4438610076904297f, 0.10211700201034546f, -2.527139902114868f, 1.4748899936676025f, 0.1734739989042282f, -0.23757700622081757f, 1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, -1.6270999908447266f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.13376200199127197f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 1.2237199544906616f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, -0.8389049768447876f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 0.8918229937553406f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f, 0.16622799634933472f, 0.43455401062965393f, -2.5752899646759033f, -0.9586619734764099f, -2.2397799491882324f, 2.667759895324707f, 0.5426009893417358f, 1.7610700130462646f, -1.081339955329895f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.85284f, -0.0393656f, -0.127353f, 1.43115f, -0.302294f, -1.0402f, 0.655023f, -0.587614f, 1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -0.346357f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.104506f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, 1.42026f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, -0.343435f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, -1.46717f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f, 1.23741f, -0.527402f, -0.39954f, -0.0128623f, 1.3644f, 0.985755f, -0.718118f, -0.1008f, 1.24327f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.000614278f, -1.21221f, 0.443861f, 0.102117f, -2.52714f, 1.47489f, 0.173474f, -0.237577f, 1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, -1.6271f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.133762f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 1.22372f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, -0.838905f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 0.891823f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f, 0.166228f, 0.434554f, -2.57529f, -0.958662f, -2.23978f, 2.66776f, 0.542601f, 1.76107f, -1.08134f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_SAME_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_SAME_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.8528399467468262f, -0.03936560079455376f, -0.1273529976606369f, 1.431149959564209f, -0.302293986082077f, -1.0401999950408936f, 0.6550229787826538f, -0.5876139998435974f, 1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -0.34635698795318604f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1045060008764267f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, 1.420259952545166f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, -0.34343498945236206f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, -1.467170000076294f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f, 1.2374099493026733f, -0.5274019837379456f, -0.3995400071144104f, -0.01286229956895113f, 1.364400029182434f, 0.9857550263404846f, -0.7181180119514465f, -0.10080000013113022f, 1.2432700395584106f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 1, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.0006142780184745789f, -1.2122100591659546f, 0.4438610076904297f, 0.10211700201034546f, -2.527139902114868f, 1.4748899936676025f, 0.1734739989042282f, -0.23757700622081757f, 1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, -1.6270999908447266f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.13376200199127197f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 1.2237199544906616f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, -0.8389049768447876f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 0.8918229937553406f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f, 0.16622799634933472f, 0.43455401062965393f, -2.5752899646759033f, -0.9586619734764099f, -2.2397799491882324f, 2.667759895324707f, 0.5426009893417358f, 1.7610700130462646f, -1.081339955329895f}}},
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
return examples_1_H3_W2_SAME_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nhwc() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_nhwc;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nhwc_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f}}},
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
return examples_1_H3_W2_VALID_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nchw() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_nchw;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_nchw_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f}}},
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
return examples_1_H3_W2_VALID_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.72003f, 1.55816f, 0.667546f, 2.23663f, 0.0661516f, 0.290254f, 0.770222f, -1.58197f, -0.850595f, -0.484224f, 0.949967f, -0.577263f, -0.871949f, 2.34132f, -0.135965f, -0.985713f, 0.815147f, 1.03114f, -1.41915f, -0.515534f, -0.373639f, -1.50604f, 0.673113f, 3.06139f, -0.388578f, -1.76707f, -0.315667f, -1.03815f, 0.432787f, -1.41643f, 1.12944f, -0.175806f, -0.846415f, 1.40095f, 0.70832f, 2.19562f, -2.61266f, -0.705383f, 1.26124f, 1.46545f, -2.35761f, 2.04494f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.28735f, 1.91315f, 2.51734f, 0.375841f, 0.637563f, 2.653f, 2.72959f, 1.17389f, -2.12119f, 2.91417f, -2.24246f, 0.0497045f, -0.127107f, -0.144473f, -0.393284f, -2.02346f, -0.239178f, -0.246508f, 1.29277f, 1.32963f, 0.117521f, 0.0665713f, 1.09438f, -1.31426f, 2.52594f, -0.969211f, 0.515478f, -1.60926f, 0.135211f, 0.786415f, -1.14382f, -0.739102f, -1.01731f, 0.281615f, 2.36311f, 1.93872f, -0.150491f, 3.45217f, 2.28219f, 1.18282f, -2.25086f, 3.05468f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_1_H3_W2_VALID_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_1_H3_W2_VALID_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.7200299501419067f, 1.5581599473953247f, 0.6675459742546082f, 2.2366299629211426f, 0.06615159660577774f, 0.29025399684906006f, 0.770222008228302f, -1.581969976425171f, -0.8505949974060059f, -0.48422399163246155f, 0.9499670267105103f, -0.5772629976272583f, -0.8719490170478821f, 2.341320037841797f, -0.1359650045633316f, -0.985713005065918f, 0.8151469826698303f, 1.0311399698257446f, -1.4191499948501587f, -0.515533983707428f, -0.37363898754119873f, -1.5060399770736694f, 0.6731129884719849f, 3.061389923095703f, -0.38857799768447876f, -1.7670700550079346f, -0.3156670033931732f, -1.0381499528884888f, 0.4327870011329651f, -1.4164299964904785f, 1.1294399499893188f, -0.17580600082874298f, -0.8464149832725525f, 1.4009499549865723f, 0.7083200216293335f, 2.195620059967041f, -2.6126599311828613f, -0.7053830027580261f, 1.261240005493164f, 1.4654500484466553f, -2.357609987258911f, 2.0449399948120117f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 1, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.287350058555603f, 1.9131499528884888f, 2.5173399448394775f, 0.3758409917354584f, 0.6375629901885986f, 2.6530001163482666f, 2.7295899391174316f, 1.1738899946212769f, -2.121190071105957f, 2.914170026779175f, -2.242460012435913f, 0.049704499542713165f, -0.12710699439048767f, -0.14447300136089325f, -0.39328399300575256f, -2.0234599113464355f, -0.23917800188064575f, -0.24650800228118896f, 1.2927700281143188f, 1.3296300172805786f, 0.1175210028886795f, 0.06657130271196365f, 1.0943800210952759f, -1.3142600059509277f, 2.52593994140625f, -0.9692109823226929f, 0.5154780149459839f, -1.6092599630355835f, 0.1352110058069229f, 0.7864149808883667f, -1.14382004737854f, -0.7391020059585571f, -1.0173100233078003f, 0.2816149890422821f, 2.363110065460205f, 1.9387199878692627f, -0.15049099922180176f, 3.452169895172119f, 2.2821900844573975f, 1.1828199625015259f, -2.2508599758148193f, 3.054680109024048f}}},
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
return examples_1_H3_W2_VALID_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nhwc() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 1.74987f, -0.876718f, 0.989692f, 0.298548f, 0.522103f, -0.536896f, -0.179382f, -0.966914f, 1.33708f, 1.37042f, -0.495494f, 1.43859f, -1.548f, -0.430026f, -0.662793f, -0.0867897f, -0.900658f, -0.524396f, 0.255731f, -0.779081f, 0.12666f, 0.915651f, -0.444765f, -0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.952311f, -0.35811f, 0.403449f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, -0.126476f, -0.185224f, -0.114779f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, 0.318668f, 0.893795f, -0.0600559f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, 1.13613f, -1.22109f, 1.4649f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.407478f, -0.343106f, -0.0353232f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845042f, 2.48496f, 0.385019f, -0.201145f, 0.533332f, -0.904872f, -0.333518f, -0.581063f, -2.07065f, 0.118687f, -1.86708f, -0.601987f, 0.432037f, 1.73923f, 0.590007f, 0.419788f, 0.314198f, 2.12817f, 0.570793f, -1.15998f, -0.348587f, -1.10231f, -2.13091f, 0.134467f, -0.460382f, 0.138338f, 3.455f, 0.679068f, -0.190282f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0700466f, -0.110245f, 0.0141003f, -0.621007f, -0.979104f, 1.24104f, 0.580398f, -0.512997f, 0.900559f, -0.683229f, -1.0162f, 1.0089f, -0.0752488f, 0.110969f, 0.270558f, 0.756819f, -0.10753f, -0.371484f, 0.149005f, 0.0973829f, 0.155766f, -0.476502f, 0.259481f, 1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.453742f, -0.288829f, -0.0948694f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 0.280837f, 0.902901f, -1.90127f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.35182f, -1.85457f, 1.22364f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, -0.840276f, -0.455854f, -0.224913f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, 0.771629f, 0.949634f, 0.0838967f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746903f, 0.012994f, -0.790398f, -1.1055f, 1.77789f, 0.430923f, 0.818241f, -0.731412f, 0.979546f, -2.48707f, -1.53658f, -1.66798f, -1.04585f, -0.667911f, 1.00299f, -2.20339f, 0.137826f, -2.31281f, 0.755535f, 0.495396f, 0.549629f, 0.713128f, 0.751369f, 0.283996f, -0.814532f, 1.4866f, 1.12105f, 0.927998f, 0.517938f, -0.612661f, -1.47756f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_nhwc;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 1.74987f, -0.876718f, 0.989692f, 0.298548f, 0.522103f, -0.536896f, -0.179382f, -0.966914f, 1.33708f, 1.37042f, -0.495494f, 1.43859f, -1.548f, -0.430026f, -0.662793f, -0.0867897f, -0.900658f, -0.524396f, 0.255731f, -0.779081f, 0.12666f, 0.915651f, -0.444765f, -0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.952311f, -0.35811f, 0.403449f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, -0.126476f, -0.185224f, -0.114779f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, 0.318668f, 0.893795f, -0.0600559f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, 1.13613f, -1.22109f, 1.4649f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.407478f, -0.343106f, -0.0353232f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845042f, 2.48496f, 0.385019f, -0.201145f, 0.533332f, -0.904872f, -0.333518f, -0.581063f, -2.07065f, 0.118687f, -1.86708f, -0.601987f, 0.432037f, 1.73923f, 0.590007f, 0.419788f, 0.314198f, 2.12817f, 0.570793f, -1.15998f, -0.348587f, -1.10231f, -2.13091f, 0.134467f, -0.460382f, 0.138338f, 3.455f, 0.679068f, -0.190282f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0700466f, -0.110245f, 0.0141003f, -0.621007f, -0.979104f, 1.24104f, 0.580398f, -0.512997f, 0.900559f, -0.683229f, -1.0162f, 1.0089f, -0.0752488f, 0.110969f, 0.270558f, 0.756819f, -0.10753f, -0.371484f, 0.149005f, 0.0973829f, 0.155766f, -0.476502f, 0.259481f, 1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.453742f, -0.288829f, -0.0948694f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 0.280837f, 0.902901f, -1.90127f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.35182f, -1.85457f, 1.22364f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, -0.840276f, -0.455854f, -0.224913f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, 0.771629f, 0.949634f, 0.0838967f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746903f, 0.012994f, -0.790398f, -1.1055f, 1.77789f, 0.430923f, 0.818241f, -0.731412f, 0.979546f, -2.48707f, -1.53658f, -1.66798f, -1.04585f, -0.667911f, 1.00299f, -2.20339f, 0.137826f, -2.31281f, 0.755535f, 0.495396f, 0.549629f, 0.713128f, 0.751369f, 0.283996f, -0.814532f, 1.4866f, 1.12105f, 0.927998f, 0.517938f, -0.612661f, -1.47756f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nhwc_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-1.2785300016403198f, 1.7498699426651f, -0.8767179846763611f, 0.989691972732544f, 0.29854801297187805f, 0.5221030116081238f, -0.5368959903717041f, -0.17938199639320374f, -0.9669139981269836f, 1.3370800018310547f, 1.370419979095459f, -0.49549400806427f, 1.4385900497436523f, -1.5479999780654907f, -0.43002599477767944f, -0.662792980670929f, -0.08678969740867615f, -0.9006580114364624f, -0.5243960022926331f, 0.2557309865951538f, -0.7790809869766235f, 0.12666000425815582f, 0.9156510233879089f, -0.44476500153541565f, -0.18684199452400208f, -1.8730800151824951f, 1.2113499641418457f, -0.3850089907646179f, 1.7203199863433838f, -1.5603599548339844f, -1.2305899858474731f, 1.2369400262832642f, 0.0020001500379294157f, 0.3595220148563385f, 1.6008399724960327f, 0.434006005525589f, -0.28294500708580017f, 2.372920036315918f, -1.2865300178527832f, 0.08478370308876038f, -0.35209301114082336f, -2.396589994430542f, 0.1492460072040558f, 0.9203510284423828f, -1.343459963798523f, 0.9523109793663025f, -0.3581100106239319f, 0.40344899892807007f, 0.4847959876060486f, -1.1998900175094604f, -0.6842979788780212f, -1.4130100011825562f, 0.10317700356245041f, -0.3070389926433563f, 1.1774100065231323f, 2.589359998703003f, -2.7623701095581055f, -1.215649962425232f, -1.0961899757385254f, 1.1743199825286865f, 0.5121430158615112f, 0.7713789939880371f, 0.39987900853157043f, -0.05330929905176163f, 0.2908639907836914f, 0.9556300044059753f, 1.1632800102233887f, 1.8076800107955933f, -1.5256400108337402f, -0.1264760047197342f, -0.1852239966392517f, -0.11477900296449661f, 1.2247999906539917f, 0.23712700605392456f, -0.21329699456691742f, -0.619940996170044f, 0.4979439973831177f, -1.6868799924850464f, 1.5931400060653687f, -0.1273369938135147f, 0.11141899973154068f, 1.1371899843215942f, 1.6853699684143066f, -0.4796440005302429f, 1.186079978942871f, -2.527440071105957f, 1.3413599729537964f, 0.5482969880104065f, -2.0838000774383545f, 2.6458499431610107f, -0.9933540225028992f, 0.1282380074262619f, 1.2609200477600098f, 0.318668007850647f, 0.8937950134277344f, -0.06005590036511421f, -0.6291260123252869f, -0.9492290019989014f, 2.258280038833618f, -1.9609999656677246f, 0.0058959899470210075f, -0.18785400688648224f, -1.0240299701690674f, 0.39612099528312683f, 1.3703999519348145f, 3.9935500621795654f, 0.4342209994792938f, 0.2744640111923218f, -0.5624380111694336f, -0.9148709774017334f, 0.5391290187835693f, -0.9286869764328003f, 0.834954023361206f, 0.8441780209541321f, -0.5660529732704163f, -0.9573410153388977f, 0.9333360195159912f, 1.1361299753189087f, -1.2210899591445923f, 1.464900016784668f, -0.41466599702835083f, -0.4528209865093231f, -0.7060059905052185f, -1.7265700101852417f, -0.7265740036964417f, -0.09793619811534882f, -0.4786689877510071f, 1.7870299816131592f, -0.6392880082130432f, 1.4856499433517456f, -0.1799039989709854f, 1.0100300312042236f, -0.31711798906326294f, -0.6753870248794556f, 1.909690022468567f, -1.383430004119873f, 0.69725501537323f, -0.2922550141811371f, 1.8163399696350098f, 0.7178009748458862f, 0.8624789714813232f, -0.407478004693985f, -0.3431060016155243f, -0.03532319888472557f, -0.48189300298690796f, -0.13556499779224396f, -2.9594099521636963f, 0.24784600734710693f, 2.677570104598999f, -2.239989995956421f, -0.5196729898452759f, 0.25444701313972473f, 0.4152829945087433f, -1.0106500387191772f, 0.5079110264778137f, 0.9799259901046753f, -0.18430399894714355f, -0.0009504369809292257f, -0.7343479990959167f, -0.19668500125408173f, -0.7132409811019897f, 0.5949720144271851f, 0.08450420200824738f, 2.4849600791931152f, 0.3850190043449402f, -0.2011449933052063f, 0.5333319902420044f, -0.9048720002174377f, -0.33351799845695496f, -0.5810629725456238f, -2.070650100708008f, 0.11868699640035629f, -1.8670799732208252f, -0.6019870042800903f, 0.43203699588775635f, 1.7392300367355347f, 0.5900070071220398f, 0.4197880029678345f, 0.31419798731803894f, 2.1281700134277344f, 0.5707929730415344f, -1.159980058670044f, -0.3485870063304901f, -1.1023099422454834f, -2.1309099197387695f, 0.1344670057296753f, -0.46038201451301575f, 0.13833799958229065f, 3.4549999237060547f, 0.6790680289268494f, -0.19028200209140778f, -0.0307461004704237f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7857400178909302f, 0.07004660367965698f, -0.1102449968457222f, 0.014100300148129463f, -0.6210070252418518f, -0.9791039824485779f, 1.2410399913787842f, 0.5803980231285095f, -0.5129969716072083f, 0.9005590081214905f, -0.6832290291786194f, -1.0161999464035034f, 1.0089000463485718f, -0.07524880021810532f, 0.11096899956464767f, 0.2705579996109009f, 0.7568190097808838f, -0.10752999782562256f, -0.37148401141166687f, 0.1490049958229065f, 0.09738290309906006f, 0.15576599538326263f, -0.476502001285553f, 0.2594810128211975f, 1.0670900344848633f, -1.1653399467468262f, 1.5269399881362915f, -0.7972450256347656f, 0.8027359843254089f, -0.9971089959144592f, 2.2660999298095703f, -1.4554799795150757f, 2.155060052871704f, -1.3368200063705444f, 1.152250051498413f, -3.0932400226593018f, 0.9434570074081421f, 0.8852109909057617f, 0.9879440069198608f, -0.34587499499320984f, -0.11470799893140793f, 1.7107000350952148f, 0.10474500060081482f, 0.828324019908905f, -2.4996399879455566f, -0.45374199748039246f, -0.2888289988040924f, -0.09486939758062363f, -0.4894149899482727f, 1.7488900423049927f, -0.37825700640678406f, -2.102370023727417f, 0.6130220293998718f, -2.5225000381469727f, -0.7467849850654602f, 3.638159990310669f, -1.9286999702453613f, 0.774278998374939f, -0.6139169931411743f, -0.6500110030174255f, 1.0375299453735352f, -0.17792299389839172f, 0.8918150067329407f, -1.003730058670044f, 1.8385900259017944f, -1.5923899412155151f, -0.06626229733228683f, 0.21880599856376648f, -1.0880000591278076f, 0.2808369994163513f, 0.9029009938240051f, -1.9012700319290161f, 3.047339916229248f, -1.5730199813842773f, 1.1088099479675293f, -0.980368971824646f, -3.8530499935150146f, -0.9558590054512024f, 1.649090051651001f, 2.3357300758361816f, 0.31143999099731445f, -0.5943750143051147f, 0.325747013092041f, -0.9525660276412964f, -0.613448977470398f, 2.8507299423217773f, 1.9469200372695923f, 1.129770040512085f, 1.13510000705719f, -0.4496519863605499f, 0.11876499652862549f, -0.19954699277877808f, 2.872999906539917f, 1.3518199920654297f, -1.8545700311660767f, 1.223639965057373f, 1.3804899454116821f, 2.383419990539551f, 0.8823210000991821f, 1.037950038909912f, -0.3215709924697876f, -2.602020025253296f, -1.6371999979019165f, 1.093019962310791f, 0.4617680013179779f, 1.8485000133514404f, -0.1589280068874359f, 4.288710117340088f, -0.437375009059906f, -1.5793999433517456f, 1.5986900329589844f, 0.08118639886379242f, 0.9120540022850037f, 0.4521760046482086f, 2.018120050430298f, 2.6290700435638428f, 1.5030399560928345f, -0.8402760028839111f, -0.45585399866104126f, -0.22491300106048584f, 0.609824001789093f, -0.11105000227689743f, 3.3563499450683594f, 2.023859977722168f, 1.4687000513076782f, -0.7083650231361389f, -0.5089920163154602f, -3.026020050048828f, -0.7572500109672546f, 1.8527699708938599f, 2.9281699657440186f, -0.17299699783325195f, -1.132789969444275f, -0.35563600063323975f, -0.3376689851284027f, -0.5887519717216492f, 2.0575900077819824f, 1.0650999546051025f, 0.8847579956054688f, -0.07121119648218155f, 3.81318998336792f, 0.7716289758682251f, 0.9496340155601501f, 0.0838966965675354f, -2.1926400661468506f, 0.11452099680900574f, 0.5435559749603271f, -1.6319700479507446f, -0.267441987991333f, 1.1570099592208862f, -2.378619909286499f, 2.5764598846435547f, 0.5312079787254333f, 0.9498999714851379f, -0.23144100606441498f, 1.5146100521087646f, 1.588879942893982f, 0.8959310054779053f, -0.7530840039253235f, 0.5452510118484497f, 0.7469030022621155f, 0.012993999756872654f, -0.7903980016708374f, -1.1054999828338623f, 1.7778899669647217f, 0.4309230148792267f, 0.8182410001754761f, -0.7314119935035706f, 0.9795460104942322f, -2.487070083618164f, -1.536579966545105f, -1.6679799556732178f, -1.0458500385284424f, -0.6679109930992126f, 1.0029900074005127f, -2.203389883041382f, 0.13782599568367004f, -2.312809944152832f, 0.7555350065231323f, 0.4953959882259369f, 0.5496289730072021f, 0.7131279706954956f, 0.7513689994812012f, 0.28399598598480225f, -0.8145319819450378f, 1.4866000413894653f, 1.1210500001907349f, 0.9279980063438416f, 0.5179380178451538f, -0.6126610040664673f, -1.477560043334961f, -1.4242199659347534f}}},
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
return examples_3_H3_W2_SAME_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nchw() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 0.989692f, -0.536896f, 1.33708f, 1.43859f, -0.662793f, -0.524396f, 0.12666f, -0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.952311f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, -0.126476f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, 0.318668f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, 1.13613f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.407478f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845042f, -0.201145f, -0.333518f, 0.118687f, 0.432037f, 0.419788f, 0.570793f, -1.10231f, -0.460382f, 0.679068f, 1.74987f, 0.298548f, -0.179382f, 1.37042f, -1.548f, -0.0867897f, 0.255731f, 0.915651f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -0.35811f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, -0.185224f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, 0.893795f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -1.22109f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.343106f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 0.533332f, -0.581063f, -1.86708f, 1.73923f, 0.314198f, -1.15998f, -2.13091f, 0.138338f, -0.190282f, -0.876718f, 0.522103f, -0.966914f, -0.495494f, -0.430026f, -0.900658f, -0.779081f, -0.444765f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, 0.403449f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.114779f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, -0.0600559f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, 1.4649f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -0.0353232f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f, -0.904872f, -2.07065f, -0.601987f, 0.590007f, 2.12817f, -0.348587f, 0.134467f, 3.455f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0141003f, 1.24104f, 0.900559f, 1.0089f, 0.270558f, -0.371484f, 0.155766f, 1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.453742f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 0.280837f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.35182f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, -0.840276f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, 0.771629f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, 0.430923f, 0.979546f, -1.66798f, 1.00299f, -2.31281f, 0.549629f, 0.283996f, 1.12105f, -0.612661f, 0.0700466f, -0.621007f, 0.580398f, -0.683229f, -0.0752488f, 0.756819f, 0.149005f, -0.476502f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, -0.288829f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, 0.902901f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, -1.85457f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.455854f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.949634f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746903f, -1.1055f, 0.818241f, -2.48707f, -1.04585f, -2.20339f, 0.755535f, 0.713128f, -0.814532f, 0.927998f, -1.47756f, -0.110245f, -0.979104f, -0.512997f, -1.0162f, 0.110969f, -0.10753f, 0.0973829f, 0.259481f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.0948694f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, -1.90127f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 1.22364f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, -0.224913f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.0838967f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.012994f, 1.77789f, -0.731412f, -1.53658f, -0.667911f, 0.137826f, 0.495396f, 0.751369f, 1.4866f, 0.517938f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_nchw;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 0.989692f, -0.536896f, 1.33708f, 1.43859f, -0.662793f, -0.524396f, 0.12666f, -0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.952311f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, -0.126476f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, 0.318668f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, 1.13613f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.407478f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845042f, -0.201145f, -0.333518f, 0.118687f, 0.432037f, 0.419788f, 0.570793f, -1.10231f, -0.460382f, 0.679068f, 1.74987f, 0.298548f, -0.179382f, 1.37042f, -1.548f, -0.0867897f, 0.255731f, 0.915651f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -0.35811f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, -0.185224f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, 0.893795f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -1.22109f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.343106f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 0.533332f, -0.581063f, -1.86708f, 1.73923f, 0.314198f, -1.15998f, -2.13091f, 0.138338f, -0.190282f, -0.876718f, 0.522103f, -0.966914f, -0.495494f, -0.430026f, -0.900658f, -0.779081f, -0.444765f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, 0.403449f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.114779f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, -0.0600559f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, 1.4649f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -0.0353232f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f, -0.904872f, -2.07065f, -0.601987f, 0.590007f, 2.12817f, -0.348587f, 0.134467f, 3.455f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0141003f, 1.24104f, 0.900559f, 1.0089f, 0.270558f, -0.371484f, 0.155766f, 1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.453742f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 0.280837f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.35182f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, -0.840276f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, 0.771629f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, 0.430923f, 0.979546f, -1.66798f, 1.00299f, -2.31281f, 0.549629f, 0.283996f, 1.12105f, -0.612661f, 0.0700466f, -0.621007f, 0.580398f, -0.683229f, -0.0752488f, 0.756819f, 0.149005f, -0.476502f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, -0.288829f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, 0.902901f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, -1.85457f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.455854f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.949634f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746903f, -1.1055f, 0.818241f, -2.48707f, -1.04585f, -2.20339f, 0.755535f, 0.713128f, -0.814532f, 0.927998f, -1.47756f, -0.110245f, -0.979104f, -0.512997f, -1.0162f, 0.110969f, -0.10753f, 0.0973829f, 0.259481f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.0948694f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, -1.90127f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 1.22364f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, -0.224913f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.0838967f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.012994f, 1.77789f, -0.731412f, -1.53658f, -0.667911f, 0.137826f, 0.495396f, 0.751369f, 1.4866f, 0.517938f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_nchw_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-1.2785300016403198f, 0.989691972732544f, -0.5368959903717041f, 1.3370800018310547f, 1.4385900497436523f, -0.662792980670929f, -0.5243960022926331f, 0.12666000425815582f, -0.18684199452400208f, -0.3850089907646179f, -1.2305899858474731f, 0.3595220148563385f, -0.28294500708580017f, 0.08478370308876038f, 0.1492460072040558f, 0.9523109793663025f, 0.4847959876060486f, -1.4130100011825562f, 1.1774100065231323f, -1.215649962425232f, 0.5121430158615112f, -0.05330929905176163f, 1.1632800102233887f, -0.1264760047197342f, 1.2247999906539917f, -0.619940996170044f, 1.5931400060653687f, 1.1371899843215942f, 1.186079978942871f, 0.5482969880104065f, -0.9933540225028992f, 0.318668007850647f, -0.6291260123252869f, -1.9609999656677246f, -1.0240299701690674f, 3.9935500621795654f, -0.5624380111694336f, -0.9286869764328003f, -0.5660529732704163f, 1.1361299753189087f, -0.41466599702835083f, -1.7265700101852417f, -0.4786689877510071f, 1.4856499433517456f, -0.31711798906326294f, -1.383430004119873f, 1.8163399696350098f, -0.407478004693985f, -0.48189300298690796f, 0.24784600734710693f, -0.5196729898452759f, -1.0106500387191772f, -0.18430399894714355f, -0.19668500125408173f, 0.08450420200824738f, -0.2011449933052063f, -0.33351799845695496f, 0.11868699640035629f, 0.43203699588775635f, 0.4197880029678345f, 0.5707929730415344f, -1.1023099422454834f, -0.46038201451301575f, 0.6790680289268494f, 1.7498699426651f, 0.29854801297187805f, -0.17938199639320374f, 1.370419979095459f, -1.5479999780654907f, -0.08678969740867615f, 0.2557309865951538f, 0.9156510233879089f, -1.8730800151824951f, 1.7203199863433838f, 1.2369400262832642f, 1.6008399724960327f, 2.372920036315918f, -0.35209301114082336f, 0.9203510284423828f, -0.3581100106239319f, -1.1998900175094604f, 0.10317700356245041f, 2.589359998703003f, -1.0961899757385254f, 0.7713789939880371f, 0.2908639907836914f, 1.8076800107955933f, -0.1852239966392517f, 0.23712700605392456f, 0.4979439973831177f, -0.1273369938135147f, 1.6853699684143066f, -2.527440071105957f, -2.0838000774383545f, 0.1282380074262619f, 0.8937950134277344f, -0.9492290019989014f, 0.0058959899470210075f, 0.39612099528312683f, 0.4342209994792938f, -0.9148709774017334f, 0.834954023361206f, -0.9573410153388977f, -1.2210899591445923f, -0.4528209865093231f, -0.7265740036964417f, 1.7870299816131592f, -0.1799039989709854f, -0.6753870248794556f, 0.69725501537323f, 0.7178009748458862f, -0.3431060016155243f, -0.13556499779224396f, 2.677570104598999f, 0.25444701313972473f, 0.5079110264778137f, -0.0009504369809292257f, -0.7132409811019897f, 2.4849600791931152f, 0.5333319902420044f, -0.5810629725456238f, -1.8670799732208252f, 1.7392300367355347f, 0.31419798731803894f, -1.159980058670044f, -2.1309099197387695f, 0.13833799958229065f, -0.19028200209140778f, -0.8767179846763611f, 0.5221030116081238f, -0.9669139981269836f, -0.49549400806427f, -0.43002599477767944f, -0.9006580114364624f, -0.7790809869766235f, -0.44476500153541565f, 1.2113499641418457f, -1.5603599548339844f, 0.0020001500379294157f, 0.434006005525589f, -1.2865300178527832f, -2.396589994430542f, -1.343459963798523f, 0.40344899892807007f, -0.6842979788780212f, -0.3070389926433563f, -2.7623701095581055f, 1.1743199825286865f, 0.39987900853157043f, 0.9556300044059753f, -1.5256400108337402f, -0.11477900296449661f, -0.21329699456691742f, -1.6868799924850464f, 0.11141899973154068f, -0.4796440005302429f, 1.3413599729537964f, 2.6458499431610107f, 1.2609200477600098f, -0.06005590036511421f, 2.258280038833618f, -0.18785400688648224f, 1.3703999519348145f, 0.2744640111923218f, 0.5391290187835693f, 0.8441780209541321f, 0.9333360195159912f, 1.464900016784668f, -0.7060059905052185f, -0.09793619811534882f, -0.6392880082130432f, 1.0100300312042236f, 1.909690022468567f, -0.2922550141811371f, 0.8624789714813232f, -0.03532319888472557f, -2.9594099521636963f, -2.239989995956421f, 0.4152829945087433f, 0.9799259901046753f, -0.7343479990959167f, 0.5949720144271851f, 0.3850190043449402f, -0.9048720002174377f, -2.070650100708008f, -0.6019870042800903f, 0.5900070071220398f, 2.1281700134277344f, -0.3485870063304901f, 0.1344670057296753f, 3.4549999237060547f, -0.0307461004704237f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7857400178909302f, 0.014100300148129463f, 1.2410399913787842f, 0.9005590081214905f, 1.0089000463485718f, 0.2705579996109009f, -0.37148401141166687f, 0.15576599538326263f, 1.0670900344848633f, -0.7972450256347656f, 2.2660999298095703f, -1.3368200063705444f, 0.9434570074081421f, -0.34587499499320984f, 0.10474500060081482f, -0.45374199748039246f, -0.4894149899482727f, -2.102370023727417f, -0.7467849850654602f, 0.774278998374939f, 1.0375299453735352f, -1.003730058670044f, -0.06626229733228683f, 0.2808369994163513f, 3.047339916229248f, -0.980368971824646f, 1.649090051651001f, -0.5943750143051147f, -0.613448977470398f, 1.129770040512085f, 0.11876499652862549f, 1.3518199920654297f, 1.3804899454116821f, 1.037950038909912f, -1.6371999979019165f, 1.8485000133514404f, -0.437375009059906f, 0.08118639886379242f, 2.018120050430298f, -0.8402760028839111f, 0.609824001789093f, 2.023859977722168f, -0.5089920163154602f, 1.8527699708938599f, -1.132789969444275f, -0.5887519717216492f, 0.8847579956054688f, 0.7716289758682251f, -2.1926400661468506f, -1.6319700479507446f, -2.378619909286499f, 0.9498999714851379f, 1.588879942893982f, 0.5452510118484497f, -0.7903980016708374f, 0.4309230148792267f, 0.9795460104942322f, -1.6679799556732178f, 1.0029900074005127f, -2.312809944152832f, 0.5496289730072021f, 0.28399598598480225f, 1.1210500001907349f, -0.6126610040664673f, 0.07004660367965698f, -0.6210070252418518f, 0.5803980231285095f, -0.6832290291786194f, -0.07524880021810532f, 0.7568190097808838f, 0.1490049958229065f, -0.476502001285553f, -1.1653399467468262f, 0.8027359843254089f, -1.4554799795150757f, 1.152250051498413f, 0.8852109909057617f, -0.11470799893140793f, 0.828324019908905f, -0.2888289988040924f, 1.7488900423049927f, 0.6130220293998718f, 3.638159990310669f, -0.6139169931411743f, -0.17792299389839172f, 1.8385900259017944f, 0.21880599856376648f, 0.9029009938240051f, -1.5730199813842773f, -3.8530499935150146f, 2.3357300758361816f, 0.325747013092041f, 2.8507299423217773f, 1.13510000705719f, -0.19954699277877808f, -1.8545700311660767f, 2.383419990539551f, -0.3215709924697876f, 1.093019962310791f, -0.1589280068874359f, -1.5793999433517456f, 0.9120540022850037f, 2.6290700435638428f, -0.45585399866104126f, -0.11105000227689743f, 1.4687000513076782f, -3.026020050048828f, 2.9281699657440186f, -0.35563600063323975f, 2.0575900077819824f, -0.07121119648218155f, 0.9496340155601501f, 0.11452099680900574f, -0.267441987991333f, 2.5764598846435547f, -0.23144100606441498f, 0.8959310054779053f, 0.7469030022621155f, -1.1054999828338623f, 0.8182410001754761f, -2.487070083618164f, -1.0458500385284424f, -2.203389883041382f, 0.7555350065231323f, 0.7131279706954956f, -0.8145319819450378f, 0.9279980063438416f, -1.477560043334961f, -0.1102449968457222f, -0.9791039824485779f, -0.5129969716072083f, -1.0161999464035034f, 0.11096899956464767f, -0.10752999782562256f, 0.09738290309906006f, 0.2594810128211975f, 1.5269399881362915f, -0.9971089959144592f, 2.155060052871704f, -3.0932400226593018f, 0.9879440069198608f, 1.7107000350952148f, -2.4996399879455566f, -0.09486939758062363f, -0.37825700640678406f, -2.5225000381469727f, -1.9286999702453613f, -0.6500110030174255f, 0.8918150067329407f, -1.5923899412155151f, -1.0880000591278076f, -1.9012700319290161f, 1.1088099479675293f, -0.9558590054512024f, 0.31143999099731445f, -0.9525660276412964f, 1.9469200372695923f, -0.4496519863605499f, 2.872999906539917f, 1.223639965057373f, 0.8823210000991821f, -2.602020025253296f, 0.4617680013179779f, 4.288710117340088f, 1.5986900329589844f, 0.4521760046482086f, 1.5030399560928345f, -0.22491300106048584f, 3.3563499450683594f, -0.7083650231361389f, -0.7572500109672546f, -0.17299699783325195f, -0.3376689851284027f, 1.0650999546051025f, 3.81318998336792f, 0.0838966965675354f, 0.5435559749603271f, 1.1570099592208862f, 0.5312079787254333f, 1.5146100521087646f, -0.7530840039253235f, 0.012993999756872654f, 1.7778899669647217f, -0.7314119935035706f, -1.536579966545105f, -0.6679109930992126f, 0.13782599568367004f, 0.4953959882259369f, 0.7513689994812012f, 1.4866000413894653f, 0.5179380178451538f, -1.4242199659347534f}}},
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
return examples_3_H3_W2_SAME_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 1.74987f, -0.876718f, 0.989692f, 0.298548f, 0.522103f, -0.536896f, -0.179382f, -0.966914f, 1.33708f, 1.37042f, -0.495494f, 1.43859f, -1.548f, -0.430026f, -0.662793f, -0.0867897f, -0.900658f, -0.524396f, 0.255731f, -0.779081f, 0.12666f, 0.915651f, -0.444765f, -0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.952311f, -0.35811f, 0.403449f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, -0.126476f, -0.185224f, -0.114779f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, 0.318668f, 0.893795f, -0.0600559f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, 1.13613f, -1.22109f, 1.4649f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.407478f, -0.343106f, -0.0353232f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845042f, 2.48496f, 0.385019f, -0.201145f, 0.533332f, -0.904872f, -0.333518f, -0.581063f, -2.07065f, 0.118687f, -1.86708f, -0.601987f, 0.432037f, 1.73923f, 0.590007f, 0.419788f, 0.314198f, 2.12817f, 0.570793f, -1.15998f, -0.348587f, -1.10231f, -2.13091f, 0.134467f, -0.460382f, 0.138338f, 3.455f, 0.679068f, -0.190282f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0700466f, -0.110245f, 0.0141003f, -0.621007f, -0.979104f, 1.24104f, 0.580398f, -0.512997f, 0.900559f, -0.683229f, -1.0162f, 1.0089f, -0.0752488f, 0.110969f, 0.270558f, 0.756819f, -0.10753f, -0.371484f, 0.149005f, 0.0973829f, 0.155766f, -0.476502f, 0.259481f, 1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.453742f, -0.288829f, -0.0948694f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 0.280837f, 0.902901f, -1.90127f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.35182f, -1.85457f, 1.22364f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, -0.840276f, -0.455854f, -0.224913f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, 0.771629f, 0.949634f, 0.0838967f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746903f, 0.012994f, -0.790398f, -1.1055f, 1.77789f, 0.430923f, 0.818241f, -0.731412f, 0.979546f, -2.48707f, -1.53658f, -1.66798f, -1.04585f, -0.667911f, 1.00299f, -2.20339f, 0.137826f, -2.31281f, 0.755535f, 0.495396f, 0.549629f, 0.713128f, 0.751369f, 0.283996f, -0.814532f, 1.4866f, 1.12105f, 0.927998f, 0.517938f, -0.612661f, -1.47756f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 1.74987f, -0.876718f, 0.989692f, 0.298548f, 0.522103f, -0.536896f, -0.179382f, -0.966914f, 1.33708f, 1.37042f, -0.495494f, 1.43859f, -1.548f, -0.430026f, -0.662793f, -0.0867897f, -0.900658f, -0.524396f, 0.255731f, -0.779081f, 0.12666f, 0.915651f, -0.444765f, -0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.952311f, -0.35811f, 0.403449f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, -0.126476f, -0.185224f, -0.114779f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, 0.318668f, 0.893795f, -0.0600559f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, 1.13613f, -1.22109f, 1.4649f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.407478f, -0.343106f, -0.0353232f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845042f, 2.48496f, 0.385019f, -0.201145f, 0.533332f, -0.904872f, -0.333518f, -0.581063f, -2.07065f, 0.118687f, -1.86708f, -0.601987f, 0.432037f, 1.73923f, 0.590007f, 0.419788f, 0.314198f, 2.12817f, 0.570793f, -1.15998f, -0.348587f, -1.10231f, -2.13091f, 0.134467f, -0.460382f, 0.138338f, 3.455f, 0.679068f, -0.190282f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0700466f, -0.110245f, 0.0141003f, -0.621007f, -0.979104f, 1.24104f, 0.580398f, -0.512997f, 0.900559f, -0.683229f, -1.0162f, 1.0089f, -0.0752488f, 0.110969f, 0.270558f, 0.756819f, -0.10753f, -0.371484f, 0.149005f, 0.0973829f, 0.155766f, -0.476502f, 0.259481f, 1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.453742f, -0.288829f, -0.0948694f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 0.280837f, 0.902901f, -1.90127f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.35182f, -1.85457f, 1.22364f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, -0.840276f, -0.455854f, -0.224913f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, 0.771629f, 0.949634f, 0.0838967f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746903f, 0.012994f, -0.790398f, -1.1055f, 1.77789f, 0.430923f, 0.818241f, -0.731412f, 0.979546f, -2.48707f, -1.53658f, -1.66798f, -1.04585f, -0.667911f, 1.00299f, -2.20339f, 0.137826f, -2.31281f, 0.755535f, 0.495396f, 0.549629f, 0.713128f, 0.751369f, 0.283996f, -0.814532f, 1.4866f, 1.12105f, 0.927998f, 0.517938f, -0.612661f, -1.47756f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-1.2785300016403198f, 1.7498699426651f, -0.8767179846763611f, 0.989691972732544f, 0.29854801297187805f, 0.5221030116081238f, -0.5368959903717041f, -0.17938199639320374f, -0.9669139981269836f, 1.3370800018310547f, 1.370419979095459f, -0.49549400806427f, 1.4385900497436523f, -1.5479999780654907f, -0.43002599477767944f, -0.662792980670929f, -0.08678969740867615f, -0.9006580114364624f, -0.5243960022926331f, 0.2557309865951538f, -0.7790809869766235f, 0.12666000425815582f, 0.9156510233879089f, -0.44476500153541565f, -0.18684199452400208f, -1.8730800151824951f, 1.2113499641418457f, -0.3850089907646179f, 1.7203199863433838f, -1.5603599548339844f, -1.2305899858474731f, 1.2369400262832642f, 0.0020001500379294157f, 0.3595220148563385f, 1.6008399724960327f, 0.434006005525589f, -0.28294500708580017f, 2.372920036315918f, -1.2865300178527832f, 0.08478370308876038f, -0.35209301114082336f, -2.396589994430542f, 0.1492460072040558f, 0.9203510284423828f, -1.343459963798523f, 0.9523109793663025f, -0.3581100106239319f, 0.40344899892807007f, 0.4847959876060486f, -1.1998900175094604f, -0.6842979788780212f, -1.4130100011825562f, 0.10317700356245041f, -0.3070389926433563f, 1.1774100065231323f, 2.589359998703003f, -2.7623701095581055f, -1.215649962425232f, -1.0961899757385254f, 1.1743199825286865f, 0.5121430158615112f, 0.7713789939880371f, 0.39987900853157043f, -0.05330929905176163f, 0.2908639907836914f, 0.9556300044059753f, 1.1632800102233887f, 1.8076800107955933f, -1.5256400108337402f, -0.1264760047197342f, -0.1852239966392517f, -0.11477900296449661f, 1.2247999906539917f, 0.23712700605392456f, -0.21329699456691742f, -0.619940996170044f, 0.4979439973831177f, -1.6868799924850464f, 1.5931400060653687f, -0.1273369938135147f, 0.11141899973154068f, 1.1371899843215942f, 1.6853699684143066f, -0.4796440005302429f, 1.186079978942871f, -2.527440071105957f, 1.3413599729537964f, 0.5482969880104065f, -2.0838000774383545f, 2.6458499431610107f, -0.9933540225028992f, 0.1282380074262619f, 1.2609200477600098f, 0.318668007850647f, 0.8937950134277344f, -0.06005590036511421f, -0.6291260123252869f, -0.9492290019989014f, 2.258280038833618f, -1.9609999656677246f, 0.0058959899470210075f, -0.18785400688648224f, -1.0240299701690674f, 0.39612099528312683f, 1.3703999519348145f, 3.9935500621795654f, 0.4342209994792938f, 0.2744640111923218f, -0.5624380111694336f, -0.9148709774017334f, 0.5391290187835693f, -0.9286869764328003f, 0.834954023361206f, 0.8441780209541321f, -0.5660529732704163f, -0.9573410153388977f, 0.9333360195159912f, 1.1361299753189087f, -1.2210899591445923f, 1.464900016784668f, -0.41466599702835083f, -0.4528209865093231f, -0.7060059905052185f, -1.7265700101852417f, -0.7265740036964417f, -0.09793619811534882f, -0.4786689877510071f, 1.7870299816131592f, -0.6392880082130432f, 1.4856499433517456f, -0.1799039989709854f, 1.0100300312042236f, -0.31711798906326294f, -0.6753870248794556f, 1.909690022468567f, -1.383430004119873f, 0.69725501537323f, -0.2922550141811371f, 1.8163399696350098f, 0.7178009748458862f, 0.8624789714813232f, -0.407478004693985f, -0.3431060016155243f, -0.03532319888472557f, -0.48189300298690796f, -0.13556499779224396f, -2.9594099521636963f, 0.24784600734710693f, 2.677570104598999f, -2.239989995956421f, -0.5196729898452759f, 0.25444701313972473f, 0.4152829945087433f, -1.0106500387191772f, 0.5079110264778137f, 0.9799259901046753f, -0.18430399894714355f, -0.0009504369809292257f, -0.7343479990959167f, -0.19668500125408173f, -0.7132409811019897f, 0.5949720144271851f, 0.08450420200824738f, 2.4849600791931152f, 0.3850190043449402f, -0.2011449933052063f, 0.5333319902420044f, -0.9048720002174377f, -0.33351799845695496f, -0.5810629725456238f, -2.070650100708008f, 0.11868699640035629f, -1.8670799732208252f, -0.6019870042800903f, 0.43203699588775635f, 1.7392300367355347f, 0.5900070071220398f, 0.4197880029678345f, 0.31419798731803894f, 2.1281700134277344f, 0.5707929730415344f, -1.159980058670044f, -0.3485870063304901f, -1.1023099422454834f, -2.1309099197387695f, 0.1344670057296753f, -0.46038201451301575f, 0.13833799958229065f, 3.4549999237060547f, 0.6790680289268494f, -0.19028200209140778f, -0.0307461004704237f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7857400178909302f, 0.07004660367965698f, -0.1102449968457222f, 0.014100300148129463f, -0.6210070252418518f, -0.9791039824485779f, 1.2410399913787842f, 0.5803980231285095f, -0.5129969716072083f, 0.9005590081214905f, -0.6832290291786194f, -1.0161999464035034f, 1.0089000463485718f, -0.07524880021810532f, 0.11096899956464767f, 0.2705579996109009f, 0.7568190097808838f, -0.10752999782562256f, -0.37148401141166687f, 0.1490049958229065f, 0.09738290309906006f, 0.15576599538326263f, -0.476502001285553f, 0.2594810128211975f, 1.0670900344848633f, -1.1653399467468262f, 1.5269399881362915f, -0.7972450256347656f, 0.8027359843254089f, -0.9971089959144592f, 2.2660999298095703f, -1.4554799795150757f, 2.155060052871704f, -1.3368200063705444f, 1.152250051498413f, -3.0932400226593018f, 0.9434570074081421f, 0.8852109909057617f, 0.9879440069198608f, -0.34587499499320984f, -0.11470799893140793f, 1.7107000350952148f, 0.10474500060081482f, 0.828324019908905f, -2.4996399879455566f, -0.45374199748039246f, -0.2888289988040924f, -0.09486939758062363f, -0.4894149899482727f, 1.7488900423049927f, -0.37825700640678406f, -2.102370023727417f, 0.6130220293998718f, -2.5225000381469727f, -0.7467849850654602f, 3.638159990310669f, -1.9286999702453613f, 0.774278998374939f, -0.6139169931411743f, -0.6500110030174255f, 1.0375299453735352f, -0.17792299389839172f, 0.8918150067329407f, -1.003730058670044f, 1.8385900259017944f, -1.5923899412155151f, -0.06626229733228683f, 0.21880599856376648f, -1.0880000591278076f, 0.2808369994163513f, 0.9029009938240051f, -1.9012700319290161f, 3.047339916229248f, -1.5730199813842773f, 1.1088099479675293f, -0.980368971824646f, -3.8530499935150146f, -0.9558590054512024f, 1.649090051651001f, 2.3357300758361816f, 0.31143999099731445f, -0.5943750143051147f, 0.325747013092041f, -0.9525660276412964f, -0.613448977470398f, 2.8507299423217773f, 1.9469200372695923f, 1.129770040512085f, 1.13510000705719f, -0.4496519863605499f, 0.11876499652862549f, -0.19954699277877808f, 2.872999906539917f, 1.3518199920654297f, -1.8545700311660767f, 1.223639965057373f, 1.3804899454116821f, 2.383419990539551f, 0.8823210000991821f, 1.037950038909912f, -0.3215709924697876f, -2.602020025253296f, -1.6371999979019165f, 1.093019962310791f, 0.4617680013179779f, 1.8485000133514404f, -0.1589280068874359f, 4.288710117340088f, -0.437375009059906f, -1.5793999433517456f, 1.5986900329589844f, 0.08118639886379242f, 0.9120540022850037f, 0.4521760046482086f, 2.018120050430298f, 2.6290700435638428f, 1.5030399560928345f, -0.8402760028839111f, -0.45585399866104126f, -0.22491300106048584f, 0.609824001789093f, -0.11105000227689743f, 3.3563499450683594f, 2.023859977722168f, 1.4687000513076782f, -0.7083650231361389f, -0.5089920163154602f, -3.026020050048828f, -0.7572500109672546f, 1.8527699708938599f, 2.9281699657440186f, -0.17299699783325195f, -1.132789969444275f, -0.35563600063323975f, -0.3376689851284027f, -0.5887519717216492f, 2.0575900077819824f, 1.0650999546051025f, 0.8847579956054688f, -0.07121119648218155f, 3.81318998336792f, 0.7716289758682251f, 0.9496340155601501f, 0.0838966965675354f, -2.1926400661468506f, 0.11452099680900574f, 0.5435559749603271f, -1.6319700479507446f, -0.267441987991333f, 1.1570099592208862f, -2.378619909286499f, 2.5764598846435547f, 0.5312079787254333f, 0.9498999714851379f, -0.23144100606441498f, 1.5146100521087646f, 1.588879942893982f, 0.8959310054779053f, -0.7530840039253235f, 0.5452510118484497f, 0.7469030022621155f, 0.012993999756872654f, -0.7903980016708374f, -1.1054999828338623f, 1.7778899669647217f, 0.4309230148792267f, 0.8182410001754761f, -0.7314119935035706f, 0.9795460104942322f, -2.487070083618164f, -1.536579966545105f, -1.6679799556732178f, -1.0458500385284424f, -0.6679109930992126f, 1.0029900074005127f, -2.203389883041382f, 0.13782599568367004f, -2.312809944152832f, 0.7555350065231323f, 0.4953959882259369f, 0.5496289730072021f, 0.7131279706954956f, 0.7513689994812012f, 0.28399598598480225f, -0.8145319819450378f, 1.4866000413894653f, 1.1210500001907349f, 0.9279980063438416f, 0.5179380178451538f, -0.6126610040664673f, -1.477560043334961f, -1.4242199659347534f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 0.989692f, -0.536896f, 1.33708f, 1.43859f, -0.662793f, -0.524396f, 0.12666f, -0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.952311f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, -0.126476f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, 0.318668f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, 1.13613f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.407478f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845042f, -0.201145f, -0.333518f, 0.118687f, 0.432037f, 0.419788f, 0.570793f, -1.10231f, -0.460382f, 0.679068f, 1.74987f, 0.298548f, -0.179382f, 1.37042f, -1.548f, -0.0867897f, 0.255731f, 0.915651f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -0.35811f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, -0.185224f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, 0.893795f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -1.22109f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.343106f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 0.533332f, -0.581063f, -1.86708f, 1.73923f, 0.314198f, -1.15998f, -2.13091f, 0.138338f, -0.190282f, -0.876718f, 0.522103f, -0.966914f, -0.495494f, -0.430026f, -0.900658f, -0.779081f, -0.444765f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, 0.403449f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.114779f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, -0.0600559f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, 1.4649f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -0.0353232f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f, -0.904872f, -2.07065f, -0.601987f, 0.590007f, 2.12817f, -0.348587f, 0.134467f, 3.455f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0141003f, 1.24104f, 0.900559f, 1.0089f, 0.270558f, -0.371484f, 0.155766f, 1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.453742f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 0.280837f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.35182f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, -0.840276f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, 0.771629f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, 0.430923f, 0.979546f, -1.66798f, 1.00299f, -2.31281f, 0.549629f, 0.283996f, 1.12105f, -0.612661f, 0.0700466f, -0.621007f, 0.580398f, -0.683229f, -0.0752488f, 0.756819f, 0.149005f, -0.476502f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, -0.288829f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, 0.902901f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, -1.85457f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.455854f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.949634f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746903f, -1.1055f, 0.818241f, -2.48707f, -1.04585f, -2.20339f, 0.755535f, 0.713128f, -0.814532f, 0.927998f, -1.47756f, -0.110245f, -0.979104f, -0.512997f, -1.0162f, 0.110969f, -0.10753f, 0.0973829f, 0.259481f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.0948694f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, -1.90127f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 1.22364f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, -0.224913f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.0838967f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.012994f, 1.77789f, -0.731412f, -1.53658f, -0.667911f, 0.137826f, 0.495396f, 0.751369f, 1.4866f, 0.517938f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-1.27853f, 0.989692f, -0.536896f, 1.33708f, 1.43859f, -0.662793f, -0.524396f, 0.12666f, -0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.952311f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, -0.126476f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, 0.318668f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, 1.13613f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.407478f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845042f, -0.201145f, -0.333518f, 0.118687f, 0.432037f, 0.419788f, 0.570793f, -1.10231f, -0.460382f, 0.679068f, 1.74987f, 0.298548f, -0.179382f, 1.37042f, -1.548f, -0.0867897f, 0.255731f, 0.915651f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -0.35811f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, -0.185224f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, 0.893795f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -1.22109f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.343106f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 0.533332f, -0.581063f, -1.86708f, 1.73923f, 0.314198f, -1.15998f, -2.13091f, 0.138338f, -0.190282f, -0.876718f, 0.522103f, -0.966914f, -0.495494f, -0.430026f, -0.900658f, -0.779081f, -0.444765f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, 0.403449f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.114779f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, -0.0600559f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, 1.4649f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -0.0353232f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f, -0.904872f, -2.07065f, -0.601987f, 0.590007f, 2.12817f, -0.348587f, 0.134467f, 3.455f, -0.0307461f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.78574f, 0.0141003f, 1.24104f, 0.900559f, 1.0089f, 0.270558f, -0.371484f, 0.155766f, 1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.453742f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 0.280837f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.35182f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, -0.840276f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, 0.771629f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, 0.430923f, 0.979546f, -1.66798f, 1.00299f, -2.31281f, 0.549629f, 0.283996f, 1.12105f, -0.612661f, 0.0700466f, -0.621007f, 0.580398f, -0.683229f, -0.0752488f, 0.756819f, 0.149005f, -0.476502f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, -0.288829f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, 0.902901f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, -1.85457f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.455854f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.949634f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746903f, -1.1055f, 0.818241f, -2.48707f, -1.04585f, -2.20339f, 0.755535f, 0.713128f, -0.814532f, 0.927998f, -1.47756f, -0.110245f, -0.979104f, -0.512997f, -1.0162f, 0.110969f, -0.10753f, 0.0973829f, 0.259481f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.0948694f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, -1.90127f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 1.22364f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, -0.224913f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.0838967f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.012994f, 1.77789f, -0.731412f, -1.53658f, -0.667911f, 0.137826f, 0.495396f, 0.751369f, 1.4866f, 0.517938f, -1.42422f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_SAME_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_SAME_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-1.2785300016403198f, 0.989691972732544f, -0.5368959903717041f, 1.3370800018310547f, 1.4385900497436523f, -0.662792980670929f, -0.5243960022926331f, 0.12666000425815582f, -0.18684199452400208f, -0.3850089907646179f, -1.2305899858474731f, 0.3595220148563385f, -0.28294500708580017f, 0.08478370308876038f, 0.1492460072040558f, 0.9523109793663025f, 0.4847959876060486f, -1.4130100011825562f, 1.1774100065231323f, -1.215649962425232f, 0.5121430158615112f, -0.05330929905176163f, 1.1632800102233887f, -0.1264760047197342f, 1.2247999906539917f, -0.619940996170044f, 1.5931400060653687f, 1.1371899843215942f, 1.186079978942871f, 0.5482969880104065f, -0.9933540225028992f, 0.318668007850647f, -0.6291260123252869f, -1.9609999656677246f, -1.0240299701690674f, 3.9935500621795654f, -0.5624380111694336f, -0.9286869764328003f, -0.5660529732704163f, 1.1361299753189087f, -0.41466599702835083f, -1.7265700101852417f, -0.4786689877510071f, 1.4856499433517456f, -0.31711798906326294f, -1.383430004119873f, 1.8163399696350098f, -0.407478004693985f, -0.48189300298690796f, 0.24784600734710693f, -0.5196729898452759f, -1.0106500387191772f, -0.18430399894714355f, -0.19668500125408173f, 0.08450420200824738f, -0.2011449933052063f, -0.33351799845695496f, 0.11868699640035629f, 0.43203699588775635f, 0.4197880029678345f, 0.5707929730415344f, -1.1023099422454834f, -0.46038201451301575f, 0.6790680289268494f, 1.7498699426651f, 0.29854801297187805f, -0.17938199639320374f, 1.370419979095459f, -1.5479999780654907f, -0.08678969740867615f, 0.2557309865951538f, 0.9156510233879089f, -1.8730800151824951f, 1.7203199863433838f, 1.2369400262832642f, 1.6008399724960327f, 2.372920036315918f, -0.35209301114082336f, 0.9203510284423828f, -0.3581100106239319f, -1.1998900175094604f, 0.10317700356245041f, 2.589359998703003f, -1.0961899757385254f, 0.7713789939880371f, 0.2908639907836914f, 1.8076800107955933f, -0.1852239966392517f, 0.23712700605392456f, 0.4979439973831177f, -0.1273369938135147f, 1.6853699684143066f, -2.527440071105957f, -2.0838000774383545f, 0.1282380074262619f, 0.8937950134277344f, -0.9492290019989014f, 0.0058959899470210075f, 0.39612099528312683f, 0.4342209994792938f, -0.9148709774017334f, 0.834954023361206f, -0.9573410153388977f, -1.2210899591445923f, -0.4528209865093231f, -0.7265740036964417f, 1.7870299816131592f, -0.1799039989709854f, -0.6753870248794556f, 0.69725501537323f, 0.7178009748458862f, -0.3431060016155243f, -0.13556499779224396f, 2.677570104598999f, 0.25444701313972473f, 0.5079110264778137f, -0.0009504369809292257f, -0.7132409811019897f, 2.4849600791931152f, 0.5333319902420044f, -0.5810629725456238f, -1.8670799732208252f, 1.7392300367355347f, 0.31419798731803894f, -1.159980058670044f, -2.1309099197387695f, 0.13833799958229065f, -0.19028200209140778f, -0.8767179846763611f, 0.5221030116081238f, -0.9669139981269836f, -0.49549400806427f, -0.43002599477767944f, -0.9006580114364624f, -0.7790809869766235f, -0.44476500153541565f, 1.2113499641418457f, -1.5603599548339844f, 0.0020001500379294157f, 0.434006005525589f, -1.2865300178527832f, -2.396589994430542f, -1.343459963798523f, 0.40344899892807007f, -0.6842979788780212f, -0.3070389926433563f, -2.7623701095581055f, 1.1743199825286865f, 0.39987900853157043f, 0.9556300044059753f, -1.5256400108337402f, -0.11477900296449661f, -0.21329699456691742f, -1.6868799924850464f, 0.11141899973154068f, -0.4796440005302429f, 1.3413599729537964f, 2.6458499431610107f, 1.2609200477600098f, -0.06005590036511421f, 2.258280038833618f, -0.18785400688648224f, 1.3703999519348145f, 0.2744640111923218f, 0.5391290187835693f, 0.8441780209541321f, 0.9333360195159912f, 1.464900016784668f, -0.7060059905052185f, -0.09793619811534882f, -0.6392880082130432f, 1.0100300312042236f, 1.909690022468567f, -0.2922550141811371f, 0.8624789714813232f, -0.03532319888472557f, -2.9594099521636963f, -2.239989995956421f, 0.4152829945087433f, 0.9799259901046753f, -0.7343479990959167f, 0.5949720144271851f, 0.3850190043449402f, -0.9048720002174377f, -2.070650100708008f, -0.6019870042800903f, 0.5900070071220398f, 2.1281700134277344f, -0.3485870063304901f, 0.1344670057296753f, 3.4549999237060547f, -0.0307461004704237f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7857400178909302f, 0.014100300148129463f, 1.2410399913787842f, 0.9005590081214905f, 1.0089000463485718f, 0.2705579996109009f, -0.37148401141166687f, 0.15576599538326263f, 1.0670900344848633f, -0.7972450256347656f, 2.2660999298095703f, -1.3368200063705444f, 0.9434570074081421f, -0.34587499499320984f, 0.10474500060081482f, -0.45374199748039246f, -0.4894149899482727f, -2.102370023727417f, -0.7467849850654602f, 0.774278998374939f, 1.0375299453735352f, -1.003730058670044f, -0.06626229733228683f, 0.2808369994163513f, 3.047339916229248f, -0.980368971824646f, 1.649090051651001f, -0.5943750143051147f, -0.613448977470398f, 1.129770040512085f, 0.11876499652862549f, 1.3518199920654297f, 1.3804899454116821f, 1.037950038909912f, -1.6371999979019165f, 1.8485000133514404f, -0.437375009059906f, 0.08118639886379242f, 2.018120050430298f, -0.8402760028839111f, 0.609824001789093f, 2.023859977722168f, -0.5089920163154602f, 1.8527699708938599f, -1.132789969444275f, -0.5887519717216492f, 0.8847579956054688f, 0.7716289758682251f, -2.1926400661468506f, -1.6319700479507446f, -2.378619909286499f, 0.9498999714851379f, 1.588879942893982f, 0.5452510118484497f, -0.7903980016708374f, 0.4309230148792267f, 0.9795460104942322f, -1.6679799556732178f, 1.0029900074005127f, -2.312809944152832f, 0.5496289730072021f, 0.28399598598480225f, 1.1210500001907349f, -0.6126610040664673f, 0.07004660367965698f, -0.6210070252418518f, 0.5803980231285095f, -0.6832290291786194f, -0.07524880021810532f, 0.7568190097808838f, 0.1490049958229065f, -0.476502001285553f, -1.1653399467468262f, 0.8027359843254089f, -1.4554799795150757f, 1.152250051498413f, 0.8852109909057617f, -0.11470799893140793f, 0.828324019908905f, -0.2888289988040924f, 1.7488900423049927f, 0.6130220293998718f, 3.638159990310669f, -0.6139169931411743f, -0.17792299389839172f, 1.8385900259017944f, 0.21880599856376648f, 0.9029009938240051f, -1.5730199813842773f, -3.8530499935150146f, 2.3357300758361816f, 0.325747013092041f, 2.8507299423217773f, 1.13510000705719f, -0.19954699277877808f, -1.8545700311660767f, 2.383419990539551f, -0.3215709924697876f, 1.093019962310791f, -0.1589280068874359f, -1.5793999433517456f, 0.9120540022850037f, 2.6290700435638428f, -0.45585399866104126f, -0.11105000227689743f, 1.4687000513076782f, -3.026020050048828f, 2.9281699657440186f, -0.35563600063323975f, 2.0575900077819824f, -0.07121119648218155f, 0.9496340155601501f, 0.11452099680900574f, -0.267441987991333f, 2.5764598846435547f, -0.23144100606441498f, 0.8959310054779053f, 0.7469030022621155f, -1.1054999828338623f, 0.8182410001754761f, -2.487070083618164f, -1.0458500385284424f, -2.203389883041382f, 0.7555350065231323f, 0.7131279706954956f, -0.8145319819450378f, 0.9279980063438416f, -1.477560043334961f, -0.1102449968457222f, -0.9791039824485779f, -0.5129969716072083f, -1.0161999464035034f, 0.11096899956464767f, -0.10752999782562256f, 0.09738290309906006f, 0.2594810128211975f, 1.5269399881362915f, -0.9971089959144592f, 2.155060052871704f, -3.0932400226593018f, 0.9879440069198608f, 1.7107000350952148f, -2.4996399879455566f, -0.09486939758062363f, -0.37825700640678406f, -2.5225000381469727f, -1.9286999702453613f, -0.6500110030174255f, 0.8918150067329407f, -1.5923899412155151f, -1.0880000591278076f, -1.9012700319290161f, 1.1088099479675293f, -0.9558590054512024f, 0.31143999099731445f, -0.9525660276412964f, 1.9469200372695923f, -0.4496519863605499f, 2.872999906539917f, 1.223639965057373f, 0.8823210000991821f, -2.602020025253296f, 0.4617680013179779f, 4.288710117340088f, 1.5986900329589844f, 0.4521760046482086f, 1.5030399560928345f, -0.22491300106048584f, 3.3563499450683594f, -0.7083650231361389f, -0.7572500109672546f, -0.17299699783325195f, -0.3376689851284027f, 1.0650999546051025f, 3.81318998336792f, 0.0838966965675354f, 0.5435559749603271f, 1.1570099592208862f, 0.5312079787254333f, 1.5146100521087646f, -0.7530840039253235f, 0.012993999756872654f, 1.7778899669647217f, -0.7314119935035706f, -1.536579966545105f, -0.6679109930992126f, 0.13782599568367004f, 0.4953959882259369f, 0.7513689994812012f, 1.4866000413894653f, 0.5179380178451538f, -1.4242199659347534f}}},
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
return examples_3_H3_W2_SAME_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nhwc() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845044f, 2.48496f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746904f, 0.0129939f, -0.790398f, -1.1055f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_nhwc;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845044f, 2.48496f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746904f, 0.0129939f, -0.790398f, -1.1055f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nhwc_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.18684199452400208f, -1.8730800151824951f, 1.2113499641418457f, -0.3850089907646179f, 1.7203199863433838f, -1.5603599548339844f, -1.2305899858474731f, 1.2369400262832642f, 0.0020001500379294157f, 0.3595220148563385f, 1.6008399724960327f, 0.434006005525589f, -0.28294500708580017f, 2.372920036315918f, -1.2865300178527832f, 0.08478370308876038f, -0.35209301114082336f, -2.396589994430542f, 0.1492460072040558f, 0.9203510284423828f, -1.343459963798523f, 0.4847959876060486f, -1.1998900175094604f, -0.6842979788780212f, -1.4130100011825562f, 0.10317700356245041f, -0.3070389926433563f, 1.1774100065231323f, 2.589359998703003f, -2.7623701095581055f, -1.215649962425232f, -1.0961899757385254f, 1.1743199825286865f, 0.5121430158615112f, 0.7713789939880371f, 0.39987900853157043f, -0.05330929905176163f, 0.2908639907836914f, 0.9556300044059753f, 1.1632800102233887f, 1.8076800107955933f, -1.5256400108337402f, 1.2247999906539917f, 0.23712700605392456f, -0.21329699456691742f, -0.619940996170044f, 0.4979439973831177f, -1.6868799924850464f, 1.5931400060653687f, -0.1273369938135147f, 0.11141899973154068f, 1.1371899843215942f, 1.6853699684143066f, -0.4796440005302429f, 1.186079978942871f, -2.527440071105957f, 1.3413599729537964f, 0.5482969880104065f, -2.0838000774383545f, 2.6458499431610107f, -0.9933540225028992f, 0.1282380074262619f, 1.2609200477600098f, -0.6291260123252869f, -0.9492290019989014f, 2.258280038833618f, -1.9609999656677246f, 0.0058959899470210075f, -0.18785400688648224f, -1.0240299701690674f, 0.39612099528312683f, 1.3703999519348145f, 3.9935500621795654f, 0.4342209994792938f, 0.2744640111923218f, -0.5624380111694336f, -0.9148709774017334f, 0.5391290187835693f, -0.9286869764328003f, 0.834954023361206f, 0.8441780209541321f, -0.5660529732704163f, -0.9573410153388977f, 0.9333360195159912f, -0.41466599702835083f, -0.4528209865093231f, -0.7060059905052185f, -1.7265700101852417f, -0.7265740036964417f, -0.09793619811534882f, -0.4786689877510071f, 1.7870299816131592f, -0.6392880082130432f, 1.4856499433517456f, -0.1799039989709854f, 1.0100300312042236f, -0.31711798906326294f, -0.6753870248794556f, 1.909690022468567f, -1.383430004119873f, 0.69725501537323f, -0.2922550141811371f, 1.8163399696350098f, 0.7178009748458862f, 0.8624789714813232f, -0.48189300298690796f, -0.13556499779224396f, -2.9594099521636963f, 0.24784600734710693f, 2.677570104598999f, -2.239989995956421f, -0.5196729898452759f, 0.25444701313972473f, 0.4152829945087433f, -1.0106500387191772f, 0.5079110264778137f, 0.9799259901046753f, -0.18430399894714355f, -0.0009504369809292257f, -0.7343479990959167f, -0.19668500125408173f, -0.7132409811019897f, 0.5949720144271851f, 0.08450440317392349f, 2.4849600791931152f, 0.3850190043449402f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0670900344848633f, -1.1653399467468262f, 1.5269399881362915f, -0.7972450256347656f, 0.8027359843254089f, -0.9971089959144592f, 2.2660999298095703f, -1.4554799795150757f, 2.155060052871704f, -1.3368200063705444f, 1.152250051498413f, -3.0932400226593018f, 0.9434570074081421f, 0.8852109909057617f, 0.9879440069198608f, -0.34587499499320984f, -0.11470799893140793f, 1.7107000350952148f, 0.10474500060081482f, 0.828324019908905f, -2.4996399879455566f, -0.4894149899482727f, 1.7488900423049927f, -0.37825700640678406f, -2.102370023727417f, 0.6130220293998718f, -2.5225000381469727f, -0.7467849850654602f, 3.638159990310669f, -1.9286999702453613f, 0.774278998374939f, -0.6139169931411743f, -0.6500110030174255f, 1.0375299453735352f, -0.17792299389839172f, 0.8918150067329407f, -1.003730058670044f, 1.8385900259017944f, -1.5923899412155151f, -0.06626229733228683f, 0.21880599856376648f, -1.0880000591278076f, 3.047339916229248f, -1.5730199813842773f, 1.1088099479675293f, -0.980368971824646f, -3.8530499935150146f, -0.9558590054512024f, 1.649090051651001f, 2.3357300758361816f, 0.31143999099731445f, -0.5943750143051147f, 0.325747013092041f, -0.9525660276412964f, -0.613448977470398f, 2.8507299423217773f, 1.9469200372695923f, 1.129770040512085f, 1.13510000705719f, -0.4496519863605499f, 0.11876499652862549f, -0.19954699277877808f, 2.872999906539917f, 1.3804899454116821f, 2.383419990539551f, 0.8823210000991821f, 1.037950038909912f, -0.3215709924697876f, -2.602020025253296f, -1.6371999979019165f, 1.093019962310791f, 0.4617680013179779f, 1.8485000133514404f, -0.1589280068874359f, 4.288710117340088f, -0.437375009059906f, -1.5793999433517456f, 1.5986900329589844f, 0.08118639886379242f, 0.9120540022850037f, 0.4521760046482086f, 2.018120050430298f, 2.6290700435638428f, 1.5030399560928345f, 0.609824001789093f, -0.11105000227689743f, 3.3563499450683594f, 2.023859977722168f, 1.4687000513076782f, -0.7083650231361389f, -0.5089920163154602f, -3.026020050048828f, -0.7572500109672546f, 1.8527699708938599f, 2.9281699657440186f, -0.17299699783325195f, -1.132789969444275f, -0.35563600063323975f, -0.3376689851284027f, -0.5887519717216492f, 2.0575900077819824f, 1.0650999546051025f, 0.8847579956054688f, -0.07121119648218155f, 3.81318998336792f, -2.1926400661468506f, 0.11452099680900574f, 0.5435559749603271f, -1.6319700479507446f, -0.267441987991333f, 1.1570099592208862f, -2.378619909286499f, 2.5764598846435547f, 0.5312079787254333f, 0.9498999714851379f, -0.23144100606441498f, 1.5146100521087646f, 1.588879942893982f, 0.8959310054779053f, -0.7530840039253235f, 0.5452510118484497f, 0.7469040155410767f, 0.01299390010535717f, -0.7903980016708374f, -1.1054999828338623f, 1.7778899669647217f}}},
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
return examples_3_H3_W2_VALID_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nchw() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845044f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746904f, -1.1055f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.0129939f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_nchw;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845044f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746904f, -1.1055f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.0129939f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_nchw_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.18684199452400208f, -0.3850089907646179f, -1.2305899858474731f, 0.3595220148563385f, -0.28294500708580017f, 0.08478370308876038f, 0.1492460072040558f, 0.4847959876060486f, -1.4130100011825562f, 1.1774100065231323f, -1.215649962425232f, 0.5121430158615112f, -0.05330929905176163f, 1.1632800102233887f, 1.2247999906539917f, -0.619940996170044f, 1.5931400060653687f, 1.1371899843215942f, 1.186079978942871f, 0.5482969880104065f, -0.9933540225028992f, -0.6291260123252869f, -1.9609999656677246f, -1.0240299701690674f, 3.9935500621795654f, -0.5624380111694336f, -0.9286869764328003f, -0.5660529732704163f, -0.41466599702835083f, -1.7265700101852417f, -0.4786689877510071f, 1.4856499433517456f, -0.31711798906326294f, -1.383430004119873f, 1.8163399696350098f, -0.48189300298690796f, 0.24784600734710693f, -0.5196729898452759f, -1.0106500387191772f, -0.18430399894714355f, -0.19668500125408173f, 0.08450440317392349f, -1.8730800151824951f, 1.7203199863433838f, 1.2369400262832642f, 1.6008399724960327f, 2.372920036315918f, -0.35209301114082336f, 0.9203510284423828f, -1.1998900175094604f, 0.10317700356245041f, 2.589359998703003f, -1.0961899757385254f, 0.7713789939880371f, 0.2908639907836914f, 1.8076800107955933f, 0.23712700605392456f, 0.4979439973831177f, -0.1273369938135147f, 1.6853699684143066f, -2.527440071105957f, -2.0838000774383545f, 0.1282380074262619f, -0.9492290019989014f, 0.0058959899470210075f, 0.39612099528312683f, 0.4342209994792938f, -0.9148709774017334f, 0.834954023361206f, -0.9573410153388977f, -0.4528209865093231f, -0.7265740036964417f, 1.7870299816131592f, -0.1799039989709854f, -0.6753870248794556f, 0.69725501537323f, 0.7178009748458862f, -0.13556499779224396f, 2.677570104598999f, 0.25444701313972473f, 0.5079110264778137f, -0.0009504369809292257f, -0.7132409811019897f, 2.4849600791931152f, 1.2113499641418457f, -1.5603599548339844f, 0.0020001500379294157f, 0.434006005525589f, -1.2865300178527832f, -2.396589994430542f, -1.343459963798523f, -0.6842979788780212f, -0.3070389926433563f, -2.7623701095581055f, 1.1743199825286865f, 0.39987900853157043f, 0.9556300044059753f, -1.5256400108337402f, -0.21329699456691742f, -1.6868799924850464f, 0.11141899973154068f, -0.4796440005302429f, 1.3413599729537964f, 2.6458499431610107f, 1.2609200477600098f, 2.258280038833618f, -0.18785400688648224f, 1.3703999519348145f, 0.2744640111923218f, 0.5391290187835693f, 0.8441780209541321f, 0.9333360195159912f, -0.7060059905052185f, -0.09793619811534882f, -0.6392880082130432f, 1.0100300312042236f, 1.909690022468567f, -0.2922550141811371f, 0.8624789714813232f, -2.9594099521636963f, -2.239989995956421f, 0.4152829945087433f, 0.9799259901046753f, -0.7343479990959167f, 0.5949720144271851f, 0.3850190043449402f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0670900344848633f, -0.7972450256347656f, 2.2660999298095703f, -1.3368200063705444f, 0.9434570074081421f, -0.34587499499320984f, 0.10474500060081482f, -0.4894149899482727f, -2.102370023727417f, -0.7467849850654602f, 0.774278998374939f, 1.0375299453735352f, -1.003730058670044f, -0.06626229733228683f, 3.047339916229248f, -0.980368971824646f, 1.649090051651001f, -0.5943750143051147f, -0.613448977470398f, 1.129770040512085f, 0.11876499652862549f, 1.3804899454116821f, 1.037950038909912f, -1.6371999979019165f, 1.8485000133514404f, -0.437375009059906f, 0.08118639886379242f, 2.018120050430298f, 0.609824001789093f, 2.023859977722168f, -0.5089920163154602f, 1.8527699708938599f, -1.132789969444275f, -0.5887519717216492f, 0.8847579956054688f, -2.1926400661468506f, -1.6319700479507446f, -2.378619909286499f, 0.9498999714851379f, 1.588879942893982f, 0.5452510118484497f, -0.7903980016708374f, -1.1653399467468262f, 0.8027359843254089f, -1.4554799795150757f, 1.152250051498413f, 0.8852109909057617f, -0.11470799893140793f, 0.828324019908905f, 1.7488900423049927f, 0.6130220293998718f, 3.638159990310669f, -0.6139169931411743f, -0.17792299389839172f, 1.8385900259017944f, 0.21880599856376648f, -1.5730199813842773f, -3.8530499935150146f, 2.3357300758361816f, 0.325747013092041f, 2.8507299423217773f, 1.13510000705719f, -0.19954699277877808f, 2.383419990539551f, -0.3215709924697876f, 1.093019962310791f, -0.1589280068874359f, -1.5793999433517456f, 0.9120540022850037f, 2.6290700435638428f, -0.11105000227689743f, 1.4687000513076782f, -3.026020050048828f, 2.9281699657440186f, -0.35563600063323975f, 2.0575900077819824f, -0.07121119648218155f, 0.11452099680900574f, -0.267441987991333f, 2.5764598846435547f, -0.23144100606441498f, 0.8959310054779053f, 0.7469040155410767f, -1.1054999828338623f, 1.5269399881362915f, -0.9971089959144592f, 2.155060052871704f, -3.0932400226593018f, 0.9879440069198608f, 1.7107000350952148f, -2.4996399879455566f, -0.37825700640678406f, -2.5225000381469727f, -1.9286999702453613f, -0.6500110030174255f, 0.8918150067329407f, -1.5923899412155151f, -1.0880000591278076f, 1.1088099479675293f, -0.9558590054512024f, 0.31143999099731445f, -0.9525660276412964f, 1.9469200372695923f, -0.4496519863605499f, 2.872999906539917f, 0.8823210000991821f, -2.602020025253296f, 0.4617680013179779f, 4.288710117340088f, 1.5986900329589844f, 0.4521760046482086f, 1.5030399560928345f, 3.3563499450683594f, -0.7083650231361389f, -0.7572500109672546f, -0.17299699783325195f, -0.3376689851284027f, 1.0650999546051025f, 3.81318998336792f, 0.5435559749603271f, 1.1570099592208862f, 0.5312079787254333f, 1.5146100521087646f, -0.7530840039253235f, 0.01299390010535717f, 1.7778899669647217f}}},
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
return examples_3_H3_W2_VALID_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845044f, 2.48496f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746904f, 0.0129939f, -0.790398f, -1.1055f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.644628f, -0.918393f, 0.153672f, 0.868562f, -0.358177f, -0.134931f, -0.247565f, 0.22174f, -0.259157f, -0.284296f, -0.538065f, 0.765559f, 0.41986f, -0.556241f, 0.658494f, 0.214355f, -0.850169f, -0.252893f, -0.478935f, 0.530526f, -0.0700663f, -0.988729f, -0.303061f, 0.150845f, 0.829915f, 0.476349f, 0.406537f, -0.355343f, 0.757145f, -0.356362f, 0.800482f, -0.713861f, 0.210483f, -0.634303f, 0.718236f, -0.752038f, 0.457547f, -0.550769f, -0.551178f, 0.446766f, -0.227462f, 0.216348f, -0.852806f, -0.351486f, 0.55906f, -0.668493f, -0.303493f, -0.363763f, -0.162837f, 0.0701012f, 0.756097f, -0.142269f, 0.329724f, -0.656317f, -0.998086f, -0.652949f, -0.40316f, -0.893682f, 0.432744f, 0.612362f, -0.869588f, -0.71327f, -0.398092f, -0.0423559f, 0.436576f, -0.925272f, 0.176549f, 0.822904f, 0.096833f, -0.296802f, -0.427195f, 0.031654f, -0.254479f, 0.244905f, 0.0948254f, 0.643769f, -0.90391f, 0.352665f, -0.901179f, 0.266159f, -0.968068f, -0.615401f, -0.388975f, 0.939052f, -0.116289f, 0.107523f, -0.0582711f, 0.435172f, 0.334675f, 0.459711f, 0.717436f, 0.496627f, -0.680175f, -0.415066f, 0.339848f, 0.506004f, -0.337808f, -0.107218f, -0.172496f, 0.870638f, 0.931872f, -0.953884f, 0.903042f, 0.760078f, 0.209727f, -0.285384f, -0.45514f, 0.113194f, 0.0756611f, 0.0924435f, -0.472863f, 0.960609f, -0.160385f, -0.839445f, 0.457097f, 0.163348f, 0.344867f, -0.131619f, 0.688715f, -0.540827f, 0.571259f, -0.95587f, 0.506164f, -0.155839f, 0.0789621f, 0.756772f, -0.662069f, 0.242908f, 0.460821f, 0.177872f, -0.289839f, -0.640603f, 0.702598f, -0.506406f, -0.568262f, -0.0713716f, 0.413792f, 0.159673f, -0.305208f, 0.133816f, -0.160254f, 0.787323f, -0.753244f, 0.600721f, 0.263186f, -0.162387f, 0.477962f, -0.702951f, -0.731036f, -0.939481f, -0.524519f, 0.934072f, -0.511637f, -0.503499f, 0.106236f, -0.323684f, 0.534444f, -0.843745f, 0.364171f, 0.0370358f, -0.168801f, -0.404559f, -0.814178f, 0.91745f, -0.334276f, 0.66925f, -0.801201f, 0.156511f, -0.427949f, 0.379153f, 0.818597f, -0.649902f, 0.427087f, -0.586015f, -0.559789f, -0.833923f, 0.0892409f, -0.621251f, 0.213826f, 0.465509f, 0.4704f, 0.380261f, 0.413067f, 0.180822f, 0.172866f, 0.59614f, 0.825575f, 0.662916f, -0.704381f, -0.297631f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -1.87308f, 1.21135f, -0.385009f, 1.72032f, -1.56036f, -1.23059f, 1.23694f, 0.00200015f, 0.359522f, 1.60084f, 0.434006f, -0.282945f, 2.37292f, -1.28653f, 0.0847837f, -0.352093f, -2.39659f, 0.149246f, 0.920351f, -1.34346f, 0.484796f, -1.19989f, -0.684298f, -1.41301f, 0.103177f, -0.307039f, 1.17741f, 2.58936f, -2.76237f, -1.21565f, -1.09619f, 1.17432f, 0.512143f, 0.771379f, 0.399879f, -0.0533093f, 0.290864f, 0.95563f, 1.16328f, 1.80768f, -1.52564f, 1.2248f, 0.237127f, -0.213297f, -0.619941f, 0.497944f, -1.68688f, 1.59314f, -0.127337f, 0.111419f, 1.13719f, 1.68537f, -0.479644f, 1.18608f, -2.52744f, 1.34136f, 0.548297f, -2.0838f, 2.64585f, -0.993354f, 0.128238f, 1.26092f, -0.629126f, -0.949229f, 2.25828f, -1.961f, 0.00589599f, -0.187854f, -1.02403f, 0.396121f, 1.3704f, 3.99355f, 0.434221f, 0.274464f, -0.562438f, -0.914871f, 0.539129f, -0.928687f, 0.834954f, 0.844178f, -0.566053f, -0.957341f, 0.933336f, -0.414666f, -0.452821f, -0.706006f, -1.72657f, -0.726574f, -0.0979362f, -0.478669f, 1.78703f, -0.639288f, 1.48565f, -0.179904f, 1.01003f, -0.317118f, -0.675387f, 1.90969f, -1.38343f, 0.697255f, -0.292255f, 1.81634f, 0.717801f, 0.862479f, -0.481893f, -0.135565f, -2.95941f, 0.247846f, 2.67757f, -2.23999f, -0.519673f, 0.254447f, 0.415283f, -1.01065f, 0.507911f, 0.979926f, -0.184304f, -0.000950437f, -0.734348f, -0.196685f, -0.713241f, 0.594972f, 0.0845044f, 2.48496f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, -0.00387601f, -0.552251f, 0.166084f, -0.28482f, -0.152143f, -0.719885f, -0.869386f, -0.745598f, 0.823947f, 0.473183f, -0.331337f, 0.187631f, 0.0426571f, -0.826897f, -0.755085f, -0.472453f, -0.0233656f, 0.0483436f, 0.933418f, -0.961974f, 0.0125783f, 0.219742f, 0.342604f, -0.15166f, 0.0934905f, 0.783221f, 0.129664f, 0.838844f, -0.271388f, 0.924519f, 0.342843f, 0.274418f, 0.350817f, 0.841638f, -0.543993f, -0.00283395f, -0.128467f, -0.682943f, -0.319117f, 0.84634f, 0.283003f, 0.32865f, 0.0293755f, -0.0335696f, 0.591266f, -0.0743476f, -0.741271f, 0.462056f, -0.583625f, -0.590183f, 0.6234f, 0.535269f, -0.670818f, -0.955642f, -0.770173f, 0.479986f, 0.664377f, 0.399445f, -0.968874f, -0.276263f, -0.901951f, 0.544104f, -0.958981f, 0.482658f, -0.807284f, 0.305369f, -0.947818f, 0.827498f, -0.382887f, -0.805741f, -0.796678f, -0.299804f, -0.229828f, 0.818783f, -0.103055f, -0.45568f, -0.227827f, 0.543743f, -0.96073f, 0.946747f, -0.857182f, -0.96426f, -0.292411f, -0.715614f, 0.765278f, -0.475043f, -0.590142f, -0.238507f, 0.673002f, -0.473357f, -0.319626f, 0.936014f, 0.486607f, 0.580844f, 0.425352f, -0.800994f, 0.290763f, -0.494953f, -0.441162f, 0.718677f, -0.828427f, 0.96965f, 7.53637e-05f, -0.699973f, -0.526886f, -0.352682f, 0.799466f, 0.332789f, 0.723389f, 0.407659f, -0.934084f, -0.284705f, 0.961484f, -0.700395f, -0.985808f, -0.595342f, -0.691721f, 0.49448f, -0.0842649f, 0.0390966f, 0.298938f, -0.128094f, -0.97158f, 0.86393f, 0.270606f, -0.468986f, -0.256605f, 0.47215f, -0.273117f, -0.590343f, -0.826529f, -0.725381f, -0.194821f, -0.259661f, -0.0949207f, -0.180302f, 0.0446834f, -0.222133f, -0.40393f, 0.295772f, -0.92949f, 0.580079f, -0.169856f, 0.330311f, 0.0173551f, -0.635823f, 0.475942f, 0.907175f, 0.242777f, -0.512208f, 0.362463f, 0.0496289f, 0.65171f, 0.990057f, 0.690733f, -0.469013f, -0.101311f, -0.68372f, -0.157841f, -0.677711f, -0.708224f, -0.659437f, -0.407607f, 0.677033f, 0.89032f, 0.228307f, -0.749514f, 0.772958f, 0.054701f, 0.551705f, 0.917052f, -0.895022f, -0.702397f, 0.484142f, 0.108648f, 0.833347f, 0.478872f, -0.984112f, 0.387176f, -0.73299f, 0.7526f, 0.443312f, -0.0987856f, 0.125415f, 0.10876f, -0.498108f, 0.43209f, 0.344609f, 0.928941f, -0.130732f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -1.16534f, 1.52694f, -0.797245f, 0.802736f, -0.997109f, 2.2661f, -1.45548f, 2.15506f, -1.33682f, 1.15225f, -3.09324f, 0.943457f, 0.885211f, 0.987944f, -0.345875f, -0.114708f, 1.7107f, 0.104745f, 0.828324f, -2.49964f, -0.489415f, 1.74889f, -0.378257f, -2.10237f, 0.613022f, -2.5225f, -0.746785f, 3.63816f, -1.9287f, 0.774279f, -0.613917f, -0.650011f, 1.03753f, -0.177923f, 0.891815f, -1.00373f, 1.83859f, -1.59239f, -0.0662623f, 0.218806f, -1.088f, 3.04734f, -1.57302f, 1.10881f, -0.980369f, -3.85305f, -0.955859f, 1.64909f, 2.33573f, 0.31144f, -0.594375f, 0.325747f, -0.952566f, -0.613449f, 2.85073f, 1.94692f, 1.12977f, 1.1351f, -0.449652f, 0.118765f, -0.199547f, 2.873f, 1.38049f, 2.38342f, 0.882321f, 1.03795f, -0.321571f, -2.60202f, -1.6372f, 1.09302f, 0.461768f, 1.8485f, -0.158928f, 4.28871f, -0.437375f, -1.5794f, 1.59869f, 0.0811864f, 0.912054f, 0.452176f, 2.01812f, 2.62907f, 1.50304f, 0.609824f, -0.11105f, 3.35635f, 2.02386f, 1.4687f, -0.708365f, -0.508992f, -3.02602f, -0.75725f, 1.85277f, 2.92817f, -0.172997f, -1.13279f, -0.355636f, -0.337669f, -0.588752f, 2.05759f, 1.0651f, 0.884758f, -0.0712112f, 3.81319f, -2.19264f, 0.114521f, 0.543556f, -1.63197f, -0.267442f, 1.15701f, -2.37862f, 2.57646f, 0.531208f, 0.9499f, -0.231441f, 1.51461f, 1.58888f, 0.895931f, -0.753084f, 0.545251f, 0.746904f, 0.0129939f, -0.790398f, -1.1055f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.6446279883384705f, -0.9183930158615112f, 0.153671994805336f, 0.8685619831085205f, -0.3581770062446594f, -0.13493099808692932f, -0.24756500124931335f, 0.2217400074005127f, -0.2591570019721985f, -0.2842960059642792f, -0.5380650162696838f, 0.7655590176582336f, 0.41986000537872314f, -0.556240975856781f, 0.6584939956665039f, 0.214355006814003f, -0.8501690030097961f, -0.25289300084114075f, -0.47893500328063965f, 0.5305259823799133f, -0.07006630301475525f, -0.9887290000915527f, -0.30306100845336914f, 0.150845006108284f, 0.8299149870872498f, 0.47634899616241455f, 0.4065369963645935f, -0.3553430140018463f, 0.7571449875831604f, -0.356361985206604f, 0.8004819750785828f, -0.7138609886169434f, 0.21048299968242645f, -0.6343029737472534f, 0.7182360291481018f, -0.7520380020141602f, 0.45754700899124146f, -0.5507689714431763f, -0.551177978515625f, 0.4467659890651703f, -0.22746199369430542f, 0.21634800732135773f, -0.8528059720993042f, -0.3514859974384308f, 0.5590599775314331f, -0.6684929728507996f, -0.3034929931163788f, -0.3637630045413971f, -0.16283699870109558f, 0.07010120153427124f, 0.7560970187187195f, -0.14226900041103363f, 0.3297240138053894f, -0.6563169956207275f, -0.9980859756469727f, -0.6529489755630493f, -0.4031600058078766f, -0.8936820030212402f, 0.43274399638175964f, 0.6123620271682739f, -0.8695880174636841f, -0.7132700085639954f, -0.39809200167655945f, -0.04235589876770973f, 0.4365760087966919f, -0.9252719879150391f, 0.1765490025281906f, 0.8229039907455444f, 0.0968329980969429f, -0.29680201411247253f, -0.4271950125694275f, 0.0316540002822876f, -0.25447899103164673f, 0.2449049949645996f, 0.09482540190219879f, 0.6437690258026123f, -0.9039099812507629f, 0.3526650071144104f, -0.9011790156364441f, 0.2661589980125427f, -0.96806800365448f, -0.615401029586792f, -0.38897499442100525f, 0.9390519857406616f, -0.11628899723291397f, 0.10752300173044205f, -0.058271098881959915f, 0.4351719915866852f, 0.33467501401901245f, 0.4597109854221344f, 0.7174360156059265f, 0.49662700295448303f, -0.6801750063896179f, -0.4150660037994385f, 0.33984801173210144f, 0.5060039758682251f, -0.3378080129623413f, -0.10721799731254578f, -0.1724960058927536f, 0.8706380128860474f, 0.9318720102310181f, -0.9538840055465698f, 0.9030420184135437f, 0.7600780129432678f, 0.20972700417041779f, -0.28538399934768677f, -0.45513999462127686f, 0.11319400370121002f, 0.07566110044717789f, 0.09244350343942642f, -0.47286298871040344f, 0.9606090188026428f, -0.16038499772548676f, -0.8394449949264526f, 0.45709699392318726f, 0.1633480042219162f, 0.3448669910430908f, -0.13161900639533997f, 0.6887149810791016f, -0.5408269762992859f, 0.5712590217590332f, -0.9558699727058411f, 0.506164014339447f, -0.1558389961719513f, 0.07896210253238678f, 0.756771981716156f, -0.6620690226554871f, 0.24290800094604492f, 0.4608210027217865f, 0.17787200212478638f, -0.2898389995098114f, -0.6406030058860779f, 0.702597975730896f, -0.5064060091972351f, -0.568261981010437f, -0.07137160003185272f, 0.4137920141220093f, 0.15967300534248352f, -0.3052079975605011f, 0.13381600379943848f, -0.16025400161743164f, 0.787322998046875f, -0.7532439827919006f, 0.600721001625061f, 0.2631860077381134f, -0.16238699853420258f, 0.4779619872570038f, -0.7029510140419006f, -0.7310360074043274f, -0.9394810199737549f, -0.5245190262794495f, 0.9340720176696777f, -0.5116369724273682f, -0.5034989714622498f, 0.10623600333929062f, -0.3236840069293976f, 0.5344439744949341f, -0.8437449932098389f, 0.36417099833488464f, 0.03703580051660538f, -0.16880099475383759f, -0.4045589864253998f, -0.8141779899597168f, 0.9174500107765198f, -0.3342759907245636f, 0.6692500114440918f, -0.8012009859085083f, 0.15651099383831024f, -0.4279490113258362f, 0.3791530132293701f, 0.8185970187187195f, -0.6499019861221313f, 0.4270870089530945f, -0.586014986038208f, -0.5597890019416809f, -0.8339229822158813f, 0.0892409011721611f, -0.6212509870529175f, 0.2138260006904602f, 0.46550899744033813f, 0.47040000557899475f, 0.38026100397109985f, 0.4130670130252838f, 0.1808219999074936f, 0.17286600172519684f, 0.5961400270462036f, 0.8255749940872192f, 0.6629160046577454f, -0.704380989074707f, -0.29763099551200867f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.18684199452400208f, -1.8730800151824951f, 1.2113499641418457f, -0.3850089907646179f, 1.7203199863433838f, -1.5603599548339844f, -1.2305899858474731f, 1.2369400262832642f, 0.0020001500379294157f, 0.3595220148563385f, 1.6008399724960327f, 0.434006005525589f, -0.28294500708580017f, 2.372920036315918f, -1.2865300178527832f, 0.08478370308876038f, -0.35209301114082336f, -2.396589994430542f, 0.1492460072040558f, 0.9203510284423828f, -1.343459963798523f, 0.4847959876060486f, -1.1998900175094604f, -0.6842979788780212f, -1.4130100011825562f, 0.10317700356245041f, -0.3070389926433563f, 1.1774100065231323f, 2.589359998703003f, -2.7623701095581055f, -1.215649962425232f, -1.0961899757385254f, 1.1743199825286865f, 0.5121430158615112f, 0.7713789939880371f, 0.39987900853157043f, -0.05330929905176163f, 0.2908639907836914f, 0.9556300044059753f, 1.1632800102233887f, 1.8076800107955933f, -1.5256400108337402f, 1.2247999906539917f, 0.23712700605392456f, -0.21329699456691742f, -0.619940996170044f, 0.4979439973831177f, -1.6868799924850464f, 1.5931400060653687f, -0.1273369938135147f, 0.11141899973154068f, 1.1371899843215942f, 1.6853699684143066f, -0.4796440005302429f, 1.186079978942871f, -2.527440071105957f, 1.3413599729537964f, 0.5482969880104065f, -2.0838000774383545f, 2.6458499431610107f, -0.9933540225028992f, 0.1282380074262619f, 1.2609200477600098f, -0.6291260123252869f, -0.9492290019989014f, 2.258280038833618f, -1.9609999656677246f, 0.0058959899470210075f, -0.18785400688648224f, -1.0240299701690674f, 0.39612099528312683f, 1.3703999519348145f, 3.9935500621795654f, 0.4342209994792938f, 0.2744640111923218f, -0.5624380111694336f, -0.9148709774017334f, 0.5391290187835693f, -0.9286869764328003f, 0.834954023361206f, 0.8441780209541321f, -0.5660529732704163f, -0.9573410153388977f, 0.9333360195159912f, -0.41466599702835083f, -0.4528209865093231f, -0.7060059905052185f, -1.7265700101852417f, -0.7265740036964417f, -0.09793619811534882f, -0.4786689877510071f, 1.7870299816131592f, -0.6392880082130432f, 1.4856499433517456f, -0.1799039989709854f, 1.0100300312042236f, -0.31711798906326294f, -0.6753870248794556f, 1.909690022468567f, -1.383430004119873f, 0.69725501537323f, -0.2922550141811371f, 1.8163399696350098f, 0.7178009748458862f, 0.8624789714813232f, -0.48189300298690796f, -0.13556499779224396f, -2.9594099521636963f, 0.24784600734710693f, 2.677570104598999f, -2.239989995956421f, -0.5196729898452759f, 0.25444701313972473f, 0.4152829945087433f, -1.0106500387191772f, 0.5079110264778137f, 0.9799259901046753f, -0.18430399894714355f, -0.0009504369809292257f, -0.7343479990959167f, -0.19668500125408173f, -0.7132409811019897f, 0.5949720144271851f, 0.08450440317392349f, 2.4849600791931152f, 0.3850190043449402f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 8, 8, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, -0.0038760099560022354f, -0.5522509813308716f, 0.16608400642871857f, -0.28481999039649963f, -0.15214300155639648f, -0.719884991645813f, -0.8693860173225403f, -0.7455980181694031f, 0.823947012424469f, 0.4731830060482025f, -0.33133700489997864f, 0.18763099610805511f, 0.04265709966421127f, -0.8268970251083374f, -0.7550849914550781f, -0.4724529981613159f, -0.023365600034594536f, 0.04834359884262085f, 0.9334179759025574f, -0.961974024772644f, 0.012578300200402737f, 0.21974200010299683f, 0.34260401129722595f, -0.1516599953174591f, 0.09349049627780914f, 0.7832210063934326f, 0.12966400384902954f, 0.8388440012931824f, -0.2713879942893982f, 0.9245190024375916f, 0.3428429961204529f, 0.2744179964065552f, 0.3508169949054718f, 0.8416380286216736f, -0.5439929962158203f, -0.002833950100466609f, -0.12846699357032776f, -0.6829429864883423f, -0.31911700963974f, 0.846340000629425f, 0.2830030024051666f, 0.32864999771118164f, 0.029375499114394188f, -0.03356960043311119f, 0.5912659764289856f, -0.0743476003408432f, -0.7412710189819336f, 0.4620560109615326f, -0.5836250185966492f, -0.5901830196380615f, 0.6233999729156494f, 0.535269021987915f, -0.6708179712295532f, -0.9556419849395752f, -0.7701730132102966f, 0.4799860119819641f, 0.664376974105835f, 0.3994449973106384f, -0.9688739776611328f, -0.2762629985809326f, -0.901951014995575f, 0.5441039800643921f, -0.9589809775352478f, 0.4826579988002777f, -0.8072839975357056f, 0.30536898970603943f, -0.9478179812431335f, 0.8274980187416077f, -0.38288700580596924f, -0.805741012096405f, -0.7966780066490173f, -0.2998040020465851f, -0.22982800006866455f, 0.8187829852104187f, -0.10305500030517578f, -0.4556800127029419f, -0.22782699763774872f, 0.5437430143356323f, -0.9607300162315369f, 0.9467470049858093f, -0.8571820259094238f, -0.9642599821090698f, -0.2924109995365143f, -0.7156140208244324f, 0.7652779817581177f, -0.47504299879074097f, -0.590142011642456f, -0.2385070025920868f, 0.6730020046234131f, -0.4733569920063019f, -0.31962600350379944f, 0.9360139966011047f, 0.48660698533058167f, 0.580843985080719f, 0.42535200715065f, -0.8009939789772034f, 0.2907629907131195f, -0.4949530065059662f, -0.44116199016571045f, 0.7186769843101501f, -0.8284270167350769f, 0.9696499705314636f, 7.536369957961142e-05f, -0.6999729871749878f, -0.526885986328125f, -0.3526819944381714f, 0.7994660139083862f, 0.33278900384902954f, 0.7233890295028687f, 0.40765899419784546f, -0.9340839982032776f, -0.2847050130367279f, 0.9614840149879456f, -0.7003949880599976f, -0.9858080148696899f, -0.5953419804573059f, -0.6917210221290588f, 0.4944800138473511f, -0.08426489681005478f, 0.03909660130739212f, 0.29893800616264343f, -0.12809400260448456f, -0.9715800285339355f, 0.8639299869537354f, 0.27060601115226746f, -0.46898600459098816f, -0.25660499930381775f, 0.4721499979496002f, -0.2731170058250427f, -0.5903429985046387f, -0.8265290260314941f, -0.7253810167312622f, -0.19482100009918213f, -0.2596609890460968f, -0.09492070227861404f, -0.1803019940853119f, 0.04468340054154396f, -0.22213299572467804f, -0.40393000841140747f, 0.295771986246109f, -0.9294899702072144f, 0.5800790190696716f, -0.169855996966362f, 0.33031100034713745f, 0.017355099320411682f, -0.6358230113983154f, 0.4759419858455658f, 0.9071750044822693f, 0.2427770048379898f, -0.5122079849243164f, 0.36246299743652344f, 0.04962889850139618f, 0.6517099738121033f, 0.9900569915771484f, 0.690733015537262f, -0.4690130054950714f, -0.10131099820137024f, -0.6837199926376343f, -0.15784099698066711f, -0.677711009979248f, -0.7082239985466003f, -0.6594370007514954f, -0.40760698914527893f, 0.677033007144928f, 0.8903200030326843f, 0.22830699384212494f, -0.7495139837265015f, 0.772957980632782f, 0.054701000452041626f, 0.551705002784729f, 0.9170519709587097f, -0.8950219750404358f, -0.7023969888687134f, 0.484142005443573f, 0.10864800214767456f, 0.8333470225334167f, 0.47887200117111206f, -0.984112024307251f, 0.3871760070323944f, -0.732990026473999f, 0.7526000142097473f, 0.44331198930740356f, -0.09878560155630112f, 0.12541499733924866f, 0.10875999927520752f, -0.49810799956321716f, 0.43209001421928406f, 0.34460899233818054f, 0.928941011428833f, -0.130731999874115f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 6, 7, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0670900344848633f, -1.1653399467468262f, 1.5269399881362915f, -0.7972450256347656f, 0.8027359843254089f, -0.9971089959144592f, 2.2660999298095703f, -1.4554799795150757f, 2.155060052871704f, -1.3368200063705444f, 1.152250051498413f, -3.0932400226593018f, 0.9434570074081421f, 0.8852109909057617f, 0.9879440069198608f, -0.34587499499320984f, -0.11470799893140793f, 1.7107000350952148f, 0.10474500060081482f, 0.828324019908905f, -2.4996399879455566f, -0.4894149899482727f, 1.7488900423049927f, -0.37825700640678406f, -2.102370023727417f, 0.6130220293998718f, -2.5225000381469727f, -0.7467849850654602f, 3.638159990310669f, -1.9286999702453613f, 0.774278998374939f, -0.6139169931411743f, -0.6500110030174255f, 1.0375299453735352f, -0.17792299389839172f, 0.8918150067329407f, -1.003730058670044f, 1.8385900259017944f, -1.5923899412155151f, -0.06626229733228683f, 0.21880599856376648f, -1.0880000591278076f, 3.047339916229248f, -1.5730199813842773f, 1.1088099479675293f, -0.980368971824646f, -3.8530499935150146f, -0.9558590054512024f, 1.649090051651001f, 2.3357300758361816f, 0.31143999099731445f, -0.5943750143051147f, 0.325747013092041f, -0.9525660276412964f, -0.613448977470398f, 2.8507299423217773f, 1.9469200372695923f, 1.129770040512085f, 1.13510000705719f, -0.4496519863605499f, 0.11876499652862549f, -0.19954699277877808f, 2.872999906539917f, 1.3804899454116821f, 2.383419990539551f, 0.8823210000991821f, 1.037950038909912f, -0.3215709924697876f, -2.602020025253296f, -1.6371999979019165f, 1.093019962310791f, 0.4617680013179779f, 1.8485000133514404f, -0.1589280068874359f, 4.288710117340088f, -0.437375009059906f, -1.5793999433517456f, 1.5986900329589844f, 0.08118639886379242f, 0.9120540022850037f, 0.4521760046482086f, 2.018120050430298f, 2.6290700435638428f, 1.5030399560928345f, 0.609824001789093f, -0.11105000227689743f, 3.3563499450683594f, 2.023859977722168f, 1.4687000513076782f, -0.7083650231361389f, -0.5089920163154602f, -3.026020050048828f, -0.7572500109672546f, 1.8527699708938599f, 2.9281699657440186f, -0.17299699783325195f, -1.132789969444275f, -0.35563600063323975f, -0.3376689851284027f, -0.5887519717216492f, 2.0575900077819824f, 1.0650999546051025f, 0.8847579956054688f, -0.07121119648218155f, 3.81318998336792f, -2.1926400661468506f, 0.11452099680900574f, 0.5435559749603271f, -1.6319700479507446f, -0.267441987991333f, 1.1570099592208862f, -2.378619909286499f, 2.5764598846435547f, 0.5312079787254333f, 0.9498999714851379f, -0.23144100606441498f, 1.5146100521087646f, 1.588879942893982f, 0.8959310054779053f, -0.7530840039253235f, 0.5452510118484497f, 0.7469040155410767f, 0.01299390010535717f, -0.7903980016708374f, -1.1054999828338623f, 1.7778899669647217f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845044f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746904f, -1.1055f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.0129939f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.869931f, 0.153672f, -0.134931f, -0.259157f, 0.765559f, 0.658494f, -0.252893f, -0.0700663f, 0.150845f, 0.406537f, -0.356362f, 0.210483f, -0.752038f, -0.551178f, 0.216348f, 0.55906f, -0.363763f, 0.756097f, -0.656317f, -0.40316f, 0.612362f, -0.398092f, -0.925272f, 0.096833f, 0.031654f, 0.0948254f, 0.352665f, -0.968068f, 0.939052f, -0.0582711f, 0.459711f, -0.680175f, 0.506004f, -0.172496f, -0.953884f, 0.209727f, 0.113194f, -0.472863f, -0.839445f, 0.344867f, -0.540827f, 0.506164f, 0.756772f, 0.460821f, -0.640603f, -0.568262f, 0.159673f, -0.160254f, 0.600721f, 0.477962f, -0.939481f, -0.511637f, -0.323684f, 0.364171f, -0.404559f, -0.334276f, 0.156511f, 0.818597f, -0.586015f, 0.0892409f, 0.465509f, 0.413067f, 0.59614f, -0.704381f, 0.644628f, 0.868562f, -0.247565f, -0.284296f, 0.41986f, 0.214355f, -0.478935f, -0.988729f, 0.829915f, -0.355343f, 0.800482f, -0.634303f, 0.457547f, 0.446766f, -0.852806f, -0.668493f, -0.162837f, -0.142269f, -0.998086f, -0.893682f, -0.869588f, -0.0423559f, 0.176549f, -0.296802f, -0.254479f, 0.643769f, -0.901179f, -0.615401f, -0.116289f, 0.435172f, 0.717436f, -0.415066f, -0.337808f, 0.870638f, 0.903042f, -0.285384f, 0.0756611f, 0.960609f, 0.457097f, -0.131619f, 0.571259f, -0.155839f, -0.662069f, 0.177872f, 0.702598f, -0.0713716f, -0.305208f, 0.787323f, 0.263186f, -0.702951f, -0.524519f, -0.503499f, 0.534444f, 0.0370358f, -0.814178f, 0.66925f, -0.427949f, -0.649902f, -0.559789f, -0.621251f, 0.4704f, 0.180822f, 0.825575f, -0.297631f, -0.918393f, -0.358177f, 0.22174f, -0.538065f, -0.556241f, -0.850169f, 0.530526f, -0.303061f, 0.476349f, 0.757145f, -0.713861f, 0.718236f, -0.550769f, -0.227462f, -0.351486f, -0.303493f, 0.0701012f, 0.329724f, -0.652949f, 0.432744f, -0.71327f, 0.436576f, 0.822904f, -0.427195f, 0.244905f, -0.90391f, 0.266159f, -0.388975f, 0.107523f, 0.334675f, 0.496627f, 0.339848f, -0.107218f, 0.931872f, 0.760078f, -0.45514f, 0.0924435f, -0.160385f, 0.163348f, 0.688715f, -0.95587f, 0.0789621f, 0.242908f, -0.289839f, -0.506406f, 0.413792f, 0.133816f, -0.753244f, -0.162387f, -0.731036f, 0.934072f, 0.106236f, -0.843745f, -0.168801f, 0.91745f, -0.801201f, 0.379153f, 0.427087f, -0.833923f, 0.213826f, 0.380261f, 0.172866f, 0.662916f, 0.697778f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.186842f, -0.385009f, -1.23059f, 0.359522f, -0.282945f, 0.0847837f, 0.149246f, 0.484796f, -1.41301f, 1.17741f, -1.21565f, 0.512143f, -0.0533093f, 1.16328f, 1.2248f, -0.619941f, 1.59314f, 1.13719f, 1.18608f, 0.548297f, -0.993354f, -0.629126f, -1.961f, -1.02403f, 3.99355f, -0.562438f, -0.928687f, -0.566053f, -0.414666f, -1.72657f, -0.478669f, 1.48565f, -0.317118f, -1.38343f, 1.81634f, -0.481893f, 0.247846f, -0.519673f, -1.01065f, -0.184304f, -0.196685f, 0.0845044f, -1.87308f, 1.72032f, 1.23694f, 1.60084f, 2.37292f, -0.352093f, 0.920351f, -1.19989f, 0.103177f, 2.58936f, -1.09619f, 0.771379f, 0.290864f, 1.80768f, 0.237127f, 0.497944f, -0.127337f, 1.68537f, -2.52744f, -2.0838f, 0.128238f, -0.949229f, 0.00589599f, 0.396121f, 0.434221f, -0.914871f, 0.834954f, -0.957341f, -0.452821f, -0.726574f, 1.78703f, -0.179904f, -0.675387f, 0.697255f, 0.717801f, -0.135565f, 2.67757f, 0.254447f, 0.507911f, -0.000950437f, -0.713241f, 2.48496f, 1.21135f, -1.56036f, 0.00200015f, 0.434006f, -1.28653f, -2.39659f, -1.34346f, -0.684298f, -0.307039f, -2.76237f, 1.17432f, 0.399879f, 0.95563f, -1.52564f, -0.213297f, -1.68688f, 0.111419f, -0.479644f, 1.34136f, 2.64585f, 1.26092f, 2.25828f, -0.187854f, 1.3704f, 0.274464f, 0.539129f, 0.844178f, 0.933336f, -0.706006f, -0.0979362f, -0.639288f, 1.01003f, 1.90969f, -0.292255f, 0.862479f, -2.95941f, -2.23999f, 0.415283f, 0.979926f, -0.734348f, 0.594972f, 0.385019f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-0.295335f, 0.166084f, -0.719885f, 0.823947f, 0.187631f, -0.755085f, 0.0483436f, 0.0125783f, -0.15166f, 0.129664f, 0.924519f, 0.350817f, -0.00283395f, -0.319117f, 0.32865f, 0.591266f, 0.462056f, 0.6234f, -0.955642f, 0.664377f, -0.276263f, -0.958981f, 0.305369f, -0.382887f, -0.299804f, -0.103055f, 0.543743f, -0.857182f, -0.715614f, -0.590142f, -0.473357f, 0.486607f, -0.800994f, -0.441162f, 0.96965f, -0.526886f, 0.332789f, -0.934084f, -0.700395f, -0.691721f, 0.0390966f, -0.97158f, -0.468986f, -0.273117f, -0.725381f, -0.0949207f, -0.222133f, -0.92949f, 0.330311f, 0.475942f, -0.512208f, 0.65171f, -0.469013f, -0.157841f, -0.659437f, 0.89032f, 0.772958f, 0.917052f, 0.484142f, 0.478872f, -0.73299f, -0.0987856f, -0.498108f, 0.928941f, -0.00387601f, -0.28482f, -0.869386f, 0.473183f, 0.0426571f, -0.472453f, 0.933418f, 0.219742f, 0.0934905f, 0.838844f, 0.342843f, 0.841638f, -0.128467f, 0.84634f, 0.0293755f, -0.0743476f, -0.583625f, 0.535269f, -0.770173f, 0.399445f, -0.901951f, 0.482658f, -0.947818f, -0.805741f, -0.229828f, -0.45568f, -0.96073f, -0.96426f, 0.765278f, -0.238507f, -0.319626f, 0.580844f, 0.290763f, 0.718677f, 7.53637e-05f, -0.352682f, 0.723389f, -0.284705f, -0.985808f, 0.49448f, 0.298938f, 0.86393f, -0.256605f, -0.590343f, -0.194821f, -0.180302f, -0.40393f, 0.580079f, 0.0173551f, 0.907175f, 0.362463f, 0.990057f, -0.101311f, -0.677711f, -0.407607f, 0.228307f, 0.054701f, -0.895022f, 0.108648f, -0.984112f, 0.7526f, 0.125415f, 0.43209f, -0.130732f, -0.552251f, -0.152143f, -0.745598f, -0.331337f, -0.826897f, -0.0233656f, -0.961974f, 0.342604f, 0.783221f, -0.271388f, 0.274418f, -0.543993f, -0.682943f, 0.283003f, -0.0335696f, -0.741271f, -0.590183f, -0.670818f, 0.479986f, -0.968874f, 0.544104f, -0.807284f, 0.827498f, -0.796678f, 0.818783f, -0.227827f, 0.946747f, -0.292411f, -0.475043f, 0.673002f, 0.936014f, 0.425352f, -0.494953f, -0.828427f, -0.699973f, 0.799466f, 0.407659f, 0.961484f, -0.595342f, -0.0842649f, -0.128094f, 0.270606f, 0.47215f, -0.826529f, -0.259661f, 0.0446834f, 0.295772f, -0.169856f, -0.635823f, 0.242777f, 0.0496289f, 0.690733f, -0.68372f, -0.708224f, 0.677033f, -0.749514f, 0.551705f, -0.702397f, 0.833347f, 0.387176f, 0.443312f, 0.10876f, 0.344609f, -0.0569167f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.06709f, -0.797245f, 2.2661f, -1.33682f, 0.943457f, -0.345875f, 0.104745f, -0.489415f, -2.10237f, -0.746785f, 0.774279f, 1.03753f, -1.00373f, -0.0662623f, 3.04734f, -0.980369f, 1.64909f, -0.594375f, -0.613449f, 1.12977f, 0.118765f, 1.38049f, 1.03795f, -1.6372f, 1.8485f, -0.437375f, 0.0811864f, 2.01812f, 0.609824f, 2.02386f, -0.508992f, 1.85277f, -1.13279f, -0.588752f, 0.884758f, -2.19264f, -1.63197f, -2.37862f, 0.9499f, 1.58888f, 0.545251f, -0.790398f, -1.16534f, 0.802736f, -1.45548f, 1.15225f, 0.885211f, -0.114708f, 0.828324f, 1.74889f, 0.613022f, 3.63816f, -0.613917f, -0.177923f, 1.83859f, 0.218806f, -1.57302f, -3.85305f, 2.33573f, 0.325747f, 2.85073f, 1.1351f, -0.199547f, 2.38342f, -0.321571f, 1.09302f, -0.158928f, -1.5794f, 0.912054f, 2.62907f, -0.11105f, 1.4687f, -3.02602f, 2.92817f, -0.355636f, 2.05759f, -0.0712112f, 0.114521f, -0.267442f, 2.57646f, -0.231441f, 0.895931f, 0.746904f, -1.1055f, 1.52694f, -0.997109f, 2.15506f, -3.09324f, 0.987944f, 1.7107f, -2.49964f, -0.378257f, -2.5225f, -1.9287f, -0.650011f, 0.891815f, -1.59239f, -1.088f, 1.10881f, -0.955859f, 0.31144f, -0.952566f, 1.94692f, -0.449652f, 2.873f, 0.882321f, -2.60202f, 0.461768f, 4.28871f, 1.59869f, 0.452176f, 1.50304f, 3.35635f, -0.708365f, -0.75725f, -0.172997f, -0.337669f, 1.0651f, 3.81319f, 0.543556f, 1.15701f, 0.531208f, 1.51461f, -0.753084f, 0.0129939f, 1.77789f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_3_H3_W2_VALID_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_3_H3_W2_VALID_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.8699309825897217f, 0.153671994805336f, -0.13493099808692932f, -0.2591570019721985f, 0.7655590176582336f, 0.6584939956665039f, -0.25289300084114075f, -0.07006630301475525f, 0.150845006108284f, 0.4065369963645935f, -0.356361985206604f, 0.21048299968242645f, -0.7520380020141602f, -0.551177978515625f, 0.21634800732135773f, 0.5590599775314331f, -0.3637630045413971f, 0.7560970187187195f, -0.6563169956207275f, -0.4031600058078766f, 0.6123620271682739f, -0.39809200167655945f, -0.9252719879150391f, 0.0968329980969429f, 0.0316540002822876f, 0.09482540190219879f, 0.3526650071144104f, -0.96806800365448f, 0.9390519857406616f, -0.058271098881959915f, 0.4597109854221344f, -0.6801750063896179f, 0.5060039758682251f, -0.1724960058927536f, -0.9538840055465698f, 0.20972700417041779f, 0.11319400370121002f, -0.47286298871040344f, -0.8394449949264526f, 0.3448669910430908f, -0.5408269762992859f, 0.506164014339447f, 0.756771981716156f, 0.4608210027217865f, -0.6406030058860779f, -0.568261981010437f, 0.15967300534248352f, -0.16025400161743164f, 0.600721001625061f, 0.4779619872570038f, -0.9394810199737549f, -0.5116369724273682f, -0.3236840069293976f, 0.36417099833488464f, -0.4045589864253998f, -0.3342759907245636f, 0.15651099383831024f, 0.8185970187187195f, -0.586014986038208f, 0.0892409011721611f, 0.46550899744033813f, 0.4130670130252838f, 0.5961400270462036f, -0.704380989074707f, 0.6446279883384705f, 0.8685619831085205f, -0.24756500124931335f, -0.2842960059642792f, 0.41986000537872314f, 0.214355006814003f, -0.47893500328063965f, -0.9887290000915527f, 0.8299149870872498f, -0.3553430140018463f, 0.8004819750785828f, -0.6343029737472534f, 0.45754700899124146f, 0.4467659890651703f, -0.8528059720993042f, -0.6684929728507996f, -0.16283699870109558f, -0.14226900041103363f, -0.9980859756469727f, -0.8936820030212402f, -0.8695880174636841f, -0.04235589876770973f, 0.1765490025281906f, -0.29680201411247253f, -0.25447899103164673f, 0.6437690258026123f, -0.9011790156364441f, -0.615401029586792f, -0.11628899723291397f, 0.4351719915866852f, 0.7174360156059265f, -0.4150660037994385f, -0.3378080129623413f, 0.8706380128860474f, 0.9030420184135437f, -0.28538399934768677f, 0.07566110044717789f, 0.9606090188026428f, 0.45709699392318726f, -0.13161900639533997f, 0.5712590217590332f, -0.1558389961719513f, -0.6620690226554871f, 0.17787200212478638f, 0.702597975730896f, -0.07137160003185272f, -0.3052079975605011f, 0.787322998046875f, 0.2631860077381134f, -0.7029510140419006f, -0.5245190262794495f, -0.5034989714622498f, 0.5344439744949341f, 0.03703580051660538f, -0.8141779899597168f, 0.6692500114440918f, -0.4279490113258362f, -0.6499019861221313f, -0.5597890019416809f, -0.6212509870529175f, 0.47040000557899475f, 0.1808219999074936f, 0.8255749940872192f, -0.29763099551200867f, -0.9183930158615112f, -0.3581770062446594f, 0.2217400074005127f, -0.5380650162696838f, -0.556240975856781f, -0.8501690030097961f, 0.5305259823799133f, -0.30306100845336914f, 0.47634899616241455f, 0.7571449875831604f, -0.7138609886169434f, 0.7182360291481018f, -0.5507689714431763f, -0.22746199369430542f, -0.3514859974384308f, -0.3034929931163788f, 0.07010120153427124f, 0.3297240138053894f, -0.6529489755630493f, 0.43274399638175964f, -0.7132700085639954f, 0.4365760087966919f, 0.8229039907455444f, -0.4271950125694275f, 0.2449049949645996f, -0.9039099812507629f, 0.2661589980125427f, -0.38897499442100525f, 0.10752300173044205f, 0.33467501401901245f, 0.49662700295448303f, 0.33984801173210144f, -0.10721799731254578f, 0.9318720102310181f, 0.7600780129432678f, -0.45513999462127686f, 0.09244350343942642f, -0.16038499772548676f, 0.1633480042219162f, 0.6887149810791016f, -0.9558699727058411f, 0.07896210253238678f, 0.24290800094604492f, -0.2898389995098114f, -0.5064060091972351f, 0.4137920141220093f, 0.13381600379943848f, -0.7532439827919006f, -0.16238699853420258f, -0.7310360074043274f, 0.9340720176696777f, 0.10623600333929062f, -0.8437449932098389f, -0.16880099475383759f, 0.9174500107765198f, -0.8012009859085083f, 0.3791530132293701f, 0.4270870089530945f, -0.8339229822158813f, 0.2138260006904602f, 0.38026100397109985f, 0.17286600172519684f, 0.6629160046577454f, 0.6977779865264893f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.18684199452400208f, -0.3850089907646179f, -1.2305899858474731f, 0.3595220148563385f, -0.28294500708580017f, 0.08478370308876038f, 0.1492460072040558f, 0.4847959876060486f, -1.4130100011825562f, 1.1774100065231323f, -1.215649962425232f, 0.5121430158615112f, -0.05330929905176163f, 1.1632800102233887f, 1.2247999906539917f, -0.619940996170044f, 1.5931400060653687f, 1.1371899843215942f, 1.186079978942871f, 0.5482969880104065f, -0.9933540225028992f, -0.6291260123252869f, -1.9609999656677246f, -1.0240299701690674f, 3.9935500621795654f, -0.5624380111694336f, -0.9286869764328003f, -0.5660529732704163f, -0.41466599702835083f, -1.7265700101852417f, -0.4786689877510071f, 1.4856499433517456f, -0.31711798906326294f, -1.383430004119873f, 1.8163399696350098f, -0.48189300298690796f, 0.24784600734710693f, -0.5196729898452759f, -1.0106500387191772f, -0.18430399894714355f, -0.19668500125408173f, 0.08450440317392349f, -1.8730800151824951f, 1.7203199863433838f, 1.2369400262832642f, 1.6008399724960327f, 2.372920036315918f, -0.35209301114082336f, 0.9203510284423828f, -1.1998900175094604f, 0.10317700356245041f, 2.589359998703003f, -1.0961899757385254f, 0.7713789939880371f, 0.2908639907836914f, 1.8076800107955933f, 0.23712700605392456f, 0.4979439973831177f, -0.1273369938135147f, 1.6853699684143066f, -2.527440071105957f, -2.0838000774383545f, 0.1282380074262619f, -0.9492290019989014f, 0.0058959899470210075f, 0.39612099528312683f, 0.4342209994792938f, -0.9148709774017334f, 0.834954023361206f, -0.9573410153388977f, -0.4528209865093231f, -0.7265740036964417f, 1.7870299816131592f, -0.1799039989709854f, -0.6753870248794556f, 0.69725501537323f, 0.7178009748458862f, -0.13556499779224396f, 2.677570104598999f, 0.25444701313972473f, 0.5079110264778137f, -0.0009504369809292257f, -0.7132409811019897f, 2.4849600791931152f, 1.2113499641418457f, -1.5603599548339844f, 0.0020001500379294157f, 0.434006005525589f, -1.2865300178527832f, -2.396589994430542f, -1.343459963798523f, -0.6842979788780212f, -0.3070389926433563f, -2.7623701095581055f, 1.1743199825286865f, 0.39987900853157043f, 0.9556300044059753f, -1.5256400108337402f, -0.21329699456691742f, -1.6868799924850464f, 0.11141899973154068f, -0.4796440005302429f, 1.3413599729537964f, 2.6458499431610107f, 1.2609200477600098f, 2.258280038833618f, -0.18785400688648224f, 1.3703999519348145f, 0.2744640111923218f, 0.5391290187835693f, 0.8441780209541321f, 0.9333360195159912f, -0.7060059905052185f, -0.09793619811534882f, -0.6392880082130432f, 1.0100300312042236f, 1.909690022468567f, -0.2922550141811371f, 0.8624789714813232f, -2.9594099521636963f, -2.239989995956421f, 0.4152829945087433f, 0.9799259901046753f, -0.7343479990959167f, 0.5949720144271851f, 0.3850190043449402f}}},
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
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 3, 8, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-0.29533499479293823f, 0.16608400642871857f, -0.719884991645813f, 0.823947012424469f, 0.18763099610805511f, -0.7550849914550781f, 0.04834359884262085f, 0.012578300200402737f, -0.1516599953174591f, 0.12966400384902954f, 0.9245190024375916f, 0.3508169949054718f, -0.002833950100466609f, -0.31911700963974f, 0.32864999771118164f, 0.5912659764289856f, 0.4620560109615326f, 0.6233999729156494f, -0.9556419849395752f, 0.664376974105835f, -0.2762629985809326f, -0.9589809775352478f, 0.30536898970603943f, -0.38288700580596924f, -0.2998040020465851f, -0.10305500030517578f, 0.5437430143356323f, -0.8571820259094238f, -0.7156140208244324f, -0.590142011642456f, -0.4733569920063019f, 0.48660698533058167f, -0.8009939789772034f, -0.44116199016571045f, 0.9696499705314636f, -0.526885986328125f, 0.33278900384902954f, -0.9340839982032776f, -0.7003949880599976f, -0.6917210221290588f, 0.03909660130739212f, -0.9715800285339355f, -0.46898600459098816f, -0.2731170058250427f, -0.7253810167312622f, -0.09492070227861404f, -0.22213299572467804f, -0.9294899702072144f, 0.33031100034713745f, 0.4759419858455658f, -0.5122079849243164f, 0.6517099738121033f, -0.4690130054950714f, -0.15784099698066711f, -0.6594370007514954f, 0.8903200030326843f, 0.772957980632782f, 0.9170519709587097f, 0.484142005443573f, 0.47887200117111206f, -0.732990026473999f, -0.09878560155630112f, -0.49810799956321716f, 0.928941011428833f, -0.0038760099560022354f, -0.28481999039649963f, -0.8693860173225403f, 0.4731830060482025f, 0.04265709966421127f, -0.4724529981613159f, 0.9334179759025574f, 0.21974200010299683f, 0.09349049627780914f, 0.8388440012931824f, 0.3428429961204529f, 0.8416380286216736f, -0.12846699357032776f, 0.846340000629425f, 0.029375499114394188f, -0.0743476003408432f, -0.5836250185966492f, 0.535269021987915f, -0.7701730132102966f, 0.3994449973106384f, -0.901951014995575f, 0.4826579988002777f, -0.9478179812431335f, -0.805741012096405f, -0.22982800006866455f, -0.4556800127029419f, -0.9607300162315369f, -0.9642599821090698f, 0.7652779817581177f, -0.2385070025920868f, -0.31962600350379944f, 0.580843985080719f, 0.2907629907131195f, 0.7186769843101501f, 7.536369957961142e-05f, -0.3526819944381714f, 0.7233890295028687f, -0.2847050130367279f, -0.9858080148696899f, 0.4944800138473511f, 0.29893800616264343f, 0.8639299869537354f, -0.25660499930381775f, -0.5903429985046387f, -0.19482100009918213f, -0.1803019940853119f, -0.40393000841140747f, 0.5800790190696716f, 0.017355099320411682f, 0.9071750044822693f, 0.36246299743652344f, 0.9900569915771484f, -0.10131099820137024f, -0.677711009979248f, -0.40760698914527893f, 0.22830699384212494f, 0.054701000452041626f, -0.8950219750404358f, 0.10864800214767456f, -0.984112024307251f, 0.7526000142097473f, 0.12541499733924866f, 0.43209001421928406f, -0.130731999874115f, -0.5522509813308716f, -0.15214300155639648f, -0.7455980181694031f, -0.33133700489997864f, -0.8268970251083374f, -0.023365600034594536f, -0.961974024772644f, 0.34260401129722595f, 0.7832210063934326f, -0.2713879942893982f, 0.2744179964065552f, -0.5439929962158203f, -0.6829429864883423f, 0.2830030024051666f, -0.03356960043311119f, -0.7412710189819336f, -0.5901830196380615f, -0.6708179712295532f, 0.4799860119819641f, -0.9688739776611328f, 0.5441039800643921f, -0.8072839975357056f, 0.8274980187416077f, -0.7966780066490173f, 0.8187829852104187f, -0.22782699763774872f, 0.9467470049858093f, -0.2924109995365143f, -0.47504299879074097f, 0.6730020046234131f, 0.9360139966011047f, 0.42535200715065f, -0.4949530065059662f, -0.8284270167350769f, -0.6999729871749878f, 0.7994660139083862f, 0.40765899419784546f, 0.9614840149879456f, -0.5953419804573059f, -0.08426489681005478f, -0.12809400260448456f, 0.27060601115226746f, 0.4721499979496002f, -0.8265290260314941f, -0.2596609890460968f, 0.04468340054154396f, 0.295771986246109f, -0.169855996966362f, -0.6358230113983154f, 0.2427770048379898f, 0.04962889850139618f, 0.690733015537262f, -0.6837199926376343f, -0.7082239985466003f, 0.677033007144928f, -0.7495139837265015f, 0.551705002784729f, -0.7023969888687134f, 0.8333470225334167f, 0.3871760070323944f, 0.44331198930740356f, 0.10875999927520752f, 0.34460899233818054f, -0.056916698813438416f}}},
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
  .operandDimensions = {{0, {1, 3, 6, 7}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0670900344848633f, -0.7972450256347656f, 2.2660999298095703f, -1.3368200063705444f, 0.9434570074081421f, -0.34587499499320984f, 0.10474500060081482f, -0.4894149899482727f, -2.102370023727417f, -0.7467849850654602f, 0.774278998374939f, 1.0375299453735352f, -1.003730058670044f, -0.06626229733228683f, 3.047339916229248f, -0.980368971824646f, 1.649090051651001f, -0.5943750143051147f, -0.613448977470398f, 1.129770040512085f, 0.11876499652862549f, 1.3804899454116821f, 1.037950038909912f, -1.6371999979019165f, 1.8485000133514404f, -0.437375009059906f, 0.08118639886379242f, 2.018120050430298f, 0.609824001789093f, 2.023859977722168f, -0.5089920163154602f, 1.8527699708938599f, -1.132789969444275f, -0.5887519717216492f, 0.8847579956054688f, -2.1926400661468506f, -1.6319700479507446f, -2.378619909286499f, 0.9498999714851379f, 1.588879942893982f, 0.5452510118484497f, -0.7903980016708374f, -1.1653399467468262f, 0.8027359843254089f, -1.4554799795150757f, 1.152250051498413f, 0.8852109909057617f, -0.11470799893140793f, 0.828324019908905f, 1.7488900423049927f, 0.6130220293998718f, 3.638159990310669f, -0.6139169931411743f, -0.17792299389839172f, 1.8385900259017944f, 0.21880599856376648f, -1.5730199813842773f, -3.8530499935150146f, 2.3357300758361816f, 0.325747013092041f, 2.8507299423217773f, 1.13510000705719f, -0.19954699277877808f, 2.383419990539551f, -0.3215709924697876f, 1.093019962310791f, -0.1589280068874359f, -1.5793999433517456f, 0.9120540022850037f, 2.6290700435638428f, -0.11105000227689743f, 1.4687000513076782f, -3.026020050048828f, 2.9281699657440186f, -0.35563600063323975f, 2.0575900077819824f, -0.07121119648218155f, 0.11452099680900574f, -0.267441987991333f, 2.5764598846435547f, -0.23144100606441498f, 0.8959310054779053f, 0.7469040155410767f, -1.1054999828338623f, 1.5269399881362915f, -0.9971089959144592f, 2.155060052871704f, -3.0932400226593018f, 0.9879440069198608f, 1.7107000350952148f, -2.4996399879455566f, -0.37825700640678406f, -2.5225000381469727f, -1.9286999702453613f, -0.6500110030174255f, 0.8918150067329407f, -1.5923899412155151f, -1.0880000591278076f, 1.1088099479675293f, -0.9558590054512024f, 0.31143999099731445f, -0.9525660276412964f, 1.9469200372695923f, -0.4496519863605499f, 2.872999906539917f, 0.8823210000991821f, -2.602020025253296f, 0.4617680013179779f, 4.288710117340088f, 1.5986900329589844f, 0.4521760046482086f, 1.5030399560928345f, 3.3563499450683594f, -0.7083650231361389f, -0.7572500109672546f, -0.17299699783325195f, -0.3376689851284027f, 1.0650999546051025f, 3.81318998336792f, 0.5435559749603271f, 1.1570099592208862f, 0.5312079787254333f, 1.5146100521087646f, -0.7530840039253235f, 0.01299390010535717f, 1.7778899669647217f}}},
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
return examples_3_H3_W2_VALID_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_relaxed() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_relaxed;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_weight_as_input() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}, {1, {3, 2, 2, 1}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}, {1, {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}, {1, {3, 2, 2, 1}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}, {1, {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_dynamic_output_shape;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}, {1, {3, 2, 2, 1}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}, {1, {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input;
};

std::vector<MixedTypedExample>& get_examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed() {
static std::vector<MixedTypedExample> examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 4, 1}}, {1, {3, 2, 2, 1}}, {2, {3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {1, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {129, 129, 129, 129, 130, 130, 130, 130, 129, 130, 131, 132, 129, 130, 131, 132}}, {1, {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {2, 1, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {145, 129, 132, 145, 129, 132, 144, 131, 130, 164, 131, 130}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nhwc;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nchw;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_dynamic_output_shape_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc_float16;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nchw;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nchw_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_dynamic_output_shape_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_dynamic_output_shape_nchw_float16;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nhwc_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_relaxed_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nhwc_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_nhwc_float16_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nchw_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_relaxed_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_nchw_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_nchw_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_zero_sized_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_nchw_float16_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_dynamic_output_shape_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_dynamic_output_shape_nhwc_float16_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nchw_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_relaxed_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f}}, {2, {0.0f}}},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
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
return examples_zero_sized_dynamic_output_shape_nchw_relaxed_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {0}}, {2, {0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
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
return examples_zero_sized_dynamic_output_shape_nchw_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 1, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f}}},
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{1, {0}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f}}, {2, {0.0f}}},
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
return examples_zero_sized_dynamic_output_shape_nchw_float16_2;
};

