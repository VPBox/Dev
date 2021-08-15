// clang-format off
// Generated file (from: strided_slice_float_7.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begins = model->addOperand(&type1);
  auto ends = model->addOperand(&type1);
  auto strides = model->addOperand(&type1);
  auto beginMask = model->addOperand(&type2);
  auto endMask = model->addOperand(&type2);
  auto shrinkAxisMask = model->addOperand(&type2);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t begins_init[] = {-1};
  model->setOperandValue(begins, begins_init, sizeof(int32_t) * 1);
  static int32_t ends_init[] = {-4};
  model->setOperandValue(ends, ends_init, sizeof(int32_t) * 1);
  static int32_t strides_init[] = {-1};
  model->setOperandValue(strides, strides_init, sizeof(int32_t) * 1);
  static int32_t beginMask_init[] = {0};
  model->setOperandValue(beginMask, beginMask_init, sizeof(int32_t) * 1);
  static int32_t endMask_init[] = {0};
  model->setOperandValue(endMask, endMask_init, sizeof(int32_t) * 1);
  static int32_t shrinkAxisMask_init[] = {0};
  model->setOperandValue(shrinkAxisMask, shrinkAxisMask_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_STRIDED_SLICE, {input, begins, ends, strides, beginMask, endMask, shrinkAxisMask}, {output});
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
  OperandType type0(Type::TENSOR_FLOAT32, {3});
  OperandType type1(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto begins = model->addOperand(&type1);
  auto ends = model->addOperand(&type1);
  auto strides = model->addOperand(&type1);
  auto beginMask = model->addOperand(&type2);
  auto endMask = model->addOperand(&type2);
  auto shrinkAxisMask = model->addOperand(&type2);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t begins_init[] = {-1};
  model->setOperandValue(begins, begins_init, sizeof(int32_t) * 1);
  static int32_t ends_init[] = {-4};
  model->setOperandValue(ends, ends_init, sizeof(int32_t) * 1);
  static int32_t strides_init[] = {-1};
  model->setOperandValue(strides, strides_init, sizeof(int32_t) * 1);
  static int32_t beginMask_init[] = {0};
  model->setOperandValue(beginMask, beginMask_init, sizeof(int32_t) * 1);
  static int32_t endMask_init[] = {0};
  model->setOperandValue(endMask, endMask_init, sizeof(int32_t) * 1);
  static int32_t shrinkAxisMask_init[] = {0};
  model->setOperandValue(shrinkAxisMask, shrinkAxisMask_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_STRIDED_SLICE, {input, begins, ends, strides, beginMask, endMask, shrinkAxisMask}, {output});
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

