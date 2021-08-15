// clang-format off
// Generated file (from: conv2d_dilation.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type8);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type11);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type2);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type2);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type8);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type11);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type18(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type14);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type14);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type16);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type18(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type7);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static float op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 4);
  static float op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op2_init[] = {2, 2, 2, 2};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 4);
  static int32_t op3_init[] = {0};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op2_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 4);
  static _Float16 op3_init[] = {0.0f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 1);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type15);
  auto op2 = model->addOperand(&type8);
  auto op3 = model->addOperand(&type9);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type17);
  auto op2 = model->addOperand(&type11);
  auto op3 = model->addOperand(&type12);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
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
  static int32_t param6_init[] = {0};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, layout, param7, param8}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type1);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type1);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static uint8_t op21_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {0};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static uint8_t op21_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {0};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type28);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type28);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op21_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {0};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static float op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 9);
  static float op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op21_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 9);
  static int32_t op31_init[] = {0};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type28);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op21_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 9);
  static _Float16 op31_init[] = {0.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 1);
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto op21 = model->addOperand(&type1);
  auto op31 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type26);
  auto op21 = model->addOperand(&type23);
  auto op31 = model->addOperand(&type9);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type28);
  auto op21 = model->addOperand(&type10);
  auto op31 = model->addOperand(&type12);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param9_init[] = {0};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {0};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param16_init[] = {3};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static int32_t param17_init[] = {3};
  model->setOperandValue(param17, param17_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op11, op21, op31, param9, param10, param11, param12, param13, param14, param15, layout, param16, param17}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type2);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type2);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static uint8_t op22_init[] = {2, 2, 2, 2};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 4);
  static int32_t op32_init[] = {0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type10);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 4);
  static _Float16 op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type10);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type14);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type14);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type15);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op22_init[] = {2, 2, 2, 2};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 4);
  static int32_t op32_init[] = {0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type18(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type17);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 4);
  static _Float16 op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type14(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type15);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type18(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type17);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op22_init[] = {2, 2, 2, 2};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 4);
  static int32_t op32_init[] = {0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type10);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 4);
  static _Float16 op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type1);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.5f, 0);
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type10);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static float op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 4);
  static float op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type15);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op22_init[] = {2, 2, 2, 2};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 4);
  static int32_t op32_init[] = {0};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type17);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op22_init[] = {0.25f, 0.25f, 0.25f, 0.25f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 4);
  static _Float16 op32_init[] = {0.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 1);
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type13);
  auto op22 = model->addOperand(&type2);
  auto op32 = model->addOperand(&type3);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.5f, 0);
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op12 = model->addOperand(&type15);
  auto op22 = model->addOperand(&type8);
  auto op32 = model->addOperand(&type9);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type17);
  auto op22 = model->addOperand(&type11);
  auto op32 = model->addOperand(&type12);
  auto param18 = model->addOperand(&type4);
  auto param19 = model->addOperand(&type4);
  auto param20 = model->addOperand(&type4);
  auto param21 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param18_init[] = {2};
  model->setOperandValue(param18, param18_init, sizeof(int32_t) * 1);
  static int32_t param19_init[] = {1};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {1};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static int32_t param21_init[] = {0};
  model->setOperandValue(param21, param21_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param22_init[] = {1};
  model->setOperandValue(param22, param22_init, sizeof(int32_t) * 1);
  static int32_t param23_init[] = {1};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op12, op22, op32, param18, param19, param20, param21, layout, param22, param23}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_3(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type1);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type1);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type22);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type23);
  // Phase 2, operations
  static uint8_t op23_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type24);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type22);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type24);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type13);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type13);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type26);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type27);
  // Phase 2, operations
  static uint8_t op23_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type26);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type22);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op23_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type24);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 9, 9, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type5);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type22(Type::TENSOR_QUANT8_ASYMM, {1, 9, 9, 1}, 0.5f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type22);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type24(Type::TENSOR_FLOAT16, {1, 9, 9, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type24);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static float op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(float) * 9);
  static float op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(float) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type26);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op23_init[] = {8, 16, 24, 32, 40, 48, 56, 64, 72};
  model->setOperandValue(op23, op23_init, sizeof(uint8_t) * 9);
  static int32_t op33_init[] = {0};
  model->setOperandValue(op33, op33_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op23_init[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  model->setOperandValue(op23, op23_init, sizeof(_Float16) * 9);
  static _Float16 op33_init[] = {0.0f};
  model->setOperandValue(op33, op33_init, sizeof(_Float16) * 1);
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 9, 9});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type25);
  auto op23 = model->addOperand(&type1);
  auto op33 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {1, 1, 9, 9}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op13 = model->addOperand(&type26);
  auto op23 = model->addOperand(&type23);
  auto op33 = model->addOperand(&type9);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type28(Type::TENSOR_FLOAT16, {1, 1, 9, 9});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op13 = model->addOperand(&type28);
  auto op23 = model->addOperand(&type10);
  auto op33 = model->addOperand(&type12);
  auto param24 = model->addOperand(&type4);
  auto param25 = model->addOperand(&type4);
  auto param26 = model->addOperand(&type4);
  auto param27 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type4);
  auto op43 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param24_init[] = {2};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static int32_t param25_init[] = {1};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {1};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param28_init[] = {3};
  model->setOperandValue(param28, param28_init, sizeof(int32_t) * 1);
  static int32_t param29_init[] = {3};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op13, op23, op33, param24, param25, param26, param27, layout, param28, param29}, {op43});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op13, op23, op33},
    {op43});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_4(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type1);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type1);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 6, 6, 1}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type29);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type23);
  // Phase 2, operations
  static uint8_t op24_init[] = {8, 16, 24, 32};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 4);
  static int32_t op34_init[] = {0};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 6, 6, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type30);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type10);
  // Phase 2, operations
  static _Float16 op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 4);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type1);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.125f, 0);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 6, 6, 1}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type29);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_weight_as_input_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 6, 6, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type30);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_weight_as_input_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type13);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type13);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 1, 6, 6}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type32);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type27);
  // Phase 2, operations
  static uint8_t op24_init[] = {8, 16, 24, 32};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 4);
  static int32_t op34_init[] = {0};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type33);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 4);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 1, 6, 6}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type32);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_weight_as_input_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type33);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_nchw_weight_as_input_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 6, 6, 1}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type29);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op24_init[] = {8, 16, 24, 32};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 4);
  static int32_t op34_init[] = {0};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 6, 6, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type30);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 4);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 6, 6, 1});
  // Phase 1, operands
  auto op14 = model->addOperand(&type6);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 6, 6, 1}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type29);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_weight_as_input_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 6, 6, 1});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type30);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_weight_as_input_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static float op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(float) * 4);
  static float op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(float) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 1, 6, 6}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type32);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type20);
  // Phase 2, operations
  static uint8_t op24_init[] = {8, 16, 24, 32};
  model->setOperandValue(op24, op24_init, sizeof(uint8_t) * 4);
  static int32_t op34_init[] = {0};
  model->setOperandValue(op34, op34_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type33);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type21);
  // Phase 2, operations
  static _Float16 op24_init[] = {1.0f, 2.0f, 3.0f, 4.0f};
  model->setOperandValue(op24, op24_init, sizeof(_Float16) * 4);
  static _Float16 op34_init[] = {0.0f};
  model->setOperandValue(op34, op34_init, sizeof(_Float16) * 1);
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_relaxed_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {1});
  OperandType type31(Type::TENSOR_FLOAT32, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type31);
  auto op24 = model->addOperand(&type2);
  auto op34 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_relaxed_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_quant8_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.125f, 0);
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 1, 6, 6}, 0.5f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.125f, 0);
  OperandType type9(Type::TENSOR_INT32, {1}, 0.0625f, 0);
  // Phase 1, operands
  auto op14 = model->addOperand(&type32);
  auto op24 = model->addOperand(&type8);
  auto op34 = model->addOperand(&type9);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_quant8_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_weight_as_input_float16_5(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type12(Type::TENSOR_FLOAT16, {1});
  OperandType type21(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type33(Type::TENSOR_FLOAT16, {1, 1, 6, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op14 = model->addOperand(&type33);
  auto op24 = model->addOperand(&type11);
  auto op34 = model->addOperand(&type12);
  auto param30 = model->addOperand(&type4);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto param34 = model->addOperand(&type4);
  auto param35 = model->addOperand(&type4);
  auto op44 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param30_init[] = {1};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static int32_t param31_init[] = {2};
  model->setOperandValue(param31, param31_init, sizeof(int32_t) * 1);
  static int32_t param32_init[] = {2};
  model->setOperandValue(param32, param32_init, sizeof(int32_t) * 1);
  static int32_t param33_init[] = {0};
  model->setOperandValue(param33, param33_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param34_init[] = {3};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {3};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_CONV_2D, {op14, op24, op34, param30, param31, param32, param33, layout, param34, param35}, {op44});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op14, op24, op34},
    {op44});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_weight_as_input_float16_5(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

