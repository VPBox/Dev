// clang-format off
// Generated file (from: transpose_float_1_relaxed.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 4, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {2, 3, 4, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {4, 2, 3, 5}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

