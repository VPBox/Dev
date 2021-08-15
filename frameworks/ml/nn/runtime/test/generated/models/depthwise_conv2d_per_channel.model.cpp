// clang-format off
// Generated file (from: depthwise_conv2d_per_channel.mod.py). Do not edit
void CreateModel_same(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 0);
  OperandType type1(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f},3));
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 1.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type3);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 4, 2, 0, 2, 2, 2, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_same(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_same_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 0);
  OperandType type11(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f},3));
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 1.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type3);
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
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_same_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_same_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 0);
  OperandType type1(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f},3));
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 0);
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type12);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 4, 2, 0, 2, 2, 2, 0};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_same_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_same_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 0);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 0);
  OperandType type13(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f},3));
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type12);
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
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, param7}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_same_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_different(Model *model) {
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0f, 128);
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type8);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {2};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, param13, param14, param15}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_different(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_different_weight_as_input(Model *model) {
  OperandType type14(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0f, 128);
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type14);
  auto op31 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {2};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, param13, param14, param15}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_different_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_different_dynamic_output_shape(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 16);
  static int32_t op31_init[] = {4, 4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 4);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {2};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, param13, param14, param15}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_different_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_different_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type16(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type16);
  auto op31 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {2};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op11, op21, op31, param8, param9, param10, param11, param12, param13, param14, param15}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_different_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_nhwc(Model *model) {
  OperandType type10(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0f, 128);
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto op22 = model->addOperand(&type10);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 16);
  static int32_t op32_init[] = {4, 4, 4, 4};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 4);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_nhwc_weight_as_input(Model *model) {
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 1.0f, 128);
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto op22 = model->addOperand(&type17);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_nchw(Model *model) {
  OperandType type10(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto op22 = model->addOperand(&type10);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 16);
  static int32_t op32_init[] = {4, 4, 4, 4};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 4);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_nchw_weight_as_input(Model *model) {
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 2}, 1.0f, 128);
  OperandType type20(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto op22 = model->addOperand(&type20);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_dynamic_output_shape_nhwc(Model *model) {
  OperandType type10(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto op22 = model->addOperand(&type10);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 16);
  static int32_t op32_init[] = {4, 4, 4, 4};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 4);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.5f, 128);
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto op22 = model->addOperand(&type21);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_dynamic_output_shape_nchw(Model *model) {
  OperandType type10(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto op22 = model->addOperand(&type10);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op22_init[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 16);
  static int32_t op32_init[] = {4, 4, 4, 4};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 4);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layout_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.5f, 128);
  OperandType type22(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {1, 2, 2, 4}, 0.0f, 0, SymmPerChannelQuantParams({1.0f, 0.5f, 1.0f, 0.5f},3));
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto op22 = model->addOperand(&type22);
  auto op32 = model->addOperand(&type7);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto layout = model->addOperand(&type9);
  auto op42 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {0};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {1};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op12, op22, op32, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_layout_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

