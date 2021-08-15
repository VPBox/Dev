// clang-format off
// Generated file (from: fully_connected_quant8_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {4, 1, 5, 1}, 0.5f, 127);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {3, 10}, 0.5f, 127);
  OperandType type2(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 1.0f, 127);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto b0 = model->addOperand(&type2);
  auto act_relu = model->addOperand(&type4);
  auto op3 = model->addOperand(&type3);
  // Phase 2, operations
  static uint8_t op2_init[] = {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 30);
  static int32_t b0_init[] = {4, 8, 12};
  model->setOperandValue(b0, b0_init, sizeof(int32_t) * 3);
  static int32_t act_relu_init[] = {1};
  model->setOperandValue(act_relu, act_relu_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_FULLY_CONNECTED, {op1, op2, b0, act_relu}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op3});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {4, 1, 5, 1}, 0.5f, 127);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {3, 10}, 0.5f, 127);
  OperandType type2(Type::TENSOR_INT32, {3}, 0.25f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 1.0f, 127);
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto b0 = model->addOperand(&type2);
  auto act_relu = model->addOperand(&type4);
  auto op3 = model->addOperand(&type5);
  // Phase 2, operations
  static uint8_t op2_init[] = {129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 30);
  static int32_t b0_init[] = {4, 8, 12};
  model->setOperandValue(b0, b0_init, sizeof(int32_t) * 3);
  static int32_t act_relu_init[] = {1};
  model->setOperandValue(act_relu, act_relu_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_FULLY_CONNECTED, {op1, op2, b0, act_relu}, {op3});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op3});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

