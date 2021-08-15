// clang-format off
// Generated file (from: transpose_conv2d_large.mod.py). Do not edit
void CreateModel_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_QUANT8_ASYMM, {25, 32, 32, 16}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {25, 1, 1, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {16, 1, 1, 1}, 0.5f, 0);
  OperandType type9(Type::TENSOR_INT32, {16}, 0.25f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto shape = model->addOperand(&type4);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto act = model->addOperand(&type5);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type10);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 16);
  static int32_t shape_init[] = {25, 32, 32, 16};
  model->setOperandValue(shape, shape_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {32};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {32};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE_CONV_2D, {op1, op2, op3, shape, param, param1, param2, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {25, 1, 1, 1}, 0.25f, 100);
  OperandType type12(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {16, 1, 1, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},0));
  OperandType type13(Type::TENSOR_INT32, {16}, 0.0f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {25, 32, 32, 16}, 0.5f, 80);
  OperandType type4(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type12);
  auto op3 = model->addOperand(&type13);
  auto shape = model->addOperand(&type4);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto act = model->addOperand(&type5);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 16);
  static int32_t shape_init[] = {25, 32, 32, 16};
  model->setOperandValue(shape, shape_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {32};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {32};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE_CONV_2D, {op1, op2, op3, shape, param, param1, param2, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {25, 1, 1, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {16, 1, 1, 1}, 0.5f, 0);
  OperandType type9(Type::TENSOR_INT32, {16}, 0.25f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto shape = model->addOperand(&type4);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto act = model->addOperand(&type5);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 16);
  static int32_t shape_init[] = {25, 32, 32, 16};
  model->setOperandValue(shape, shape_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {32};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {32};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE_CONV_2D, {op1, op2, op3, shape, param, param1, param2, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {25, 1, 1, 1}, 0.25f, 100);
  OperandType type12(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {16, 1, 1, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},0));
  OperandType type13(Type::TENSOR_INT32, {16}, 0.0f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type4(Type::TENSOR_INT32, {4});
  OperandType type5(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type12);
  auto op3 = model->addOperand(&type13);
  auto shape = model->addOperand(&type4);
  auto param = model->addOperand(&type5);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto act = model->addOperand(&type5);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static int8_t op2_init[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 16);
  static int32_t op3_init[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 16);
  static int32_t shape_init[] = {25, 32, 32, 16};
  model->setOperandValue(shape, shape_init, sizeof(int32_t) * 4);
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {32};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {32};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE_CONV_2D, {op1, op2, op3, shape, param, param1, param2, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

