// clang-format off
// Generated file (from: squeeze_relaxed.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type1(Type::TENSOR_INT32, {2});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 2});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto squeezeDims = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t squeezeDims_init[] = {1, 2};
  model->setOperandValue(squeezeDims, squeezeDims_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_SQUEEZE, {input, squeezeDims}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
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
  OperandType type0(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type1(Type::TENSOR_INT32, {2});
  OperandType type3(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto squeezeDims = model->addOperand(&type1);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t squeezeDims_init[] = {1, 2};
  model->setOperandValue(squeezeDims, squeezeDims_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_SQUEEZE, {input, squeezeDims}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

