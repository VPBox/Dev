// clang-format off
// Generated file (from: roi_pooling.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples_nhwc() {
static std::vector<MixedTypedExample> examples_nhwc = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {16, 16, 32, 32, 0, 0, 48, 48, 16, 0, 32, 48, 0, 16, 48, 32, 64, 64, 64, 64}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_float16() {
static std::vector<MixedTypedExample> examples_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {16, 16, 32, 32, 0, 0, 48, 48, 16, 0, 32, 48, 0, 16, 48, 32, 64, 64, 64, 64}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_float16() {
static std::vector<MixedTypedExample> examples_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {16, 16, 32, 32, 0, 0, 48, 48, 16, 0, 32, 48, 0, 16, 48, 32, 64, 64, 64, 64}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {16, 16, 32, 32, 0, 0, 48, 48, 16, 0, 32, 48, 0, 16, 48, 32, 64, 64, 64, 64}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {1, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f}}, {1, {2.0f, 2.0f, 4.0f, 4.0f, 0.0f, 0.0f, 6.0f, 6.0f, 2.0f, 0.0f, 4.0f, 6.0f, 0.0f, 2.0f, 6.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 8.88f, 7.41f, 5.6f, 9.95f, 4.37f, 0.1f, 7.64f, 6.5f, 9.47f, 7.55f, 3.0f, 0.89f, 3.01f, 6.3f, 4.4f, 1.64f, 6.74f, 6.16f, 8.6f, 5.85f, 3.17f, 7.12f, 6.79f, 5.77f, 6.62f, 5.13f, 8.44f, 5.08f, 7.12f, 2.84f, 1.19f, 8.37f, 0.9f, 7.86f, 9.69f, 1.97f, 1.31f, 4.42f, 9.89f, 0.18f, 9.0f, 9.3f, 0.44f, 5.05f, 6.47f, 1.09f, 9.5f, 1.3f, 2.18f, 2.05f, 7.74f, 7.66f, 0.65f, 4.18f, 7.14f, 5.35f, 7.9f, 1.04f, 1.47f, 9.01f, 0.95f, 4.07f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 2.64f, 0.86f, 4.86f, 2.38f, 2.45f, 8.77f, 0.06f, 3.6f, 9.28f, 5.84f, 8.97f, 6.89f, 1.43f, 3.9f, 5.91f, 7.4f, 9.25f, 3.12f, 4.92f, 1.87f, 3.22f, 9.5f, 6.73f, 2.07f, 7.3f, 3.07f, 4.97f, 0.24f, 8.91f, 1.09f, 0.27f, 7.29f, 6.94f, 2.31f, 6.88f, 4.33f, 1.37f, 0.86f, 0.46f, 6.07f, 3.81f, 0.86f, 6.99f, 4.36f, 1.92f, 8.19f, 3.57f, 7.9f, 6.78f, 4.64f, 6.82f, 6.18f, 9.63f, 2.63f, 2.33f, 1.36f, 2.7f, 9.99f, 9.85f, 8.06f, 4.8f, 7.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 8.6f, 7.12f, 6.79f, 5.13f, 8.44f, 7.86f, 9.69f, 4.42f, 9.89f, 9.3f, 6.47f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 9.5f, 6.73f, 9.5f, 9.28f, 6.89f, 8.97f, 6.18f, 9.63f, 9.99f, 9.85f, 9.99f, 9.85f, 7.29f, 6.94f, 7.29f, 6.94f, 2.31f, 6.88f, 7.9f, 6.78f, 7.9f, 6.82f, 4.64f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 8.88f, 7.41f, 5.6f, 9.95f, 4.37f, 0.1f, 7.64f, 6.5f, 9.47f, 7.55f, 3.0f, 0.89f, 3.01f, 6.3f, 4.4f, 1.64f, 6.74f, 6.16f, 8.6f, 5.85f, 3.17f, 7.12f, 6.79f, 5.77f, 6.62f, 5.13f, 8.44f, 5.08f, 7.12f, 2.84f, 1.19f, 8.37f, 0.9f, 7.86f, 9.69f, 1.97f, 1.31f, 4.42f, 9.89f, 0.18f, 9.0f, 9.3f, 0.44f, 5.05f, 6.47f, 1.09f, 9.5f, 1.3f, 2.18f, 2.05f, 7.74f, 7.66f, 0.65f, 4.18f, 7.14f, 5.35f, 7.9f, 1.04f, 1.47f, 9.01f, 0.95f, 4.07f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 2.64f, 0.86f, 4.86f, 2.38f, 2.45f, 8.77f, 0.06f, 3.6f, 9.28f, 5.84f, 8.97f, 6.89f, 1.43f, 3.9f, 5.91f, 7.4f, 9.25f, 3.12f, 4.92f, 1.87f, 3.22f, 9.5f, 6.73f, 2.07f, 7.3f, 3.07f, 4.97f, 0.24f, 8.91f, 1.09f, 0.27f, 7.29f, 6.94f, 2.31f, 6.88f, 4.33f, 1.37f, 0.86f, 0.46f, 6.07f, 3.81f, 0.86f, 6.99f, 4.36f, 1.92f, 8.19f, 3.57f, 7.9f, 6.78f, 4.64f, 6.82f, 6.18f, 9.63f, 2.63f, 2.33f, 1.36f, 2.7f, 9.99f, 9.85f, 8.06f, 4.8f, 7.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 8.6f, 7.12f, 6.79f, 5.13f, 8.44f, 7.86f, 9.69f, 4.42f, 9.89f, 9.3f, 6.47f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 9.5f, 6.73f, 9.5f, 9.28f, 6.89f, 8.97f, 6.18f, 9.63f, 9.99f, 9.85f, 9.99f, 9.85f, 7.29f, 6.94f, 7.29f, 6.94f, 2.31f, 6.88f, 7.9f, 6.78f, 7.9f, 6.82f, 4.64f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {221, 222, 185, 140, 249, 109, 2, 191, 162, 237, 189, 75, 22, 75, 158, 110, 41, 168, 154, 215, 146, 79, 178, 170, 144, 166, 128, 211, 127, 178, 71, 30, 209, 22, 197, 242, 49, 33, 111, 247, 5, 225, 233, 11, 126, 162, 27, 238, 32, 55, 51, 194, 192, 16, 104, 178, 134, 198, 26, 37, 225, 24, 102, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 137, 66, 22, 122, 60, 61, 219, 2, 90, 232, 146, 224, 172, 36, 98, 148, 185, 231, 78, 123, 47, 80, 238, 168, 52, 183, 77, 124, 6, 223, 27, 7, 182, 174, 58, 172, 108, 34, 22, 12, 152, 95, 22, 175, 109, 48, 205, 89, 198, 170, 116, 171, 154, 241, 66, 58, 34, 68, 250, 246, 202, 120, 195, 136}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {32, 32, 192, 64, 32, 32, 224, 96, 56, 8, 200, 88, 8, 56, 40, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {154, 215, 178, 170, 128, 211, 197, 242, 111, 247, 233, 162, 197, 247, 233, 247, 233, 238, 197, 247, 233, 247, 233, 238, 238, 168, 238, 232, 172, 224, 154, 241, 250, 246, 250, 246, 182, 174, 182, 174, 58, 172, 198, 170, 198, 171, 116, 171}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {8.84000015258789f, 8.880000114440918f, 7.409999847412109f, 5.599999904632568f, 9.949999809265137f, 4.369999885559082f, 0.10000000149011612f, 7.639999866485596f, 6.5f, 9.470000267028809f, 7.550000190734863f, 3.0f, 0.8899999856948853f, 3.009999990463257f, 6.300000190734863f, 4.400000095367432f, 1.6399999856948853f, 6.739999771118164f, 6.159999847412109f, 8.600000381469727f, 5.849999904632568f, 3.1700000762939453f, 7.119999885559082f, 6.789999961853027f, 5.769999980926514f, 6.619999885559082f, 5.130000114440918f, 8.4399995803833f, 5.079999923706055f, 7.119999885559082f, 2.8399999141693115f, 1.190000057220459f, 8.369999885559082f, 0.8999999761581421f, 7.860000133514404f, 9.6899995803833f, 1.9700000286102295f, 1.309999942779541f, 4.420000076293945f, 9.890000343322754f, 0.18000000715255737f, 9.0f, 9.300000190734863f, 0.4399999976158142f, 5.050000190734863f, 6.46999979019165f, 1.090000033378601f, 9.5f, 1.2999999523162842f, 2.180000066757202f, 2.049999952316284f, 7.739999771118164f, 7.659999847412109f, 0.6499999761581421f, 4.179999828338623f, 7.139999866485596f, 5.349999904632568f, 7.900000095367432f, 1.0399999618530273f, 1.4700000286102295f, 9.010000228881836f, 0.949999988079071f, 4.070000171661377f, 0.6499999761581421f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.46999979019165f, 2.640000104904175f, 0.8600000143051147f, 4.860000133514404f, 2.380000114440918f, 2.450000047683716f, 8.770000457763672f, 0.05999999865889549f, 3.5999999046325684f, 9.279999732971191f, 5.840000152587891f, 8.970000267028809f, 6.889999866485596f, 1.4299999475479126f, 3.9000000953674316f, 5.909999847412109f, 7.400000095367432f, 9.25f, 3.119999885559082f, 4.920000076293945f, 1.8700000047683716f, 3.2200000286102295f, 9.5f, 6.730000019073486f, 2.069999933242798f, 7.300000190734863f, 3.069999933242798f, 4.96999979019165f, 0.23999999463558197f, 8.90999984741211f, 1.090000033378601f, 0.27000001072883606f, 7.289999961853027f, 6.940000057220459f, 2.309999942779541f, 6.880000114440918f, 4.329999923706055f, 1.3700000047683716f, 0.8600000143051147f, 0.46000000834465027f, 6.070000171661377f, 3.809999942779541f, 0.8600000143051147f, 6.989999771118164f, 4.360000133514404f, 1.9199999570846558f, 8.1899995803833f, 3.569999933242798f, 7.900000095367432f, 6.78000020980835f, 4.639999866485596f, 6.820000171661377f, 6.179999828338623f, 9.630000114440918f, 2.630000114440918f, 2.3299999237060547f, 1.3600000143051147f, 2.700000047683716f, 9.989999771118164f, 9.850000381469727f, 8.0600004196167f, 4.800000190734863f, 7.800000190734863f, 5.429999828338623f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {6.159999847412109f, 8.600000381469727f, 7.119999885559082f, 6.789999961853027f, 5.130000114440918f, 8.4399995803833f, 7.860000133514404f, 9.6899995803833f, 4.420000076293945f, 9.890000343322754f, 9.300000190734863f, 6.46999979019165f, 7.860000133514404f, 9.890000343322754f, 9.300000190734863f, 9.890000343322754f, 9.300000190734863f, 9.5f, 7.860000133514404f, 9.890000343322754f, 9.300000190734863f, 9.890000343322754f, 9.300000190734863f, 9.5f, 9.5f, 6.730000019073486f, 9.5f, 9.279999732971191f, 6.889999866485596f, 8.970000267028809f, 6.179999828338623f, 9.630000114440918f, 9.989999771118164f, 9.850000381469727f, 9.989999771118164f, 9.850000381469727f, 7.289999961853027f, 6.940000057220459f, 7.289999961853027f, 6.940000057220459f, 2.309999942779541f, 6.880000114440918f, 7.900000095367432f, 6.78000020980835f, 7.900000095367432f, 6.820000171661377f, 4.639999866485596f, 6.820000171661377f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_2() {
static std::vector<MixedTypedExample> examples_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 7.41f, 9.95f, 0.1f, 6.5f, 7.55f, 0.89f, 6.3f, 1.64f, 6.16f, 5.85f, 7.12f, 5.77f, 5.13f, 5.08f, 2.84f, 8.37f, 7.86f, 1.97f, 4.42f, 0.18f, 9.3f, 5.05f, 1.09f, 1.3f, 2.05f, 7.66f, 4.18f, 5.35f, 1.04f, 9.01f, 4.07f, 8.88f, 5.6f, 4.37f, 7.64f, 9.47f, 3.0f, 3.01f, 4.4f, 6.74f, 8.6f, 3.17f, 6.79f, 6.62f, 8.44f, 7.12f, 1.19f, 0.9f, 9.69f, 1.31f, 9.89f, 9.0f, 0.44f, 6.47f, 9.5f, 2.18f, 7.74f, 0.65f, 7.14f, 7.9f, 1.47f, 0.95f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 0.86f, 2.38f, 8.77f, 3.6f, 5.84f, 6.89f, 3.9f, 7.4f, 3.12f, 1.87f, 9.5f, 2.07f, 3.07f, 0.24f, 1.09f, 7.29f, 2.31f, 4.33f, 0.86f, 6.07f, 0.86f, 4.36f, 8.19f, 7.9f, 4.64f, 6.18f, 2.63f, 1.36f, 9.99f, 8.06f, 7.8f, 2.64f, 4.86f, 2.45f, 0.06f, 9.28f, 8.97f, 1.43f, 5.91f, 9.25f, 4.92f, 3.22f, 6.73f, 7.3f, 4.97f, 8.91f, 0.27f, 6.94f, 6.88f, 1.37f, 0.46f, 3.81f, 6.99f, 1.92f, 3.57f, 6.78f, 6.82f, 9.63f, 2.33f, 2.7f, 9.85f, 4.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 7.12f, 5.13f, 7.86f, 4.42f, 9.3f, 8.6f, 6.79f, 8.44f, 9.69f, 9.89f, 6.47f, 7.86f, 9.3f, 9.3f, 7.86f, 9.3f, 9.3f, 9.89f, 9.89f, 9.5f, 9.89f, 9.89f, 9.5f, 9.5f, 9.5f, 6.89f, 6.18f, 9.99f, 9.99f, 6.73f, 9.28f, 8.97f, 9.63f, 9.85f, 9.85f, 7.29f, 7.29f, 2.31f, 7.9f, 7.9f, 4.64f, 6.94f, 6.94f, 6.88f, 6.78f, 6.82f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 7.41f, 9.95f, 0.1f, 6.5f, 7.55f, 0.89f, 6.3f, 1.64f, 6.16f, 5.85f, 7.12f, 5.77f, 5.13f, 5.08f, 2.84f, 8.37f, 7.86f, 1.97f, 4.42f, 0.18f, 9.3f, 5.05f, 1.09f, 1.3f, 2.05f, 7.66f, 4.18f, 5.35f, 1.04f, 9.01f, 4.07f, 8.88f, 5.6f, 4.37f, 7.64f, 9.47f, 3.0f, 3.01f, 4.4f, 6.74f, 8.6f, 3.17f, 6.79f, 6.62f, 8.44f, 7.12f, 1.19f, 0.9f, 9.69f, 1.31f, 9.89f, 9.0f, 0.44f, 6.47f, 9.5f, 2.18f, 7.74f, 0.65f, 7.14f, 7.9f, 1.47f, 0.95f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 0.86f, 2.38f, 8.77f, 3.6f, 5.84f, 6.89f, 3.9f, 7.4f, 3.12f, 1.87f, 9.5f, 2.07f, 3.07f, 0.24f, 1.09f, 7.29f, 2.31f, 4.33f, 0.86f, 6.07f, 0.86f, 4.36f, 8.19f, 7.9f, 4.64f, 6.18f, 2.63f, 1.36f, 9.99f, 8.06f, 7.8f, 2.64f, 4.86f, 2.45f, 0.06f, 9.28f, 8.97f, 1.43f, 5.91f, 9.25f, 4.92f, 3.22f, 6.73f, 7.3f, 4.97f, 8.91f, 0.27f, 6.94f, 6.88f, 1.37f, 0.46f, 3.81f, 6.99f, 1.92f, 3.57f, 6.78f, 6.82f, 9.63f, 2.33f, 2.7f, 9.85f, 4.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 7.12f, 5.13f, 7.86f, 4.42f, 9.3f, 8.6f, 6.79f, 8.44f, 9.69f, 9.89f, 6.47f, 7.86f, 9.3f, 9.3f, 7.86f, 9.3f, 9.3f, 9.89f, 9.89f, 9.5f, 9.89f, 9.89f, 9.5f, 9.5f, 9.5f, 6.89f, 6.18f, 9.99f, 9.99f, 6.73f, 9.28f, 8.97f, 9.63f, 9.85f, 9.85f, 7.29f, 7.29f, 2.31f, 7.9f, 7.9f, 4.64f, 6.94f, 6.94f, 6.88f, 6.78f, 6.82f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {221, 185, 249, 2, 162, 189, 22, 158, 41, 154, 146, 178, 144, 128, 127, 71, 209, 197, 49, 111, 5, 233, 126, 27, 32, 51, 192, 104, 134, 26, 225, 102, 222, 140, 109, 191, 237, 75, 75, 110, 168, 215, 79, 170, 166, 211, 178, 30, 22, 242, 33, 247, 225, 11, 162, 238, 55, 194, 16, 178, 198, 37, 24, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 137, 22, 60, 219, 90, 146, 172, 98, 185, 78, 47, 238, 52, 77, 6, 27, 182, 58, 108, 22, 152, 22, 109, 205, 198, 116, 154, 66, 34, 250, 202, 195, 66, 122, 61, 2, 232, 224, 36, 148, 231, 123, 80, 168, 183, 124, 223, 7, 174, 172, 34, 12, 95, 175, 48, 89, 170, 171, 241, 58, 68, 246, 120, 136}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {32, 32, 192, 64, 32, 32, 224, 96, 56, 8, 200, 88, 8, 56, 40, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {154, 178, 128, 197, 111, 233, 215, 170, 211, 242, 247, 162, 197, 233, 233, 197, 233, 233, 247, 247, 238, 247, 247, 238, 238, 238, 172, 154, 250, 250, 168, 232, 224, 241, 246, 246, 182, 182, 58, 198, 198, 116, 174, 174, 172, 170, 171, 171}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {8.84000015258789f, 7.409999847412109f, 9.949999809265137f, 0.10000000149011612f, 6.5f, 7.550000190734863f, 0.8899999856948853f, 6.300000190734863f, 1.6399999856948853f, 6.159999847412109f, 5.849999904632568f, 7.119999885559082f, 5.769999980926514f, 5.130000114440918f, 5.079999923706055f, 2.8399999141693115f, 8.369999885559082f, 7.860000133514404f, 1.9700000286102295f, 4.420000076293945f, 0.18000000715255737f, 9.300000190734863f, 5.050000190734863f, 1.090000033378601f, 1.2999999523162842f, 2.049999952316284f, 7.659999847412109f, 4.179999828338623f, 5.349999904632568f, 1.0399999618530273f, 9.010000228881836f, 4.070000171661377f, 8.880000114440918f, 5.599999904632568f, 4.369999885559082f, 7.639999866485596f, 9.470000267028809f, 3.0f, 3.009999990463257f, 4.400000095367432f, 6.739999771118164f, 8.600000381469727f, 3.1700000762939453f, 6.789999961853027f, 6.619999885559082f, 8.4399995803833f, 7.119999885559082f, 1.190000057220459f, 0.8999999761581421f, 9.6899995803833f, 1.309999942779541f, 9.890000343322754f, 9.0f, 0.4399999976158142f, 6.46999979019165f, 9.5f, 2.180000066757202f, 7.739999771118164f, 0.6499999761581421f, 7.139999866485596f, 7.900000095367432f, 1.4700000286102295f, 0.949999988079071f, 0.6499999761581421f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.46999979019165f, 0.8600000143051147f, 2.380000114440918f, 8.770000457763672f, 3.5999999046325684f, 5.840000152587891f, 6.889999866485596f, 3.9000000953674316f, 7.400000095367432f, 3.119999885559082f, 1.8700000047683716f, 9.5f, 2.069999933242798f, 3.069999933242798f, 0.23999999463558197f, 1.090000033378601f, 7.289999961853027f, 2.309999942779541f, 4.329999923706055f, 0.8600000143051147f, 6.070000171661377f, 0.8600000143051147f, 4.360000133514404f, 8.1899995803833f, 7.900000095367432f, 4.639999866485596f, 6.179999828338623f, 2.630000114440918f, 1.3600000143051147f, 9.989999771118164f, 8.0600004196167f, 7.800000190734863f, 2.640000104904175f, 4.860000133514404f, 2.450000047683716f, 0.05999999865889549f, 9.279999732971191f, 8.970000267028809f, 1.4299999475479126f, 5.909999847412109f, 9.25f, 4.920000076293945f, 3.2200000286102295f, 6.730000019073486f, 7.300000190734863f, 4.96999979019165f, 8.90999984741211f, 0.27000001072883606f, 6.940000057220459f, 6.880000114440918f, 1.3700000047683716f, 0.46000000834465027f, 3.809999942779541f, 6.989999771118164f, 1.9199999570846558f, 3.569999933242798f, 6.78000020980835f, 6.820000171661377f, 9.630000114440918f, 2.3299999237060547f, 2.700000047683716f, 9.850000381469727f, 4.800000190734863f, 5.429999828338623f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {6.159999847412109f, 7.119999885559082f, 5.130000114440918f, 7.860000133514404f, 4.420000076293945f, 9.300000190734863f, 8.600000381469727f, 6.789999961853027f, 8.4399995803833f, 9.6899995803833f, 9.890000343322754f, 6.46999979019165f, 7.860000133514404f, 9.300000190734863f, 9.300000190734863f, 7.860000133514404f, 9.300000190734863f, 9.300000190734863f, 9.890000343322754f, 9.890000343322754f, 9.5f, 9.890000343322754f, 9.890000343322754f, 9.5f, 9.5f, 9.5f, 6.889999866485596f, 6.179999828338623f, 9.989999771118164f, 9.989999771118164f, 6.730000019073486f, 9.279999732971191f, 8.970000267028809f, 9.630000114440918f, 9.850000381469727f, 9.850000381469727f, 7.289999961853027f, 7.289999961853027f, 2.309999942779541f, 7.900000095367432f, 7.900000095367432f, 4.639999866485596f, 6.940000057220459f, 6.940000057220459f, 6.880000114440918f, 6.78000020980835f, 6.820000171661377f, 6.820000171661377f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 8.88f, 7.41f, 5.6f, 9.95f, 4.37f, 0.1f, 7.64f, 6.5f, 9.47f, 7.55f, 3.0f, 0.89f, 3.01f, 6.3f, 4.4f, 1.64f, 6.74f, 6.16f, 8.6f, 5.85f, 3.17f, 7.12f, 6.79f, 5.77f, 6.62f, 5.13f, 8.44f, 5.08f, 7.12f, 2.84f, 1.19f, 8.37f, 0.9f, 7.86f, 9.69f, 1.97f, 1.31f, 4.42f, 9.89f, 0.18f, 9.0f, 9.3f, 0.44f, 5.05f, 6.47f, 1.09f, 9.5f, 1.3f, 2.18f, 2.05f, 7.74f, 7.66f, 0.65f, 4.18f, 7.14f, 5.35f, 7.9f, 1.04f, 1.47f, 9.01f, 0.95f, 4.07f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 2.64f, 0.86f, 4.86f, 2.38f, 2.45f, 8.77f, 0.06f, 3.6f, 9.28f, 5.84f, 8.97f, 6.89f, 1.43f, 3.9f, 5.91f, 7.4f, 9.25f, 3.12f, 4.92f, 1.87f, 3.22f, 9.5f, 6.73f, 2.07f, 7.3f, 3.07f, 4.97f, 0.24f, 8.91f, 1.09f, 0.27f, 7.29f, 6.94f, 2.31f, 6.88f, 4.33f, 1.37f, 0.86f, 0.46f, 6.07f, 3.81f, 0.86f, 6.99f, 4.36f, 1.92f, 8.19f, 3.57f, 7.9f, 6.78f, 4.64f, 6.82f, 6.18f, 9.63f, 2.63f, 2.33f, 1.36f, 2.7f, 9.99f, 9.85f, 8.06f, 4.8f, 7.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 8.6f, 7.12f, 6.79f, 5.13f, 8.44f, 7.86f, 9.69f, 4.42f, 9.89f, 9.3f, 6.47f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 9.5f, 6.73f, 9.5f, 9.28f, 6.89f, 8.97f, 6.18f, 9.63f, 9.99f, 9.85f, 9.99f, 9.85f, 7.29f, 6.94f, 7.29f, 6.94f, 2.31f, 6.88f, 7.9f, 6.78f, 7.9f, 6.82f, 4.64f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 8.88f, 7.41f, 5.6f, 9.95f, 4.37f, 0.1f, 7.64f, 6.5f, 9.47f, 7.55f, 3.0f, 0.89f, 3.01f, 6.3f, 4.4f, 1.64f, 6.74f, 6.16f, 8.6f, 5.85f, 3.17f, 7.12f, 6.79f, 5.77f, 6.62f, 5.13f, 8.44f, 5.08f, 7.12f, 2.84f, 1.19f, 8.37f, 0.9f, 7.86f, 9.69f, 1.97f, 1.31f, 4.42f, 9.89f, 0.18f, 9.0f, 9.3f, 0.44f, 5.05f, 6.47f, 1.09f, 9.5f, 1.3f, 2.18f, 2.05f, 7.74f, 7.66f, 0.65f, 4.18f, 7.14f, 5.35f, 7.9f, 1.04f, 1.47f, 9.01f, 0.95f, 4.07f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 2.64f, 0.86f, 4.86f, 2.38f, 2.45f, 8.77f, 0.06f, 3.6f, 9.28f, 5.84f, 8.97f, 6.89f, 1.43f, 3.9f, 5.91f, 7.4f, 9.25f, 3.12f, 4.92f, 1.87f, 3.22f, 9.5f, 6.73f, 2.07f, 7.3f, 3.07f, 4.97f, 0.24f, 8.91f, 1.09f, 0.27f, 7.29f, 6.94f, 2.31f, 6.88f, 4.33f, 1.37f, 0.86f, 0.46f, 6.07f, 3.81f, 0.86f, 6.99f, 4.36f, 1.92f, 8.19f, 3.57f, 7.9f, 6.78f, 4.64f, 6.82f, 6.18f, 9.63f, 2.63f, 2.33f, 1.36f, 2.7f, 9.99f, 9.85f, 8.06f, 4.8f, 7.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 8.6f, 7.12f, 6.79f, 5.13f, 8.44f, 7.86f, 9.69f, 4.42f, 9.89f, 9.3f, 6.47f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 7.86f, 9.89f, 9.3f, 9.89f, 9.3f, 9.5f, 9.5f, 6.73f, 9.5f, 9.28f, 6.89f, 8.97f, 6.18f, 9.63f, 9.99f, 9.85f, 9.99f, 9.85f, 7.29f, 6.94f, 7.29f, 6.94f, 2.31f, 6.88f, 7.9f, 6.78f, 7.9f, 6.82f, 4.64f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {221, 222, 185, 140, 249, 109, 2, 191, 162, 237, 189, 75, 22, 75, 158, 110, 41, 168, 154, 215, 146, 79, 178, 170, 144, 166, 128, 211, 127, 178, 71, 30, 209, 22, 197, 242, 49, 33, 111, 247, 5, 225, 233, 11, 126, 162, 27, 238, 32, 55, 51, 194, 192, 16, 104, 178, 134, 198, 26, 37, 225, 24, 102, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 137, 66, 22, 122, 60, 61, 219, 2, 90, 232, 146, 224, 172, 36, 98, 148, 185, 231, 78, 123, 47, 80, 238, 168, 52, 183, 77, 124, 6, 223, 27, 7, 182, 174, 58, 172, 108, 34, 22, 12, 152, 95, 22, 175, 109, 48, 205, 89, 198, 170, 116, 171, 154, 241, 66, 58, 34, 68, 250, 246, 202, 120, 195, 136}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {32, 32, 192, 64, 32, 32, 224, 96, 56, 8, 200, 88, 8, 56, 40, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {154, 215, 178, 170, 128, 211, 197, 242, 111, 247, 233, 162, 197, 247, 233, 247, 233, 238, 197, 247, 233, 247, 233, 238, 238, 168, 238, 232, 172, 224, 154, 241, 250, 246, 250, 246, 182, 174, 182, 174, 58, 172, 198, 170, 198, 171, 116, 171}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 8, 2}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {8.84000015258789f, 8.880000114440918f, 7.409999847412109f, 5.599999904632568f, 9.949999809265137f, 4.369999885559082f, 0.10000000149011612f, 7.639999866485596f, 6.5f, 9.470000267028809f, 7.550000190734863f, 3.0f, 0.8899999856948853f, 3.009999990463257f, 6.300000190734863f, 4.400000095367432f, 1.6399999856948853f, 6.739999771118164f, 6.159999847412109f, 8.600000381469727f, 5.849999904632568f, 3.1700000762939453f, 7.119999885559082f, 6.789999961853027f, 5.769999980926514f, 6.619999885559082f, 5.130000114440918f, 8.4399995803833f, 5.079999923706055f, 7.119999885559082f, 2.8399999141693115f, 1.190000057220459f, 8.369999885559082f, 0.8999999761581421f, 7.860000133514404f, 9.6899995803833f, 1.9700000286102295f, 1.309999942779541f, 4.420000076293945f, 9.890000343322754f, 0.18000000715255737f, 9.0f, 9.300000190734863f, 0.4399999976158142f, 5.050000190734863f, 6.46999979019165f, 1.090000033378601f, 9.5f, 1.2999999523162842f, 2.180000066757202f, 2.049999952316284f, 7.739999771118164f, 7.659999847412109f, 0.6499999761581421f, 4.179999828338623f, 7.139999866485596f, 5.349999904632568f, 7.900000095367432f, 1.0399999618530273f, 1.4700000286102295f, 9.010000228881836f, 0.949999988079071f, 4.070000171661377f, 0.6499999761581421f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.46999979019165f, 2.640000104904175f, 0.8600000143051147f, 4.860000133514404f, 2.380000114440918f, 2.450000047683716f, 8.770000457763672f, 0.05999999865889549f, 3.5999999046325684f, 9.279999732971191f, 5.840000152587891f, 8.970000267028809f, 6.889999866485596f, 1.4299999475479126f, 3.9000000953674316f, 5.909999847412109f, 7.400000095367432f, 9.25f, 3.119999885559082f, 4.920000076293945f, 1.8700000047683716f, 3.2200000286102295f, 9.5f, 6.730000019073486f, 2.069999933242798f, 7.300000190734863f, 3.069999933242798f, 4.96999979019165f, 0.23999999463558197f, 8.90999984741211f, 1.090000033378601f, 0.27000001072883606f, 7.289999961853027f, 6.940000057220459f, 2.309999942779541f, 6.880000114440918f, 4.329999923706055f, 1.3700000047683716f, 0.8600000143051147f, 0.46000000834465027f, 6.070000171661377f, 3.809999942779541f, 0.8600000143051147f, 6.989999771118164f, 4.360000133514404f, 1.9199999570846558f, 8.1899995803833f, 3.569999933242798f, 7.900000095367432f, 6.78000020980835f, 4.639999866485596f, 6.820000171661377f, 6.179999828338623f, 9.630000114440918f, 2.630000114440918f, 2.3299999237060547f, 1.3600000143051147f, 2.700000047683716f, 9.989999771118164f, 9.850000381469727f, 8.0600004196167f, 4.800000190734863f, 7.800000190734863f, 5.429999828338623f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {6.159999847412109f, 8.600000381469727f, 7.119999885559082f, 6.789999961853027f, 5.130000114440918f, 8.4399995803833f, 7.860000133514404f, 9.6899995803833f, 4.420000076293945f, 9.890000343322754f, 9.300000190734863f, 6.46999979019165f, 7.860000133514404f, 9.890000343322754f, 9.300000190734863f, 9.890000343322754f, 9.300000190734863f, 9.5f, 7.860000133514404f, 9.890000343322754f, 9.300000190734863f, 9.890000343322754f, 9.300000190734863f, 9.5f, 9.5f, 6.730000019073486f, 9.5f, 9.279999732971191f, 6.889999866485596f, 8.970000267028809f, 6.179999828338623f, 9.630000114440918f, 9.989999771118164f, 9.850000381469727f, 9.989999771118164f, 9.850000381469727f, 7.289999961853027f, 6.940000057220459f, 7.289999961853027f, 6.940000057220459f, 2.309999942779541f, 6.880000114440918f, 7.900000095367432f, 6.78000020980835f, 7.900000095367432f, 6.820000171661377f, 4.639999866485596f, 6.820000171661377f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 7.41f, 9.95f, 0.1f, 6.5f, 7.55f, 0.89f, 6.3f, 1.64f, 6.16f, 5.85f, 7.12f, 5.77f, 5.13f, 5.08f, 2.84f, 8.37f, 7.86f, 1.97f, 4.42f, 0.18f, 9.3f, 5.05f, 1.09f, 1.3f, 2.05f, 7.66f, 4.18f, 5.35f, 1.04f, 9.01f, 4.07f, 8.88f, 5.6f, 4.37f, 7.64f, 9.47f, 3.0f, 3.01f, 4.4f, 6.74f, 8.6f, 3.17f, 6.79f, 6.62f, 8.44f, 7.12f, 1.19f, 0.9f, 9.69f, 1.31f, 9.89f, 9.0f, 0.44f, 6.47f, 9.5f, 2.18f, 7.74f, 0.65f, 7.14f, 7.9f, 1.47f, 0.95f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 0.86f, 2.38f, 8.77f, 3.6f, 5.84f, 6.89f, 3.9f, 7.4f, 3.12f, 1.87f, 9.5f, 2.07f, 3.07f, 0.24f, 1.09f, 7.29f, 2.31f, 4.33f, 0.86f, 6.07f, 0.86f, 4.36f, 8.19f, 7.9f, 4.64f, 6.18f, 2.63f, 1.36f, 9.99f, 8.06f, 7.8f, 2.64f, 4.86f, 2.45f, 0.06f, 9.28f, 8.97f, 1.43f, 5.91f, 9.25f, 4.92f, 3.22f, 6.73f, 7.3f, 4.97f, 8.91f, 0.27f, 6.94f, 6.88f, 1.37f, 0.46f, 3.81f, 6.99f, 1.92f, 3.57f, 6.78f, 6.82f, 9.63f, 2.33f, 2.7f, 9.85f, 4.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 7.12f, 5.13f, 7.86f, 4.42f, 9.3f, 8.6f, 6.79f, 8.44f, 9.69f, 9.89f, 6.47f, 7.86f, 9.3f, 9.3f, 7.86f, 9.3f, 9.3f, 9.89f, 9.89f, 9.5f, 9.89f, 9.89f, 9.5f, 9.5f, 9.5f, 6.89f, 6.18f, 9.99f, 9.99f, 6.73f, 9.28f, 8.97f, 9.63f, 9.85f, 9.85f, 7.29f, 7.29f, 2.31f, 7.9f, 7.9f, 4.64f, 6.94f, 6.94f, 6.88f, 6.78f, 6.82f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {8.84f, 7.41f, 9.95f, 0.1f, 6.5f, 7.55f, 0.89f, 6.3f, 1.64f, 6.16f, 5.85f, 7.12f, 5.77f, 5.13f, 5.08f, 2.84f, 8.37f, 7.86f, 1.97f, 4.42f, 0.18f, 9.3f, 5.05f, 1.09f, 1.3f, 2.05f, 7.66f, 4.18f, 5.35f, 1.04f, 9.01f, 4.07f, 8.88f, 5.6f, 4.37f, 7.64f, 9.47f, 3.0f, 3.01f, 4.4f, 6.74f, 8.6f, 3.17f, 6.79f, 6.62f, 8.44f, 7.12f, 1.19f, 0.9f, 9.69f, 1.31f, 9.89f, 9.0f, 0.44f, 6.47f, 9.5f, 2.18f, 7.74f, 0.65f, 7.14f, 7.9f, 1.47f, 0.95f, 0.65f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.47f, 0.86f, 2.38f, 8.77f, 3.6f, 5.84f, 6.89f, 3.9f, 7.4f, 3.12f, 1.87f, 9.5f, 2.07f, 3.07f, 0.24f, 1.09f, 7.29f, 2.31f, 4.33f, 0.86f, 6.07f, 0.86f, 4.36f, 8.19f, 7.9f, 4.64f, 6.18f, 2.63f, 1.36f, 9.99f, 8.06f, 7.8f, 2.64f, 4.86f, 2.45f, 0.06f, 9.28f, 8.97f, 1.43f, 5.91f, 9.25f, 4.92f, 3.22f, 6.73f, 7.3f, 4.97f, 8.91f, 0.27f, 6.94f, 6.88f, 1.37f, 0.46f, 3.81f, 6.99f, 1.92f, 3.57f, 6.78f, 6.82f, 9.63f, 2.33f, 2.7f, 9.85f, 4.8f, 5.43f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {6.16f, 7.12f, 5.13f, 7.86f, 4.42f, 9.3f, 8.6f, 6.79f, 8.44f, 9.69f, 9.89f, 6.47f, 7.86f, 9.3f, 9.3f, 7.86f, 9.3f, 9.3f, 9.89f, 9.89f, 9.5f, 9.89f, 9.89f, 9.5f, 9.5f, 9.5f, 6.89f, 6.18f, 9.99f, 9.99f, 6.73f, 9.28f, 8.97f, 9.63f, 9.85f, 9.85f, 7.29f, 7.29f, 2.31f, 7.9f, 7.9f, 4.64f, 6.94f, 6.94f, 6.88f, 6.78f, 6.82f, 6.82f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {221, 185, 249, 2, 162, 189, 22, 158, 41, 154, 146, 178, 144, 128, 127, 71, 209, 197, 49, 111, 5, 233, 126, 27, 32, 51, 192, 104, 134, 26, 225, 102, 222, 140, 109, 191, 237, 75, 75, 110, 168, 215, 79, 170, 166, 211, 178, 30, 22, 242, 33, 247, 225, 11, 162, 238, 55, 194, 16, 178, 198, 37, 24, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 137, 22, 60, 219, 90, 146, 172, 98, 185, 78, 47, 238, 52, 77, 6, 27, 182, 58, 108, 22, 152, 22, 109, 205, 198, 116, 154, 66, 34, 250, 202, 195, 66, 122, 61, 2, 232, 224, 36, 148, 231, 123, 80, 168, 183, 124, 223, 7, 174, 172, 34, 12, 95, 175, 48, 89, 170, 171, 241, 58, 68, 246, 120, 136}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {32, 32, 192, 64, 32, 32, 224, 96, 56, 8, 200, 88, 8, 56, 40, 88}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {154, 178, 128, 197, 111, 233, 215, 170, 211, 242, 247, 162, 197, 233, 233, 197, 233, 233, 247, 247, 238, 247, 247, 238, 238, 238, 172, 154, 250, 250, 168, 232, 224, 241, 246, 246, 182, 182, 58, 198, 198, 116, 174, 174, 172, 170, 171, 171}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 4, 8}}, {1, {4, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {8.84000015258789f, 7.409999847412109f, 9.949999809265137f, 0.10000000149011612f, 6.5f, 7.550000190734863f, 0.8899999856948853f, 6.300000190734863f, 1.6399999856948853f, 6.159999847412109f, 5.849999904632568f, 7.119999885559082f, 5.769999980926514f, 5.130000114440918f, 5.079999923706055f, 2.8399999141693115f, 8.369999885559082f, 7.860000133514404f, 1.9700000286102295f, 4.420000076293945f, 0.18000000715255737f, 9.300000190734863f, 5.050000190734863f, 1.090000033378601f, 1.2999999523162842f, 2.049999952316284f, 7.659999847412109f, 4.179999828338623f, 5.349999904632568f, 1.0399999618530273f, 9.010000228881836f, 4.070000171661377f, 8.880000114440918f, 5.599999904632568f, 4.369999885559082f, 7.639999866485596f, 9.470000267028809f, 3.0f, 3.009999990463257f, 4.400000095367432f, 6.739999771118164f, 8.600000381469727f, 3.1700000762939453f, 6.789999961853027f, 6.619999885559082f, 8.4399995803833f, 7.119999885559082f, 1.190000057220459f, 0.8999999761581421f, 9.6899995803833f, 1.309999942779541f, 9.890000343322754f, 9.0f, 0.4399999976158142f, 6.46999979019165f, 9.5f, 2.180000066757202f, 7.739999771118164f, 0.6499999761581421f, 7.139999866485596f, 7.900000095367432f, 1.4700000286102295f, 0.949999988079071f, 0.6499999761581421f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.46999979019165f, 0.8600000143051147f, 2.380000114440918f, 8.770000457763672f, 3.5999999046325684f, 5.840000152587891f, 6.889999866485596f, 3.9000000953674316f, 7.400000095367432f, 3.119999885559082f, 1.8700000047683716f, 9.5f, 2.069999933242798f, 3.069999933242798f, 0.23999999463558197f, 1.090000033378601f, 7.289999961853027f, 2.309999942779541f, 4.329999923706055f, 0.8600000143051147f, 6.070000171661377f, 0.8600000143051147f, 4.360000133514404f, 8.1899995803833f, 7.900000095367432f, 4.639999866485596f, 6.179999828338623f, 2.630000114440918f, 1.3600000143051147f, 9.989999771118164f, 8.0600004196167f, 7.800000190734863f, 2.640000104904175f, 4.860000133514404f, 2.450000047683716f, 0.05999999865889549f, 9.279999732971191f, 8.970000267028809f, 1.4299999475479126f, 5.909999847412109f, 9.25f, 4.920000076293945f, 3.2200000286102295f, 6.730000019073486f, 7.300000190734863f, 4.96999979019165f, 8.90999984741211f, 0.27000001072883606f, 6.940000057220459f, 6.880000114440918f, 1.3700000047683716f, 0.46000000834465027f, 3.809999942779541f, 6.989999771118164f, 1.9199999570846558f, 3.569999933242798f, 6.78000020980835f, 6.820000171661377f, 9.630000114440918f, 2.3299999237060547f, 2.700000047683716f, 9.850000381469727f, 4.800000190734863f, 5.429999828338623f}}, {1, {4.0f, 4.0f, 24.0f, 8.0f, 4.0f, 4.0f, 28.0f, 12.0f, 7.0f, 1.0f, 25.0f, 11.0f, 1.0f, 7.0f, 5.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {6.159999847412109f, 7.119999885559082f, 5.130000114440918f, 7.860000133514404f, 4.420000076293945f, 9.300000190734863f, 8.600000381469727f, 6.789999961853027f, 8.4399995803833f, 9.6899995803833f, 9.890000343322754f, 6.46999979019165f, 7.860000133514404f, 9.300000190734863f, 9.300000190734863f, 7.860000133514404f, 9.300000190734863f, 9.300000190734863f, 9.890000343322754f, 9.890000343322754f, 9.5f, 9.890000343322754f, 9.890000343322754f, 9.5f, 9.5f, 9.5f, 6.889999866485596f, 6.179999828338623f, 9.989999771118164f, 9.989999771118164f, 6.730000019073486f, 9.279999732971191f, 8.970000267028809f, 9.630000114440918f, 9.850000381469727f, 9.850000381469727f, 7.289999961853027f, 7.289999961853027f, 2.309999942779541f, 7.900000095367432f, 7.900000095367432f, 4.639999866485596f, 6.940000057220459f, 6.940000057220459f, 6.880000114440918f, 6.78000020980835f, 6.820000171661377f, 6.820000171661377f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_nhwc_3() {
static std::vector<MixedTypedExample> examples_nhwc_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_3;
};

std::vector<MixedTypedExample>& get_examples_nhwc_relaxed_3() {
static std::vector<MixedTypedExample> examples_nhwc_relaxed_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_relaxed_3;
};

std::vector<MixedTypedExample>& get_examples_nhwc_quant8_3() {
static std::vector<MixedTypedExample> examples_nhwc_quant8_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 16, 16, 32, 0, 0, 24, 48, 8, 0, 16, 48, 0, 16, 24, 32, 0, 0, 0, 0}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 88, 88, 88, 88}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_quant8_3;
};

std::vector<MixedTypedExample>& get_examples_nhwc_float16_3() {
static std::vector<MixedTypedExample> examples_nhwc_float16_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nhwc_float16_3;
};

std::vector<MixedTypedExample>& get_examples_nchw_3() {
static std::vector<MixedTypedExample> examples_nchw_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_3;
};

std::vector<MixedTypedExample>& get_examples_nchw_relaxed_3() {
static std::vector<MixedTypedExample> examples_nchw_relaxed_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_relaxed_3;
};

std::vector<MixedTypedExample>& get_examples_nchw_quant8_3() {
static std::vector<MixedTypedExample> examples_nchw_quant8_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 16, 16, 32, 0, 0, 24, 48, 8, 0, 16, 48, 0, 16, 24, 32, 0, 0, 0, 0}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 88, 88, 88, 88}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_quant8_3;
};

std::vector<MixedTypedExample>& get_examples_nchw_float16_3() {
static std::vector<MixedTypedExample> examples_nchw_float16_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_nchw_float16_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_relaxed_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_relaxed_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_relaxed_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_quant8_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_quant8_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 16, 16, 32, 0, 0, 24, 48, 8, 0, 16, 48, 0, 16, 24, 32, 0, 0, 0, 0}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 88, 88, 88, 88}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_quant8_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nhwc_float16_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nhwc_float16_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 4, 4, 1}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 1}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nhwc_float16_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_relaxed_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_relaxed_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_relaxed_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_quant8_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_quant8_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 88, 124, 144, 108, 96, 120, 164, 132, 156, 120, 140, 100, 120, 168, 116, 148, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{1, {8, 16, 16, 32, 0, 0, 24, 48, 8, 0, 16, 48, 0, 16, 24, 32, 0, 0, 0, 0}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {120, 164, 120, 140, 124, 164, 168, 148, 124, 164, 168, 140, 120, 164, 156, 140, 88, 88, 88, 88}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_quant8_3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_nchw_float16_3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_nchw_float16_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 1, 4, 4}}, {1, {5, 4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, -1.0f, 4.0f, -5.0f, -8.0f, -2.0f, 9.0f, 1.0f, 7.0f, -2.0f, 3.0f, -7.0f, -2.0f, 10.0f, -3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {1.0f, 2.0f, 2.0f, 4.0f, 0.0f, 0.0f, 3.0f, 6.0f, 1.0f, 0.0f, 2.0f, 6.0f, 0.0f, 2.0f, 3.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 1, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {-2.0f, 9.0f, -2.0f, 3.0f, -1.0f, 9.0f, 10.0f, 5.0f, -1.0f, 9.0f, 10.0f, 3.0f, -2.0f, 9.0f, 7.0f, 3.0f, -10.0f, -10.0f, -10.0f, -10.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_nchw_float16_3;
};

