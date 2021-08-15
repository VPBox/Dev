// clang-format off
// Generated file (from: transpose_quant8_1.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 3, 4, 5}, 1.0f, 0);
  OperandType type1(Type::TENSOR_INT32, {4});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {4, 2, 3, 5}, 1.0f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t perms_init[] = {2, 0, 1, 3};
  model->setOperandValue(perms, perms_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {2, 3, 4, 5}, 1.0f, 0);
  OperandType type1(Type::TENSOR_INT32, {4});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 0);
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto perms = model->addOperand(&type1);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t perms_init[] = {2, 0, 1, 3};
  model->setOperandValue(perms, perms_init, sizeof(int32_t) * 4);
  model->addOperation(ANEURALNETWORKS_TRANSPOSE, {input, perms}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

