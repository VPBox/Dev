// clang-format off
// Generated file (from: gather_higher_rank.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
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
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2}, 0.5f, 127);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type6(Type::TENSOR_INT32, {1, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type6);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 3, 2});
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
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
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2}, 0.5f, 127);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3, 2});
  OperandType type10(Type::TENSOR_INT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type6(Type::TENSOR_INT32, {1, 3, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type6);
  auto param = model->addOperand(&type3);
  auto indices = model->addOperand(&type1);
  auto output0 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_GATHER, {input0, param, indices}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, indices},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

