// clang-format off
// Generated file (from: fully_connected_quant8_large.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 5}, 0.2f, 0);
  OperandType type1(Type::TENSOR_INT32, {1}, 0.04f, 0);
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1}, 1.0f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  auto b0 = model->addOperand(&type1);
  auto act = model->addOperand(&type3);
  auto op3 = model->addOperand(&type2);
  // Phase 2, operations
  static uint8_t op2_init[] = {10, 20, 20, 20, 10};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 5);
  static int32_t b0_init[] = {10};
  model->setOperandValue(b0, b0_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_FULLY_CONNECTED, {op1, op2, b0, act}, {op3});
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
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 5}, 0.2f, 0);
  OperandType type1(Type::TENSOR_INT32, {1}, 0.04f, 0);
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 1.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  auto b0 = model->addOperand(&type1);
  auto act = model->addOperand(&type3);
  auto op3 = model->addOperand(&type4);
  // Phase 2, operations
  static uint8_t op2_init[] = {10, 20, 20, 20, 10};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 5);
  static int32_t b0_init[] = {10};
  model->setOperandValue(b0, b0_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_FULLY_CONNECTED, {op1, op2, b0, act}, {op3});
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

