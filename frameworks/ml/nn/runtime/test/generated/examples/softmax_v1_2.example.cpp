// clang-format off
// Generated file (from: softmax_v1_2.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_relaxed() {
static std::vector<MixedTypedExample> examples_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_float16() {
static std::vector<MixedTypedExample> examples_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_quant8() {
static std::vector<MixedTypedExample> examples_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_2() {
static std::vector<MixedTypedExample> examples_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_relaxed_2() {
static std::vector<MixedTypedExample> examples_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_relaxed_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_relaxed_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_float16_2() {
static std::vector<MixedTypedExample> examples_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_float16_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_float16_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_quant8_2() {
static std::vector<MixedTypedExample> examples_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_quant8_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_float16_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 196, 124, 196, 124, 192, 120, 192, 120, 192, 120, 192, 120, 188, 116, 188, 116, 188, 116, 188, 116, 184, 112, 184, 112, 184, 112, 184, 112, 132, 60, 132, 60, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 165, 165, 165, 165, 61, 61, 61, 61, 61, 61, 61, 61, 22, 22, 22, 22, 22, 22, 22, 22, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 196, 124, 196, 124, 192, 120, 192, 120, 192, 120, 192, 120, 188, 116, 188, 116, 188, 116, 188, 116, 184, 112, 184, 112, 184, 112, 184, 112, 132, 60, 132, 60, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 165, 165, 165, 165, 61, 61, 61, 61, 61, 61, 61, 61, 22, 22, 22, 22, 22, 22, 22, 22, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60, 196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0, 165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60, 196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0, 165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f, 0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.643914213228014f, 0.236882800924671f, 0.236882800924671f, 0.087144312427294f, 0.087144312427294f, 0.032058600957022f, 0.032058600957022f, 7.246299848982885e-08f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f, 0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.643914213228014f, 0.236882800924671f, 0.087144312427294f, 0.032058600957022f, 7.246299848982885e-08f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f, 17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 17.0f, -1.0f, 16.0f, -2.0f, 16.0f, -2.0f, 15.0f, -3.0f, 15.0f, -3.0f, 14.0f, -4.0f, 14.0f, -4.0f, 1.0f, -17.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f, 17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f, 0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f, 17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, -1.0f, 16.0f, -2.0f, 15.0f, -3.0f, 14.0f, -4.0f, 1.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.6439142227172852f, 0.23688280582427979f, 0.23688280582427979f, 0.08714431524276733f, 0.08714431524276733f, 0.03205860033631325f, 0.03205860033631325f, 7.246299560392799e-08f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f, -1.0f, -2.0f, -3.0f, -4.0f, -17.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f, 0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {17.0f, 16.0f, 15.0f, 14.0f, 1.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.6439142227172852f, 0.23688280582427979f, 0.08714431524276733f, 0.03205860033631325f, 7.246299560392799e-08f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 196, 124, 196, 124, 192, 120, 192, 120, 192, 120, 192, 120, 188, 116, 188, 116, 188, 116, 188, 116, 184, 112, 184, 112, 184, 112, 184, 112, 132, 60, 132, 60, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 165, 165, 165, 165, 61, 61, 61, 61, 61, 61, 61, 61, 22, 22, 22, 22, 22, 22, 22, 22, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 196, 124, 196, 124, 192, 120, 192, 120, 192, 120, 192, 120, 188, 116, 188, 116, 188, 116, 188, 116, 184, 112, 184, 112, 184, 112, 184, 112, 132, 60, 132, 60, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 165, 165, 165, 165, 61, 61, 61, 61, 61, 61, 61, 61, 22, 22, 22, 22, 22, 22, 22, 22, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60, 196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0, 165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60, 196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0, 165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis3() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 196, 124, 192, 120, 192, 120, 188, 116, 188, 116, 184, 112, 184, 112, 132, 60, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 165, 165, 61, 61, 61, 61, 22, 22, 22, 22, 8, 8, 8, 8, 0, 0, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60, 196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0, 165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60, 196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 124, 192, 120, 188, 116, 184, 112, 132, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 165, 61, 61, 22, 22, 8, 8, 0, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis1() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132, 124, 120, 116, 112, 60}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0, 165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim1_axis0;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {196, 192, 188, 184, 132}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {165, 61, 22, 8, 0}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_relaxed_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_relaxed_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_relaxed_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_float16_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_float16_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_float16_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 132, 124, 132, 124, 136, 120, 136, 120, 136, 120, 136, 120, 140, 116, 140, 116, 140, 116, 140, 116, 144, 112, 144, 112, 144, 112, 144, 112, 148, 108, 148, 108, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 132, 124, 132, 124, 136, 120, 136, 120, 136, 120, 136, 120, 140, 116, 140, 116, 140, 116, 140, 116, 144, 112, 144, 112, 144, 112, 144, 112, 148, 108, 148, 108, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108, 132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108, 132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_quant8_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_quant8_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_quant8_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.2f, 0.2f, 0.2f, 0.2f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f, 1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 1.0f, -1.0f, 2.0f, -2.0f, 2.0f, -2.0f, 3.0f, -3.0f, 3.0f, -3.0f, 4.0f, -4.0f, 4.0f, -4.0f, 5.0f, -5.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f, 5.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_float16_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_float16_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f, 0.20000000298023224f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_float16_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 132, 124, 132, 124, 136, 120, 136, 120, 136, 120, 136, 120, 140, 116, 140, 116, 140, 116, 140, 116, 144, 112, 144, 112, 144, 112, 144, 112, 148, 108, 148, 108, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 132, 124, 132, 124, 136, 120, 136, 120, 136, 120, 136, 120, 140, 116, 140, 116, 140, 116, 140, 116, 144, 112, 144, 112, 144, 112, 144, 112, 148, 108, 148, 108, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108, 132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108, 132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis3_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis3_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 132, 124, 136, 120, 136, 120, 140, 116, 140, 116, 144, 112, 144, 112, 148, 108, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108, 132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis2_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis2_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108, 132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 124, 136, 120, 140, 116, 144, 112, 148, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis1_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis1_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148, 124, 120, 116, 112, 108}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51, 51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim1_axis0_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim1_axis0_2;
};

std::vector<MixedTypedExample>& get_examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2() {
static std::vector<MixedTypedExample> examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {132, 136, 140, 144, 148}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {51, 51, 51, 51, 51}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2;
};

std::vector<MixedTypedExample>& get_examples_zero_sized() {
static std::vector<MixedTypedExample> examples_zero_sized = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_relaxed = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_quant8 = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_float16 = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_float16;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_dynamic_output_shape;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_relaxed() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_relaxed = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_dynamic_output_shape_relaxed;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_quant8() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_quant8 = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_dynamic_output_shape_quant8;
};

std::vector<MixedTypedExample>& get_examples_zero_sized_dynamic_output_shape_float16() {
static std::vector<MixedTypedExample> examples_zero_sized_dynamic_output_shape_float16 = {
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
  .operandDimensions = {{0, {0}}, {1, {0}}, {2, {0, 2, 2, 1}}},
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
return examples_zero_sized_dynamic_output_shape_float16;
};

