// clang-format off
// Generated file (from: embedding_lookup_relaxed.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_INT32, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3, 2, 4});
  // Phase 1, operands
  auto index = model->addOperand(&type0);
  auto value = model->addOperand(&type1);
  auto output = model->addOperand(&type1);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_EMBEDDING_LOOKUP, {index, value}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {index, value},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_INT32, {3});
  OperandType type1(Type::TENSOR_FLOAT32, {3, 2, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto index = model->addOperand(&type0);
  auto value = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_EMBEDDING_LOOKUP, {index, value}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {index, value},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

