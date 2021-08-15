// clang-format off
// Generated file (from: svdf.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 10});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 40});
  OperandType type5(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 4});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights_feature = model->addOperand(&type1);
  auto weights_time = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto state_in = model->addOperand(&type4);
  auto rank_param = model->addOperand(&type5);
  auto activation_param = model->addOperand(&type5);
  auto state_out = model->addOperand(&type4);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t rank_param_init[] = {1};
  model->setOperandValue(rank_param, rank_param_init, sizeof(int32_t) * 1);
  static int32_t activation_param_init[] = {0};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SVDF, {input, weights_feature, weights_time, bias, state_in, rank_param, activation_param}, {state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights_feature, weights_time, bias, state_in},
    {state_out, output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 10});
  OperandType type3(Type::TENSOR_FLOAT32, {4});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 40});
  OperandType type5(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto weights_feature = model->addOperand(&type1);
  auto weights_time = model->addOperand(&type2);
  auto bias = model->addOperand(&type3);
  auto state_in = model->addOperand(&type4);
  auto rank_param = model->addOperand(&type5);
  auto activation_param = model->addOperand(&type5);
  auto state_out = model->addOperand(&type7);
  auto output = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t rank_param_init[] = {1};
  model->setOperandValue(rank_param, rank_param_init, sizeof(int32_t) * 1);
  static int32_t activation_param_init[] = {0};
  model->setOperandValue(activation_param, activation_param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SVDF, {input, weights_feature, weights_time, bias, state_in, rank_param, activation_param}, {state_out, output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input, weights_feature, weights_time, bias, state_in},
    {state_out, output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

