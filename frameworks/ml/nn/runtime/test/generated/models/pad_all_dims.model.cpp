// clang-format off
// Generated file (from: pad_all_dims.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 8, 8, 6});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
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

void CreateModel_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type3(Type::TENSOR_FLOAT16, {1, 1, 2, 3});
  OperandType type4(Type::TENSOR_FLOAT16, {4, 8, 8, 6});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {4, 8, 8, 6});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 3}, 2.3f, 0);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {4, 8, 8, 6}, 2.3f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
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

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type3(Type::TENSOR_FLOAT16, {1, 1, 2, 3});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 3}, 2.3f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.3f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto paddings = model->addOperand(&type1);
  auto output0 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  model->addOperation(ANEURALNETWORKS_PAD, {input0, paddings}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

