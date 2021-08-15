// clang-format off
// Generated file (from: hashtable_lookup_quant8.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_INT32, {4});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {3, 2}, 0.5f, 0);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {4, 2}, 0.5f, 0);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {4}, 1.0f, 0);
  // Phase 1, operands
  auto lookup = model->addOperand(&type0);
  auto key = model->addOperand(&type1);
  auto value = model->addOperand(&type2);
  auto output = model->addOperand(&type3);
  auto hits = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_HASHTABLE_LOOKUP, {lookup, key, value}, {output, hits});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {lookup, key, value},
    {output, hits});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_INT32, {4});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {3, 2}, 0.5f, 0);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.5f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {0}, 1.0f, 0);
  // Phase 1, operands
  auto lookup = model->addOperand(&type0);
  auto key = model->addOperand(&type1);
  auto value = model->addOperand(&type2);
  auto output = model->addOperand(&type5);
  auto hits = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_HASHTABLE_LOOKUP, {lookup, key, value}, {output, hits});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {lookup, key, value},
    {output, hits});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

