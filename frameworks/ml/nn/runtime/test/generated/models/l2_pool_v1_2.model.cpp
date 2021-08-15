// clang-format off
// Generated file (from: l2_pool_v1_2.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type19);
  auto param = model->addOperand(&type2);
  auto param1 = model->addOperand(&type2);
  auto param2 = model->addOperand(&type2);
  auto param3 = model->addOperand(&type2);
  auto param4 = model->addOperand(&type2);
  auto param5 = model->addOperand(&type2);
  auto param6 = model->addOperand(&type2);
  auto param7 = model->addOperand(&type2);
  auto param8 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {0};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {0};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static int32_t param2_init[] = {0};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 1);
  static int32_t param3_init[] = {0};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 1);
  static int32_t param4_init[] = {1};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {1};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static int32_t param6_init[] = {1};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {0};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op1, param, param1, param2, param3, param4, param5, param6, param7, param8, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2, 4, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 2, 4, 1});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2, 4, 1});
  OperandType type4(Type::TENSOR_FLOAT32, {1, 1, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type4);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 2, 4, 1});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 2, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 1, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 2, 4});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 1, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 1, 2, 4});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 1, 1, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 2, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1, 2, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 2, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 2, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 1, 2, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT32, {1, 1, 2, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 1, 2, 4});
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto param9 = model->addOperand(&type2);
  auto param10 = model->addOperand(&type2);
  auto param11 = model->addOperand(&type2);
  auto param12 = model->addOperand(&type2);
  auto param13 = model->addOperand(&type2);
  auto param14 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {2};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {2};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {2};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {0};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op11, param9, param10, param11, param12, param13, param14, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type28);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type28);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type29);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type30);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type30);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 3, 1, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type28);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type5);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 2, 2, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type28);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type30(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type30);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type30(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type30);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param15 = model->addOperand(&type2);
  auto param16 = model->addOperand(&type2);
  auto param17 = model->addOperand(&type2);
  auto param18 = model->addOperand(&type2);
  auto param19 = model->addOperand(&type2);
  auto param20 = model->addOperand(&type2);
  auto param21 = model->addOperand(&type2);
  auto param22 = model->addOperand(&type2);
  auto param23 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {0};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {0};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {2};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static int32_t param22_init[] = {2};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {0};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {op12, param15, param16, param17, param18, param19, param20, param21, param22, param23, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type16(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type15);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type16);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type16(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type15);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type16);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type34(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type36(Type::TENSOR_FLOAT16, {0, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type41(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type40);
  auto roi = model->addOperand(&type38);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type37);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type37);
  auto param29 = model->addOperand(&type37);
  auto param30 = model->addOperand(&type37);
  auto scoresOut = model->addOperand(&type41);
  auto roiOut = model->addOperand(&type39);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type35);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type37);
  auto param34 = model->addOperand(&type37);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type34);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type36);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static _Float16 param25_init[] = {0.30000001192092896f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.4000000059604645f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static _Float16 param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(_Float16) * 1);
  static _Float16 param30_init[] = {0.30000001192092896f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type16(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type42);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type16);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type16(Type::TENSOR_FLOAT32, {0, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type42);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type16);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type36(Type::TENSOR_FLOAT16, {0, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type41(Type::TENSOR_FLOAT16, {0});
  OperandType type43(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores = model->addOperand(&type40);
  auto roi = model->addOperand(&type38);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type37);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type37);
  auto param29 = model->addOperand(&type37);
  auto param30 = model->addOperand(&type37);
  auto scoresOut = model->addOperand(&type41);
  auto roiOut = model->addOperand(&type39);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type35);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type37);
  auto param34 = model->addOperand(&type37);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type43);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type36);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static _Float16 param25_init[] = {0.30000001192092896f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.4000000059604645f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static _Float16 param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(_Float16) * 1);
  static _Float16 param30_init[] = {0.30000001192092896f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type15);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type15);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type44(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type40);
  auto roi = model->addOperand(&type38);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type37);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type37);
  auto param29 = model->addOperand(&type37);
  auto param30 = model->addOperand(&type37);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type39);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type35);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type37);
  auto param34 = model->addOperand(&type37);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type34);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static _Float16 param25_init[] = {0.30000001192092896f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.4000000059604645f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static _Float16 param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(_Float16) * 1);
  static _Float16 param30_init[] = {0.30000001192092896f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type42);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type13);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type13);
  auto param29 = model->addOperand(&type13);
  auto param30 = model->addOperand(&type13);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type14);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type13);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type42);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type20);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static float param25_init[] = {0.3f};
  model->setOperandValue(param25, param25_init, sizeof(float) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.4f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static float param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(float) * 1);
  static float param30_init[] = {0.3f};
  model->setOperandValue(param30, param30_init, sizeof(float) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static float param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static float param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(float) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type43(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type44(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type40);
  auto roi = model->addOperand(&type38);
  auto param24 = model->addOperand(&type12);
  auto param25 = model->addOperand(&type37);
  auto param26 = model->addOperand(&type2);
  auto param27 = model->addOperand(&type2);
  auto param28 = model->addOperand(&type37);
  auto param29 = model->addOperand(&type37);
  auto param30 = model->addOperand(&type37);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type39);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type35);
  auto param31 = model->addOperand(&type2);
  auto param32 = model->addOperand(&type2);
  auto param33 = model->addOperand(&type37);
  auto param34 = model->addOperand(&type37);
  auto param35 = model->addOperand(&type2);
  auto param36 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type43);
  auto param37 = model->addOperand(&type2);
  auto param38 = model->addOperand(&type2);
  auto param39 = model->addOperand(&type2);
  auto param40 = model->addOperand(&type2);
  auto param41 = model->addOperand(&type2);
  auto param42 = model->addOperand(&type2);
  auto param43 = model->addOperand(&type2);
  auto param44 = model->addOperand(&type2);
  auto param45 = model->addOperand(&type2);
  auto out = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param24_init[] = {0};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static _Float16 param25_init[] = {0.30000001192092896f};
  model->setOperandValue(param25, param25_init, sizeof(_Float16) * 1);
  static int32_t param26_init[] = {-1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.4000000059604645f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static _Float16 param29_init[] = {1.0f};
  model->setOperandValue(param29, param29_init, sizeof(_Float16) * 1);
  static _Float16 param30_init[] = {0.30000001192092896f};
  model->setOperandValue(param30, param30_init, sizeof(_Float16) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static _Float16 param33_init[] = {2.0f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static _Float16 param34_init[] = {2.0f};
  model->setOperandValue(param34, param34_init, sizeof(_Float16) * 1);
  static int32_t param35_init[] = {4};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static int32_t param36_init[] = {4};
  model->setOperandValue(param36, param36_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param37_init[] = {0};
  model->setOperandValue(param37, param37_init, sizeof(int32_t) * 1);
  static int32_t param38_init[] = {0};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {0};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static int32_t param40_init[] = {0};
  model->setOperandValue(param40, param40_init, sizeof(int32_t) * 1);
  static int32_t param41_init[] = {1};
  model->setOperandValue(param41, param41_init, sizeof(int32_t) * 1);
  static int32_t param42_init[] = {1};
  model->setOperandValue(param42, param42_init, sizeof(int32_t) * 1);
  static int32_t param43_init[] = {2};
  model->setOperandValue(param43, param43_init, sizeof(int32_t) * 1);
  static int32_t param44_init[] = {2};
  model->setOperandValue(param44, param44_init, sizeof(int32_t) * 1);
  static int32_t param45_init[] = {0};
  model->setOperandValue(param45, param45_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param24, param25, param26, param27, param28, param29, param30}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param31, param32, param33, param34, param35, param36, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap, param37, param38, param39, param40, param41, param42, param43, param44, param45, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type15);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type15);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type15);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type15);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type34(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type41(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type40);
  auto roi1 = model->addOperand(&type38);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type37);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type37);
  auto param51 = model->addOperand(&type37);
  auto param52 = model->addOperand(&type37);
  auto scoresOut1 = model->addOperand(&type41);
  auto roiOut1 = model->addOperand(&type39);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type35);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type37);
  auto param56 = model->addOperand(&type37);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type34);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type34);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static _Float16 param47_init[] = {0.30000001192092896f};
  model->setOperandValue(param47, param47_init, sizeof(_Float16) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static _Float16 param50_init[] = {0.4000000059604645f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {0.30000001192092896f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type42);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type42);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type42);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type42);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type41(Type::TENSOR_FLOAT16, {0});
  OperandType type43(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type40);
  auto roi1 = model->addOperand(&type38);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type37);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type37);
  auto param51 = model->addOperand(&type37);
  auto param52 = model->addOperand(&type37);
  auto scoresOut1 = model->addOperand(&type41);
  auto roiOut1 = model->addOperand(&type39);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type35);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type37);
  auto param56 = model->addOperand(&type37);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type43);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type43);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static _Float16 param47_init[] = {0.30000001192092896f};
  model->setOperandValue(param47, param47_init, sizeof(_Float16) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static _Float16 param50_init[] = {0.4000000059604645f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {0.30000001192092896f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type15);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type20);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type15);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type20);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type34(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type44(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type40);
  auto roi1 = model->addOperand(&type38);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type37);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type37);
  auto param51 = model->addOperand(&type37);
  auto param52 = model->addOperand(&type37);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type39);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type35);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type37);
  auto param56 = model->addOperand(&type37);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type34);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static _Float16 param47_init[] = {0.30000001192092896f};
  model->setOperandValue(param47, param47_init, sizeof(_Float16) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static _Float16 param50_init[] = {0.4000000059604645f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {0.30000001192092896f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type42);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type20);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::FLOAT32, {});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type2(Type::INT32, {});
  OperandType type20(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type42(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type13);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type13);
  auto param51 = model->addOperand(&type13);
  auto param52 = model->addOperand(&type13);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type14);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type13);
  auto param56 = model->addOperand(&type13);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type42);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type20);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static float param47_init[] = {0.3f};
  model->setOperandValue(param47, param47_init, sizeof(float) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static float param50_init[] = {0.4f};
  model->setOperandValue(param50, param50_init, sizeof(float) * 1);
  static float param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(float) * 1);
  static float param52_init[] = {0.3f};
  model->setOperandValue(param52, param52_init, sizeof(float) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static float param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(float) * 1);
  static float param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(float) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type2(Type::INT32, {});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type37(Type::FLOAT16, {});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type39(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type40(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type43(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type44(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type40);
  auto roi1 = model->addOperand(&type38);
  auto param46 = model->addOperand(&type12);
  auto param47 = model->addOperand(&type37);
  auto param48 = model->addOperand(&type2);
  auto param49 = model->addOperand(&type2);
  auto param50 = model->addOperand(&type37);
  auto param51 = model->addOperand(&type37);
  auto param52 = model->addOperand(&type37);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type39);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type35);
  auto param53 = model->addOperand(&type2);
  auto param54 = model->addOperand(&type2);
  auto param55 = model->addOperand(&type37);
  auto param56 = model->addOperand(&type37);
  auto param57 = model->addOperand(&type2);
  auto param58 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type43);
  auto param59 = model->addOperand(&type2);
  auto param60 = model->addOperand(&type2);
  auto param61 = model->addOperand(&type2);
  auto param62 = model->addOperand(&type2);
  auto param63 = model->addOperand(&type2);
  auto param64 = model->addOperand(&type2);
  auto out1 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param46_init[] = {0};
  model->setOperandValue(param46, param46_init, sizeof(int32_t) * 1);
  static _Float16 param47_init[] = {0.30000001192092896f};
  model->setOperandValue(param47, param47_init, sizeof(_Float16) * 1);
  static int32_t param48_init[] = {-1};
  model->setOperandValue(param48, param48_init, sizeof(int32_t) * 1);
  static int32_t param49_init[] = {0};
  model->setOperandValue(param49, param49_init, sizeof(int32_t) * 1);
  static _Float16 param50_init[] = {0.4000000059604645f};
  model->setOperandValue(param50, param50_init, sizeof(_Float16) * 1);
  static _Float16 param51_init[] = {1.0f};
  model->setOperandValue(param51, param51_init, sizeof(_Float16) * 1);
  static _Float16 param52_init[] = {0.30000001192092896f};
  model->setOperandValue(param52, param52_init, sizeof(_Float16) * 1);
  static int32_t param53_init[] = {2};
  model->setOperandValue(param53, param53_init, sizeof(int32_t) * 1);
  static int32_t param54_init[] = {2};
  model->setOperandValue(param54, param54_init, sizeof(int32_t) * 1);
  static _Float16 param55_init[] = {2.0f};
  model->setOperandValue(param55, param55_init, sizeof(_Float16) * 1);
  static _Float16 param56_init[] = {2.0f};
  model->setOperandValue(param56, param56_init, sizeof(_Float16) * 1);
  static int32_t param57_init[] = {4};
  model->setOperandValue(param57, param57_init, sizeof(int32_t) * 1);
  static int32_t param58_init[] = {4};
  model->setOperandValue(param58, param58_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param59_init[] = {1};
  model->setOperandValue(param59, param59_init, sizeof(int32_t) * 1);
  static int32_t param60_init[] = {1};
  model->setOperandValue(param60, param60_init, sizeof(int32_t) * 1);
  static int32_t param61_init[] = {1};
  model->setOperandValue(param61, param61_init, sizeof(int32_t) * 1);
  static int32_t param62_init[] = {2};
  model->setOperandValue(param62, param62_init, sizeof(int32_t) * 1);
  static int32_t param63_init[] = {2};
  model->setOperandValue(param63, param63_init, sizeof(int32_t) * 1);
  static int32_t param64_init[] = {0};
  model->setOperandValue(param64, param64_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param46, param47, param48, param49, param50, param51, param52}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param53, param54, param55, param56, param57, param58, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_L2_POOL_2D, {featureMap1, param59, param60, param61, param62, param63, param64, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

