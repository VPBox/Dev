// clang-format off
// Generated file (from: unidirectional_sequence_lstm_f16_norm_peephole_projection.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples() {
static std::vector<MixedTypedExample> examples = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f, 0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f, 0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f, 0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f, -0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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
  .operandDimensions = {{0, {3, 2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f, 0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f, 0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {3, 2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {{0, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f, 0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f, -0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
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

