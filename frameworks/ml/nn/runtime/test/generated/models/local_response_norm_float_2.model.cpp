// clang-format off
// Generated file (from: local_response_norm_float_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto radius = model->addOperand(&type1);
  auto bias = model->addOperand(&type2);
  auto alpha = model->addOperand(&type2);
  auto beta = model->addOperand(&type2);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t radius_init[] = {20};
  model->setOperandValue(radius, radius_init, sizeof(int32_t) * 1);
  static float bias_init[] = {0.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 1);
  static float alpha_init[] = {1.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 1);
  static float beta_init[] = {0.5f};
  model->setOperandValue(beta, beta_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {input, radius, bias, alpha, beta}, {output});
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
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input = model->addOperand(&type0);
  auto radius = model->addOperand(&type1);
  auto bias = model->addOperand(&type2);
  auto alpha = model->addOperand(&type2);
  auto beta = model->addOperand(&type2);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t radius_init[] = {20};
  model->setOperandValue(radius, radius_init, sizeof(int32_t) * 1);
  static float bias_init[] = {0.0f};
  model->setOperandValue(bias, bias_init, sizeof(float) * 1);
  static float alpha_init[] = {1.0f};
  model->setOperandValue(alpha, alpha_init, sizeof(float) * 1);
  static float beta_init[] = {0.5f};
  model->setOperandValue(beta, beta_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {input, radius, bias, alpha, beta}, {output});
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

