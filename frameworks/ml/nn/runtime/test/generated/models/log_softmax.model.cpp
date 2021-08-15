// clang-format off
// Generated file (from: log_softmax.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type0);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_float16(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 1, 1, 2, 4});
  OperandType type6(Type::FLOAT16, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type5);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_dynamic_output_shape(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_dynamic_output_shape_relaxed(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type7);
  // Phase 2, operations
  static float param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(float) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_dynamic_output_shape_float16(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 1, 1, 2, 4});
  OperandType type6(Type::FLOAT16, {});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input0 = model->addOperand(&type5);
  auto param = model->addOperand(&type6);
  auto param1 = model->addOperand(&type2);
  auto output0 = model->addOperand(&type8);
  // Phase 2, operations
  static _Float16 param_init[] = {1.0f};
  model->setOperandValue(param, param_init, sizeof(_Float16) * 1);
  static int32_t param1_init[] = {4};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input0, param, param1}, {output0});
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

void CreateModel_2(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 1, 1, 4, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type3);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_2(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 1, 1, 4, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type3);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_2(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type6(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 1, 4, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type9);
  auto param2 = model->addOperand(&type6);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type9);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_2(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 1, 1, 4, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type7);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_2(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 1, 1, 4, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input01 = model->addOperand(&type3);
  auto param2 = model->addOperand(&type1);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type7);
  // Phase 2, operations
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_2(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type6(Type::FLOAT16, {});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT16, {1, 1, 1, 4, 2});
  // Phase 1, operands
  auto input01 = model->addOperand(&type9);
  auto param2 = model->addOperand(&type6);
  auto param3 = model->addOperand(&type2);
  auto output01 = model->addOperand(&type8);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static int32_t param3_init[] = {-1};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input01, param2, param3}, {output01});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input01},
    {output01});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_3(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 4, 1});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type4);
  // Phase 2, operations
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_3(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 4, 1});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type4);
  // Phase 2, operations
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_3(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 1, 2, 4, 1});
  OperandType type2(Type::INT32, {});
  OperandType type6(Type::FLOAT16, {});
  // Phase 1, operands
  auto input02 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_3(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 4, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type7);
  // Phase 2, operations
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_3(Model *model) {
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 4, 1});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input02 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type7);
  // Phase 2, operations
  static float param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(float) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_3(Model *model) {
  OperandType type10(Type::TENSOR_FLOAT16, {1, 1, 2, 4, 1});
  OperandType type2(Type::INT32, {});
  OperandType type6(Type::FLOAT16, {});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input02 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type6);
  auto param5 = model->addOperand(&type2);
  auto output02 = model->addOperand(&type8);
  // Phase 2, operations
  static _Float16 param4_init[] = {1.0f};
  model->setOperandValue(param4, param4_init, sizeof(_Float16) * 1);
  static int32_t param5_init[] = {-3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input02, param4, param5}, {output02});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input02},
    {output02});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type0);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_relaxed_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type0);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_float16_4(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 1, 1, 2, 4});
  OperandType type6(Type::FLOAT16, {});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type5);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type7);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_relaxed_4(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {1, 1, 1, 2, 4});
  OperandType type1(Type::FLOAT32, {});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input03 = model->addOperand(&type0);
  auto param6 = model->addOperand(&type1);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type7);
  // Phase 2, operations
  static float param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_float16_4(Model *model) {
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT16, {1, 1, 1, 2, 4});
  OperandType type6(Type::FLOAT16, {});
  OperandType type8(Type::TENSOR_FLOAT16, {0, 0, 0, 0, 0});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type6);
  auto param7 = model->addOperand(&type2);
  auto output03 = model->addOperand(&type8);
  // Phase 2, operations
  static _Float16 param6_init[] = {10.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static int32_t param7_init[] = {4};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOG_SOFTMAX, {input03, param6, param7}, {output03});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {input03},
    {output03});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

