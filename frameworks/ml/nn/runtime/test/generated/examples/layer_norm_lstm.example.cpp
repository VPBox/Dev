// clang-format off
// Generated file (from: layer_norm_lstm.mod.py). Do not edit
std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}, {2, {-0.451771229505539f, 0.376915663480759f, 0.225425109267235f, 0.232406347990036f, -0.252585828304291f, 0.330421179533005f, 0.017305245622993f, 0.366601228713989f}}, {3, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm;
};

std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}, {2, {-0.451771229505539f, 0.376915663480759f, 0.225425109267235f, 0.232406347990036f, -0.252585828304291f, 0.330421179533005f, 0.017305245622993f, 0.366601228713989f}}, {3, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape;
};

std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_2() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}, {19, {-0.451771229505539f, 0.376915663480759f, 0.225425109267235f, 0.232406347990036f, -0.252585828304291f, 0.330421179533005f, 0.017305245622993f, 0.366601228713989f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}, {2, {-0.645632147789001f, 0.518238246440887f, 0.168679088354111f, 0.555787742137909f, -0.49367481470108f, 0.475847363471985f, 0.106874041259289f, 0.50430965423584f}}, {3, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_2;
};

std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.024407668039203f, 0.128027379512787f, -0.001709178090096f, -0.006924282759428f, 0.08487406373024f, 0.06344497948885f}}, {19, {-0.451771229505539f, 0.376915663480759f, 0.225425109267235f, 0.232406347990036f, -0.252585828304291f, 0.330421179533005f, 0.017305245622993f, 0.366601228713989f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}, {2, {-0.645632147789001f, 0.518238246440887f, 0.168679088354111f, 0.555787742137909f, -0.49367481470108f, 0.475847363471985f, 0.106874041259289f, 0.50430965423584f}}, {3, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2;
};

std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_3() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}, {19, {-0.645632147789001f, 0.518238246440887f, 0.168679088354111f, 0.555787742137909f, -0.49367481470108f, 0.475847363471985f, 0.106874041259289f, 0.50430965423584f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {-0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}, {2, {-0.742560744285583f, 0.579139292240143f, 0.114988230168819f, 0.649957716464996f, -0.686565399169922f, 0.548869132995605f, 0.17313876748085f, 0.587379336357117f}}, {3, {-0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_3;
};

std::vector<MixedTypedExample>& get_examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3() {
static std::vector<MixedTypedExample> examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {4, 5}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {4, 3}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {4}}, {10, {4}}, {11, {4}}, {12, {4}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {4}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {0.5f, 0.6f, 0.7f, -0.8f, -0.9f, 0.1f, 0.2f, 0.3f, -0.4f, 0.5f, -0.8f, 0.7f, -0.6f, 0.5f, -0.4f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {-0.2f, -0.3f, 0.4f, 0.1f, -0.5f, 0.9f, -0.2f, -0.3f, -0.7f, 0.05f, -0.2f, -0.6f}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {0.05f, 0.1f, 0.25f, 0.15f}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {0.03f, 0.15f, 0.22f, 0.38f}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.013764165341854f, 0.140751048922539f, 0.039583537727594f, -0.004039138555527f, 0.139963015913963f, 0.072681039571762f}}, {19, {-0.645632147789001f, 0.518238246440887f, 0.168679088354111f, 0.555787742137909f, -0.49367481470108f, 0.475847363471985f, 0.106874041259289f, 0.50430965423584f}}, {20, {0.1f, 0.2f, 0.3f, 0.5f}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 16}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {-0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}, {2, {-0.742560744285583f, 0.579139292240143f, 0.114988230168819f, 0.649957716464996f, -0.686565399169922f, 0.548869132995605f, 0.17313876748085f, 0.587379336357117f}}, {3, {-0.004592306911945f, 0.155278354883194f, 0.083737745881081f, 0.007527053356171f, 0.161902531981468f, 0.056137066334486f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_NoCifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.3510298f, 0.4261035f, 0.2146365f, 0.2771652f, -0.1885517f, 0.32522f, 0.0203665f, 0.4896766f}}, {3, {0.02129706f, 0.140816242f, 0.0112733059f, -0.0226350538f, 0.0916948169f, 0.0769175813f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.7f, 0.8f, 0.1f, 0.2f, 0.3f, 0.3f, 0.2f, 0.9f, 0.8f, 0.1f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {19, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.3510298f, 0.4261035f, 0.2146365f, 0.2771652f, -0.1885517f, 0.32522f, 0.0203665f, 0.4896766f}}, {3, {0.02129706f, 0.140816242f, 0.0112733059f, -0.0226350538f, 0.0916948169f, 0.0769175813f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm_2() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.02129706f, 0.140816242f, 0.0112733059f, -0.0226350538f, 0.0916948169f, 0.0769175813f}}, {19, {-0.3510298f, 0.4261035f, 0.2146365f, 0.2771652f, -0.1885517f, 0.32522f, 0.0203665f, 0.4896766f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.5069088f, 0.5386363f, 0.1980069f, 0.5355753f, -0.3866257f, 0.4749442f, 0.1074765f, 0.7124508f}}, {3, {0.0132302344f, 0.152308047f, 0.0346313119f, -0.0269966982f, 0.149707705f, 0.094149217f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm_2;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.8f, 0.1f, 0.2f, 0.4f, 0.5f, 0.1f, 0.5f, 0.2f, 0.4f, 0.2f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.02129706f, 0.140816242f, 0.0112733059f, -0.0226350538f, 0.0916948169f, 0.0769175813f}}, {19, {-0.3510298f, 0.4261035f, 0.2146365f, 0.2771652f, -0.1885517f, 0.32522f, 0.0203665f, 0.4896766f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.5069088f, 0.5386363f, 0.1980069f, 0.5355753f, -0.3866257f, 0.4749442f, 0.1074765f, 0.7124508f}}, {3, {0.0132302344f, 0.152308047f, 0.0346313119f, -0.0269966982f, 0.149707705f, 0.094149217f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_2;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm_3() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0132302344f, 0.152308047f, 0.0346313119f, -0.0269966982f, 0.149707705f, 0.094149217f}}, {19, {-0.5069088f, 0.5386363f, 0.1980069f, 0.5355753f, -0.3866257f, 0.4749442f, 0.1074765f, 0.7124508f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.5736622f, 0.5952501f, 0.129295f, 0.711027f, -0.5323033f, 0.5556133f, 0.1800992f, 0.7845056f}}, {3, {-0.0123688057f, 0.165790111f, 0.0893077999f, -0.0103429332f, 0.173016444f, 0.0720508844f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm_3;
};

std::vector<MixedTypedExample>& get_examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3() {
static std::vector<MixedTypedExample> examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3 = {
// Begin of an example
{
.operands = {
//Input(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 5}}, {1, {0, 0}}, {2, {4, 5}}, {3, {4, 5}}, {4, {4, 5}}, {5, {0, 0}}, {6, {4, 3}}, {7, {4, 3}}, {8, {4, 3}}, {9, {0}}, {10, {4}}, {11, {4}}, {12, {0}}, {13, {4}}, {14, {4}}, {15, {4}}, {16, {3, 4}}, {17, {0}}, {18, {2, 3}}, {19, {2, 4}}, {20, {0}}, {21, {4}}, {22, {4}}, {23, {4}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.2f, 0.7f, 0.7f, 0.1f, 0.7f, 0.6f, 0.9f, 0.2f, 0.5f, 0.7f}}, {1, {}}, {2, {-0.6f, -0.1f, 0.3f, 0.2f, 0.9f, -0.5f, -0.2f, -0.4f, 0.3f, -0.8f, -0.4f, 0.3f, -0.5f, -0.4f, -0.6f, 0.3f, -0.4f, -0.6f, -0.5f, -0.5f}}, {3, {-0.4f, -0.3f, -0.2f, -0.1f, -0.5f, 0.5f, -0.2f, -0.3f, -0.2f, -0.6f, 0.6f, -0.1f, -0.4f, -0.3f, -0.7f, 0.7f, -0.9f, -0.5f, 0.8f, 0.6f}}, {4, {-0.8f, -0.4f, -0.2f, -0.9f, -0.1f, -0.7f, 0.3f, -0.3f, -0.8f, -0.2f, 0.6f, -0.2f, 0.4f, -0.7f, -0.3f, -0.5f, 0.1f, 0.5f, -0.6f, -0.4f}}, {5, {}}, {6, {-0.5f, -0.3f, -0.5f, -0.2f, 0.6f, 0.4f, 0.9f, 0.3f, -0.1f, 0.2f, 0.5f, 0.2f}}, {7, {-0.3f, 0.2f, 0.1f, -0.3f, 0.8f, -0.08f, -0.2f, 0.3f, 0.8f, -0.6f, -0.1f, 0.2f}}, {8, {0.3f, -0.1f, 0.1f, -0.2f, -0.5f, -0.7f, -0.2f, -0.6f, -0.1f, -0.4f, -0.7f, -0.2f}}, {9, {}}, {10, {-0.02f, -0.15f, -0.25f, -0.03f}}, {11, {0.1f, -0.1f, -0.5f, 0.05f}}, {12, {}}, {13, {0.1f, -0.3f, -0.2f, 0.1f}}, {14, {-0.05f, 0.72f, 0.25f, 0.08f}}, {15, {0.05f, -0.01f, 0.2f, 0.1f}}, {16, {-0.1f, 0.2f, 0.01f, -0.2f, 0.1f, 0.5f, 0.3f, 0.08f, 0.07f, 0.2f, -0.4f, 0.2f}}, {17, {}}, {18, {0.0132302344f, 0.152308047f, 0.0346313119f, -0.0269966982f, 0.149707705f, 0.094149217f}}, {19, {-0.5069088f, 0.5386363f, 0.1980069f, 0.5355753f, -0.3866257f, 0.4749442f, 0.1074765f, 0.7124508f}}, {20, {}}, {21, {0.2f, 0.2f, 0.4f, 0.3f}}, {22, {0.7f, 0.2f, 0.3f, 0.8f}}, {23, {0.6f, 0.2f, 0.2f, 0.5f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
},
//Output(s)
{ // See tools/test_generator/include/TestHarness.h:MixedTyped
  // int -> Dimensions map
  .operandDimensions = {{0, {2, 12}}, {1, {2, 3}}, {2, {2, 4}}, {3, {2, 3}}},
  // int -> FLOAT32 map
  .float32Operands = {{0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {1, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}, {2, {-0.5736622f, 0.5952501f, 0.129295f, 0.711027f, -0.5323033f, 0.5556133f, 0.1800992f, 0.7845056f}}, {3, {-0.0123688057f, 0.165790111f, 0.0893077999f, -0.0103429332f, 0.173016444f, 0.0720508844f}}},
  // int -> INT32 map
  .int32Operands = {},
  // int -> QUANT8_ASYMM map
  .quant8AsymmOperands = {},
  // int -> QUANT16_SYMM map
  .quant16SymmOperands = {},
  // int -> FLOAT16 map
  .float16Operands = {},
  // int -> BOOL8 map
  .bool8Operands = {},
  // int -> QUANT8_SYMM_PER_CHANNEL map
  .quant8ChannelOperands = {},
  // int -> QUANT16_ASYMM map
  .quant16AsymmOperands = {},
  // int -> QUANT8_SYMM map
  .quant8SymmOperands = {},
}
},
}, // End of an example
};
return examples_CifgPeepholeProjectionNoClippingLayerNormLstm_dynamic_output_shape_3;
};

