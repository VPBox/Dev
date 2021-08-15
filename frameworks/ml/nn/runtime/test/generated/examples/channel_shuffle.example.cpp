// clang-format off
// Generated file (from: channel_shuffle.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples_dim4_axis0() {
static std::vector<MixedTypedExample> examples_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis1() {
static std::vector<MixedTypedExample> examples_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis2() {
static std::vector<MixedTypedExample> examples_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis3() {
static std::vector<MixedTypedExample> examples_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis0() {
static std::vector<MixedTypedExample> examples_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis1() {
static std::vector<MixedTypedExample> examples_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dim2_axis0() {
static std::vector<MixedTypedExample> examples_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dim2_axis1() {
static std::vector<MixedTypedExample> examples_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis0() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis1() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis2() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis3() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis0() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis1() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_relaxed_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim2_axis0() {
static std::vector<MixedTypedExample> examples_relaxed_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim2_axis1() {
static std::vector<MixedTypedExample> examples_relaxed_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_relaxed_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_relaxed_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_relaxed_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis0() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis1() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis2() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis3() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis0() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis1() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_quant8_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim2_axis0() {
static std::vector<MixedTypedExample> examples_quant8_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim2_axis1() {
static std::vector<MixedTypedExample> examples_quant8_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_quant8_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_quant8_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_quant8_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis0() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis1() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis2() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis3() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_float16_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_float16_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis0() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis1() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_float16_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_float16_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim2_axis0() {
static std::vector<MixedTypedExample> examples_float16_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_float16_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_float16_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim2_axis1() {
static std::vector<MixedTypedExample> examples_float16_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_float16_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_float16_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_float16_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_float16_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_float16_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim2_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim2_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim2_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim2_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_relaxed_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_relaxed_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_relaxed_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 255, 255, 255, 255, 255, 255, 144, 192, 240, 255, 255, 255, 255, 255, 255, 255, 255, 255, 160, 208, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 255, 255, 255, 255, 255, 255, 148, 196, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 255, 255, 255, 255, 255, 255, 152, 200, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 255, 255, 255, 255, 255, 255, 156, 204, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 132, 180, 228, 255, 255, 255, 136, 184, 232, 255, 255, 255, 140, 188, 236, 255, 255, 255, 144, 192, 240, 255, 255, 255, 148, 196, 244, 255, 255, 255, 152, 200, 248, 255, 255, 255, 156, 204, 252, 255, 255, 255, 160, 208, 255, 255, 255, 255, 164, 212, 255, 255, 255, 255, 168, 216, 255, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 255, 255, 255, 144, 192, 240, 255, 255, 255, 160, 208, 255, 255, 255, 255, 132, 180, 228, 255, 255, 255, 148, 196, 244, 255, 255, 255, 164, 212, 255, 255, 255, 255, 136, 184, 232, 255, 255, 255, 152, 200, 248, 255, 255, 255, 168, 216, 255, 255, 255, 255, 140, 188, 236, 255, 255, 255, 156, 204, 252, 255, 255, 255, 172, 220, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim2_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 132, 180, 228, 136, 184, 232, 140, 188, 236, 144, 192, 240, 148, 196, 244, 152, 200, 248, 156, 204, 252, 160, 208, 255, 164, 212, 255, 168, 216, 255, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 176, 224, 144, 192, 240, 160, 208, 255, 132, 180, 228, 148, 196, 244, 164, 212, 255, 136, 184, 232, 152, 200, 248, 168, 216, 255, 140, 188, 236, 156, 204, 252, 172, 220, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim2_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255, 255, 255, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172, 176, 192, 208, 180, 196, 212, 184, 200, 216, 188, 204, 220, 224, 240, 255, 228, 244, 255, 232, 248, 255, 236, 252, 255}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_quant8_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_quant8_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{0, {128, 144, 160, 132, 148, 164, 136, 152, 168, 140, 156, 172}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_dim1_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 108.0f, 120.0f, 132.0f, 76.0f, 88.0f, 100.0f, 112.0f, 124.0f, 136.0f, 80.0f, 92.0f, 104.0f, 116.0f, 128.0f, 140.0f, 73.0f, 85.0f, 97.0f, 109.0f, 121.0f, 133.0f, 77.0f, 89.0f, 101.0f, 113.0f, 125.0f, 137.0f, 81.0f, 93.0f, 105.0f, 117.0f, 129.0f, 141.0f, 74.0f, 86.0f, 98.0f, 110.0f, 122.0f, 134.0f, 78.0f, 90.0f, 102.0f, 114.0f, 126.0f, 138.0f, 82.0f, 94.0f, 106.0f, 118.0f, 130.0f, 142.0f, 75.0f, 87.0f, 99.0f, 111.0f, 123.0f, 135.0f, 79.0f, 91.0f, 103.0f, 115.0f, 127.0f, 139.0f, 83.0f, 95.0f, 107.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis2;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 73.0f, 85.0f, 97.0f, 74.0f, 86.0f, 98.0f, 75.0f, 87.0f, 99.0f, 76.0f, 88.0f, 100.0f, 77.0f, 89.0f, 101.0f, 78.0f, 90.0f, 102.0f, 79.0f, 91.0f, 103.0f, 80.0f, 92.0f, 104.0f, 81.0f, 93.0f, 105.0f, 82.0f, 94.0f, 106.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 109.0f, 121.0f, 133.0f, 110.0f, 122.0f, 134.0f, 111.0f, 123.0f, 135.0f, 112.0f, 124.0f, 136.0f, 113.0f, 125.0f, 137.0f, 114.0f, 126.0f, 138.0f, 115.0f, 127.0f, 139.0f, 116.0f, 128.0f, 140.0f, 117.0f, 129.0f, 141.0f, 118.0f, 130.0f, 142.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f, 72.0f, 84.0f, 96.0f, 76.0f, 88.0f, 100.0f, 80.0f, 92.0f, 104.0f, 73.0f, 85.0f, 97.0f, 77.0f, 89.0f, 101.0f, 81.0f, 93.0f, 105.0f, 74.0f, 86.0f, 98.0f, 78.0f, 90.0f, 102.0f, 82.0f, 94.0f, 106.0f, 75.0f, 87.0f, 99.0f, 79.0f, 91.0f, 103.0f, 83.0f, 95.0f, 107.0f, 108.0f, 120.0f, 132.0f, 112.0f, 124.0f, 136.0f, 116.0f, 128.0f, 140.0f, 109.0f, 121.0f, 133.0f, 113.0f, 125.0f, 137.0f, 117.0f, 129.0f, 141.0f, 110.0f, 122.0f, 134.0f, 114.0f, 126.0f, 138.0f, 118.0f, 130.0f, 142.0f, 111.0f, 123.0f, 135.0f, 115.0f, 127.0f, 139.0f, 119.0f, 131.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis3() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis3;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim4_axis3_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim4_axis3_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f, 72.0f, 73.0f, 74.0f, 75.0f, 76.0f, 77.0f, 78.0f, 79.0f, 80.0f, 81.0f, 82.0f, 83.0f, 84.0f, 85.0f, 86.0f, 87.0f, 88.0f, 89.0f, 90.0f, 91.0f, 92.0f, 93.0f, 94.0f, 95.0f, 96.0f, 97.0f, 98.0f, 99.0f, 100.0f, 101.0f, 102.0f, 103.0f, 104.0f, 105.0f, 106.0f, 107.0f, 108.0f, 109.0f, 110.0f, 111.0f, 112.0f, 113.0f, 114.0f, 115.0f, 116.0f, 117.0f, 118.0f, 119.0f, 120.0f, 121.0f, 122.0f, 123.0f, 124.0f, 125.0f, 126.0f, 127.0f, 128.0f, 129.0f, 130.0f, 131.0f, 132.0f, 133.0f, 134.0f, 135.0f, 136.0f, 137.0f, 138.0f, 139.0f, 140.0f, 141.0f, 142.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f, 72.0f, 76.0f, 80.0f, 73.0f, 77.0f, 81.0f, 74.0f, 78.0f, 82.0f, 75.0f, 79.0f, 83.0f, 84.0f, 88.0f, 92.0f, 85.0f, 89.0f, 93.0f, 86.0f, 90.0f, 94.0f, 87.0f, 91.0f, 95.0f, 96.0f, 100.0f, 104.0f, 97.0f, 101.0f, 105.0f, 98.0f, 102.0f, 106.0f, 99.0f, 103.0f, 107.0f, 108.0f, 112.0f, 116.0f, 109.0f, 113.0f, 117.0f, 110.0f, 114.0f, 118.0f, 111.0f, 115.0f, 119.0f, 120.0f, 124.0f, 128.0f, 121.0f, 125.0f, 129.0f, 122.0f, 126.0f, 130.0f, 123.0f, 127.0f, 131.0f, 132.0f, 136.0f, 140.0f, 133.0f, 137.0f, 141.0f, 134.0f, 138.0f, 142.0f, 135.0f, 139.0f, 143.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim4_axis3_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 36.0f, 48.0f, 60.0f, 4.0f, 16.0f, 28.0f, 40.0f, 52.0f, 64.0f, 8.0f, 20.0f, 32.0f, 44.0f, 56.0f, 68.0f, 1.0f, 13.0f, 25.0f, 37.0f, 49.0f, 61.0f, 5.0f, 17.0f, 29.0f, 41.0f, 53.0f, 65.0f, 9.0f, 21.0f, 33.0f, 45.0f, 57.0f, 69.0f, 2.0f, 14.0f, 26.0f, 38.0f, 50.0f, 62.0f, 6.0f, 18.0f, 30.0f, 42.0f, 54.0f, 66.0f, 10.0f, 22.0f, 34.0f, 46.0f, 58.0f, 70.0f, 3.0f, 15.0f, 27.0f, 39.0f, 51.0f, 63.0f, 7.0f, 19.0f, 31.0f, 43.0f, 55.0f, 67.0f, 11.0f, 23.0f, 35.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 37.0f, 49.0f, 61.0f, 38.0f, 50.0f, 62.0f, 39.0f, 51.0f, 63.0f, 40.0f, 52.0f, 64.0f, 41.0f, 53.0f, 65.0f, 42.0f, 54.0f, 66.0f, 43.0f, 55.0f, 67.0f, 44.0f, 56.0f, 68.0f, 45.0f, 57.0f, 69.0f, 46.0f, 58.0f, 70.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f, 36.0f, 48.0f, 60.0f, 40.0f, 52.0f, 64.0f, 44.0f, 56.0f, 68.0f, 37.0f, 49.0f, 61.0f, 41.0f, 53.0f, 65.0f, 45.0f, 57.0f, 69.0f, 38.0f, 50.0f, 62.0f, 42.0f, 54.0f, 66.0f, 46.0f, 58.0f, 70.0f, 39.0f, 51.0f, 63.0f, 43.0f, 55.0f, 67.0f, 47.0f, 59.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis2() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim3_axis2_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim3_axis2_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 46.0f, 47.0f, 48.0f, 49.0f, 50.0f, 51.0f, 52.0f, 53.0f, 54.0f, 55.0f, 56.0f, 57.0f, 58.0f, 59.0f, 60.0f, 61.0f, 62.0f, 63.0f, 64.0f, 65.0f, 66.0f, 67.0f, 68.0f, 69.0f, 70.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f, 36.0f, 40.0f, 44.0f, 37.0f, 41.0f, 45.0f, 38.0f, 42.0f, 46.0f, 39.0f, 43.0f, 47.0f, 48.0f, 52.0f, 56.0f, 49.0f, 53.0f, 57.0f, 50.0f, 54.0f, 58.0f, 51.0f, 55.0f, 59.0f, 60.0f, 64.0f, 68.0f, 61.0f, 65.0f, 69.0f, 62.0f, 66.0f, 70.0f, 63.0f, 67.0f, 71.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim3_axis2_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim2_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim2_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim2_axis0;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim2_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim2_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 1.0f, 13.0f, 25.0f, 2.0f, 14.0f, 26.0f, 3.0f, 15.0f, 27.0f, 4.0f, 16.0f, 28.0f, 5.0f, 17.0f, 29.0f, 6.0f, 18.0f, 30.0f, 7.0f, 19.0f, 31.0f, 8.0f, 20.0f, 32.0f, 9.0f, 21.0f, 33.0f, 10.0f, 22.0f, 34.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 12.0f, 24.0f, 4.0f, 16.0f, 28.0f, 8.0f, 20.0f, 32.0f, 1.0f, 13.0f, 25.0f, 5.0f, 17.0f, 29.0f, 9.0f, 21.0f, 33.0f, 2.0f, 14.0f, 26.0f, 6.0f, 18.0f, 30.0f, 10.0f, 22.0f, 34.0f, 3.0f, 15.0f, 27.0f, 7.0f, 19.0f, 31.0f, 11.0f, 23.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim2_axis0_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim2_axis1() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim2_axis1 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim2_axis1;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim2_axis1_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim2_axis1_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f, 12.0f, 16.0f, 20.0f, 13.0f, 17.0f, 21.0f, 14.0f, 18.0f, 22.0f, 15.0f, 19.0f, 23.0f, 24.0f, 28.0f, 32.0f, 25.0f, 29.0f, 33.0f, 26.0f, 30.0f, 34.0f, 27.0f, 31.0f, 35.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim2_axis1_neg;
};

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim1_axis0() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim1_axis0 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

std::vector<MixedTypedExample>& get_examples_dynamic_output_shape_float16_dim1_axis0_neg() {
static std::vector<MixedTypedExample> examples_dynamic_output_shape_float16_dim1_axis0_neg = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {12}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.0f, 4.0f, 8.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 10.0f, 3.0f, 7.0f, 11.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_float16_dim1_axis0_neg;
};

