// clang-format off
// Generated file (from: grouped_conv2d.mod.py). Do not edit
void CreateModel_nhwc_none(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type19(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type20(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type19);
  auto op3 = model->addOperand(&type20);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type24(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type25(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type24);
  auto op3 = model->addOperand(&type25);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_none_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_none_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type32(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type33(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type32);
  auto op3 = model->addOperand(&type33);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type34(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type35(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type34);
  auto op3 = model->addOperand(&type35);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type36(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type37(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type36);
  auto op3 = model->addOperand(&type37);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type38(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type39(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type38);
  auto op3 = model->addOperand(&type39);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu1_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu1_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type40(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type41(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type40);
  auto op3 = model->addOperand(&type41);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type42(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type43(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type42);
  auto op3 = model->addOperand(&type43);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nhwc_relu6_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nhwc_relu6_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type46(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type47(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type46);
  auto op3 = model->addOperand(&type47);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type48(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type49(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type48);
  auto op3 = model->addOperand(&type49);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_none_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_none_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type51(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type52(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type51);
  auto op3 = model->addOperand(&type52);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type53(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type54(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type53);
  auto op3 = model->addOperand(&type54);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type55(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type56(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type55);
  auto op3 = model->addOperand(&type56);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type57(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type58(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type57);
  auto op3 = model->addOperand(&type58);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu1_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu1_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type5);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type16(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.05f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.5f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type59(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type60(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type59);
  auto op3 = model->addOperand(&type60);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 80);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type61(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type62(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type61);
  auto op3 = model->addOperand(&type62);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_nchw_relu6_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type29);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_nchw_relu6_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type66(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type67(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type66);
  auto op3 = model->addOperand(&type67);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type69(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type70(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type69);
  auto op3 = model->addOperand(&type70);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_none_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_none_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type72(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type73(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type72);
  auto op3 = model->addOperand(&type73);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type74(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type75(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type74);
  auto op3 = model->addOperand(&type75);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type76(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type77(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type76);
  auto op3 = model->addOperand(&type77);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type78(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type79(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type78);
  auto op3 = model->addOperand(&type79);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu1_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu1_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
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
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type80(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type81(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type80);
  auto op3 = model->addOperand(&type81);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 100);
  OperandType type4(Type::INT32, {});
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type82(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type83(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto op2 = model->addOperand(&type82);
  auto op3 = model->addOperand(&type83);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nhwc_relu6_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type26);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nhwc_relu6_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type84(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type85(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type84);
  auto op3 = model->addOperand(&type85);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type86(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type87(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type86);
  auto op3 = model->addOperand(&type87);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_none_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {0};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_none_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type88(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type89(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type88);
  auto op3 = model->addOperand(&type89);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type90(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type91(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type90);
  auto op3 = model->addOperand(&type91);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {1};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type92(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type93(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type92);
  auto op3 = model->addOperand(&type93);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type94(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type95(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type94);
  auto op3 = model->addOperand(&type95);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu1_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {2};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu1_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
  // Phase 2, operations
  static float op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(float) * 8);
  static float op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(float) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_FLOAT32, {2, 2, 2, 1});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type44(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type44);
  auto op2 = model->addOperand(&type2);
  auto op3 = model->addOperand(&type3);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type63);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
  // Phase 2, operations
  static uint8_t op2_init[] = {132, 136, 136, 132, 144, 140, 136, 132};
  model->setOperandValue(op2, op2_init, sizeof(uint8_t) * 8);
  static int32_t op3_init[] = {160, -536};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {2, 2, 2, 1}, 0.25f, 128);
  OperandType type14(Type::TENSOR_INT32, {2}, 0.0625f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type65(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.05f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type13);
  auto op3 = model->addOperand(&type14);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type65);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_quant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type17(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type18(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type17);
  auto op3 = model->addOperand(&type18);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type64(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 80);
  OperandType type96(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type97(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type96);
  auto op3 = model->addOperand(&type97);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type64);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type21(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type22(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type21);
  auto op3 = model->addOperand(&type22);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
  // Phase 2, operations
  static int8_t op2_init[] = {4, 8, 8, 4, 8, 6, 4, 2};
  model->setOperandValue(op2, op2_init, sizeof(int8_t) * 8);
  static int32_t op3_init[] = {160, -268};
  model->setOperandValue(op3, op3_init, sizeof(int32_t) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::INT32, {});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 100);
  OperandType type68(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 80);
  OperandType type98(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 2, 1}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.5f},0));
  OperandType type99(Type::TENSOR_INT32, {2}, 0.0f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type45);
  auto op2 = model->addOperand(&type98);
  auto op3 = model->addOperand(&type99);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type68);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_channelQuant8_weight_as_input_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type27);
  auto op3 = model->addOperand(&type28);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op2_init[] = {1.0f, 2.0f, 2.0f, 1.0f, 4.0f, 3.0f, 2.0f, 1.0f};
  model->setOperandValue(op2, op2_init, sizeof(_Float16) * 8);
  static _Float16 op3_init[] = {10.0f, -33.5f};
  model->setOperandValue(op3, op3_init, sizeof(_Float16) * 2);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_dynamic_output_shape_nchw_relu6_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type30(Type::TENSOR_FLOAT16, {2, 2, 2, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op1 = model->addOperand(&type50);
  auto op2 = model->addOperand(&type30);
  auto op3 = model->addOperand(&type31);
  auto param = model->addOperand(&type4);
  auto param1 = model->addOperand(&type4);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto param4 = model->addOperand(&type4);
  auto param5 = model->addOperand(&type4);
  auto param6 = model->addOperand(&type4);
  auto act = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type71);
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
  static int32_t param6_init[] = {2};
  model->setOperandValue(param6, param6_init, sizeof(int32_t) * 1);
  static int32_t act_init[] = {3};
  model->setOperandValue(act, act_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op1, op2, op3, param, param1, param2, param3, param4, param5, param6, act, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1, op2, op3},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_dynamic_output_shape_nchw_relu6_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type103);
  // Phase 2, operations
  static uint8_t op21_init[] = {100, 20, 1, 200, 10, 2, 200, 30, 1, 100, 20, 3};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 12);
  static int32_t op31_init[] = {2000, -4000};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type103);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 10.0f, 100);
  OperandType type104(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type105(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type104);
  auto op31 = model->addOperand(&type105);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type103);
  // Phase 2, operations
  static int8_t op21_init[] = {50, 10, 0, 100, 5, 1, 80, 12, 0, 40, 8, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 12);
  static int32_t op31_init[] = {1000, -1600};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type103(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 10.0f, 100);
  OperandType type106(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type107(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type106);
  auto op31 = model->addOperand(&type107);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type103);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type108(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type109(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type108);
  auto op21 = model->addOperand(&type109);
  auto op31 = model->addOperand(&type28);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type108);
  // Phase 2, operations
  static _Float16 op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 12);
  static _Float16 op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nhwc_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type108(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type110(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type108);
  auto op21 = model->addOperand(&type110);
  auto op31 = model->addOperand(&type31);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type108);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nhwc_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type111);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type111);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type111);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type111);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type113(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type113);
  // Phase 2, operations
  static uint8_t op21_init[] = {100, 20, 1, 200, 10, 2, 200, 30, 1, 100, 20, 3};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 12);
  static int32_t op31_init[] = {2000, -4000};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type113(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type113);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type104(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type105(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type113(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type104);
  auto op31 = model->addOperand(&type105);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type113);
  // Phase 2, operations
  static int8_t op21_init[] = {50, 10, 0, 100, 5, 1, 80, 12, 0, 40, 8, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 12);
  static int32_t op31_init[] = {1000, -1600};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type113(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 10.0f, 100);
  OperandType type114(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type115(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type114);
  auto op31 = model->addOperand(&type115);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type113);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type109(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type116(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type116);
  auto op21 = model->addOperand(&type109);
  auto op31 = model->addOperand(&type28);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type116);
  // Phase 2, operations
  static _Float16 op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 12);
  static _Float16 op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_nchw_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type116(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type116);
  auto op21 = model->addOperand(&type110);
  auto op31 = model->addOperand(&type31);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type116);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_nchw_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 2, 2});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type6);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static uint8_t op21_init[] = {100, 20, 1, 200, 10, 2, 200, 30, 1, 100, 20, 3};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 12);
  static int32_t op31_init[] = {2000, -4000};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type104(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type105(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type104);
  auto op31 = model->addOperand(&type105);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int8_t op21_init[] = {50, 10, 0, 100, 5, 1, 80, 12, 0, 40, 8, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 12);
  static int32_t op31_init[] = {1000, -1600};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type100(Type::TENSOR_QUANT8_ASYMM, {1, 3, 2, 2}, 0.25f, 128);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type118(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type119(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type100);
  auto op21 = model->addOperand(&type118);
  auto op31 = model->addOperand(&type119);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type108(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type109(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op11 = model->addOperand(&type108);
  auto op21 = model->addOperand(&type109);
  auto op31 = model->addOperand(&type28);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 12);
  static _Float16 op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nhwc_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type108(Type::TENSOR_FLOAT16, {1, 3, 2, 2});
  OperandType type110(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op11 = model->addOperand(&type108);
  auto op21 = model->addOperand(&type110);
  auto op31 = model->addOperand(&type31);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type71);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nhwc_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static float op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(float) * 12);
  static float op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(float) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type111(Type::TENSOR_FLOAT32, {1, 2, 3, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type7(Type::TENSOR_FLOAT32, {2, 2, 3, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type111);
  auto op21 = model->addOperand(&type7);
  auto op31 = model->addOperand(&type3);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static uint8_t op21_init[] = {100, 20, 1, 200, 10, 2, 200, 30, 1, 100, 20, 3};
  model->setOperandValue(op21, op21_init, sizeof(uint8_t) * 12);
  static int32_t op31_init[] = {2000, -4000};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type101(Type::TENSOR_QUANT8_ASYMM, {2, 2, 3, 1}, 1.0f, 0);
  OperandType type102(Type::TENSOR_INT32, {2}, 0.25f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type101);
  auto op31 = model->addOperand(&type102);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type104(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type105(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type104);
  auto op31 = model->addOperand(&type105);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int8_t op21_init[] = {50, 10, 0, 100, 5, 1, 80, 12, 0, 40, 8, 1};
  model->setOperandValue(op21, op21_init, sizeof(int8_t) * 12);
  static int32_t op31_init[] = {1000, -1600};
  model->setOperandValue(op31, op31_init, sizeof(int32_t) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type112(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 2}, 0.25f, 128);
  OperandType type117(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 10.0f, 100);
  OperandType type120(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {2, 2, 3, 1}, 0.0f, 0, SymmPerChannelQuantParams({2.0f, 2.5f},0));
  OperandType type121(Type::TENSOR_INT32, {2}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type112);
  auto op21 = model->addOperand(&type120);
  auto op31 = model->addOperand(&type121);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type117);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type109(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type116(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type28(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op11 = model->addOperand(&type116);
  auto op21 = model->addOperand(&type109);
  auto op31 = model->addOperand(&type28);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op21_init[] = {100.0f, 20.0f, 1.0f, 200.0f, 10.0f, 2.0f, 200.0f, 30.0f, 1.0f, 100.0f, 20.0f, 3.0f};
  model->setOperandValue(op21, op21_init, sizeof(_Float16) * 12);
  static _Float16 op31_init[] = {500.0f, -1000.0f};
  model->setOperandValue(op31, op31_init, sizeof(_Float16) * 2);
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_large_dynamic_output_shape_nchw_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type110(Type::TENSOR_FLOAT16, {2, 2, 3, 1});
  OperandType type116(Type::TENSOR_FLOAT16, {1, 2, 3, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op11 = model->addOperand(&type116);
  auto op21 = model->addOperand(&type110);
  auto op31 = model->addOperand(&type31);
  auto param7 = model->addOperand(&type4);
  auto param8 = model->addOperand(&type4);
  auto param9 = model->addOperand(&type4);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type71);
  // Phase 2, operations
  static int32_t param7_init[] = {1};
  model->setOperandValue(param7, param7_init, sizeof(int32_t) * 1);
  static int32_t param8_init[] = {1};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {1};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  static int32_t param10_init[] = {2};
  model->setOperandValue(param10, param10_init, sizeof(int32_t) * 1);
  static int32_t param11_init[] = {0};
  model->setOperandValue(param11, param11_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op11, op21, op31, param7, param8, param9, param10, param11, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11, op21, op31},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_large_dynamic_output_shape_nchw_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2, 2, 6});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2, 2, 6});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2, 2, 6});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type11(Type::TENSOR_FLOAT32, {1, 2, 2, 6});
  OperandType type4(Type::INT32, {});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type11);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type125(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 6}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type125);
  // Phase 2, operations
  static uint8_t op22_init[] = {4, 8, 12, 8, 4, 0, 8, 12, 12, 24, 24, 24, 36, 32, 20, 8, 4, 4};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 18);
  static int32_t op32_init[] = {80, -160, 240, -320, 400, -480};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type125(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 6}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type125);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type125(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 6}, 2.0f, 60);
  OperandType type126(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type127(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type126);
  auto op32 = model->addOperand(&type127);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type125);
  // Phase 2, operations
  static int8_t op22_init[] = {4, 8, 12, 7, 3, 0, 8, 12, 12, 20, 20, 20, 36, 32, 20, 7, 3, 3};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 18);
  static int32_t op32_init[] = {80, -133, 240, -267, 400, -400};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type125(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 6}, 2.0f, 60);
  OperandType type128(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type129(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type128);
  auto op32 = model->addOperand(&type129);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type125);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type130(Type::TENSOR_FLOAT16, {1, 2, 2, 9});
  OperandType type131(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type132(Type::TENSOR_FLOAT16, {6});
  OperandType type133(Type::TENSOR_FLOAT16, {1, 2, 2, 6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type130);
  auto op22 = model->addOperand(&type131);
  auto op32 = model->addOperand(&type132);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type133);
  // Phase 2, operations
  static _Float16 op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 18);
  static _Float16 op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nhwc_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type130(Type::TENSOR_FLOAT16, {1, 2, 2, 9});
  OperandType type133(Type::TENSOR_FLOAT16, {1, 2, 2, 6});
  OperandType type134(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type135(Type::TENSOR_FLOAT16, {6});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type130);
  auto op22 = model->addOperand(&type134);
  auto op32 = model->addOperand(&type135);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type133);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nhwc_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type137(Type::TENSOR_FLOAT32, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type137);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type137(Type::TENSOR_FLOAT32, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type137);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type137(Type::TENSOR_FLOAT32, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type137);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type137(Type::TENSOR_FLOAT32, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type137);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type139(Type::TENSOR_QUANT8_ASYMM, {1, 6, 2, 2}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type139);
  // Phase 2, operations
  static uint8_t op22_init[] = {4, 8, 12, 8, 4, 0, 8, 12, 12, 24, 24, 24, 36, 32, 20, 8, 4, 4};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 18);
  static int32_t op32_init[] = {80, -160, 240, -320, 400, -480};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type139(Type::TENSOR_QUANT8_ASYMM, {1, 6, 2, 2}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type139);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type126(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type127(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type139(Type::TENSOR_QUANT8_ASYMM, {1, 6, 2, 2}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type126);
  auto op32 = model->addOperand(&type127);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type139);
  // Phase 2, operations
  static int8_t op22_init[] = {4, 8, 12, 7, 3, 0, 8, 12, 12, 20, 20, 20, 36, 32, 20, 7, 3, 3};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 18);
  static int32_t op32_init[] = {80, -133, 240, -267, 400, -400};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type139(Type::TENSOR_QUANT8_ASYMM, {1, 6, 2, 2}, 2.0f, 60);
  OperandType type140(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type141(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type140);
  auto op32 = model->addOperand(&type141);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type139);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type131(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type132(Type::TENSOR_FLOAT16, {6});
  OperandType type142(Type::TENSOR_FLOAT16, {1, 9, 2, 2});
  OperandType type143(Type::TENSOR_FLOAT16, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type142);
  auto op22 = model->addOperand(&type131);
  auto op32 = model->addOperand(&type132);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type143);
  // Phase 2, operations
  static _Float16 op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 18);
  static _Float16 op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_nchw_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type134(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type135(Type::TENSOR_FLOAT16, {6});
  OperandType type142(Type::TENSOR_FLOAT16, {1, 9, 2, 2});
  OperandType type143(Type::TENSOR_FLOAT16, {1, 6, 2, 2});
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type142);
  auto op22 = model->addOperand(&type134);
  auto op32 = model->addOperand(&type135);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type143);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_nchw_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 2, 2, 9});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type8);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static uint8_t op22_init[] = {4, 8, 12, 8, 4, 0, 8, 12, 12, 24, 24, 24, 36, 32, 20, 8, 4, 4};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 18);
  static int32_t op32_init[] = {80, -160, 240, -320, 400, -480};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type126(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type127(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type126);
  auto op32 = model->addOperand(&type127);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int8_t op22_init[] = {4, 8, 12, 7, 3, 0, 8, 12, 12, 20, 20, 20, 36, 32, 20, 7, 3, 3};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 18);
  static int32_t op32_init[] = {80, -133, 240, -267, 400, -400};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type122(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 9}, 0.5f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type145(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type146(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type122);
  auto op22 = model->addOperand(&type145);
  auto op32 = model->addOperand(&type146);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type130(Type::TENSOR_FLOAT16, {1, 2, 2, 9});
  OperandType type131(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type132(Type::TENSOR_FLOAT16, {6});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op12 = model->addOperand(&type130);
  auto op22 = model->addOperand(&type131);
  auto op32 = model->addOperand(&type132);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 18);
  static _Float16 op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nhwc_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type130(Type::TENSOR_FLOAT16, {1, 2, 2, 9});
  OperandType type134(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type135(Type::TENSOR_FLOAT16, {6});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op12 = model->addOperand(&type130);
  auto op22 = model->addOperand(&type134);
  auto op32 = model->addOperand(&type135);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nhwc_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static float op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(float) * 18);
  static float op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(float) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_relaxed_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6});
  OperandType type136(Type::TENSOR_FLOAT32, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type63(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type9(Type::TENSOR_FLOAT32, {6, 1, 1, 3});
  // Phase 1, operands
  auto op12 = model->addOperand(&type136);
  auto op22 = model->addOperand(&type9);
  auto op32 = model->addOperand(&type10);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type63);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_relaxed_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static uint8_t op22_init[] = {4, 8, 12, 8, 4, 0, 8, 12, 12, 24, 24, 24, 36, 32, 20, 8, 4, 4};
  model->setOperandValue(op22, op22_init, sizeof(uint8_t) * 18);
  static int32_t op32_init[] = {80, -160, 240, -320, 400, -480};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_quant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type123(Type::TENSOR_QUANT8_ASYMM, {6, 1, 1, 3}, 0.25f, 0);
  OperandType type124(Type::TENSOR_INT32, {6}, 0.125f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type123);
  auto op32 = model->addOperand(&type124);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_quant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_channelQuant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type126(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type127(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type126);
  auto op32 = model->addOperand(&type127);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int8_t op22_init[] = {4, 8, 12, 7, 3, 0, 8, 12, 12, 20, 20, 20, 36, 32, 20, 7, 3, 3};
  model->setOperandValue(op22, op22_init, sizeof(int8_t) * 18);
  static int32_t op32_init[] = {80, -133, 240, -267, 400, -400};
  model->setOperandValue(op32, op32_init, sizeof(int32_t) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_channelQuant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type138(Type::TENSOR_QUANT8_ASYMM, {1, 9, 2, 2}, 0.5f, 0);
  OperandType type144(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 2.0f, 60);
  OperandType type147(Type::TENSOR_QUANT8_SYMM_PER_CHANNEL, {6, 1, 1, 3}, 0.0f, 0, SymmPerChannelQuantParams({0.25f, 0.3f, 0.25f, 0.3f, 0.25f, 0.3f},0));
  OperandType type148(Type::TENSOR_INT32, {6}, 0.0f, 0);
  OperandType type4(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type138);
  auto op22 = model->addOperand(&type147);
  auto op32 = model->addOperand(&type148);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type144);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_channelQuant8_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type131(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type132(Type::TENSOR_FLOAT16, {6});
  OperandType type142(Type::TENSOR_FLOAT16, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op12 = model->addOperand(&type142);
  auto op22 = model->addOperand(&type131);
  auto op32 = model->addOperand(&type132);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static _Float16 op22_init[] = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 3.0f, 3.0f, 6.0f, 6.0f, 6.0f, 9.0f, 8.0f, 5.0f, 2.0f, 1.0f, 1.0f};
  model->setOperandValue(op22, op22_init, sizeof(_Float16) * 18);
  static _Float16 op32_init[] = {10.0f, -20.0f, 30.0f, -40.0f, 50.0f, -60.0f};
  model->setOperandValue(op32, op32_init, sizeof(_Float16) * 6);
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_channel_dynamic_output_shape_nchw_float16_weight_as_input(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type134(Type::TENSOR_FLOAT16, {6, 1, 1, 3});
  OperandType type135(Type::TENSOR_FLOAT16, {6});
  OperandType type142(Type::TENSOR_FLOAT16, {1, 9, 2, 2});
  OperandType type4(Type::INT32, {});
  OperandType type71(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  // Phase 1, operands
  auto op12 = model->addOperand(&type142);
  auto op22 = model->addOperand(&type134);
  auto op32 = model->addOperand(&type135);
  auto param12 = model->addOperand(&type4);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type4);
  auto param15 = model->addOperand(&type4);
  auto param16 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type71);
  // Phase 2, operations
  static int32_t param12_init[] = {1};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static int32_t param13_init[] = {1};
  model->setOperandValue(param13, param13_init, sizeof(int32_t) * 1);
  static int32_t param14_init[] = {1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {3};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static int32_t param16_init[] = {0};
  model->setOperandValue(param16, param16_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_GROUPED_CONV_2D, {op12, op22, op32, param12, param13, param14, param15, param16, layout}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12, op22, op32},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_channel_dynamic_output_shape_nchw_float16_weight_as_input(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

