// clang-format off
// Generated file (from: pad_v2_low_rank_quant8.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {3}, 2.3f, 4);
  OperandType type1(Type::TENSOR_INT32, {1, 2});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {7}, 2.3f, 4);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t paddings_init[] = {3, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 2);
  static int32_t pad_value_init[] = {9};
  model->setOperandValue(pad_value, pad_value_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {3}, 2.3f, 4);
  OperandType type1(Type::TENSOR_INT32, {1, 2});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {0}, 2.3f, 4);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t paddings_init[] = {3, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 2);
  static int32_t pad_value_init[] = {9};
  model->setOperandValue(pad_value, pad_value_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

