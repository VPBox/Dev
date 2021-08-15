// clang-format off
// Generated file (from: resize_bilinear_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto width = model->addOperand(&type2);
  auto height = model->addOperand(&type2);
  auto op2 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t width_init[] = {3};
  model->setOperandValue(width, width_init, sizeof(int32_t) * 1);
  static int32_t height_init[] = {3};
  model->setOperandValue(height, height_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, width, height}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto width = model->addOperand(&type2);
  auto height = model->addOperand(&type2);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t width_init[] = {3};
  model->setOperandValue(width, width_init, sizeof(int32_t) * 1);
  static int32_t height_init[] = {3};
  model->setOperandValue(height, height_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, width, height}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

