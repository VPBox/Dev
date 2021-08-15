// clang-format off
// Generated file (from: pad_float_1.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3, 1});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 4, 7, 1});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t op2_init[] = {0, 0, 0, 2, 1, 3, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {op1, op2}, {op3});
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3, 1});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t op2_init[] = {0, 0, 0, 2, 1, 3, 0, 0};
  model->setOperandValue(op2, op2_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {op1, op2}, {op3});
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

