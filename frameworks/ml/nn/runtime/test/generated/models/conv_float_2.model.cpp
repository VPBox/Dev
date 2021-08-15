// clang-format off
// Generated file (from: conv_float_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto pad_same = model->addOperand(&type3);
  auto stride = model->addOperand(&type3);
  auto act_relu = model->addOperand(&type3);
  auto op4 = model->addOperand(&type0);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 9);
  static float op3_init[] = {-200.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
  static int32_t pad_same_init[] = {1};
  model->setOperandValue(pad_same, pad_same_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {1};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t act_relu_init[] = {1};
  model->setOperandValue(act_relu, act_relu_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, pad_same, stride, stride, act_relu}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 4, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto pad_same = model->addOperand(&type3);
  auto stride = model->addOperand(&type3);
  auto act_relu = model->addOperand(&type3);
  auto op4 = model->addOperand(&type4);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 9);
  static float op3_init[] = {-200.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
  static int32_t pad_same_init[] = {1};
  model->setOperandValue(pad_same, pad_same_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {1};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t act_relu_init[] = {1};
  model->setOperandValue(act_relu, act_relu_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, pad_same, stride, stride, act_relu}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

