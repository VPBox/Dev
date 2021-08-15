// clang-format off
// Generated file (from: space_to_batch_v1_2.mod.py). Do not edit
void CreateModel_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type3(Type::TENSOR_FLOAT32, {4, 1, 1, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type3);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type12(Type::TENSOR_FLOAT16, {4, 1, 1, 2});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type12);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  OperandType type14(Type::TENSOR_QUANT8_ASYMM, {4, 1, 1, 2}, 0.1f, 0);
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type14);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type15(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type15(Type::TENSOR_FLOAT32, {4, 2, 1, 1});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type15);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type16(Type::TENSOR_FLOAT16, {4, 2, 1, 1});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type16);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  OperandType type17(Type::TENSOR_QUANT8_ASYMM, {4, 2, 1, 1}, 0.1f, 0);
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type11(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type11);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type13(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.1f, 0);
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type20(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op1 = model->addOperand(&type13);
  auto param = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type20);
  // Phase 2, operations
  static int32_t param_init[] = {2, 2};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op1, param, paddings, layout}, {op4});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  OperandType type6(Type::TENSOR_FLOAT32, {4, 2, 2, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  OperandType type22(Type::TENSOR_FLOAT16, {4, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type21);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type22);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.5f, 0);
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {4, 2, 2, 1}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type23);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type24);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type26(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type26(Type::TENSOR_FLOAT32, {4, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  OperandType type28(Type::TENSOR_FLOAT16, {4, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type27);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.5f, 0);
  OperandType type30(Type::TENSOR_QUANT8_ASYMM, {4, 1, 2, 2}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type29);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type30);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 4, 4, 1});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type21(Type::TENSOR_FLOAT16, {1, 4, 4, 1});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type21);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type23(Type::TENSOR_QUANT8_ASYMM, {1, 4, 4, 1}, 0.5f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type23);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type25(Type::TENSOR_FLOAT32, {1, 1, 4, 4});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type25);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {1, 1, 4, 4});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type27);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type29(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 4}, 0.5f, 0);
  OperandType type31(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 0);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type29);
  auto param1 = model->addOperand(&type4);
  auto paddings = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param1_init[] = {2, 2};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 2);
  static int32_t paddings_init[] = {0, 0, 0, 0};
  model->setOperandValue(paddings, paddings_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op11, param1, paddings, layout}, {op41});
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

void CreateModel_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  OperandType type8(Type::TENSOR_FLOAT32, {6, 2, 2, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  OperandType type8(Type::TENSOR_FLOAT32, {6, 2, 2, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type8);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 5, 2, 1});
  OperandType type33(Type::TENSOR_FLOAT16, {6, 2, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 5, 2, 1}, 0.5f, 128);
  OperandType type35(Type::TENSOR_QUANT8_ASYMM, {6, 2, 2, 1}, 0.5f, 128);
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type36(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  OperandType type37(Type::TENSOR_FLOAT32, {6, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type36(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  OperandType type37(Type::TENSOR_FLOAT32, {6, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 1, 5, 2});
  OperandType type39(Type::TENSOR_FLOAT16, {6, 1, 2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type39);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 5, 2}, 0.5f, 128);
  OperandType type41(Type::TENSOR_QUANT8_ASYMM, {6, 1, 2, 2}, 0.5f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type41);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nhwc_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 5, 2, 1});
  // Phase 1, operands
  auto op12 = model->addOperand(&type7);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nhwc_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type32(Type::TENSOR_FLOAT16, {1, 5, 2, 1});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type32);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nhwc_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type34(Type::TENSOR_QUANT8_ASYMM, {1, 5, 2, 1}, 0.5f, 128);
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type34);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nchw_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type36(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type36(Type::TENSOR_FLOAT32, {1, 1, 5, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type36);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nchw_float16_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type38(Type::TENSOR_FLOAT16, {1, 1, 5, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  // Phase 1, operands
  auto op12 = model->addOperand(&type38);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_dynamic_output_shape_nchw_quant8_3(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {1, 1, 5, 2}, 0.5f, 128);
  OperandType type42(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.5f, 128);
  // Phase 1, operands
  auto op12 = model->addOperand(&type40);
  auto param2 = model->addOperand(&type4);
  auto paddings1 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op42 = model->addOperand(&type42);
  // Phase 2, operations
  static int32_t param2_init[] = {3, 2};
  model->setOperandValue(param2, param2_init, sizeof(int32_t) * 2);
  static int32_t paddings1_init[] = {1, 0, 2, 0};
  model->setOperandValue(paddings1, paddings1_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op12, param2, paddings1, layout}, {op42});
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

void CreateModel_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_FLOAT32, {6, 2, 4, 1});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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
  OperandType type10(Type::TENSOR_FLOAT32, {6, 2, 4, 1});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type10);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type43(Type::TENSOR_FLOAT16, {1, 4, 2, 1});
  OperandType type44(Type::TENSOR_FLOAT16, {6, 2, 4, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type44);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 0.25f, 128);
  OperandType type46(Type::TENSOR_QUANT8_ASYMM, {6, 2, 4, 1}, 0.25f, 128);
  // Phase 1, operands
  auto op13 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type46);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 1, 4, 2});
  OperandType type48(Type::TENSOR_FLOAT32, {6, 1, 2, 4});
  // Phase 1, operands
  auto op13 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 1, 4, 2});
  OperandType type48(Type::TENSOR_FLOAT32, {6, 1, 2, 4});
  // Phase 1, operands
  auto op13 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type48);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 1, 4, 2});
  OperandType type50(Type::TENSOR_FLOAT16, {6, 1, 2, 4});
  // Phase 1, operands
  auto op13 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type50);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 2}, 0.25f, 128);
  OperandType type52(Type::TENSOR_QUANT8_ASYMM, {6, 1, 2, 4}, 0.25f, 128);
  // Phase 1, operands
  auto op13 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type52);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nhwc_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type9(Type::TENSOR_FLOAT32, {1, 4, 2, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type9);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nhwc_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type43(Type::TENSOR_FLOAT16, {1, 4, 2, 1});
  // Phase 1, operands
  auto op13 = model->addOperand(&type43);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nhwc_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type45(Type::TENSOR_QUANT8_ASYMM, {1, 4, 2, 1}, 0.25f, 128);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  // Phase 1, operands
  auto op13 = model->addOperand(&type45);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type53);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nchw_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 1, 4, 2});
  // Phase 1, operands
  auto op13 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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
  OperandType type18(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type47(Type::TENSOR_FLOAT32, {1, 1, 4, 2});
  // Phase 1, operands
  auto op13 = model->addOperand(&type47);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type18);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nchw_float16_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type19(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type49(Type::TENSOR_FLOAT16, {1, 1, 4, 2});
  // Phase 1, operands
  auto op13 = model->addOperand(&type49);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

void CreateModel_dynamic_output_shape_nchw_quant8_4(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type2(Type::TENSOR_INT32, {2, 2});
  OperandType type4(Type::TENSOR_INT32, {2});
  OperandType type51(Type::TENSOR_QUANT8_ASYMM, {1, 1, 4, 2}, 0.25f, 128);
  OperandType type53(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 128);
  // Phase 1, operands
  auto op13 = model->addOperand(&type51);
  auto param3 = model->addOperand(&type4);
  auto paddings2 = model->addOperand(&type2);
  auto layout = model->addOperand(&type0);
  auto op43 = model->addOperand(&type53);
  // Phase 2, operations
  static int32_t param3_init[] = {3, 2};
  model->setOperandValue(param3, param3_init, sizeof(int32_t) * 2);
  static int32_t paddings2_init[] = {1, 1, 2, 4};
  model->setOperandValue(paddings2, paddings2_init, sizeof(int32_t) * 4);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_SPACE_TO_BATCH_ND, {op13, param3, paddings2, layout}, {op43});
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

