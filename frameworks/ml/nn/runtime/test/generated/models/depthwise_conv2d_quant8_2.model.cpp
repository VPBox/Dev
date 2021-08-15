// clang-format off
// Generated file (from: depthwise_conv2d_quant8_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 127);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 127);
  OperandType type2(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {1, 2, 1, 4}, 1.0f, 127);
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
  static uint8_t op2_init[] = {129, 131, 133, 135, 109, 147, 105, 151, 137, 139, 141, 143, 153, 99, 157, 95};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {4, 8, 12, 16};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
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
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 127);
  OperandType type1(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 4}, 0.5f, 127);
  OperandType type2(Type::TENSOR_INT32, {4}, 0.25f, 0);
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 1.0f, 127);
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
  static uint8_t op2_init[] = {129, 131, 133, 135, 109, 147, 105, 151, 137, 139, 141, 143, 153, 99, 157, 95};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 16);
  static int32_t op3_init[] = {4, 8, 12, 16};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 4);
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
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

