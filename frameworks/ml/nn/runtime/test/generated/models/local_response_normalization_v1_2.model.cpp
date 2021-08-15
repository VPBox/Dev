// clang-format off
// Generated file (from: local_response_normalization_v1_2.mod.py). Do not edit
void CreateModel_axis_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis3_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim4_axis3_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim3_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim2_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim2_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim2_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dim1_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis3_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis3_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_relaxed_dim1_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis3_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static float param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(float) * 1);
  static float param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type1);
  auto param1 = model->addOperand(&type5);
  auto param2 = model->addOperand(&type5);
  auto param3 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param_init[] = {20};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static _Float16 param1_init[] = {0.0f};
  model->setOperandValue(param1, param1_init, sizeof(_Float16) * 1);
  static _Float16 param2_init[] = {1.0f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {0.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param, param1, param2, param3, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis3_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim4_axis3_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim3_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim2_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim2_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim2_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim2_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim1_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dim1_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis3_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim4_axis3_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim3_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim2_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim1_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_relaxed_dim1_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis3_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim1_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_2(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static float param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(float) * 1);
  static float param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
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

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16_2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param4 = model->addOperand(&type1);
  auto param5 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type5);
  auto param7 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param4_init[] = {20};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static _Float16 param5_init[] = {9.0f};
  model->setOperandValue(param5, param5_init, sizeof(_Float16) * 1);
  static _Float16 param6_init[] = {4.0f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {0.5f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param4, param5, param6, param7, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim4_axis3_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim4_axis3_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim3_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim3_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim2_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim2_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dim1_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dim1_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type0);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim4_axis3_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim4_axis3_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim3_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim3_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim2_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim2_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_relaxed_dim1_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_relaxed_dim1_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim4_axis3_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim4_axis3_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim3_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim3_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim2_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim2_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_dim1_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_dim1_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {6, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type3);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::TENSOR_FLOAT16, {6, 2, 2, 2});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-4};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type6(Type::TENSOR_FLOAT32, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type7(Type::TENSOR_FLOAT16, {2, 6, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  OperandType type9(Type::TENSOR_FLOAT16, {2, 2, 6, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type9);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_3(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {2, 2, 2, 6});
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type23(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type0);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type10(Type::TENSOR_FLOAT16, {2, 2, 2, 6});
  OperandType type24(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim4_axis3_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type11(Type::TENSOR_FLOAT32, {6, 2, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type12(Type::TENSOR_FLOAT16, {6, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-3};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type13(Type::TENSOR_FLOAT32, {2, 6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type14(Type::TENSOR_FLOAT16, {2, 6, 2});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type14);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim3_axis2_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type17(Type::TENSOR_FLOAT32, {6, 2});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type18(Type::TENSOR_FLOAT16, {6, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-2};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim2_axis1_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {0};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static float param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(float) * 1);
  static float param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16_3(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param8 = model->addOperand(&type1);
  auto param9 = model->addOperand(&type5);
  auto param10 = model->addOperand(&type5);
  auto param11 = model->addOperand(&type5);
  auto axis = model->addOperand(&type1);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param8_init[] = {2};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static _Float16 param9_init[] = {9.0f};
  model->setOperandValue(param9, param9_init, sizeof(_Float16) * 1);
  static _Float16 param10_init[] = {4.0f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {0.5f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  static int32_t axis_init[] = {-1};
  model->setOperandValue(axis, axis_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param8, param9, param10, param11, axis}, {op2});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op2});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_axis_dynamic_output_shape_relaxed_dim1_axis0_neg_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type2(Type::FLOAT32, {});
  OperandType type21(Type::TENSOR_FLOAT32, {6});
  OperandType type29(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type21);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim1_axis0_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {6});
  OperandType type30(Type::TENSOR_FLOAT16, {0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type19(Type::TENSOR_FLOAT32, {2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type27(Type::TENSOR_FLOAT32, {0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim2_axis1_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT16, {2, 6});
  OperandType type28(Type::TENSOR_FLOAT16, {0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type15(Type::TENSOR_FLOAT32, {2, 2, 6});
  OperandType type2(Type::FLOAT32, {});
  OperandType type25(Type::TENSOR_FLOAT32, {0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto param15 = model->addOperand(&type2);
  auto op2 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static float param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(float) * 1);
  static float param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

void CreateModel_dynamic_output_shape_relaxed_dim3_axis2_float16(Model *model) {
  OperandType type1(Type::INT32, {});
  OperandType type16(Type::TENSOR_FLOAT16, {2, 2, 6});
  OperandType type26(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type5(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param12 = model->addOperand(&type1);
  auto param13 = model->addOperand(&type5);
  auto param14 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type5);
  auto op2 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {9.0f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static _Float16 param14_init[] = {4.0f};
  model->setOperandValue(param14, param14_init, sizeof(_Float16) * 1);
  static _Float16 param15_init[] = {0.5f};
  model->setOperandValue(param15, param15_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION, {op1, param12, param13, param14, param15}, {op2});
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

