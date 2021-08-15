// clang-format off
// Generated file (from: split_float_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type4(Type::TENSOR_FLOAT16, {1, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type4);
  auto output1 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type5);
  auto output1 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 3});
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type5);
  auto output1 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
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
  OperandType type3(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type6(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto axis = model->addOperand(&type1);
  auto num_splits = model->addOperand(&type1);
  auto output0 = model->addOperand(&type6);
  auto output1 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  static int32_t num_splits_init[] = {2};
  model->setOperandValue(num_splits, num_splits_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SPLIT, {input0, axis, num_splits}, {output0, output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0},
    {output0, output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

