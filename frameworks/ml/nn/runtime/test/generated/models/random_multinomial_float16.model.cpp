// clang-format off
// Generated file (from: random_multinomial_float16.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 1024});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type3(Type::TENSOR_INT32, {1, 128});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto sample_count = model->addOperand(&type1);
  auto seeds = model->addOperand(&type2);
  auto output = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t sample_count_init[] = {128};
  model->setOperandValue(sample_count, sample_count_init, sizeof(int32_t) * 1);
  static int32_t seeds_init[] = {37, 42};
  model->setOperandValue(seeds, seeds_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_RANDOM_MULTINOMIAL, {input0, sample_count, seeds}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT16, {1, 1024});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_INT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto sample_count = model->addOperand(&type1);
  auto seeds = model->addOperand(&type2);
  auto output = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t sample_count_init[] = {128};
  model->setOperandValue(sample_count, sample_count_init, sizeof(int32_t) * 1);
  static int32_t seeds_init[] = {37, 42};
  model->setOperandValue(seeds, seeds_init, sizeof(int32_t) * 2);
  model->addOperation(ANEURALNETWORKS_RANDOM_MULTINOMIAL, {input0, sample_count, seeds}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {0};
  return ignore.find(i) != ignore.end();
}

