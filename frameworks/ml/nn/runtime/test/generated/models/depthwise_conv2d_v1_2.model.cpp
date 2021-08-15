// clang-format off
// Generated file (from: depthwise_conv2d_v1_2.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.0f, 0.20000000298023224f, 0.0f, 0.25f, 0.0f, 0.0f, 0.30000001192092896f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.10000000149011612f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 16);
  static _Float16 op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type24);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.1f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static uint8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 30, 25, 0, 0, 0, 25, 10, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {200, 400, 600, 800};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.1f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type29);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type30);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.0f, 0.20000000298023224f, 0.0f, 0.25f, 0.0f, 0.0f, 0.30000001192092896f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.10000000149011612f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 16);
  static _Float16 op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type32);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type33);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type32);
  // Phase 2, operations
  static uint8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 30, 25, 0, 0, 0, 25, 10, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {200, 400, 600, 800};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type29);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type32);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type32);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.0f, 0.20000000298023224f, 0.0f, 0.25f, 0.0f, 0.0f, 0.30000001192092896f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.10000000149011612f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 16);
  static _Float16 op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type37);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static uint8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 30, 25, 0, 0, 0, 25, 10, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {200, 400, 600, 800};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.0f, 0.2f, 0.0f, 0.25f, 0.0f, 0.0f, 0.3f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.1f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type29);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.0f, 0.20000000298023224f, 0.0f, 0.25f, 0.0f, 0.0f, 0.30000001192092896f, 0.25f, 0.0f, 0.0f, 0.0f, 0.25f, 0.10000000149011612f, 0.0f, 0.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 16);
  static _Float16 op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type37);
  // Phase 2, operations
  static int8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 60, 25, 0, 0, 0, 25, 20, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {200, 800, 600, 1600};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static uint8_t op2_init[] = {25, 0, 20, 0, 25, 0, 0, 30, 25, 0, 0, 0, 25, 10, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {200, 400, 600, 800};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type27);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type29);
  auto op2 = model->addOperand(&type16);
  auto op3 = model->addOperand(&type17);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type20(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.01f, 0.005f, 0.01f, 0.005f},3));
  OperandType type23(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0001f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type22);
  auto op3 = model->addOperand(&type23);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.01f, 0);
  OperandType type26(Type::TENSOR_INT32, {4}, 0.005f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type31);
  auto op2 = model->addOperand(&type25);
  auto op3 = model->addOperand(&type26);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 1, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 1, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type39(Type::TENSOR_FLOAT16, {1, 2, 1, 4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type38);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type39);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 16);
  static _Float16 op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type43);
  // Phase 2, operations
  static uint8_t op21_init[] = {130, 132, 134, 136, 110, 148, 106, 152, 138, 140, 142, 144, 154, 100, 158, 96};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 16);
  static int32_t op31_init[] = {4, 8, 12, 16};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 100);
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type43);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 6, 16, -18, 40, -22, 48, 10, 24, 14, 32, 26, -56, 30, -64};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 16, 12, 32};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 1, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 2, 1, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type39(Type::TENSOR_FLOAT16, {1, 2, 1, 4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type38);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type39);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type43);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 100);
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type43);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type47);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type47);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type48);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type49);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 16);
  static _Float16 op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type51);
  // Phase 2, operations
  static uint8_t op21_init[] = {130, 132, 134, 136, 110, 148, 106, 152, 138, 140, 142, 144, 154, 100, 158, 96};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 16);
  static int32_t op31_init[] = {4, 8, 12, 16};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type51);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 6, 16, -18, 40, -22, 48, 10, 24, 14, 32, 26, -56, 30, -64};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 16, 12, 32};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type47);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type47);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 4, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type48);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type49);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type51);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type51);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type38);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 16);
  static _Float16 op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static uint8_t op21_init[] = {130, 132, 134, 136, 110, 148, 106, 152, 138, 140, 142, 144, 154, 100, 158, 96};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 16);
  static int32_t op31_init[] = {4, 8, 12, 16};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 6, 16, -18, 40, -22, 48, 10, 24, 14, 32, 26, -56, 30, -64};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 16, 12, 32};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type38);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 128);
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type40);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 16);
  static float op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type48);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 16);
  static _Float16 op31_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static uint8_t op21_init[] = {130, 132, 134, 136, 110, 148, 106, 152, 138, 140, 142, 144, 154, 100, 158, 96};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 16);
  static int32_t op31_init[] = {4, 8, 12, 16};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 6, 16, -18, 40, -22, 48, 10, 24, 14, 32, 26, -56, 30, -64};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 16, 12, 32};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type46);
  auto op21 = model->addOperand(&type2);
  auto op31 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type48);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type42(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type41);
  auto op31 = model->addOperand(&type42);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.25f, 0.5f, 0.25f},3));
  OperandType type45(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.5f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 100);
  // Phase 1, operands
  auto op11 = model->addOperand(&type50);
  auto op21 = model->addOperand(&type44);
  auto op31 = model->addOperand(&type45);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type9);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type9);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type55);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 8);
  static _Float16 op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type59);
  // Phase 2, operations
  static uint8_t op22_init[] = {130, 128, 130, 136, 130, 128, 130, 136};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 8);
  static int32_t op32_init[] = {1600, 3200};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 2.0f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type59);
  // Phase 2, operations
  static int8_t op22_init[] = {2, 0, 2, 4, 2, 0, 2, 4};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 8);
  static int32_t op32_init[] = {1600, 1600};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type55);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type59);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 2.0f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type59);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {1, 2, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {1, 2, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  OperandType type64(Type::TENSOR_FLOAT16, {1, 2, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type64);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 8);
  static _Float16 op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op22_init[] = {130, 128, 130, 136, 130, 128, 130, 136};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 8);
  static int32_t op32_init[] = {1600, 3200};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type65);
  // Phase 2, operations
  static int8_t op22_init[] = {2, 0, 2, 4, 2, 0, 2, 4};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 8);
  static int32_t op32_init[] = {1600, 1600};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {1, 2, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {1, 2, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  OperandType type64(Type::TENSOR_FLOAT16, {1, 2, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type64);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type65);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type65);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 8);
  static _Float16 op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static uint8_t op22_init[] = {130, 128, 130, 136, 130, 128, 130, 136};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 8);
  static int32_t op32_init[] = {1600, 3200};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int8_t op22_init[] = {2, 0, 2, 4, 2, 0, 2, 4};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 8);
  static int32_t op32_init[] = {1600, 1600};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 8);
  static float op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.0f, 0.25f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 8);
  static _Float16 op32_init[] = {100.0f, 200.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static uint8_t op22_init[] = {130, 128, 130, 136, 130, 128, 130, 136};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 8);
  static int32_t op32_init[] = {1600, 3200};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int8_t op22_init[] = {2, 0, 2, 4, 2, 0, 2, 4};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 8);
  static int32_t op32_init[] = {1600, 1600};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 2);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type54(Type::TENSOR_FLOAT16, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type53);
  auto op22 = model->addOperand(&type53);
  auto op32 = model->addOperand(&type54);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 100);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.125f, 128);
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type56);
  auto op22 = model->addOperand(&type57);
  auto op32 = model->addOperand(&type58);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 128);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.125f, 0.25f},3));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type60);
  auto op22 = model->addOperand(&type61);
  auto op32 = model->addOperand(&type62);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {1};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {1};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param13, param14, param15, param16, param17, param18, param19, param20, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {1, 1, 1, 4});
  // Phase 1, operands
  auto op13 = model->addOperand(&type16);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type67);
  // Phase 2, operations
  static _Float16 op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 16);
  static _Float16 op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 4}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type70);
  // Phase 2, operations
  static uint8_t op23_init[] = {1, 0, 40, 200, 1, 4, 80, 200, 1, 0, 120, 200, 1, 4, 160, 200};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 16);
  static int32_t op33_init[] = {48000, 56000, 64000, 72000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 4}, 50.0f, 0);
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type70);
  // Phase 2, operations
  static int8_t op23_init[] = {0, 0, 10, 50, 0, 0, 20, 50, 0, 0, 30, 50, 0, 0, 40, 50};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 16);
  static int32_t op33_init[] = {12000, 7000, 16000, 18000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {1, 1, 1, 4});
  // Phase 1, operands
  auto op13 = model->addOperand(&type16);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type67);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 4}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type70);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 4}, 50.0f, 0);
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type70);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_FLOAT32, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type73);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_FLOAT32, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type73);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type30);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type74);
  // Phase 2, operations
  static _Float16 op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 16);
  static _Float16 op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_ASYMM, {1, 4, 1, 1}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type76);
  // Phase 2, operations
  static uint8_t op23_init[] = {1, 0, 40, 200, 1, 4, 80, 200, 1, 0, 120, 200, 1, 4, 160, 200};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 16);
  static int32_t op33_init[] = {48000, 56000, 64000, 72000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_ASYMM, {1, 4, 1, 1}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type76);
  // Phase 2, operations
  static int8_t op23_init[] = {0, 0, 10, 50, 0, 0, 20, 50, 0, 0, 30, 50, 0, 0, 40, 50};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 16);
  static int32_t op33_init[] = {12000, 7000, 16000, 18000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_FLOAT32, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type73);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_FLOAT32, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type73);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type74(Type::TENSOR_FLOAT16, {1, 4, 1, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type30);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type74);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_ASYMM, {1, 4, 1, 1}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type76);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_ASYMM, {1, 4, 1, 1}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type76);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type16);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 16);
  static _Float16 op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static uint8_t op23_init[] = {1, 0, 40, 200, 1, 4, 80, 200, 1, 0, 120, 200, 1, 4, 160, 200};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 16);
  static int32_t op33_init[] = {48000, 56000, 64000, 72000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int8_t op23_init[] = {0, 0, 10, 50, 0, 0, 20, 50, 0, 0, 30, 50, 0, 0, 40, 50};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 16);
  static int32_t op33_init[] = {12000, 7000, 16000, 18000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type2);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type16);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 128);
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type41);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static float op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 16);
  static float op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type30);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 op23_init[] = {0.25f, 0.0f, 10.0f, 50.0f, 0.25f, 1.0f, 20.0f, 50.0f, 0.25f, 0.0f, 30.0f, 50.0f, 0.25f, 1.0f, 40.0f, 50.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 16);
  static _Float16 op33_init[] = {6000.0f, 7000.0f, 8000.0f, 9000.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static uint8_t op23_init[] = {1, 0, 40, 200, 1, 4, 80, 200, 1, 0, 120, 200, 1, 4, 160, 200};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 16);
  static int32_t op33_init[] = {48000, 56000, 64000, 72000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int8_t op23_init[] = {0, 0, 10, 50, 0, 0, 20, 50, 0, 0, 30, 50, 0, 0, 40, 50};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 16);
  static int32_t op33_init[] = {12000, 7000, 16000, 18000};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 4);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type28(Type::TENSOR_FLOAT32, {1, 4, 2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type34(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type2);
  auto op33 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 4});
  OperandType type17(Type::TENSOR_FLOAT16, {4});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 4, 2, 2});
  OperandType type35(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type30);
  auto op23 = model->addOperand(&type16);
  auto op33 = model->addOperand(&type17);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.25f, 0);
  OperandType type69(Type::TENSOR_INT32, {4}, 0.125f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type68);
  auto op33 = model->addOperand(&type69);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 2.0f, 1.0f, 1.0f},3));
  OperandType type72(Type::TENSOR_INT32, {4}, 0.0f, 0);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 0.5f, 128);
  OperandType type77(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 50.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type75);
  auto op23 = model->addOperand(&type71);
  auto op33 = model->addOperand(&type72);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type77);
  // Phase 2, operations
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {0};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {1};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op13, op23, op33, param21, param22, param23, param24, param25, param26, param27, param28, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type14);
  // Phase 2, operations
  static uint8_t op24_init[] = {129, 130, 131, 132, 119, 138, 117, 140, 133, 134, 135, 136, 141, 114, 143, 112};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 16);
  static int32_t op34_init[] = {2, 4, 6, 8};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 4);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_relaxed(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type14);
  // Phase 2, operations
  static uint8_t op24_init[] = {129, 130, 131, 132, 119, 138, 117, 140, 133, 134, 135, 136, 141, 114, 143, 112};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 16);
  static int32_t op34_init[] = {2, 4, 6, 8};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 4);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_weight_as_input(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_weight_as_input_relaxed(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type78);
  // Phase 2, operations
  static uint8_t op24_init[] = {129, 130, 131, 132, 119, 138, 117, 140, 133, 134, 135, 136, 141, 114, 143, 112};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 16);
  static int32_t op34_init[] = {2, 4, 6, 8};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 4);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type78);
  // Phase 2, operations
  static uint8_t op24_init[] = {129, 130, 131, 132, 119, 138, 117, 140, 133, 134, 135, 136, 141, 114, 143, 112};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 16);
  static int32_t op34_init[] = {2, 4, 6, 8};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 4);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type78);
  // Phase 2, operations
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 1.0058823529411764f, 127);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0058823529411764f, 128);
  OperandType type13(Type::TENSOR_INT32, {4}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op14 = model->addOperand(&type11);
  auto op24 = model->addOperand(&type12);
  auto op34 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type78);
  // Phase 2, operations
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, param33}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

