// clang-format off
// Generated file (from: mean_float_2_relaxed.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {2});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 3, 1});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto keepDims = model->addOperand(&type2);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t axis_init[] = {0, 2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 2);
  static int32_t keepDims_init[] = {1};
  model->setOperandValue(keepDims, keepDims_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_MEAN, {input, axis, keepDims}, {output});
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
  OperandType type0(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {2});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto keepDims = model->addOperand(&type2);
  auto output = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t axis_init[] = {0, 2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 2);
  static int32_t keepDims_init[] = {1};
  model->setOperandValue(keepDims, keepDims_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_MEAN, {input, axis, keepDims}, {output});
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

