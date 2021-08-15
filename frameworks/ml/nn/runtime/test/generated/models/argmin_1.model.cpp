// clang-format off
// Generated file (from: argmin_1.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_INT32, {2, 2});
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.0f, 0);
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type6(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2});
  OperandType type1(Type::INT32, {});
  OperandType type6(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type6(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_INT32, {2, 2});
  OperandType type6(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {2, 2}, 1.0f, 0);
  OperandType type6(Type::TENSOR_INT32, {0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_ARGMIN, {input0, axis}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

