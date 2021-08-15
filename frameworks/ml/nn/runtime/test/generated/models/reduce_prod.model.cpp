// clang-format off
// Generated file (from: reduce_prod.mod.py). Do not edit
void CreateModel(Model *model) {
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2});
  OperandType type1(Type::TENSOR_FLOAT32, {3});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type10(Type::TENSOR_FLOAT16, {3, 2});
  OperandType type11(Type::TENSOR_FLOAT16, {3});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type10);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type0(Type::TENSOR_FLOAT32, {3, 2});
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type0);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type10(Type::TENSOR_FLOAT16, {3, 2});
  OperandType type13(Type::TENSOR_FLOAT16, {0});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input0 = model->addOperand(&type10);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type3);
  auto output0 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {-1};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static bool8 param1_init[] = {false};
  model->setOperandValue(param1, param1_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input0, param, param1}, {output0});
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
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto input01 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto input01 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type14(Type::TENSOR_FLOAT16, {1});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input01 = model->addOperand(&type14);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto input01 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  OperandType type4(Type::TENSOR_FLOAT32, {1});
  // Phase 1, operands
  auto input01 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type13(Type::TENSOR_FLOAT16, {0});
  OperandType type14(Type::TENSOR_FLOAT16, {1});
  OperandType type2(Type::TENSOR_INT32, {1});
  OperandType type3(Type::BOOL, {});
  // Phase 1, operands
  auto input01 = model->addOperand(&type14);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type3);
  auto output01 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static bool8 param3_init[] = {true};
  model->setOperandValue(param3, param3_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input01, param2, param3}, {output01});
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
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {2});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {2});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type15(Type::TENSOR_FLOAT16, {4, 3, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {2});
  OperandType type3(Type::BOOL, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type12(Type::TENSOR_FLOAT32, {0});
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type13(Type::TENSOR_FLOAT16, {0});
  OperandType type15(Type::TENSOR_FLOAT16, {4, 3, 2});
  OperandType type3(Type::BOOL, {});
  OperandType type7(Type::TENSOR_INT32, {4});
  // Phase 1, operands
  auto input02 = model->addOperand(&type15);
  auto param4 = model->addOperand(&type7);
  auto param5 = model->addOperand(&type3);
  auto output02 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param4_init[] = {1, 0, -3, -3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 4);
  static bool8 param5_init[] = {false};
  model->setOperandValue(param5, param5_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input02, param4, param5}, {output02});
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
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 3, 1});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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
  OperandType type15(Type::TENSOR_FLOAT16, {4, 3, 2});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 3, 1});
  OperandType type3(Type::BOOL, {});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type15);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0});
  OperandType type3(Type::BOOL, {});
  OperandType type5(Type::TENSOR_FLOAT32, {4, 3, 2});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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
  OperandType type15(Type::TENSOR_FLOAT16, {4, 3, 2});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0});
  OperandType type3(Type::BOOL, {});
  OperandType type9(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto input03 = model->addOperand(&type15);
  auto param6 = model->addOperand(&type9);
  auto param7 = model->addOperand(&type3);
  auto output03 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param6_init[] = {0, 2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 2);
  static bool8 param7_init[] = {true};
  model->setOperandValue(param7, param7_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_REDUCE_PROD, {input03, param6, param7}, {output03});
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

