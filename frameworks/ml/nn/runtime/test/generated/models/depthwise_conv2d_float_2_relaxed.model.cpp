// clang-format off
// Generated file (from: depthwise_conv2d_float_2_relaxed.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {4});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 2, 1, 4});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto pad_valid = model->addOperand(&type3);
  auto stride = model->addOperand(&type3);
  auto channelMultiplier = model->addOperand(&type3);
  auto act_none = model->addOperand(&type3);
  auto op4 = model->addOperand(&type4);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t pad_valid_init[] = {2};
  model->setOperandValue(pad_valid, pad_valid_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {1};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t channelMultiplier_init[] = {2};
  model->setOperandValue(channelMultiplier, channelMultiplier_init, sizeof(int32_t) * 1);
  static int32_t act_none_init[] = {0};
  model->setOperandValue(act_none, act_none_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, pad_valid, stride, stride, channelMultiplier, act_none}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 4});
  OperandType type2(Type::TENSOR_FLOAT32, {4});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type1);
  auto op3 = model->addOperand(&type2);
  auto pad_valid = model->addOperand(&type3);
  auto stride = model->addOperand(&type3);
  auto channelMultiplier = model->addOperand(&type3);
  auto act_none = model->addOperand(&type3);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 3.0f, 4.0f, -9.0f, 10.0f, -11.0f, 12.0f, 5.0f, 6.0f, 7.0f, 8.0f, 13.0f, -14.0f, 15.0f, -16.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 16);
  static float op3_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 4);
  static int32_t pad_valid_init[] = {2};
  model->setOperandValue(pad_valid, pad_valid_init, sizeof(int32_t) * 1);
  static int32_t stride_init[] = {1};
  model->setOperandValue(stride, stride_init, sizeof(int32_t) * 1);
  static int32_t channelMultiplier_init[] = {2};
  model->setOperandValue(channelMultiplier, channelMultiplier_init, sizeof(int32_t) * 1);
  static int32_t act_none_init[] = {0};
  model->setOperandValue(act_none, act_none_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_DEPTHWISE_CONV_2D, {op1, op2, op3, pad_valid, stride, stride, channelMultiplier, act_none}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

