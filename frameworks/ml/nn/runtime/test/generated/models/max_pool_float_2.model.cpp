// clang-format off
// Generated file (from: max_pool_float_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 50, 70, 3});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_FLOAT32, {5, 2, 3, 3});
  // Phase 1, operands
  auto i0 = model->addOperand(&type0);
  auto padding = model->addOperand(&type1);
  auto stride = model->addOperand(&type1);
  auto filter = model->addOperand(&type1);
  auto activation = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t padding_init[] = {0};
  model->setOperandValue(padding, padding_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {20};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t filter_init[] = {20};
  model->setOperandValue(filter, filter_init, sizeof(int32_t) * 1);
  static int32_t activation_init[] = {0};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_MAX_POOL_2D, {i0, padding, padding, padding, padding, stride, stride, filter, filter, activation}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {i0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {5, 50, 70, 3});
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto i0 = model->addOperand(&type0);
  auto padding = model->addOperand(&type1);
  auto stride = model->addOperand(&type1);
  auto filter = model->addOperand(&type1);
  auto activation = model->addOperand(&type1);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t padding_init[] = {0};
  model->setOperandValue(padding, padding_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {20};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t filter_init[] = {20};
  model->setOperandValue(filter, filter_init, sizeof(int32_t) * 1);
  static int32_t activation_init[] = {0};
  model->setOperandValue(activation, activation_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_MAX_POOL_2D, {i0, padding, padding, padding, padding, stride, stride, filter, filter, activation}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {i0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

