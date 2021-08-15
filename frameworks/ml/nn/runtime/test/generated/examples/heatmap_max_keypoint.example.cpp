// clang-format off
// Generated file (from: heatmap_max_keypoint.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples_nhwc() {
static std::vector<MixedTypedExample> examples_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_float16() {
static std::vector<MixedTypedExample> examples_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827327728271484f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {9.071493148803711f, 10.005000114440918f, 7.1875f, 10.0f, 10.689666748046875f, 9.0f}}, {1, {8.224461555480957f, 8.53731632232666f, 11.729999542236328f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569671630859375f, 2.0f, 8.125f, 8.75f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw() {
static std::vector<MixedTypedExample> examples_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_float16() {
static std::vector<MixedTypedExample> examples_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827327728271484f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {9.071493148803711f, 10.005000114440918f, 7.1875f, 10.0f, 10.689666748046875f, 9.0f}}, {1, {8.224461555480957f, 8.53731632232666f, 11.729999542236328f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569671630859375f, 2.0f, 8.125f, 8.75f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 4, 4, 1}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827327728271484f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {9.071493148803711f, 10.005000114440918f, 7.1875f, 10.0f, 10.689666748046875f, 9.0f}}, {1, {8.224461555480957f, 8.53731632232666f, 11.729999542236328f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569671630859375f, 2.0f, 8.125f, 8.75f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827329175f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {9.071493f, 10.005f, 7.1875f, 10.0f, 10.689667f, 9.0f}}, {1, {8.224462f, 8.537316f, 11.73f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569672f, 2.0f, 8.125f, 8.75f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1, 4, 4}}, {1, {6, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -1.0f, 4.0f, 10.0f, -8.0f, -2.0f, 4.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -56.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f, -10.0f, -57.827327728271484f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 2.0f, -3.0f, 5.0f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f, 8.0f, 3.0f, 15.0f, 13.0f, 6.0f, 5.0f, 19.0f, 12.0f, 5.0f, 2.0f, 10.0f, 20.0f, 5.0f, 2.0f, 10.0f, 20.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {6, 1}}, {1, {6, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {9.071493148803711f, 10.005000114440918f, 7.1875f, 10.0f, 10.689666748046875f, 9.0f}}, {1, {8.224461555480957f, 8.53731632232666f, 11.729999542236328f, 9.625f, 8.875f, 9.5625f, 17.375f, 5.875f, 9.569671630859375f, 2.0f, 8.125f, 8.75f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_2() {
static std::vector<MixedTypedExample> examples_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.61f, 0.49f, 0.01f, 0.98f, 0.65f, 0.64f, 0.7f, 0.76f, 0.55f, 0.83f, 0.19f, 0.46f, 0.03f, 0.67f, 0.71f, 0.17f, 0.23f, 0.89f, 0.08f, 0.96f, 0.65f, 0.52f, 0.4f, 0.36f, 0.8f, 0.55f, 0.89f, 0.58f, 0.29f, 0.27f, 0.69f, 0.66f, 0.06f, 0.51f, 0.26f, 0.96f, 0.38f, 0.41f, 0.89f, 0.88f, 0.46f, 0.96f, 0.73f, 0.54f, 0.64f, 0.84f, 0.74f, 0.51f, 0.41f, 0.13f, 0.19f, 0.52f, 0.21f, 0.5f, 0.75f, 0.89f, 0.89f, 0.2f, 0.58f, 0.7f, 0.13f, 0.29f, 0.39f, 0.91f, 0.06f, 0.93f, 0.34f, 0.8f, 0.87f, 0.59f, 0.67f, 0.57f, 0.85f, 0.24f, 0.25f, 0.76f, 0.34f, 0.37f, 0.11f, 0.0f, 0.29f, 0.3f, 0.77f, 0.34f, 0.57f, 0.48f, 0.76f, 0.93f, 0.18f, 0.64f, 0.12f, 0.67f, 0.47f, 0.56f, 0.5f, 0.48f, 0.99f, 0.46f, 0.66f, 0.98f, 0.06f, 0.1f, 0.66f, 0.66f, 0.91f, 0.67f, 0.23f, 0.4f, 0.37f, 0.17f, 0.35f, 0.48f, 0.98f, 0.47f, 0.49f, 0.56f, 0.18f, 0.75f, 0.29f, 0.04f, 0.23f, 0.42f, 0.55f, 0.38f, 0.07f, 0.71f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.61f, 0.49f, 0.01f, 0.98f, 0.65f, 0.64f, 0.7f, 0.76f, 0.55f, 0.83f, 0.19f, 0.46f, 0.03f, 0.67f, 0.71f, 0.17f, 0.23f, 0.89f, 0.08f, 0.96f, 0.65f, 0.52f, 0.4f, 0.36f, 0.8f, 0.55f, 0.89f, 0.58f, 0.29f, 0.27f, 0.69f, 0.66f, 0.06f, 0.51f, 0.26f, 0.96f, 0.38f, 0.41f, 0.89f, 0.88f, 0.46f, 0.96f, 0.73f, 0.54f, 0.64f, 0.84f, 0.74f, 0.51f, 0.41f, 0.13f, 0.19f, 0.52f, 0.21f, 0.5f, 0.75f, 0.89f, 0.89f, 0.2f, 0.58f, 0.7f, 0.13f, 0.29f, 0.39f, 0.91f, 0.06f, 0.93f, 0.34f, 0.8f, 0.87f, 0.59f, 0.67f, 0.57f, 0.85f, 0.24f, 0.25f, 0.76f, 0.34f, 0.37f, 0.11f, 0.0f, 0.29f, 0.3f, 0.77f, 0.34f, 0.57f, 0.48f, 0.76f, 0.93f, 0.18f, 0.64f, 0.12f, 0.67f, 0.47f, 0.56f, 0.5f, 0.48f, 0.99f, 0.46f, 0.66f, 0.98f, 0.06f, 0.1f, 0.66f, 0.66f, 0.91f, 0.67f, 0.23f, 0.4f, 0.37f, 0.17f, 0.35f, 0.48f, 0.98f, 0.47f, 0.49f, 0.56f, 0.18f, 0.75f, 0.29f, 0.04f, 0.23f, 0.42f, 0.55f, 0.38f, 0.07f, 0.71f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.1899999976158142f, 0.6100000143051147f, 0.49000000953674316f, 0.009999999776482582f, 0.9800000190734863f, 0.6499999761581421f, 0.6399999856948853f, 0.699999988079071f, 0.7599999904632568f, 0.550000011920929f, 0.8299999833106995f, 0.1899999976158142f, 0.46000000834465027f, 0.029999999329447746f, 0.6700000166893005f, 0.7099999785423279f, 0.17000000178813934f, 0.23000000417232513f, 0.8899999856948853f, 0.07999999821186066f, 0.9599999785423279f, 0.6499999761581421f, 0.5199999809265137f, 0.4000000059604645f, 0.36000001430511475f, 0.800000011920929f, 0.550000011920929f, 0.8899999856948853f, 0.5799999833106995f, 0.28999999165534973f, 0.27000001072883606f, 0.6899999976158142f, 0.6600000262260437f, 0.05999999865889549f, 0.5099999904632568f, 0.25999999046325684f, 0.9599999785423279f, 0.3799999952316284f, 0.4099999964237213f, 0.8899999856948853f, 0.8799999952316284f, 0.46000000834465027f, 0.9599999785423279f, 0.7300000190734863f, 0.5400000214576721f, 0.6399999856948853f, 0.8399999737739563f, 0.7400000095367432f, 0.5099999904632568f, 0.4099999964237213f, 0.12999999523162842f, 0.1899999976158142f, 0.5199999809265137f, 0.20999999344348907f, 0.5f, 0.75f, 0.8899999856948853f, 0.8899999856948853f, 0.20000000298023224f, 0.5799999833106995f, 0.699999988079071f, 0.12999999523162842f, 0.28999999165534973f, 0.38999998569488525f, 0.9100000262260437f, 0.05999999865889549f, 0.9300000071525574f, 0.3400000035762787f, 0.800000011920929f, 0.8700000047683716f, 0.5899999737739563f, 0.6700000166893005f, 0.5699999928474426f, 0.8500000238418579f, 0.23999999463558197f, 0.25f, 0.7599999904632568f, 0.3400000035762787f, 0.3700000047683716f, 0.10999999940395355f, 0.0f, 0.28999999165534973f, 0.30000001192092896f, 0.7699999809265137f, 0.3400000035762787f, 0.5699999928474426f, 0.47999998927116394f, 0.7599999904632568f, 0.9300000071525574f, 0.18000000715255737f, 0.6399999856948853f, 0.11999999731779099f, 0.6700000166893005f, 0.4699999988079071f, 0.5600000023841858f, 0.5f, 0.47999998927116394f, 0.9900000095367432f, 0.46000000834465027f, 0.6600000262260437f, 0.9800000190734863f, 0.05999999865889549f, 0.10000000149011612f, 0.6600000262260437f, 0.6600000262260437f, 0.9100000262260437f, 0.6700000166893005f, 0.23000000417232513f, 0.4000000059604645f, 0.3700000047683716f, 0.17000000178813934f, 0.3499999940395355f, 0.47999998927116394f, 0.9800000190734863f, 0.4699999988079071f, 0.49000000953674316f, 0.5600000023841858f, 0.18000000715255737f, 0.75f, 0.28999999165534973f, 0.03999999910593033f, 0.23000000417232513f, 0.41999998688697815f, 0.550000011920929f, 0.3799999952316284f, 0.07000000029802322f, 0.7099999785423279f, 0.800000011920929f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0202100276947021f, 0.8905559778213501f, 1.0071099996566772f, 0.9451289772987366f, 0.9877979755401611f, 1.0738199949264526f, 0.9300000071525574f, 0.800000011920929f}}, {1, {7.227723121643066f, 4.25f, 8.090277671813965f, 17.75f, 8.5233793258667f, 12.589180946350098f, 8.365579605102539f, 10.12250804901123f, 12.43160343170166f, 8.934225082397461f, 4.625f, 9.239437103271484f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {147, 189, 177, 129, 226, 193, 192, 198, 204, 183, 211, 147, 174, 131, 195, 199, 145, 151, 217, 136, 224, 193, 180, 168, 164, 208, 183, 217, 186, 157, 155, 197, 194, 134, 179, 154, 224, 166, 169, 217, 216, 174, 224, 201, 182, 192, 212, 202, 179, 169, 141, 147, 180, 149, 178, 203, 217, 217, 148, 186, 198, 141, 157, 167, 219, 134, 221, 162, 208, 215, 187, 195, 185, 213, 152, 153, 204, 162, 165, 139, 128, 157, 158, 205, 162, 185, 176, 204, 221, 146, 192, 140, 195, 175, 184, 178, 176, 227, 174, 194, 226, 134, 138, 194, 194, 219, 195, 151, 168, 165, 145, 163, 176, 226, 175, 177, 184, 146, 203, 157, 132, 151, 170, 183, 166, 135, 199, 208}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {102, 89, 101, 95, 99, 107, 93, 80}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {58, 34, 65, 142, 68, 101, 67, 81, 99, 71, 37, 74, 37, 59, 211, 77}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_nchw_2() {
static std::vector<MixedTypedExample> examples_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.98f, 0.76f, 0.46f, 0.17f, 0.96f, 0.36f, 0.58f, 0.66f, 0.96f, 0.88f, 0.54f, 0.51f, 0.52f, 0.89f, 0.7f, 0.61f, 0.65f, 0.55f, 0.03f, 0.23f, 0.65f, 0.8f, 0.29f, 0.06f, 0.38f, 0.46f, 0.64f, 0.41f, 0.21f, 0.89f, 0.13f, 0.49f, 0.64f, 0.83f, 0.67f, 0.89f, 0.52f, 0.55f, 0.27f, 0.51f, 0.41f, 0.96f, 0.84f, 0.13f, 0.5f, 0.2f, 0.29f, 0.01f, 0.7f, 0.19f, 0.71f, 0.08f, 0.4f, 0.89f, 0.69f, 0.26f, 0.89f, 0.73f, 0.74f, 0.19f, 0.75f, 0.58f, 0.39f, 0.91f, 0.8f, 0.57f, 0.76f, 0.0f, 0.34f, 0.93f, 0.67f, 0.48f, 0.98f, 0.66f, 0.4f, 0.48f, 0.56f, 0.04f, 0.38f, 0.06f, 0.87f, 0.85f, 0.34f, 0.29f, 0.57f, 0.18f, 0.47f, 0.99f, 0.06f, 0.91f, 0.37f, 0.98f, 0.18f, 0.23f, 0.07f, 0.93f, 0.59f, 0.24f, 0.37f, 0.3f, 0.48f, 0.64f, 0.56f, 0.46f, 0.1f, 0.67f, 0.17f, 0.47f, 0.75f, 0.42f, 0.71f, 0.34f, 0.67f, 0.25f, 0.11f, 0.77f, 0.76f, 0.12f, 0.5f, 0.66f, 0.66f, 0.23f, 0.35f, 0.49f, 0.29f, 0.55f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.98f, 0.76f, 0.46f, 0.17f, 0.96f, 0.36f, 0.58f, 0.66f, 0.96f, 0.88f, 0.54f, 0.51f, 0.52f, 0.89f, 0.7f, 0.61f, 0.65f, 0.55f, 0.03f, 0.23f, 0.65f, 0.8f, 0.29f, 0.06f, 0.38f, 0.46f, 0.64f, 0.41f, 0.21f, 0.89f, 0.13f, 0.49f, 0.64f, 0.83f, 0.67f, 0.89f, 0.52f, 0.55f, 0.27f, 0.51f, 0.41f, 0.96f, 0.84f, 0.13f, 0.5f, 0.2f, 0.29f, 0.01f, 0.7f, 0.19f, 0.71f, 0.08f, 0.4f, 0.89f, 0.69f, 0.26f, 0.89f, 0.73f, 0.74f, 0.19f, 0.75f, 0.58f, 0.39f, 0.91f, 0.8f, 0.57f, 0.76f, 0.0f, 0.34f, 0.93f, 0.67f, 0.48f, 0.98f, 0.66f, 0.4f, 0.48f, 0.56f, 0.04f, 0.38f, 0.06f, 0.87f, 0.85f, 0.34f, 0.29f, 0.57f, 0.18f, 0.47f, 0.99f, 0.06f, 0.91f, 0.37f, 0.98f, 0.18f, 0.23f, 0.07f, 0.93f, 0.59f, 0.24f, 0.37f, 0.3f, 0.48f, 0.64f, 0.56f, 0.46f, 0.1f, 0.67f, 0.17f, 0.47f, 0.75f, 0.42f, 0.71f, 0.34f, 0.67f, 0.25f, 0.11f, 0.77f, 0.76f, 0.12f, 0.5f, 0.66f, 0.66f, 0.23f, 0.35f, 0.49f, 0.29f, 0.55f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.1899999976158142f, 0.9800000190734863f, 0.7599999904632568f, 0.46000000834465027f, 0.17000000178813934f, 0.9599999785423279f, 0.36000001430511475f, 0.5799999833106995f, 0.6600000262260437f, 0.9599999785423279f, 0.8799999952316284f, 0.5400000214576721f, 0.5099999904632568f, 0.5199999809265137f, 0.8899999856948853f, 0.699999988079071f, 0.6100000143051147f, 0.6499999761581421f, 0.550000011920929f, 0.029999999329447746f, 0.23000000417232513f, 0.6499999761581421f, 0.800000011920929f, 0.28999999165534973f, 0.05999999865889549f, 0.3799999952316284f, 0.46000000834465027f, 0.6399999856948853f, 0.4099999964237213f, 0.20999999344348907f, 0.8899999856948853f, 0.12999999523162842f, 0.49000000953674316f, 0.6399999856948853f, 0.8299999833106995f, 0.6700000166893005f, 0.8899999856948853f, 0.5199999809265137f, 0.550000011920929f, 0.27000001072883606f, 0.5099999904632568f, 0.4099999964237213f, 0.9599999785423279f, 0.8399999737739563f, 0.12999999523162842f, 0.5f, 0.20000000298023224f, 0.28999999165534973f, 0.009999999776482582f, 0.699999988079071f, 0.1899999976158142f, 0.7099999785423279f, 0.07999999821186066f, 0.4000000059604645f, 0.8899999856948853f, 0.6899999976158142f, 0.25999999046325684f, 0.8899999856948853f, 0.7300000190734863f, 0.7400000095367432f, 0.1899999976158142f, 0.75f, 0.5799999833106995f, 0.38999998569488525f, 0.9100000262260437f, 0.800000011920929f, 0.5699999928474426f, 0.7599999904632568f, 0.0f, 0.3400000035762787f, 0.9300000071525574f, 0.6700000166893005f, 0.47999998927116394f, 0.9800000190734863f, 0.6600000262260437f, 0.4000000059604645f, 0.47999998927116394f, 0.5600000023841858f, 0.03999999910593033f, 0.3799999952316284f, 0.05999999865889549f, 0.8700000047683716f, 0.8500000238418579f, 0.3400000035762787f, 0.28999999165534973f, 0.5699999928474426f, 0.18000000715255737f, 0.4699999988079071f, 0.9900000095367432f, 0.05999999865889549f, 0.9100000262260437f, 0.3700000047683716f, 0.9800000190734863f, 0.18000000715255737f, 0.23000000417232513f, 0.07000000029802322f, 0.9300000071525574f, 0.5899999737739563f, 0.23999999463558197f, 0.3700000047683716f, 0.30000001192092896f, 0.47999998927116394f, 0.6399999856948853f, 0.5600000023841858f, 0.46000000834465027f, 0.10000000149011612f, 0.6700000166893005f, 0.17000000178813934f, 0.4699999988079071f, 0.75f, 0.41999998688697815f, 0.7099999785423279f, 0.3400000035762787f, 0.6700000166893005f, 0.25f, 0.10999999940395355f, 0.7699999809265137f, 0.7599999904632568f, 0.11999999731779099f, 0.5f, 0.6600000262260437f, 0.6600000262260437f, 0.23000000417232513f, 0.3499999940395355f, 0.49000000953674316f, 0.28999999165534973f, 0.550000011920929f, 0.800000011920929f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0202100276947021f, 0.8905559778213501f, 1.0071099996566772f, 0.9451289772987366f, 0.9877979755401611f, 1.0738199949264526f, 0.9300000071525574f, 0.800000011920929f}}, {1, {7.227723121643066f, 4.25f, 8.090277671813965f, 17.75f, 8.5233793258667f, 12.589180946350098f, 8.365579605102539f, 10.12250804901123f, 12.43160343170166f, 8.934225082397461f, 4.625f, 9.239437103271484f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_quant8() {
static std::vector<MixedTypedExample> examples_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {147, 226, 204, 174, 145, 224, 164, 186, 194, 224, 216, 182, 179, 180, 217, 198, 189, 193, 183, 131, 151, 193, 208, 157, 134, 166, 174, 192, 169, 149, 217, 141, 177, 192, 211, 195, 217, 180, 183, 155, 179, 169, 224, 212, 141, 178, 148, 157, 129, 198, 147, 199, 136, 168, 217, 197, 154, 217, 201, 202, 147, 203, 186, 167, 219, 208, 185, 204, 128, 162, 221, 195, 176, 226, 194, 168, 176, 184, 132, 166, 134, 215, 213, 162, 157, 185, 146, 175, 227, 134, 219, 165, 226, 146, 151, 135, 221, 187, 152, 165, 158, 176, 192, 184, 174, 138, 195, 145, 175, 203, 170, 199, 162, 195, 153, 139, 205, 204, 140, 178, 194, 194, 151, 163, 177, 157, 183, 208}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {102, 89, 101, 95, 99, 107, 93, 80}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {58, 34, 65, 142, 68, 101, 67, 81, 99, 71, 37, 74, 37, 59, 211, 77}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.61f, 0.49f, 0.01f, 0.98f, 0.65f, 0.64f, 0.7f, 0.76f, 0.55f, 0.83f, 0.19f, 0.46f, 0.03f, 0.67f, 0.71f, 0.17f, 0.23f, 0.89f, 0.08f, 0.96f, 0.65f, 0.52f, 0.4f, 0.36f, 0.8f, 0.55f, 0.89f, 0.58f, 0.29f, 0.27f, 0.69f, 0.66f, 0.06f, 0.51f, 0.26f, 0.96f, 0.38f, 0.41f, 0.89f, 0.88f, 0.46f, 0.96f, 0.73f, 0.54f, 0.64f, 0.84f, 0.74f, 0.51f, 0.41f, 0.13f, 0.19f, 0.52f, 0.21f, 0.5f, 0.75f, 0.89f, 0.89f, 0.2f, 0.58f, 0.7f, 0.13f, 0.29f, 0.39f, 0.91f, 0.06f, 0.93f, 0.34f, 0.8f, 0.87f, 0.59f, 0.67f, 0.57f, 0.85f, 0.24f, 0.25f, 0.76f, 0.34f, 0.37f, 0.11f, 0.0f, 0.29f, 0.3f, 0.77f, 0.34f, 0.57f, 0.48f, 0.76f, 0.93f, 0.18f, 0.64f, 0.12f, 0.67f, 0.47f, 0.56f, 0.5f, 0.48f, 0.99f, 0.46f, 0.66f, 0.98f, 0.06f, 0.1f, 0.66f, 0.66f, 0.91f, 0.67f, 0.23f, 0.4f, 0.37f, 0.17f, 0.35f, 0.48f, 0.98f, 0.47f, 0.49f, 0.56f, 0.18f, 0.75f, 0.29f, 0.04f, 0.23f, 0.42f, 0.55f, 0.38f, 0.07f, 0.71f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.61f, 0.49f, 0.01f, 0.98f, 0.65f, 0.64f, 0.7f, 0.76f, 0.55f, 0.83f, 0.19f, 0.46f, 0.03f, 0.67f, 0.71f, 0.17f, 0.23f, 0.89f, 0.08f, 0.96f, 0.65f, 0.52f, 0.4f, 0.36f, 0.8f, 0.55f, 0.89f, 0.58f, 0.29f, 0.27f, 0.69f, 0.66f, 0.06f, 0.51f, 0.26f, 0.96f, 0.38f, 0.41f, 0.89f, 0.88f, 0.46f, 0.96f, 0.73f, 0.54f, 0.64f, 0.84f, 0.74f, 0.51f, 0.41f, 0.13f, 0.19f, 0.52f, 0.21f, 0.5f, 0.75f, 0.89f, 0.89f, 0.2f, 0.58f, 0.7f, 0.13f, 0.29f, 0.39f, 0.91f, 0.06f, 0.93f, 0.34f, 0.8f, 0.87f, 0.59f, 0.67f, 0.57f, 0.85f, 0.24f, 0.25f, 0.76f, 0.34f, 0.37f, 0.11f, 0.0f, 0.29f, 0.3f, 0.77f, 0.34f, 0.57f, 0.48f, 0.76f, 0.93f, 0.18f, 0.64f, 0.12f, 0.67f, 0.47f, 0.56f, 0.5f, 0.48f, 0.99f, 0.46f, 0.66f, 0.98f, 0.06f, 0.1f, 0.66f, 0.66f, 0.91f, 0.67f, 0.23f, 0.4f, 0.37f, 0.17f, 0.35f, 0.48f, 0.98f, 0.47f, 0.49f, 0.56f, 0.18f, 0.75f, 0.29f, 0.04f, 0.23f, 0.42f, 0.55f, 0.38f, 0.07f, 0.71f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.1899999976158142f, 0.6100000143051147f, 0.49000000953674316f, 0.009999999776482582f, 0.9800000190734863f, 0.6499999761581421f, 0.6399999856948853f, 0.699999988079071f, 0.7599999904632568f, 0.550000011920929f, 0.8299999833106995f, 0.1899999976158142f, 0.46000000834465027f, 0.029999999329447746f, 0.6700000166893005f, 0.7099999785423279f, 0.17000000178813934f, 0.23000000417232513f, 0.8899999856948853f, 0.07999999821186066f, 0.9599999785423279f, 0.6499999761581421f, 0.5199999809265137f, 0.4000000059604645f, 0.36000001430511475f, 0.800000011920929f, 0.550000011920929f, 0.8899999856948853f, 0.5799999833106995f, 0.28999999165534973f, 0.27000001072883606f, 0.6899999976158142f, 0.6600000262260437f, 0.05999999865889549f, 0.5099999904632568f, 0.25999999046325684f, 0.9599999785423279f, 0.3799999952316284f, 0.4099999964237213f, 0.8899999856948853f, 0.8799999952316284f, 0.46000000834465027f, 0.9599999785423279f, 0.7300000190734863f, 0.5400000214576721f, 0.6399999856948853f, 0.8399999737739563f, 0.7400000095367432f, 0.5099999904632568f, 0.4099999964237213f, 0.12999999523162842f, 0.1899999976158142f, 0.5199999809265137f, 0.20999999344348907f, 0.5f, 0.75f, 0.8899999856948853f, 0.8899999856948853f, 0.20000000298023224f, 0.5799999833106995f, 0.699999988079071f, 0.12999999523162842f, 0.28999999165534973f, 0.38999998569488525f, 0.9100000262260437f, 0.05999999865889549f, 0.9300000071525574f, 0.3400000035762787f, 0.800000011920929f, 0.8700000047683716f, 0.5899999737739563f, 0.6700000166893005f, 0.5699999928474426f, 0.8500000238418579f, 0.23999999463558197f, 0.25f, 0.7599999904632568f, 0.3400000035762787f, 0.3700000047683716f, 0.10999999940395355f, 0.0f, 0.28999999165534973f, 0.30000001192092896f, 0.7699999809265137f, 0.3400000035762787f, 0.5699999928474426f, 0.47999998927116394f, 0.7599999904632568f, 0.9300000071525574f, 0.18000000715255737f, 0.6399999856948853f, 0.11999999731779099f, 0.6700000166893005f, 0.4699999988079071f, 0.5600000023841858f, 0.5f, 0.47999998927116394f, 0.9900000095367432f, 0.46000000834465027f, 0.6600000262260437f, 0.9800000190734863f, 0.05999999865889549f, 0.10000000149011612f, 0.6600000262260437f, 0.6600000262260437f, 0.9100000262260437f, 0.6700000166893005f, 0.23000000417232513f, 0.4000000059604645f, 0.3700000047683716f, 0.17000000178813934f, 0.3499999940395355f, 0.47999998927116394f, 0.9800000190734863f, 0.4699999988079071f, 0.49000000953674316f, 0.5600000023841858f, 0.18000000715255737f, 0.75f, 0.28999999165534973f, 0.03999999910593033f, 0.23000000417232513f, 0.41999998688697815f, 0.550000011920929f, 0.3799999952316284f, 0.07000000029802322f, 0.7099999785423279f, 0.800000011920929f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0202100276947021f, 0.8905559778213501f, 1.0071099996566772f, 0.9451289772987366f, 0.9877979755401611f, 1.0738199949264526f, 0.9300000071525574f, 0.800000011920929f}}, {1, {7.227723121643066f, 4.25f, 8.090277671813965f, 17.75f, 8.5233793258667f, 12.589180946350098f, 8.365579605102539f, 10.12250804901123f, 12.43160343170166f, 8.934225082397461f, 4.625f, 9.239437103271484f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {147, 189, 177, 129, 226, 193, 192, 198, 204, 183, 211, 147, 174, 131, 195, 199, 145, 151, 217, 136, 224, 193, 180, 168, 164, 208, 183, 217, 186, 157, 155, 197, 194, 134, 179, 154, 224, 166, 169, 217, 216, 174, 224, 201, 182, 192, 212, 202, 179, 169, 141, 147, 180, 149, 178, 203, 217, 217, 148, 186, 198, 141, 157, 167, 219, 134, 221, 162, 208, 215, 187, 195, 185, 213, 152, 153, 204, 162, 165, 139, 128, 157, 158, 205, 162, 185, 176, 204, 221, 146, 192, 140, 195, 175, 184, 178, 176, 227, 174, 194, 226, 134, 138, 194, 194, 219, 195, 151, 168, 165, 145, 163, 176, 226, 175, 177, 184, 146, 203, 157, 132, 151, 170, 183, 166, 135, 199, 208}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {102, 89, 101, 95, 99, 107, 93, 80}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {58, 34, 65, 142, 68, 101, 67, 81, 99, 71, 37, 74, 37, 59, 211, 77}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.98f, 0.76f, 0.46f, 0.17f, 0.96f, 0.36f, 0.58f, 0.66f, 0.96f, 0.88f, 0.54f, 0.51f, 0.52f, 0.89f, 0.7f, 0.61f, 0.65f, 0.55f, 0.03f, 0.23f, 0.65f, 0.8f, 0.29f, 0.06f, 0.38f, 0.46f, 0.64f, 0.41f, 0.21f, 0.89f, 0.13f, 0.49f, 0.64f, 0.83f, 0.67f, 0.89f, 0.52f, 0.55f, 0.27f, 0.51f, 0.41f, 0.96f, 0.84f, 0.13f, 0.5f, 0.2f, 0.29f, 0.01f, 0.7f, 0.19f, 0.71f, 0.08f, 0.4f, 0.89f, 0.69f, 0.26f, 0.89f, 0.73f, 0.74f, 0.19f, 0.75f, 0.58f, 0.39f, 0.91f, 0.8f, 0.57f, 0.76f, 0.0f, 0.34f, 0.93f, 0.67f, 0.48f, 0.98f, 0.66f, 0.4f, 0.48f, 0.56f, 0.04f, 0.38f, 0.06f, 0.87f, 0.85f, 0.34f, 0.29f, 0.57f, 0.18f, 0.47f, 0.99f, 0.06f, 0.91f, 0.37f, 0.98f, 0.18f, 0.23f, 0.07f, 0.93f, 0.59f, 0.24f, 0.37f, 0.3f, 0.48f, 0.64f, 0.56f, 0.46f, 0.1f, 0.67f, 0.17f, 0.47f, 0.75f, 0.42f, 0.71f, 0.34f, 0.67f, 0.25f, 0.11f, 0.77f, 0.76f, 0.12f, 0.5f, 0.66f, 0.66f, 0.23f, 0.35f, 0.49f, 0.29f, 0.55f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.19f, 0.98f, 0.76f, 0.46f, 0.17f, 0.96f, 0.36f, 0.58f, 0.66f, 0.96f, 0.88f, 0.54f, 0.51f, 0.52f, 0.89f, 0.7f, 0.61f, 0.65f, 0.55f, 0.03f, 0.23f, 0.65f, 0.8f, 0.29f, 0.06f, 0.38f, 0.46f, 0.64f, 0.41f, 0.21f, 0.89f, 0.13f, 0.49f, 0.64f, 0.83f, 0.67f, 0.89f, 0.52f, 0.55f, 0.27f, 0.51f, 0.41f, 0.96f, 0.84f, 0.13f, 0.5f, 0.2f, 0.29f, 0.01f, 0.7f, 0.19f, 0.71f, 0.08f, 0.4f, 0.89f, 0.69f, 0.26f, 0.89f, 0.73f, 0.74f, 0.19f, 0.75f, 0.58f, 0.39f, 0.91f, 0.8f, 0.57f, 0.76f, 0.0f, 0.34f, 0.93f, 0.67f, 0.48f, 0.98f, 0.66f, 0.4f, 0.48f, 0.56f, 0.04f, 0.38f, 0.06f, 0.87f, 0.85f, 0.34f, 0.29f, 0.57f, 0.18f, 0.47f, 0.99f, 0.06f, 0.91f, 0.37f, 0.98f, 0.18f, 0.23f, 0.07f, 0.93f, 0.59f, 0.24f, 0.37f, 0.3f, 0.48f, 0.64f, 0.56f, 0.46f, 0.1f, 0.67f, 0.17f, 0.47f, 0.75f, 0.42f, 0.71f, 0.34f, 0.67f, 0.25f, 0.11f, 0.77f, 0.76f, 0.12f, 0.5f, 0.66f, 0.66f, 0.23f, 0.35f, 0.49f, 0.29f, 0.55f, 0.8f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.02021f, 0.890556f, 1.00711f, 0.945129f, 0.987798f, 1.07382f, 0.93f, 0.8f}}, {1, {7.227723f, 4.25f, 8.090278f, 17.75f, 8.523379f, 12.589181f, 8.36558f, 10.122508f, 12.431603f, 8.934225f, 4.625f, 9.239437f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.1899999976158142f, 0.9800000190734863f, 0.7599999904632568f, 0.46000000834465027f, 0.17000000178813934f, 0.9599999785423279f, 0.36000001430511475f, 0.5799999833106995f, 0.6600000262260437f, 0.9599999785423279f, 0.8799999952316284f, 0.5400000214576721f, 0.5099999904632568f, 0.5199999809265137f, 0.8899999856948853f, 0.699999988079071f, 0.6100000143051147f, 0.6499999761581421f, 0.550000011920929f, 0.029999999329447746f, 0.23000000417232513f, 0.6499999761581421f, 0.800000011920929f, 0.28999999165534973f, 0.05999999865889549f, 0.3799999952316284f, 0.46000000834465027f, 0.6399999856948853f, 0.4099999964237213f, 0.20999999344348907f, 0.8899999856948853f, 0.12999999523162842f, 0.49000000953674316f, 0.6399999856948853f, 0.8299999833106995f, 0.6700000166893005f, 0.8899999856948853f, 0.5199999809265137f, 0.550000011920929f, 0.27000001072883606f, 0.5099999904632568f, 0.4099999964237213f, 0.9599999785423279f, 0.8399999737739563f, 0.12999999523162842f, 0.5f, 0.20000000298023224f, 0.28999999165534973f, 0.009999999776482582f, 0.699999988079071f, 0.1899999976158142f, 0.7099999785423279f, 0.07999999821186066f, 0.4000000059604645f, 0.8899999856948853f, 0.6899999976158142f, 0.25999999046325684f, 0.8899999856948853f, 0.7300000190734863f, 0.7400000095367432f, 0.1899999976158142f, 0.75f, 0.5799999833106995f, 0.38999998569488525f, 0.9100000262260437f, 0.800000011920929f, 0.5699999928474426f, 0.7599999904632568f, 0.0f, 0.3400000035762787f, 0.9300000071525574f, 0.6700000166893005f, 0.47999998927116394f, 0.9800000190734863f, 0.6600000262260437f, 0.4000000059604645f, 0.47999998927116394f, 0.5600000023841858f, 0.03999999910593033f, 0.3799999952316284f, 0.05999999865889549f, 0.8700000047683716f, 0.8500000238418579f, 0.3400000035762787f, 0.28999999165534973f, 0.5699999928474426f, 0.18000000715255737f, 0.4699999988079071f, 0.9900000095367432f, 0.05999999865889549f, 0.9100000262260437f, 0.3700000047683716f, 0.9800000190734863f, 0.18000000715255737f, 0.23000000417232513f, 0.07000000029802322f, 0.9300000071525574f, 0.5899999737739563f, 0.23999999463558197f, 0.3700000047683716f, 0.30000001192092896f, 0.47999998927116394f, 0.6399999856948853f, 0.5600000023841858f, 0.46000000834465027f, 0.10000000149011612f, 0.6700000166893005f, 0.17000000178813934f, 0.4699999988079071f, 0.75f, 0.41999998688697815f, 0.7099999785423279f, 0.3400000035762787f, 0.6700000166893005f, 0.25f, 0.10999999940395355f, 0.7699999809265137f, 0.7599999904632568f, 0.11999999731779099f, 0.5f, 0.6600000262260437f, 0.6600000262260437f, 0.23000000417232513f, 0.3499999940395355f, 0.49000000953674316f, 0.28999999165534973f, 0.550000011920929f, 0.800000011920929f}}, {1, {5.0f, 2.0f, 10.0f, 20.0f, 1.0f, 7.0f, 30.0f, 10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0202100276947021f, 0.8905559778213501f, 1.0071099996566772f, 0.9451289772987366f, 0.9877979755401611f, 1.0738199949264526f, 0.9300000071525574f, 0.800000011920929f}}, {1, {7.227723121643066f, 4.25f, 8.090277671813965f, 17.75f, 8.5233793258667f, 12.589180946350098f, 8.365579605102539f, 10.12250804901123f, 12.43160343170166f, 8.934225082397461f, 4.625f, 9.239437103271484f, 4.625f, 7.375f, 26.375f, 9.625f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4, 4, 4}}, {1, {2, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {147, 226, 204, 174, 145, 224, 164, 186, 194, 224, 216, 182, 179, 180, 217, 198, 189, 193, 183, 131, 151, 193, 208, 157, 134, 166, 174, 192, 169, 149, 217, 141, 177, 192, 211, 195, 217, 180, 183, 155, 179, 169, 224, 212, 141, 178, 148, 157, 129, 198, 147, 199, 136, 168, 217, 197, 154, 217, 201, 202, 147, 203, 186, 167, 219, 208, 185, 204, 128, 162, 221, 195, 176, 226, 194, 168, 176, 184, 132, 166, 134, 215, 213, 162, 157, 185, 146, 175, 227, 134, 219, 165, 226, 146, 151, 135, 221, 187, 152, 165, 158, 176, 192, 184, 174, 138, 195, 145, 175, 203, 170, 199, 162, 195, 153, 139, 205, 204, 140, 178, 194, 194, 151, 163, 177, 157, 183, 208}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 4}}, {1, {2, 4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {102, 89, 101, 95, 99, 107, 93, 80}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {58, 34, 65, 142, 68, 101, 67, 81, 99, 71, 37, 74, 37, 59, 211, 77}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_quant8;
};

std::vector<MixedTypedExample>& get_examples_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 148, 122, 138, 108, 126, 136, 118, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 148, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 16, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80, 64, 24, 120, 104, 48, 40, 152, 96, 40, 16, 80, 160}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1}}, {1, {5, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {101, 110, 82, 110, 117}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {66, 68, 94, 77, 71, 76, 139, 47, 77, 16}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 148, 122, 138, 108, 126, 136, 118, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 148, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 16, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80, 64, 24, 120, 104, 48, 40, 152, 96, 40, 16, 80, 160}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1}}, {1, {5, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {101, 110, 82, 110, 117}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {66, 68, 94, 77, 71, 76, 139, 47, 77, 16}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 148, 122, 138, 108, 126, 136, 118, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 148, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 16, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80, 64, 24, 120, 104, 48, 40, 152, 96, 40, 16, 80, 160}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1}}, {1, {5, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {101, 110, 82, 110, 117}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {66, 68, 94, 77, 71, 76, 139, 47, 77, 16}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 148, 122, 138, 108, 126, 136, 118, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 126, 136, 148, 112, 124, 136, 130, 142, 124, 134, 114, 124, 132, 122, 138, 108, 16, 136, 118, 112, 124, 146, 130, 142, 124, 134, 114, 124, 132, 122, 138}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {40, 16, 80, 160, 8, 56, 240, 80, 64, 24, 120, 104, 48, 40, 152, 96, 40, 16, 80, 160}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1}}, {1, {5, 1, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {101, 110, 82, 110, 117}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {66, 68, 94, 77, 71, 76, 139, 47, 77, 16}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_quant8_2;
};

