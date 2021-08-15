// clang-format off
// Generated file (from: axis_aligned_bbox_transform.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.20000000298023224f, 0.20000000298023224f, 0.10000000149011612f, 0.10000000149011612f, 0.30000001192092896f, -0.10000000149011612f, -0.20000000298023224f, 0.10000000149011612f, -0.5f, 0.20000000298023224f, 0.20000000298023224f, -0.5f, -0.10000000149011612f, -0.10000000149011612f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2000000476837158f, 0.20000000298023224f, 0.20000000298023224f, -3.0f, -4.0f, 1.0f, -0.5f, 0.30000001192092896f, 0.5f, 0.30000001192092896f, -0.20000000298023224f, 1.100000023841858f, -0.800000011920929f, 0.10000000149011612f, 0.05000000074505806f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {144.22434997558594f, 191.27606201171875f, 475.775634765625f, 500.72393798828125f, 217.1903839111328f, 107.27606201171875f, 462.80963134765625f, 416.72393798828125f, 118.77859497070312f, 60.39673614501953f, 121.22140502929688f, 61.003265380859375f, 108.61750793457031f, 50.35723114013672f, 132.9824981689453f, 70.4427719116211f, 0.0f, 0.0f, 23.591407775878906f, 60.774227142333984f, 18.884349822998047f, 45.482086181640625f, 21.115650177001953f, 54.517913818359375f, 117.51063537597656f, 209.80947875976562f, 122.48934936523438f, 212.1905059814453f, 132.50704956054688f, 12.833123207092285f, 256.0f, 227.16685485839844f, 0.0f, 243.13748168945312f, 512.0f, 1024.0f, 512.0f, 568.7958374023438f, 512.0f, 1024.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{1, {132, 132, 130, 130, 134, 126, 124, 130, 118, 132, 132, 118, 126, 126, 178, 188, 118, 118, 148, 148, 138, 138, 98, 104, 132, 132, 68, 48, 148, 118, 134, 138, 134, 124, 150, 112, 130, 129, 118, 118}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {800, 1200, 3200, 3440, 960, 480, 976, 488, 80, 160, 160, 400, 400, 960, 1200, 2000, 3200, 800, 8000, 16000}}, {3, {4096, 4096, 1024, 2048, 2048, 2048, 8192, 4096}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {1154, 1530, 3806, 4006, 1738, 858, 3702, 3334, 950, 483, 970, 488, 869, 403, 1064, 564, 0, 0, 189, 486, 151, 364, 169, 436, 940, 1678, 980, 1698, 1060, 103, 2048, 1817, 0, 1945, 4096, 8192, 4096, 4550, 4096, 8192}}},
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.20000000298023224f, 0.20000000298023224f, 0.10000000149011612f, 0.10000000149011612f, 0.30000001192092896f, -0.10000000149011612f, -0.20000000298023224f, 0.10000000149011612f, -0.5f, 0.20000000298023224f, 0.20000000298023224f, -0.5f, -0.10000000149011612f, -0.10000000149011612f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2000000476837158f, 0.20000000298023224f, 0.20000000298023224f, -3.0f, -4.0f, 1.0f, -0.5f, 0.30000001192092896f, 0.5f, 0.30000001192092896f, -0.20000000298023224f, 1.100000023841858f, -0.800000011920929f, 0.10000000149011612f, 0.05000000074505806f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 128.0f, 256.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {144.22434997558594f, 191.27606201171875f, 475.775634765625f, 500.72393798828125f, 217.1903839111328f, 107.27606201171875f, 462.80963134765625f, 416.72393798828125f, 118.77859497070312f, 60.39673614501953f, 121.22140502929688f, 61.003265380859375f, 108.61750793457031f, 50.35723114013672f, 132.9824981689453f, 70.4427719116211f, 0.0f, 0.0f, 23.591407775878906f, 60.774227142333984f, 18.884349822998047f, 45.482086181640625f, 21.115650177001953f, 54.517913818359375f, 117.51063537597656f, 209.80947875976562f, 122.48934936523438f, 212.1905059814453f, 132.50704956054688f, 12.833123207092285f, 256.0f, 227.16685485839844f, 0.0f, 243.13748168945312f, 512.0f, 1024.0f, 512.0f, 568.7958374023438f, 512.0f, 1024.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {4, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 1, 2, 2, 3}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{1, {132, 132, 130, 130, 134, 126, 124, 130, 118, 132, 132, 118, 126, 126, 178, 188, 118, 118, 148, 148, 138, 138, 98, 104, 132, 132, 68, 48, 148, 118, 134, 138, 134, 124, 150, 112, 130, 129, 118, 118}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {800, 1200, 3200, 3440, 960, 480, 976, 488, 80, 160, 160, 400, 400, 960, 1200, 2000, 3200, 800, 8000, 16000}}, {3, {4096, 4096, 1024, 2048, 2048, 2048, 8192, 4096}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {1154, 1530, 3806, 4006, 1738, 858, 3702, 3334, 950, 483, 970, 488, 869, 403, 1064, 564, 0, 0, 189, 486, 151, 364, 169, 436, 940, 1678, 980, 1698, 1060, 103, 2048, 1817, 0, 1945, 4096, 8192, 4096, 4550, 4096, 8192}}},
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.20000000298023224f, 0.20000000298023224f, 0.10000000149011612f, 0.10000000149011612f, 0.30000001192092896f, -0.10000000149011612f, -0.20000000298023224f, 0.10000000149011612f, -0.5f, 0.20000000298023224f, 0.20000000298023224f, -0.5f, -0.10000000149011612f, -0.10000000149011612f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2000000476837158f, 0.20000000298023224f, 0.20000000298023224f, -3.0f, -4.0f, 1.0f, -0.5f, 0.30000001192092896f, 0.5f, 0.30000001192092896f, -0.20000000298023224f, 1.100000023841858f, -0.800000011920929f, 0.10000000149011612f, 0.05000000074505806f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {144.22434997558594f, 191.27606201171875f, 475.775634765625f, 500.72393798828125f, 217.1903839111328f, 107.27606201171875f, 462.80963134765625f, 416.72393798828125f, 118.77859497070312f, 60.39673614501953f, 121.22140502929688f, 61.003265380859375f, 108.61750793457031f, 50.35723114013672f, 132.9824981689453f, 70.4427719116211f, 0.0f, 0.0f, 23.591407775878906f, 60.774227142333984f, 18.884349822998047f, 45.482086181640625f, 21.115650177001953f, 54.517913818359375f, 117.51063537597656f, 209.80947875976562f, 122.48934936523438f, 212.1905059814453f, 132.50704956054688f, 12.833123207092285f, 256.0f, 227.16685485839844f, 0.0f, 243.13748168945312f, 512.0f, 1024.0f, 512.0f, 568.7958374023438f, 512.0f, 1024.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{1, {132, 132, 130, 130, 134, 126, 124, 130, 118, 132, 132, 118, 126, 126, 178, 188, 118, 118, 148, 148, 138, 138, 98, 104, 132, 132, 68, 48, 148, 118, 134, 138, 134, 124, 150, 112, 130, 129, 118, 118}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {800, 1200, 3200, 3440, 960, 480, 976, 488, 80, 160, 160, 400, 400, 960, 1200, 2000, 3200, 800, 8000, 16000}}, {3, {4096, 4096, 256, 256, 1024, 2048, 256, 256, 256, 256, 2048, 2048, 8192, 4096}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {1154, 1530, 3806, 4006, 1738, 858, 3702, 3334, 950, 483, 970, 488, 869, 403, 1064, 564, 0, 0, 189, 486, 151, 364, 169, 436, 940, 1678, 980, 1698, 1060, 103, 2048, 1817, 0, 1945, 4096, 8192, 4096, 4550, 4096, 8192}}},
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.2f, 0.2f, 0.1f, 0.1f, 0.3f, -0.1f, -0.2f, 0.1f, -0.5f, 0.2f, 0.2f, -0.5f, -0.1f, -0.1f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2f, 0.2f, 0.2f, -3.0f, -4.0f, 1.0f, -0.5f, 0.3f, 0.5f, 0.3f, -0.2f, 1.1f, -0.8f, 0.1f, 0.05f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {144.22435f, 191.276062f, 475.775635f, 500.723938f, 217.190384f, 107.276062f, 462.809631f, 416.723938f, 118.778594f, 60.396736f, 121.221406f, 61.003266f, 108.617508f, 50.357232f, 132.982498f, 70.442772f, 0.0f, 0.0f, 23.59140714f, 60.77422571f, 18.88435f, 45.48208571f, 21.11565f, 54.51791429f, 117.51063714f, 209.80948286f, 122.48935143f, 212.19050857f, 132.50705143f, 12.83312286f, 255.99999571f, 227.16685714f, 0.0f, 243.1374815f, 512.0f, 1024.0f, 512.0f, 568.7958375f, 512.0f, 1024.0f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {100.0f, 150.0f, 400.0f, 430.0f, 120.0f, 60.0f, 122.0f, 61.0f, 10.0f, 20.0f, 20.0f, 50.0f, 50.0f, 120.0f, 150.0f, 250.0f, 400.0f, 100.0f, 1000.0f, 2000.0f}}, {1, {0.20000000298023224f, 0.20000000298023224f, 0.10000000149011612f, 0.10000000149011612f, 0.30000001192092896f, -0.10000000149011612f, -0.20000000298023224f, 0.10000000149011612f, -0.5f, 0.20000000298023224f, 0.20000000298023224f, -0.5f, -0.10000000149011612f, -0.10000000149011612f, 2.5f, 3.0f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -1.5f, -1.2000000476837158f, 0.20000000298023224f, 0.20000000298023224f, -3.0f, -4.0f, 1.0f, -0.5f, 0.30000001192092896f, 0.5f, 0.30000001192092896f, -0.20000000298023224f, 1.100000023841858f, -0.800000011920929f, 0.10000000149011612f, 0.05000000074505806f, -0.5f, -0.5f}}, {3, {512.0f, 512.0f, 32.0f, 32.0f, 128.0f, 256.0f, 32.0f, 32.0f, 32.0f, 32.0f, 256.0f, 256.0f, 1024.0f, 512.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {144.22434997558594f, 191.27606201171875f, 475.775634765625f, 500.72393798828125f, 217.1903839111328f, 107.27606201171875f, 462.80963134765625f, 416.72393798828125f, 118.77859497070312f, 60.39673614501953f, 121.22140502929688f, 61.003265380859375f, 108.61750793457031f, 50.35723114013672f, 132.9824981689453f, 70.4427719116211f, 0.0f, 0.0f, 23.591407775878906f, 60.774227142333984f, 18.884349822998047f, 45.482086181640625f, 21.115650177001953f, 54.517913818359375f, 117.51063537597656f, 209.80947875976562f, 122.48934936523438f, 212.1905059814453f, 132.50704956054688f, 12.833123207092285f, 256.0f, 227.16685485839844f, 0.0f, 243.13748168945312f, 512.0f, 1024.0f, 512.0f, 568.7958374023438f, 512.0f, 1024.0f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {5, 4}}, {1, {5, 8}}, {2, {5}}, {3, {7, 2}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {{2, {0, 2, 5, 5, 6}}},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {{1, {132, 132, 130, 130, 134, 126, 124, 130, 118, 132, 132, 118, 126, 126, 178, 188, 118, 118, 148, 148, 138, 138, 98, 104, 132, 132, 68, 48, 148, 118, 134, 138, 134, 124, 150, 112, 130, 129, 118, 118}}},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {800, 1200, 3200, 3440, 960, 480, 976, 488, 80, 160, 160, 400, 400, 960, 1200, 2000, 3200, 800, 8000, 16000}}, {3, {4096, 4096, 256, 256, 1024, 2048, 256, 256, 256, 256, 2048, 2048, 8192, 4096}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {5, 8}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {{0, {1154, 1530, 3806, 4006, 1738, 858, 3702, 3334, 950, 483, 970, 488, 869, 403, 1064, 564, 0, 0, 189, 486, 151, 364, 169, 436, 940, 1678, 980, 1698, 1060, 103, 2048, 1817, 0, 1945, 4096, 8192, 4096, 4550, 4096, 8192}}},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_dynamic_output_shape_quant8_2;
};

