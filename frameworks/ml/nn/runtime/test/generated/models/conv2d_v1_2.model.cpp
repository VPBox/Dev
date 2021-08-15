// clang-format off
// Generated file (from: conv2d_v1_2.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type33);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type33);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type38);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type38);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type41(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type41);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type41(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type41);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type43);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type43);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type45(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type45);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type41(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type41);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type41(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type41);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type43);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type43);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type45(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type45);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type32);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type48);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type46);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type34(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type33);
  auto op3 = model->addOperand(&type34);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type35(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.125f},0));
  OperandType type36(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto op2 = model->addOperand(&type35);
  auto op3 = model->addOperand(&type36);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type47);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type48);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout}, {op4});
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

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static uint8_t op21_init[] = {129, 135, 141, 131, 137, 143, 133, 139, 145};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 1.0f, 50);
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 14, 4, 10, 16, 6, 12, 18};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type55);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type55);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 1.0f, 50);
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type55);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type55);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type56);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type56);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t op21_init[] = {129, 135, 141, 131, 137, 143, 133, 139, 145};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type58);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 14, 4, 10, 16, 6, 12, 18};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type59(Type::TENSOR_FLOAT16, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type59);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type59);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type56);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type56);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type58);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type58);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type59(Type::TENSOR_FLOAT16, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type59);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type59);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static uint8_t op21_init[] = {129, 135, 141, 131, 137, 143, 133, 139, 145};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 14, 4, 10, 16, 6, 12, 18};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type55);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {1, 3, 4, 1}, 0.5f, 127);
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type49);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type55(Type::TENSOR_FLOAT16, {1, 3, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type55);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static uint8_t op21_init[] = {129, 135, 141, 131, 137, 143, 133, 139, 145};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int8_t op21_init[] = {2, 8, 14, 4, 10, 16, 6, 12, 18};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 9);
  static int32_t op31_init[] = {-800};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type59(Type::TENSOR_FLOAT16, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type59);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {-200.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type56(Type::TENSOR_FLOAT32, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type56);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 127);
  OperandType type51(Type::TENSOR_INT32, {1}, 0.25f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type50);
  auto op31 = model->addOperand(&type51);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 3, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {1}, 0.0f, 0);
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 4}, 0.5f, 127);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 50);
  // Phase 1, operands
  auto op11 = model->addOperand(&type57);
  auto op21 = model->addOperand(&type53);
  auto op31 = model->addOperand(&type54);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type60);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type37(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type59(Type::TENSOR_FLOAT16, {1, 1, 3, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type59);
  auto op21 = model->addOperand(&type37);
  auto op31 = model->addOperand(&type39);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {1};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, layout}, {op41});
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

void CreateModel_channel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type61);
  // Phase 2, operations
  static uint8_t op22_init[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type61);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 2, 3, 5, 6, 8, 12, 13, 15};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type66(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type66);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 9);
  static _Float16 op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type61);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type61);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type66(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type66);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type66);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type69);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type69);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type70);
  // Phase 2, operations
  static uint8_t op22_init[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type70);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 2, 3, 5, 6, 8, 12, 13, 15};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type71(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type71);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 9);
  static _Float16 op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type69);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type69);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type70);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type70);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type71(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type71);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static uint8_t op22_init[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 2, 3, 5, 6, 8, 12, 13, 15};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type66(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type66);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 9);
  static _Float16 op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type6);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type61(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 3}, 0.5f, 0);
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type61);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type66(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type66);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static float op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 9);
  static float op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static uint8_t op22_init[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 2, 3, 5, 6, 8, 12, 13, 15};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 9);
  static int32_t op32_init[] = {0, 0, 0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type71(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type71);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 9);
  static _Float16 op32_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 3);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type69(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type69);
  auto op22 = model->addOperand(&type7);
  auto op32 = model->addOperand(&type8);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type62(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 0);
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type62);
  auto op32 = model->addOperand(&type63);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.4f, 0.3f},0));
  OperandType type65(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 1}, 0.5f, 0);
  OperandType type72(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type70);
  auto op22 = model->addOperand(&type64);
  auto op32 = model->addOperand(&type65);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type72);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type71(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type71);
  auto op22 = model->addOperand(&type67);
  auto op32 = model->addOperand(&type68);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {1};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param11, param12, param13, param14, param15, param16, param17, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type9);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type9);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type75);
  // Phase 2, operations
  static uint8_t op23_init[] = {130, 136, 142, 132, 138, 144, 134, 140, 146};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 2.0f, 0);
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type75);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 6, 12, 18};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 1.0f, 127);
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type78);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type78);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 3, 6, 9};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type81(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type81);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type81);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type75);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type75(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 2.0f, 0);
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type75);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 1.0f, 127);
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type78);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type78);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type81(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type81);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type81);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type11);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type11);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type83(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type83);
  // Phase 2, operations
  static uint8_t op23_init[] = {130, 136, 142, 132, 138, 144, 134, 140, 146};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type83(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type83);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 6, 12, 18};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type84);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type84);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 3, 6, 9};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type85);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type85);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type83(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type83);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type83(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type83);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type84);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type84);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type85);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type85);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static uint8_t op23_init[] = {130, 136, 142, 132, 138, 144, 134, 140, 146};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 6, 12, 18};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 1.0f, 127);
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type78);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type87);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 3, 6, 9};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type81(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type81);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type73(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type73);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type78(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 1.0f, 127);
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type78);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type87);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type81(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type81);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static uint8_t op23_init[] = {130, 136, 142, 132, 138, 144, 134, 140, 146};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 6, 12, 18};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 1.0f, 127);
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type84);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type87);
  // Phase 2, operations
  static int8_t op23_init[] = {2, 8, 14, 2, 5, 8, 3, 6, 9};
  model->setOperandValue(op23, op23_init, sizeof(int8_t) * 9);
  static int32_t op33_init[] = {0, 0, 0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type85);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 3);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {3, 1, 1, 3});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type11);
  auto op23 = model->addOperand(&type7);
  auto op33 = model->addOperand(&type8);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type74(Type::TENSOR_QUANT8_ASYMM, {3, 1, 1, 3}, 0.5f, 128);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type74);
  auto op33 = model->addOperand(&type63);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type82(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 0.5f, 128);
  OperandType type86(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type82);
  auto op23 = model->addOperand(&type76);
  auto op33 = model->addOperand(&type77);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type86);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type79(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 1.0f, 1.005f},0));
  OperandType type80(Type::TENSOR_INT32, {3}, 0.0f, 0);
  OperandType type84(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 3}, 1.0f, 127);
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op13 = model->addOperand(&type84);
  auto op23 = model->addOperand(&type79);
  auto op33 = model->addOperand(&type80);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type87);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
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

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type67(Type::TENSOR_FLOAT16, {3, 1, 1, 3});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  // Phase 1, operands
  auto op13 = model->addOperand(&type85);
  auto op23 = model->addOperand(&type67);
  auto op33 = model->addOperand(&type68);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto param24 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {0};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param18, param19, param20, param21, param22, param23, param24, layout}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8, 8, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type12);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type12(Type::TENSOR_FLOAT32, {1, 8, 8, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type12);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type89(Type::TENSOR_FLOAT16, {1, 8, 8, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type88);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type89);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type91(Type::TENSOR_FLOAT32, {1, 1, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type91);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type91(Type::TENSOR_FLOAT32, {1, 1, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type91);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type93(Type::TENSOR_FLOAT16, {1, 1, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type92);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type93);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  // Phase 1, operands
  auto op14 = model->addOperand(&type88);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_SAME_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type92);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op44 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param25, param26, param27, param28, layout}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_SAME_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 6, 7, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type13);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 6, 7, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type13);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type94(Type::TENSOR_FLOAT16, {1, 6, 7, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type88);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type94);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type95(Type::TENSOR_FLOAT32, {1, 1, 6, 7});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type95);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type95(Type::TENSOR_FLOAT32, {1, 1, 6, 7});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type95);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type96(Type::TENSOR_FLOAT16, {1, 1, 6, 7});
  // Phase 1, operands
  auto op14 = model->addOperand(&type92);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type96);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op14 = model->addOperand(&type10);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  // Phase 1, operands
  auto op14 = model->addOperand(&type88);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 3, 2, 3});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type90);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type3);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type46);
  // Phase 2, operations
  static float op24_init[] = {-0.966213f, -0.467474f, -0.82203f, -0.579455f, 0.0278809f, -0.79946f, -0.684259f, 0.563238f, 0.37289f, 0.738216f, 0.386045f, -0.917775f, 0.184325f, -0.270568f, 0.82236f, 0.0973683f, -0.941308f, -0.144706f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 18);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_1_H3_W2_VALID_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type39(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type85(Type::TENSOR_FLOAT16, {1, 3, 2, 3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  // Phase 1, operands
  auto op14 = model->addOperand(&type92);
  auto op24 = model->addOperand(&type85);
  auto op34 = model->addOperand(&type39);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op45 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op24_init[] = {-0.9662129878997803f, -0.4674740135669708f, -0.8220300078392029f, -0.5794550180435181f, 0.027880899608135223f, -0.7994599938392639f, -0.6842589974403381f, 0.5632380247116089f, 0.37288999557495117f, 0.738215982913971f, 0.38604500889778137f, -0.9177749752998352f, 0.18432499468326569f, -0.27056801319122314f, 0.8223599791526794f, 0.09736829996109009f, -0.9413080215454102f, -0.14470599591732025f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 18);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {2};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {0};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param29, param30, param31, param32, layout}, {op45});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op45});
  assert(model->isValid());
}

inline bool is_ignored_1_H3_W2_VALID_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type10);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type10);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type88);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type88);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type90);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type90);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type92);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type92);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type88);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_SAME_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type92);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param33 = model->addOperand(&type4);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto param36 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op46 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param33_init[] = {1};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static int32_t param34_init[] = {1};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {1};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {0};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param33, param34, param35, param36, layout}, {op46});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op46});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_SAME_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type15(Type::TENSOR_FLOAT32, {1, 6, 7, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type15);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type15(Type::TENSOR_FLOAT32, {1, 6, 7, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type15);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  OperandType type98(Type::TENSOR_FLOAT16, {1, 6, 7, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type88);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type98);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type99(Type::TENSOR_FLOAT32, {1, 3, 6, 7});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type99);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  OperandType type99(Type::TENSOR_FLOAT32, {1, 3, 6, 7});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type99);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_FLOAT16, {1, 3, 6, 7});
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type92);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type100);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 8, 8, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type10);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type88(Type::TENSOR_FLOAT16, {1, 8, 8, 3});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type88);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT32, {3, 3, 2, 3});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {3});
  OperandType type90(Type::TENSOR_FLOAT32, {1, 3, 8, 8});
  // Phase 1, operands
  auto op15 = model->addOperand(&type90);
  auto op25 = model->addOperand(&type14);
  auto op35 = model->addOperand(&type8);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type46);
  // Phase 2, operations
  static float op25_init[] = {-0.966213f, -0.579455f, -0.684259f, 0.738216f, 0.184325f, 0.0973683f, -0.176863f, -0.23936f, -0.000233404f, 0.055546f, -0.232658f, -0.316404f, -0.012904f, 0.320705f, -0.326657f, -0.919674f, 0.868081f, -0.824608f, -0.467474f, 0.0278809f, 0.563238f, 0.386045f, -0.270568f, -0.941308f, -0.779227f, -0.261492f, -0.774804f, -0.79665f, 0.22473f, -0.414312f, 0.685897f, -0.327792f, 0.77395f, -0.714578f, -0.972365f, 0.0696099f, -0.82203f, -0.79946f, 0.37289f, -0.917775f, 0.82236f, -0.144706f, -0.167188f, 0.268062f, 0.702641f, -0.412223f, 0.755759f, 0.721547f, -0.43637f, -0.274905f, -0.269165f, 0.16102f, 0.819857f, -0.312008f};
  model->setOperandValue(op25, op25_init, sizeof(float) * 54);
  static float op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(float) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3_H3_W2_VALID_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type68(Type::TENSOR_FLOAT16, {3});
  OperandType type92(Type::TENSOR_FLOAT16, {1, 3, 8, 8});
  OperandType type97(Type::TENSOR_FLOAT16, {3, 3, 2, 3});
  // Phase 1, operands
  auto op15 = model->addOperand(&type92);
  auto op25 = model->addOperand(&type97);
  auto op35 = model->addOperand(&type68);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type4);
  auto param39 = model->addOperand(&type4);
  auto param40 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op47 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 op25_init[] = {-0.9662129878997803f, -0.5794550180435181f, -0.6842589974403381f, 0.738215982913971f, 0.18432499468326569f, 0.09736829996109009f, -0.17686299979686737f, -0.2393600046634674f, -0.0002334040036657825f, 0.055546000599861145f, -0.2326579988002777f, -0.3164040148258209f, -0.012903999537229538f, 0.32070499658584595f, -0.3266569972038269f, -0.9196739792823792f, 0.8680809736251831f, -0.8246080279350281f, -0.4674740135669708f, 0.027880899608135223f, 0.5632380247116089f, 0.38604500889778137f, -0.27056801319122314f, -0.9413080215454102f, -0.7792270183563232f, -0.2614920139312744f, -0.7748039960861206f, -0.7966499924659729f, 0.2247299998998642f, -0.4143120050430298f, 0.6858969926834106f, -0.3277919888496399f, 0.7739499807357788f, -0.7145779728889465f, -0.9723650217056274f, 0.06960990279912949f, -0.8220300078392029f, -0.7994599938392639f, 0.37288999557495117f, -0.9177749752998352f, 0.8223599791526794f, -0.14470599591732025f, -0.16718800365924835f, 0.2680619955062866f, 0.7026410102844238f, -0.4122230112552643f, 0.7557590007781982f, 0.72154700756073f, -0.43636998534202576f, -0.2749049961566925f, -0.2691650092601776f, 0.16101999580860138f, 0.8198570013046265f, -0.31200799345970154f};
  model->setOperandValue(op25, op25_init, sizeof(_Float16) * 54);
  static _Float16 op35_init[] = {0.0f, 0.0f, 0.0f};
  model->setOperandValue(op35, op35_init, sizeof(_Float16) * 3);
  static int32_t param37_init[] = {2};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {1};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {1};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op15, op25, op35, param37, param38, param39, param40, layout}, {op47});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op15},
    {op47});
  assert(model->isValid());
}

inline bool is_ignored_3_H3_W2_VALID_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 3}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type19);
  // Phase 2, operations
  static uint8_t op26_init[] = {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129};
  model->setOperandValue(op26, op26_init, sizeof(uint8_t) * 12);
  static int32_t op36_init[] = {1, 2, 3};
  model->setOperandValue(op36, op36_init, sizeof(int32_t) * 3);
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16},
    {op48});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 3}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type19);
  // Phase 2, operations
  static uint8_t op26_init[] = {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129};
  model->setOperandValue(op26, op26_init, sizeof(uint8_t) * 12);
  static int32_t op36_init[] = {1, 2, 3};
  model->setOperandValue(op36, op36_init, sizeof(int32_t) * 3);
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16},
    {op48});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_weight_as_input(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 3}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16, op26, op36},
    {op48});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_weight_as_input_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 1, 2, 3}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16, op26, op36},
    {op48});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type87);
  // Phase 2, operations
  static uint8_t op26_init[] = {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129};
  model->setOperandValue(op26, op26_init, sizeof(uint8_t) * 12);
  static int32_t op36_init[] = {1, 2, 3};
  model->setOperandValue(op36, op36_init, sizeof(int32_t) * 3);
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16},
    {op48});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type87);
  // Phase 2, operations
  static uint8_t op26_init[] = {129, 130, 131, 132, 127, 129, 127, 129, 127, 127, 129, 129};
  model->setOperandValue(op26, op26_init, sizeof(uint8_t) * 12);
  static int32_t op36_init[] = {1, 2, 3};
  model->setOperandValue(op36, op36_init, sizeof(int32_t) * 3);
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16},
    {op48});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type87);
  // Phase 2, operations
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16, op26, op36},
    {op48});
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed(Model *model) {
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {2, 2, 4, 1}, 1.0058823529411764f, 128);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {3, 2, 2, 1}, 1.0058823529411764f, 128);
  OperandType type18(Type::TENSOR_INT32, {3}, 1.0117993079584775f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type87(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op16 = model->addOperand(&type16);
  auto op26 = model->addOperand(&type17);
  auto op36 = model->addOperand(&type18);
  auto param41 = model->addOperand(&type4);
  auto param42 = model->addOperand(&type4);
  auto param43 = model->addOperand(&type4);
  auto param44 = model->addOperand(&type4);
  auto op48 = model->addOperand(&type87);
  // Phase 2, operations
  static int32_t param41_init[] = {2};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {2};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {0};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op16, op26, op36, param41, param42, param43, param44}, {op48});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op16, op26, op36},
    {op48});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_quant_output_multiplier_gt_1_dynamic_output_shape_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type28);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type31);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type28);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type31);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type102(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type104(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type107);
  auto roi = model->addOperand(&type105);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type108);
  auto roiOut = model->addOperand(&type106);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type103);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type102);
  auto weights = model->addOperand(&type109);
  auto bias = model->addOperand(&type101);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type104);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights_init[] = {158, 168};
  model->setOperandValue(weights, weights_init, sizeof(uint8_t) * 2);
  static int32_t bias_init[] = {100, 200};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type111(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type113(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type118(Type::TENSOR_FLOAT16, {0});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type117);
  auto roi = model->addOperand(&type115);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type114);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type114);
  auto param50 = model->addOperand(&type114);
  auto param51 = model->addOperand(&type114);
  auto scoresOut = model->addOperand(&type118);
  auto roiOut = model->addOperand(&type116);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type112);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type114);
  auto param55 = model->addOperand(&type114);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type111);
  auto weights = model->addOperand(&type119);
  auto bias = model->addOperand(&type110);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type113);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static _Float16 param46_init[] = {0.30000001192092896f};
  model->setOperandValue(param46, param46_init, sizeof(_Float16) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static _Float16 param49_init[] = {0.4000000059604645f};
  model->setOperandValue(param49, param49_init, sizeof(_Float16) * 1);
  static _Float16 param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {0.30000001192092896f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static _Float16 param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(_Float16) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(_Float16) * 2);
  static _Float16 bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(_Float16) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type120);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type31);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type120);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type31);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type104(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type121(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type107);
  auto roi = model->addOperand(&type105);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type108);
  auto roiOut = model->addOperand(&type106);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type103);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type121);
  auto weights = model->addOperand(&type109);
  auto bias = model->addOperand(&type101);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type104);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights_init[] = {158, 168};
  model->setOperandValue(weights, weights_init, sizeof(uint8_t) * 2);
  static int32_t bias_init[] = {100, 200};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type113(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type118(Type::TENSOR_FLOAT16, {0});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type122(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type117);
  auto roi = model->addOperand(&type115);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type114);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type114);
  auto param50 = model->addOperand(&type114);
  auto param51 = model->addOperand(&type114);
  auto scoresOut = model->addOperand(&type118);
  auto roiOut = model->addOperand(&type116);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type112);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type114);
  auto param55 = model->addOperand(&type114);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type122);
  auto weights = model->addOperand(&type119);
  auto bias = model->addOperand(&type110);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type113);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static _Float16 param46_init[] = {0.30000001192092896f};
  model->setOperandValue(param46, param46_init, sizeof(_Float16) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static _Float16 param49_init[] = {0.4000000059604645f};
  model->setOperandValue(param49, param49_init, sizeof(_Float16) * 1);
  static _Float16 param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {0.30000001192092896f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static _Float16 param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(_Float16) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(_Float16) * 2);
  static _Float16 bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(_Float16) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type28);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type46);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type28);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type46);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type102(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type107);
  auto roi = model->addOperand(&type105);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type108);
  auto roiOut = model->addOperand(&type106);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type103);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type102);
  auto weights = model->addOperand(&type109);
  auto bias = model->addOperand(&type101);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type123);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights_init[] = {158, 168};
  model->setOperandValue(weights, weights_init, sizeof(uint8_t) * 2);
  static int32_t bias_init[] = {100, 200};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type111(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type124(Type::TENSOR_FLOAT16, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type117);
  auto roi = model->addOperand(&type115);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type114);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type114);
  auto param50 = model->addOperand(&type114);
  auto param51 = model->addOperand(&type114);
  auto scoresOut = model->addOperand(&type124);
  auto roiOut = model->addOperand(&type116);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type112);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type114);
  auto param55 = model->addOperand(&type114);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type111);
  auto weights = model->addOperand(&type119);
  auto bias = model->addOperand(&type110);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static _Float16 param46_init[] = {0.30000001192092896f};
  model->setOperandValue(param46, param46_init, sizeof(_Float16) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static _Float16 param49_init[] = {0.4000000059604645f};
  model->setOperandValue(param49, param49_init, sizeof(_Float16) * 1);
  static _Float16 param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {0.30000001192092896f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static _Float16 param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(_Float16) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(_Float16) * 2);
  static _Float16 bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(_Float16) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type120);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type46);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type20);
  auto roi = model->addOperand(&type21);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type22);
  auto roiOut = model->addOperand(&type24);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type27);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type120);
  auto weights = model->addOperand(&type29);
  auto bias = model->addOperand(&type30);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type46);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(float) * 2);
  static float bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type121(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type107);
  auto roi = model->addOperand(&type105);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type26);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type26);
  auto param50 = model->addOperand(&type26);
  auto param51 = model->addOperand(&type26);
  auto scoresOut = model->addOperand(&type108);
  auto roiOut = model->addOperand(&type106);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type103);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type26);
  auto param55 = model->addOperand(&type26);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type121);
  auto weights = model->addOperand(&type109);
  auto bias = model->addOperand(&type101);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type123);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static float param46_init[] = {0.3f};
  model->setOperandValue(param46, param46_init, sizeof(float) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static float param49_init[] = {0.4f};
  model->setOperandValue(param49, param49_init, sizeof(float) * 1);
  static float param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {0.3f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static float param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(float) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights_init[] = {158, 168};
  model->setOperandValue(weights, weights_init, sizeof(uint8_t) * 2);
  static int32_t bias_init[] = {100, 200};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type122(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type124(Type::TENSOR_FLOAT16, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores = model->addOperand(&type117);
  auto roi = model->addOperand(&type115);
  auto param45 = model->addOperand(&type25);
  auto param46 = model->addOperand(&type114);
  auto param47 = model->addOperand(&type4);
  auto param48 = model->addOperand(&type4);
  auto param49 = model->addOperand(&type114);
  auto param50 = model->addOperand(&type114);
  auto param51 = model->addOperand(&type114);
  auto scoresOut = model->addOperand(&type124);
  auto roiOut = model->addOperand(&type116);
  auto classesOut = model->addOperand(&type23);
  auto batchSplitOut = model->addOperand(&type23);
  auto in = model->addOperand(&type112);
  auto param52 = model->addOperand(&type4);
  auto param53 = model->addOperand(&type4);
  auto param54 = model->addOperand(&type114);
  auto param55 = model->addOperand(&type114);
  auto param56 = model->addOperand(&type4);
  auto param57 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type122);
  auto weights = model->addOperand(&type119);
  auto bias = model->addOperand(&type110);
  auto param58 = model->addOperand(&type4);
  auto param59 = model->addOperand(&type4);
  auto param60 = model->addOperand(&type4);
  auto param61 = model->addOperand(&type4);
  auto param62 = model->addOperand(&type4);
  auto param63 = model->addOperand(&type4);
  auto param64 = model->addOperand(&type4);
  auto out = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  static _Float16 param46_init[] = {0.30000001192092896f};
  model->setOperandValue(param46, param46_init, sizeof(_Float16) * 1);
  static int32_t param47_init[] = {-1};
  model->setOperandValue(param47, param47_init, sizeof(int32_t) * 1);
  static int32_t param48_init[] = {0};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static _Float16 param49_init[] = {0.4000000059604645f};
  model->setOperandValue(param49, param49_init, sizeof(_Float16) * 1);
  static _Float16 param50_init[] = {1.0f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {0.30000001192092896f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static int32_t param52_init[] = {2};
  model->setOperandValue(param52, param52_init, sizeof(int32_t) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static _Float16 param54_init[] = {2.0f};
  model->setOperandValue(param54, param54_init, sizeof(_Float16) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static int32_t param56_init[] = {4};
  model->setOperandValue(param56, param56_init, sizeof(int32_t) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights, weights_init, sizeof(_Float16) * 2);
  static _Float16 bias_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias, bias_init, sizeof(_Float16) * 2);
  static int32_t param58_init[] = {0};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static int32_t param59_init[] = {0};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {0};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {0};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {1};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {1};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param45, param46, param47, param48, param49, param50, param51}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param52, param53, param54, param55, param56, param57, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param58, param59, param60, param61, param62, param63, param64, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type28);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type28);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type102(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type104(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type107);
  auto roi1 = model->addOperand(&type105);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type108);
  auto roiOut1 = model->addOperand(&type106);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type103);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type102);
  auto weights1 = model->addOperand(&type109);
  auto bias1 = model->addOperand(&type101);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type104);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights1_init[] = {158, 168};
  model->setOperandValue(weights1, weights1_init, sizeof(uint8_t) * 2);
  static int32_t bias1_init[] = {100, 200};
  model->setOperandValue(bias1, bias1_init, sizeof(int32_t) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type111(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type113(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type118(Type::TENSOR_FLOAT16, {0});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type117);
  auto roi1 = model->addOperand(&type115);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type114);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type114);
  auto param70 = model->addOperand(&type114);
  auto param71 = model->addOperand(&type114);
  auto scoresOut1 = model->addOperand(&type118);
  auto roiOut1 = model->addOperand(&type116);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type112);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type114);
  auto param75 = model->addOperand(&type114);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type111);
  auto weights1 = model->addOperand(&type119);
  auto bias1 = model->addOperand(&type110);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type113);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static _Float16 param66_init[] = {0.30000001192092896f};
  model->setOperandValue(param66, param66_init, sizeof(_Float16) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static _Float16 param69_init[] = {0.4000000059604645f};
  model->setOperandValue(param69, param69_init, sizeof(_Float16) * 1);
  static _Float16 param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(_Float16) * 1);
  static _Float16 param71_init[] = {0.30000001192092896f};
  model->setOperandValue(param71, param71_init, sizeof(_Float16) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static _Float16 param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(_Float16) * 1);
  static _Float16 param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(_Float16) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(_Float16) * 2);
  static _Float16 bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(_Float16) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type120);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type31(Type::TENSOR_FLOAT32, {0, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type120);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type31);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type104(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type121(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type107);
  auto roi1 = model->addOperand(&type105);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type108);
  auto roiOut1 = model->addOperand(&type106);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type103);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type121);
  auto weights1 = model->addOperand(&type109);
  auto bias1 = model->addOperand(&type101);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type104);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights1_init[] = {158, 168};
  model->setOperandValue(weights1, weights1_init, sizeof(uint8_t) * 2);
  static int32_t bias1_init[] = {100, 200};
  model->setOperandValue(bias1, bias1_init, sizeof(int32_t) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type113(Type::TENSOR_FLOAT16, {0, 2, 2, 2});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type118(Type::TENSOR_FLOAT16, {0});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type122(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type117);
  auto roi1 = model->addOperand(&type115);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type114);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type114);
  auto param70 = model->addOperand(&type114);
  auto param71 = model->addOperand(&type114);
  auto scoresOut1 = model->addOperand(&type118);
  auto roiOut1 = model->addOperand(&type116);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type112);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type114);
  auto param75 = model->addOperand(&type114);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type122);
  auto weights1 = model->addOperand(&type119);
  auto bias1 = model->addOperand(&type110);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type113);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static _Float16 param66_init[] = {0.30000001192092896f};
  model->setOperandValue(param66, param66_init, sizeof(_Float16) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static _Float16 param69_init[] = {0.4000000059604645f};
  model->setOperandValue(param69, param69_init, sizeof(_Float16) * 1);
  static _Float16 param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(_Float16) * 1);
  static _Float16 param71_init[] = {0.30000001192092896f};
  model->setOperandValue(param71, param71_init, sizeof(_Float16) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static _Float16 param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(_Float16) * 1);
  static _Float16 param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(_Float16) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(_Float16) * 2);
  static _Float16 bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(_Float16) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type28);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type46);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type28(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type28);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type46);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type102(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type107);
  auto roi1 = model->addOperand(&type105);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type108);
  auto roiOut1 = model->addOperand(&type106);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type103);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type102);
  auto weights1 = model->addOperand(&type109);
  auto bias1 = model->addOperand(&type101);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type123);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights1_init[] = {158, 168};
  model->setOperandValue(weights1, weights1_init, sizeof(uint8_t) * 2);
  static int32_t bias1_init[] = {100, 200};
  model->setOperandValue(bias1, bias1_init, sizeof(int32_t) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type111(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type124(Type::TENSOR_FLOAT16, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type117);
  auto roi1 = model->addOperand(&type115);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type114);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type114);
  auto param70 = model->addOperand(&type114);
  auto param71 = model->addOperand(&type114);
  auto scoresOut1 = model->addOperand(&type124);
  auto roiOut1 = model->addOperand(&type116);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type112);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type114);
  auto param75 = model->addOperand(&type114);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type111);
  auto weights1 = model->addOperand(&type119);
  auto bias1 = model->addOperand(&type110);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static _Float16 param66_init[] = {0.30000001192092896f};
  model->setOperandValue(param66, param66_init, sizeof(_Float16) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static _Float16 param69_init[] = {0.4000000059604645f};
  model->setOperandValue(param69, param69_init, sizeof(_Float16) * 1);
  static _Float16 param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(_Float16) * 1);
  static _Float16 param71_init[] = {0.30000001192092896f};
  model->setOperandValue(param71, param71_init, sizeof(_Float16) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static _Float16 param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(_Float16) * 1);
  static _Float16 param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(_Float16) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(_Float16) * 2);
  static _Float16 bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(_Float16) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type120);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type46);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type120(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type22(Type::TENSOR_FLOAT32, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type24(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type29(Type::TENSOR_FLOAT32, {2, 1, 1, 1});
  OperandType type30(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type20);
  auto roi1 = model->addOperand(&type21);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type22);
  auto roiOut1 = model->addOperand(&type24);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type27);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type120);
  auto weights1 = model->addOperand(&type29);
  auto bias1 = model->addOperand(&type30);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type46);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(float) * 2);
  static float bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(float) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_INT32, {2}, 0.01f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type105(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type106(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type107(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type108(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type109(Type::TENSOR_QUANT8_ASYMM, {2, 1, 1, 1}, 0.1f, 128);
  OperandType type121(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type26(Type::FLOAT32, {});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type107);
  auto roi1 = model->addOperand(&type105);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type26);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type26);
  auto param70 = model->addOperand(&type26);
  auto param71 = model->addOperand(&type26);
  auto scoresOut1 = model->addOperand(&type108);
  auto roiOut1 = model->addOperand(&type106);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type103);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type26);
  auto param75 = model->addOperand(&type26);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type121);
  auto weights1 = model->addOperand(&type109);
  auto bias1 = model->addOperand(&type101);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type123);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static float param66_init[] = {0.3f};
  model->setOperandValue(param66, param66_init, sizeof(float) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static float param69_init[] = {0.4f};
  model->setOperandValue(param69, param69_init, sizeof(float) * 1);
  static float param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(float) * 1);
  static float param71_init[] = {0.3f};
  model->setOperandValue(param71, param71_init, sizeof(float) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static float param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(float) * 1);
  static float param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(float) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static uint8_t weights1_init[] = {158, 168};
  model->setOperandValue(weights1, weights1_init, sizeof(uint8_t) * 2);
  static int32_t bias1_init[] = {100, 200};
  model->setOperandValue(bias1, bias1_init, sizeof(int32_t) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2});
  OperandType type112(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type114(Type::FLOAT16, {});
  OperandType type115(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type116(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type117(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type119(Type::TENSOR_FLOAT16, {2, 1, 1, 1});
  OperandType type122(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type124(Type::TENSOR_FLOAT16, {0});
  OperandType type23(Type::TENSOR_INT32, {0});
  OperandType type25(Type::TENSOR_INT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type48(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type117);
  auto roi1 = model->addOperand(&type115);
  auto param65 = model->addOperand(&type25);
  auto param66 = model->addOperand(&type114);
  auto param67 = model->addOperand(&type4);
  auto param68 = model->addOperand(&type4);
  auto param69 = model->addOperand(&type114);
  auto param70 = model->addOperand(&type114);
  auto param71 = model->addOperand(&type114);
  auto scoresOut1 = model->addOperand(&type124);
  auto roiOut1 = model->addOperand(&type116);
  auto classesOut1 = model->addOperand(&type23);
  auto batchSplitOut1 = model->addOperand(&type23);
  auto in1 = model->addOperand(&type112);
  auto param72 = model->addOperand(&type4);
  auto param73 = model->addOperand(&type4);
  auto param74 = model->addOperand(&type114);
  auto param75 = model->addOperand(&type114);
  auto param76 = model->addOperand(&type4);
  auto param77 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type122);
  auto weights1 = model->addOperand(&type119);
  auto bias1 = model->addOperand(&type110);
  auto param78 = model->addOperand(&type4);
  auto param79 = model->addOperand(&type4);
  auto param80 = model->addOperand(&type4);
  auto param81 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type48);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param65_init[] = {0};
  model->setOperandValue(param65, param65_init, sizeof(int32_t) * 1);
  static _Float16 param66_init[] = {0.30000001192092896f};
  model->setOperandValue(param66, param66_init, sizeof(_Float16) * 1);
  static int32_t param67_init[] = {-1};
  model->setOperandValue(param67, param67_init, sizeof(int32_t) * 1);
  static int32_t param68_init[] = {0};
  model->setOperandValue(param68, param68_init, sizeof(int32_t) * 1);
  static _Float16 param69_init[] = {0.4000000059604645f};
  model->setOperandValue(param69, param69_init, sizeof(_Float16) * 1);
  static _Float16 param70_init[] = {1.0f};
  model->setOperandValue(param70, param70_init, sizeof(_Float16) * 1);
  static _Float16 param71_init[] = {0.30000001192092896f};
  model->setOperandValue(param71, param71_init, sizeof(_Float16) * 1);
  static int32_t param72_init[] = {2};
  model->setOperandValue(param72, param72_init, sizeof(int32_t) * 1);
  static int32_t param73_init[] = {2};
  model->setOperandValue(param73, param73_init, sizeof(int32_t) * 1);
  static _Float16 param74_init[] = {2.0f};
  model->setOperandValue(param74, param74_init, sizeof(_Float16) * 1);
  static _Float16 param75_init[] = {2.0f};
  model->setOperandValue(param75, param75_init, sizeof(_Float16) * 1);
  static int32_t param76_init[] = {4};
  model->setOperandValue(param76, param76_init, sizeof(int32_t) * 1);
  static int32_t param77_init[] = {4};
  model->setOperandValue(param77, param77_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 weights1_init[] = {3.0f, 4.0f};
  model->setOperandValue(weights1, weights1_init, sizeof(_Float16) * 2);
  static _Float16 bias1_init[] = {1.0f, 2.0f};
  model->setOperandValue(bias1, bias1_init, sizeof(_Float16) * 2);
  static int32_t param78_init[] = {1};
  model->setOperandValue(param78, param78_init, sizeof(int32_t) * 1);
  static int32_t param79_init[] = {1};
  model->setOperandValue(param79, param79_init, sizeof(int32_t) * 1);
  static int32_t param80_init[] = {1};
  model->setOperandValue(param80, param80_init, sizeof(int32_t) * 1);
  static int32_t param81_init[] = {0};
  model->setOperandValue(param81, param81_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param65, param66, param67, param68, param69, param70, param71}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param72, param73, param74, param75, param76, param77, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap1, weights1, bias1, param78, param79, param80, param81, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

