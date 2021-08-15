// clang-format off
// Generated file (from: pad_v2_all_dims.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 8, 8, 6});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static float pad_value_init[] = {3.9f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(float) * 1);
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

void CreateModel_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type4(Type::TENSOR_FLOAT16, {1, 1, 2, 3});
  OperandType type5(Type::TENSOR_FLOAT16, {4, 8, 8, 6});
  OperandType type6(Type::FLOAT16, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type6);
  auto output0 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static _Float16 pad_value_init[] = {3.9000000953674316f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
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
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 8, 8, 6});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static float pad_value_init[] = {3.9f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
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

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static float pad_value_init[] = {3.9f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(float) * 1);
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

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {4, 2});
  OperandType type4(Type::TENSOR_FLOAT16, {1, 1, 2, 3});
  OperandType type6(Type::FLOAT16, {});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type6);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static _Float16 pad_value_init[] = {3.9000000953674316f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
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
  OperandType type2(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto paddings = model->addOperand(&type1);
  auto pad_value = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t paddings_init[] = {1, 2, 3, 4, 3, 3, 2, 1};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 8);
  static float pad_value_init[] = {3.9f};
  model->setOperandValue(pad_value, pad_value_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_PAD_V2, {input0, paddings, pad_value}, {output0});
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

