// clang-format off
// Generated file (from: pow.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
    {output});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent = model->addOperand(&type1);
  auto output = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
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
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  OperandType type7(Type::TENSOR_FLOAT16, {1});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent = model->addOperand(&type7);
  auto output = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent = model->addOperand(&type1);
  auto output = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type1(Type::TENSOR_FLOAT32, {1});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent = model->addOperand(&type1);
  auto output = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
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
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  OperandType type7(Type::TENSOR_FLOAT16, {1});
  OperandType type9(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent = model->addOperand(&type7);
  auto output = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent}, {output});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent},
    {output});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent1 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {2, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent1 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type11(Type::TENSOR_FLOAT16, {2, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent1 = model->addOperand(&type10);
  auto output1 = model->addOperand(&type11);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent1 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent1 = model->addOperand(&type2);
  auto output1 = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  OperandType type9(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent1 = model->addOperand(&type10);
  auto output1 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent1}, {output1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent1},
    {output1});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {3, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {3, 2, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent2 = model->addOperand(&type3);
  auto output2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {3, 1, 2});
  OperandType type5(Type::TENSOR_FLOAT32, {3, 2, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent2 = model->addOperand(&type3);
  auto output2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT16, {3, 1, 2});
  OperandType type13(Type::TENSOR_FLOAT16, {3, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent2 = model->addOperand(&type12);
  auto output2 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {3, 1, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent2 = model->addOperand(&type3);
  auto output2 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {3, 1, 2});
  // Phase 1, operands
  auto base = model->addOperand(&type0);
  auto exponent2 = model->addOperand(&type3);
  auto output2 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT16, {3, 1, 2});
  OperandType type15(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 1});
  // Phase 1, operands
  auto base = model->addOperand(&type6);
  auto exponent2 = model->addOperand(&type12);
  auto output2 = model->addOperand(&type15);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_POW, {base, exponent2}, {output2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {base, exponent2},
    {output2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

