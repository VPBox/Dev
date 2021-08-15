// clang-format off
// Generated file (from: batch_to_space_v1_2.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type6(Type::TENSOR_FLOAT16, {4, 1, 1, 2});
  OperandType type7(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {4, 1, 1, 2}, 0.1f, 0);
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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
  OperandType type10(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {4, 2, 1, 1});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type7(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type7);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {4, 2, 1, 1}, 0.1f, 0);
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type9);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type6(Type::TENSOR_FLOAT16, {4, 1, 1, 2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type6);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type8(Type::TENSOR_QUANT8_ASYMM, {4, 1, 1, 2}, 0.1f, 0);
  // Phase 1, operands
  auto op1 = model->addOperand(&type8);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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
  OperandType type10(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type10);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type11(Type::TENSOR_FLOAT16, {4, 2, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type12(Type::TENSOR_QUANT8_ASYMM, {4, 2, 1, 1}, 0.1f, 0);
  OperandType type15(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type12);
  auto param = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op1, param, layout}, {op4});
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

void CreateModel_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type4);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type4);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type5);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {4, 2, 2, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type16);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {4, 2, 2, 1}, 0.5f, 128);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.5f, 128);
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type18);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type20);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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
  OperandType type20(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type20);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {4, 1, 2, 2});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {4, 1, 2, 2}, 0.5f, 128);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.5f, 128);
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type4);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::TENSOR_INT32, {2});
  OperandType type4(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type4);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type16(Type::TENSOR_FLOAT16, {4, 2, 2, 1});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type16);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {4, 2, 2, 1}, 0.5f, 128);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 128);
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type18);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type20(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type20);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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
  OperandType type13(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type20(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type20);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type13);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type14(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type22(Type::TENSOR_FLOAT16, {4, 1, 2, 2});
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type22);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

void CreateModel_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {4, 1, 2, 2}, 0.5f, 128);
  OperandType type26(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 128);
  OperandType type3(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type24);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_BATCH_TO_SPACE_ND, {op11, param1, layout}, {op41});
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

