// clang-format off
// Generated file (from: conv2d_per_channel.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type1(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 3}, 1.0f, 128);
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
  auto op4 = model->addOperand(&type3);
  // Phase 2, operations
  static int8_t op2_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 6);
  static int32_t op3_init[] = {4, 4, 4};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 3);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type18(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 3}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type18);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type1(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
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
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int8_t op2_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 6);
  static int32_t op3_init[] = {4, 4, 4};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 3);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type20(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type20);
  auto op3 = model->addOperand(&type2);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type19);
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
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_nhwc(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 3}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  // Phase 1, operands
  auto op11 = model->addOperand(&type0);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type3);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 6);
  static int32_t op31_init[] = {4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 3);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 3}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type0);
  auto op21 = model->addOperand(&type21);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_nchw(Model *model) {
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 1}, 0.5f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 6);
  static int32_t op31_init[] = {4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 3);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_nchw_weight_as_input(Model *model) {
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 1}, 0.5f, 128);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 1.0f, 128);
  OperandType type24(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type24);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  // Phase 1, operands
  auto op11 = model->addOperand(&type0);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 6);
  static int32_t op31_init[] = {4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 3);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 1, 2}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type25(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type0);
  auto op21 = model->addOperand(&type25);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_dynamic_output_shape_nchw(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 1}, 0.5f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type6(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type6);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int8_t op21_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 6);
  static int32_t op31_init[] = {4, 4, 4};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 3);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_layouts_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 1}, 0.5f, 128);
  OperandType type26(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type26);
  auto op31 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {1};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {0};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, param12, param13, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_layouts_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 0.5f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.5f, 128);
  OperandType type16(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 3}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type8(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type13);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type13);
  auto param19 = model->addOperand(&type13);
  auto param20 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type13);
  auto param24 = model->addOperand(&type13);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto featureMap = model->addOperand(&type15);
  auto weights = model->addOperand(&type16);
  auto bias = model->addOperand(&type2);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto out = model->addOperand(&type17);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {1, 1, 10, 10, 0, 0, 10, 10};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int8_t weights_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(weights, weights_init, sizeof(int8_t) * 6);
  static int32_t bias_init[] = {4, 4, 4};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 3);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {1};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param14, param15, param16, param17, param18, param19, param20}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param21, param22, param23, param24, param25, param26, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param27, param28, param29, param30, param31, param32, param33, layout}, {out});
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

void CreateModel_zero_sized_nchw(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.5f, 128);
  OperandType type16(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 0.5f, 128);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 3, 2, 2}, 1.0f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type8(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type13);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type13);
  auto param19 = model->addOperand(&type13);
  auto param20 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type27);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type13);
  auto param24 = model->addOperand(&type13);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto featureMap = model->addOperand(&type15);
  auto weights = model->addOperand(&type16);
  auto bias = model->addOperand(&type2);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto out = model->addOperand(&type28);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {1, 1, 10, 10, 0, 0, 10, 10};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int8_t weights_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(weights, weights_init, sizeof(int8_t) * 6);
  static int32_t bias_init[] = {4, 4, 4};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 3);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {1};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param14, param15, param16, param17, param18, param19, param20}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param21, param22, param23, param24, param25, param26, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param27, param28, param29, param30, param31, param32, param33, layout}, {out});
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

void CreateModel_zero_sized_dynamic_output_shape_nhwc(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 2}, 0.5f, 128);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.5f, 128);
  OperandType type16(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type8(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type13);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type13);
  auto param19 = model->addOperand(&type13);
  auto param20 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type13);
  auto param24 = model->addOperand(&type13);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto featureMap = model->addOperand(&type15);
  auto weights = model->addOperand(&type16);
  auto bias = model->addOperand(&type2);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto out = model->addOperand(&type19);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {1, 1, 10, 10, 0, 0, 10, 10};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int8_t weights_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(weights, weights_init, sizeof(int8_t) * 6);
  static int32_t bias_init[] = {4, 4, 4};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 3);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {1};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param14, param15, param16, param17, param18, param19, param20}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param21, param22, param23, param24, param25, param26, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param27, param28, param29, param30, param31, param32, param33, layout}, {out});
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

void CreateModel_zero_sized_dynamic_output_shape_nchw(Model *model) {
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 2}, 0.5f, 128);
  OperandType type16(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {3, 1, 1, 2}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.75f, 1.0f},0));
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 128);
  OperandType type2(Type::TENSOR_INT32, {3});
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 1}, 0.5f, 128);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::BOOL, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type8(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param14 = model->addOperand(&type12);
  auto param15 = model->addOperand(&type13);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type13);
  auto param19 = model->addOperand(&type13);
  auto param20 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type27);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type13);
  auto param24 = model->addOperand(&type13);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto layout = model->addOperand(&type5);
  auto featureMap = model->addOperand(&type15);
  auto weights = model->addOperand(&type16);
  auto bias = model->addOperand(&type2);
  auto param27 = model->addOperand(&type4);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto out = model->addOperand(&type19);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {1, 1, 10, 10, 0, 0, 10, 10};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static float param15_init[] = {0.3f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  static int32_t param16_init[] = {-1};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static float param18_init[] = {0.4f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static float param19_init[] = {1.0f};
  model->setOperandValue(param19, param19_init, sizeof(float) * 1);
  static float param20_init[] = {0.3f};
  model->setOperandValue(param20, param20_init, sizeof(float) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static float param23_init[] = {2.0f};
  model->setOperandValue(param23, param23_init, sizeof(float) * 1);
  static float param24_init[] = {2.0f};
  model->setOperandValue(param24, param24_init, sizeof(float) * 1);
  static int32_t param25_init[] = {4};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {4};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int8_t weights_init[] = {1, 2, 1, 2, 1, 2};
  model->setOperandValue(weights, weights_init, sizeof(int8_t) * 6);
  static int32_t bias_init[] = {4, 4, 4};
  model->setOperandValue(bias, bias_init, sizeof(int32_t) * 3);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static int32_t param28_init[] = {0};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {0};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {1};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {1};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param14, param15, param16, param17, param18, param19, param20}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param21, param22, param23, param24, param25, param26, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_CONV_2D, {featureMap, weights, bias, param27, param28, param29, param30, param31, param32, param33, layout}, {out});
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

