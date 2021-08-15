// clang-format off
// Generated file (from: avg_pool_quant8_5.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 4, 1}, 0.0625f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto pad_same = model->addOperand(&type1);
  auto cons2 = model->addOperand(&type1);
  auto act_none = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t pad_same_init[] = {1};
  model->setOperandValue(pad_same, pad_same_init, sizeof(int32_t) * 1);
  static int32_t cons2_init[] = {2};
  model->setOperandValue(cons2, cons2_init, sizeof(int32_t) * 1);
  static int32_t act_none_init[] = {0};
  model->setOperandValue(act_none, act_none_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_AVERAGE_POOL_2D, {op1, pad_same, cons2, cons2, cons2, cons2, act_none}, {op3});
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
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 2, 4, 1}, 0.0625f, 0);
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto pad_same = model->addOperand(&type1);
  auto cons2 = model->addOperand(&type1);
  auto act_none = model->addOperand(&type1);
  auto op3 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t pad_same_init[] = {1};
  model->setOperandValue(pad_same, pad_same_init, sizeof(int32_t) * 1);
  static int32_t cons2_init[] = {2};
  model->setOperandValue(cons2, cons2_init, sizeof(int32_t) * 1);
  static int32_t act_none_init[] = {0};
  model->setOperandValue(act_none, act_none_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_AVERAGE_POOL_2D, {op1, pad_same, cons2, cons2, cons2, cons2, act_none}, {op3});
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

