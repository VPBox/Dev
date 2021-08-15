// clang-format off
// Generated file (from: pad_low_rank_quant8.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {3}, 2.3f, 0);
  OperandType type1(Type::TENSOR_INT32, {1, 2});
  OperandType type2(Type::TENSOR_QUANT8_ASYMM, {7}, 2.3f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t paddings_init[] = {3, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
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
  OperandType type0(Type::TENSOR_QUANT8_ASYMM, {3}, 2.3f, 0);
  OperandType type1(Type::TENSOR_INT32, {1, 2});
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {0}, 2.3f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t paddings_init[] = {3, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
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

