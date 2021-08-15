// clang-format off
// Generated file (from: tile_3.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 6, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 6, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type3(Type::TENSOR_FLOAT16, {1, 2, 3});
  OperandType type4(Type::TENSOR_FLOAT16, {2, 6, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3}, 0.5f, 127);
  OperandType type6(Type::TENSOR_QUANT8_ASYMM, {2, 6, 3}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_int32(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type7(Type::TENSOR_INT32, {1, 2, 3});
  OperandType type8(Type::TENSOR_INT32, {2, 6, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type7);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 2, 3});
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type9(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type10(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT16, {1, 2, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type3);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.5f, 127);
  OperandType type5(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3}, 0.5f, 127);
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_int32(Model *model) {
  OperandType type1(Type::TENSOR_INT32, {3});
  OperandType type12(Type::TENSOR_INT32, {0, 0, 0});
  OperandType type7(Type::TENSOR_INT32, {1, 2, 3});
  // Phase 1, operands
  auto input0 = model->addOperand(&type7);
  auto multipliers = model->addOperand(&type1);
  auto output0 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_TILE, {input0, multipliers}, {output0});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input0, multipliers},
    {output0});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_int32(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

