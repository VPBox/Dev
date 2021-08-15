// clang-format off
// Generated file (from: softmax_v1_2.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_dim3_axis2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_relaxed_dim3_axis2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type15(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_dim1_axis0(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_dim3_axis2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_dim1_axis0(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_dim3_axis2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_dynamic_output_shape_dim3_axis2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
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

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_dim1_axis0(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_dim3_axis2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type15);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_dim1_axis0(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_dim3_axis2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param = model->addOperand(&type2);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0_2(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim3_axis2_2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim1_axis0_2(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_dim3_axis2_2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type15(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_dim1_axis0_2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_dim3_axis2_2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_float16_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_2(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_dim1_axis0_2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_quant8_dim3_axis2_2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_quant8_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim1_axis0_2(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_dim3_axis2_2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0_2(Model *model) {
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2_2(Model *model) {
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type14(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_dim1_axis0_2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_dim3_axis2_2(Model *model) {
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param1_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_2(Model *model) {
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_dim1_axis0_2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_quant8_dim3_axis2_2(Model *model) {
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param1 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param1_init[] = {1e-06f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param1}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_quant8_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type40);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type40);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type41);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type41);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type42);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type42);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis3_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type43);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type43);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type44);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type44);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type45);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type45);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type46);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type46);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type48);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type48);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type50);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type50);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type52);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type52);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis3_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type54);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type54);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type56);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type56);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type58);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type58);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type60);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type60);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param2 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param2 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param2, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_neg(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type33);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type34);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type35);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type36);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type37);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type38);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type39);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type40);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type40);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type41);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type41);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type42);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type42);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis3_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim4_axis3_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type43);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type43);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type44);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type44);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type45);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type45);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type46);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type46);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_float16_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_float16_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type48);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  OperandType type48(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type48);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type50);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  OperandType type50(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type50);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type52);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type52);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis3_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim4_axis3_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type54);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type54);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type56);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  OperandType type56(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type56);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type58);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type58);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type60);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type60(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type60);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_quant8_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {5}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_quant8_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT32, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type33);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT32, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type34);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT32, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type35);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 5});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type36(Type::TENSOR_FLOAT32, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type36);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type37(Type::TENSOR_FLOAT32, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type37);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 2, 5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type38(Type::TENSOR_FLOAT32, {5, 2});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type38);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type39(Type::TENSOR_FLOAT32, {2, 5});
  OperandType type61(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type39);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type61);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT32, {5});
  OperandType type2(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type5);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type40(Type::TENSOR_FLOAT16, {5, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type40);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type41(Type::TENSOR_FLOAT16, {2, 5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type41);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT16, {2, 2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type42);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim4_axis3_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 2, 5});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type43(Type::TENSOR_FLOAT16, {5, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type44(Type::TENSOR_FLOAT16, {2, 5, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type18(Type::TENSOR_FLOAT16, {2, 2, 5});
  OperandType type29(Type::TENSOR_FLOAT16, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type45(Type::TENSOR_FLOAT16, {5, 2});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type46(Type::TENSOR_FLOAT16, {2, 5});
  OperandType type62(Type::TENSOR_FLOAT16, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type46);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type62);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_float16_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type17(Type::TENSOR_FLOAT16, {5});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param3 = model->addOperand(&type15);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static _Float16 param3_init[] = {9.999999974752427e-07f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_float16_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type47(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type49(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 5}, 0.25f, 128);
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim4_axis3_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {5, 2, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type53);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {2, 5, 2}, 0.25f, 128);
  // Phase 1, operands
  auto op1 = model->addOperand(&type55);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {2, 2, 5}, 0.25f, 128);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type23);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type32);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim3_axis2_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type57(Type::TENSOR_QUANT8_ASYMM, {5, 2}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type57);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type59(Type::TENSOR_QUANT8_ASYMM, {2, 5}, 0.25f, 128);
  OperandType type63(Type::TENSOR_QUANT8_ASYMM, {0, 0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type59);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type63);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim2_axis1_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_QUANT8_ASYMM, {5}, 0.25f, 128);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0}, 0.00390625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type31);
  // Phase 2, operations
  static float param3_init[] = {1e-06f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {op1, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_quant8_dim1_axis0_neg_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type10);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type11);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_relaxed(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type10);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type11);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_quant8(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type66(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.00390625f, 0);
  OperandType type67(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type69);
  auto roi = model->addOperand(&type67);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type70);
  auto roiOut = model->addOperand(&type68);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type65);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type64);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type66);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type72(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type73(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type74(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type75(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type76(Type::TENSOR_FLOAT16, {0});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type75);
  auto roi = model->addOperand(&type73);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type15);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type15);
  auto param9 = model->addOperand(&type15);
  auto param10 = model->addOperand(&type15);
  auto scoresOut = model->addOperand(&type76);
  auto roiOut = model->addOperand(&type74);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type72);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type15);
  auto param14 = model->addOperand(&type15);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type71);
  auto param17 = model->addOperand(&type15);
  auto out = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {0.30000001192092896f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {0.4000000059604645f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {0.30000001192092896f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type10);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_relaxed(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type5(Type::TENSOR_FLOAT32, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type3);
  auto roi = model->addOperand(&type4);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type5);
  auto roiOut = model->addOperand(&type7);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type10);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type11);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type25);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_quant8(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.00390625f, 0);
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type67(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type68(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type69(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type70(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type69);
  auto roi = model->addOperand(&type67);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type2);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto scoresOut = model->addOperand(&type70);
  auto roiOut = model->addOperand(&type68);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type65);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type64);
  auto param17 = model->addOperand(&type2);
  auto out = model->addOperand(&type30);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {0.3f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static float param8_init[] = {0.4f};
  model->setOperandValue(param8, param8_init, sizeof(float) * 1);
  static float param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {0.3f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::FLOAT16, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {0});
  OperandType type6(Type::TENSOR_INT32, {0});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type72(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type73(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type74(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type75(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type8(Type::TENSOR_INT32, {1});
  OperandType type9(Type::BOOL, {});
  // Phase 1, operands
  auto scores = model->addOperand(&type75);
  auto roi = model->addOperand(&type73);
  auto param4 = model->addOperand(&type8);
  auto param5 = model->addOperand(&type15);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type1);
  auto param8 = model->addOperand(&type15);
  auto param9 = model->addOperand(&type15);
  auto param10 = model->addOperand(&type15);
  auto scoresOut = model->addOperand(&type28);
  auto roiOut = model->addOperand(&type74);
  auto classesOut = model->addOperand(&type6);
  auto batchSplitOut = model->addOperand(&type6);
  auto in = model->addOperand(&type72);
  auto param11 = model->addOperand(&type1);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type15);
  auto param14 = model->addOperand(&type15);
  auto param15 = model->addOperand(&type1);
  auto param16 = model->addOperand(&type1);
  auto layout = model->addOperand(&type9);
  auto featureMap = model->addOperand(&type71);
  auto param17 = model->addOperand(&type15);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param4_init[] = {0};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {0.30000001192092896f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static int32_t param6_init[] = {-1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {0};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static _Float16 param8_init[] = {0.4000000059604645f};
  model->setOperandValue(param8, param8_init, sizeof(_Float16) * 1);
  static _Float16 param9_init[] = {1.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {0.30000001192092896f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {2.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {2.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static int32_t param15_init[] = {4};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {4};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param4, param5, param6, param7, param8, param9, param10}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param11, param12, param13, param14, param15, param16, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_SOFTMAX, {featureMap, param17}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

