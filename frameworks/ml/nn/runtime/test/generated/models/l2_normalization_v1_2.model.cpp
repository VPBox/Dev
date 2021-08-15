// clang-format off
// Generated file (from: l2_normalization_v1_2.mod.py). Do not edit
void CreateModel_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim4_axis3_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dim4_axis3_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim4_axis3_float16(Model *model) {
  OperandType type2(Type::TENSOR_FLOAT16, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim4_axis3_quant8(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.1f, 32);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim4_axis3_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim3_axis2(Model *model) {
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim3_axis2_relaxed(Model *model) {
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dim3_axis2_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim3_axis2_float16(Model *model) {
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim3_axis2_quant8(Model *model) {
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.1f, 32);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim3_axis2_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim2_axis1(Model *model) {
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim2_axis1_relaxed(Model *model) {
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dim2_axis1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim2_axis1_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim2_axis1_quant8(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.1f, 32);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim2_axis1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0_relaxed(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dim1_axis0_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0_quant8(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {3}, 0.1f, 32);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim1_axis0_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim4_axis3_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim4_axis3_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim4_axis3_float16(Model *model) {
  OperandType type2(Type::TENSOR_FLOAT16, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type2);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim4_axis3_quant8(Model *model) {
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.1f, 32);
  OperandType type4(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim4_axis3_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim3_axis2(Model *model) {
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim3_axis2_relaxed(Model *model) {
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim3_axis2_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim3_axis2_quant8(Model *model) {
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.1f, 32);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim3_axis2_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim2_axis1(Model *model) {
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim2_axis1_relaxed(Model *model) {
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim2_axis1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim2_axis1_quant8(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.1f, 32);
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim2_axis1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0_relaxed(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim1_axis0_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0_quant8(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {3}, 0.1f, 32);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {3}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim1_axis0_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim4_axis3_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim4_axis3_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim4_axis3_float16(Model *model) {
  OperandType type18(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT16, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim4_axis3_quant8(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0078125f, 128);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.1f, 32);
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim4_axis3_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim3_axis2(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim3_axis2_relaxed(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim3_axis2_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim3_axis2_float16(Model *model) {
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim3_axis2_quant8(Model *model) {
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.1f, 32);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim3_axis2_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim2_axis1(Model *model) {
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim2_axis1_relaxed(Model *model) {
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim2_axis1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim2_axis1_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim2_axis1_quant8(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.1f, 32);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim2_axis1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  OperandType type26(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0_relaxed(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  OperandType type26(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim1_axis0_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {3});
  OperandType type27(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0_quant8(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {3}, 0.1f, 32);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim1_axis0_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim4_axis3_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 3});
  OperandType type17(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim4_axis3_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim4_axis3_float16(Model *model) {
  OperandType type18(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT16, {2, 2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim4_axis3_quant8(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.0078125f, 128);
  OperandType type3(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 3}, 0.1f, 32);
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim4_axis3_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2_relaxed(Model *model) {
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim3_axis2_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT16, {2, 2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2_quant8(Model *model) {
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.0078125f, 128);
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3}, 0.1f, 32);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim3_axis2_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1(Model *model) {
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1_relaxed(Model *model) {
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {2, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim2_axis1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {2, 3});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1_quant8(Model *model) {
  OperandType type11(Type::TENSOR_QUANT8_ASYMM, {2, 3}, 0.1f, 32);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim2_axis1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  OperandType type26(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0_relaxed(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {3});
  OperandType type26(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim1_axis0_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {3});
  OperandType type27(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0_quant8(Model *model) {
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {3}, 0.1f, 32);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0}, 0.0078125f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  model->addOperation(ANEURALNETWORKS_L2_NORMALIZATION, {op1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim1_axis0_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

