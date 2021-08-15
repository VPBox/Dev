// clang-format off
// Generated file (from: resize_bilinear_v1_2.mod.py). Do not edit
void CreateModel_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.01f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type24);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type25);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type24);
  auto param = model->addOperand(&type3);
  auto param1 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param_init[] = {3};
  model->setOperandValue(param, param_init, sizeof(int32_t) * 1);
  static int32_t param1_init[] = {3};
  model->setOperandValue(param1, param1_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param, param1, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type2(Type::TENSOR_FLOAT32, {1, 3, 3, 1});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type2);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type19);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type21(Type::TENSOR_FLOAT32, {1, 1, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type21);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type23(Type::TENSOR_FLOAT16, {1, 1, 3, 3});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type23);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.01f, 0);
  OperandType type25(Type::TENSOR_QUANT8_ASYMM, {1, 1, 3, 3}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type24);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type25);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type1(Type::TENSOR_FLOAT32, {1, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type1);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type16);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type18);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type20(Type::TENSOR_FLOAT32, {1, 1, 2, 2});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type20);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type26);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type22(Type::TENSOR_FLOAT16, {1, 1, 2, 2});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type22);
  auto param2 = model->addOperand(&type29);
  auto param3 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(_Float16) * 1);
  static _Float16 param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type24(Type::TENSOR_QUANT8_ASYMM, {1, 1, 2, 2}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op1 = model->addOperand(&type24);
  auto param2 = model->addOperand(&type4);
  auto param3 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op4 = model->addOperand(&type28);
  // Phase 2, operations
  static float param2_init[] = {1.5f};
  model->setOperandValue(param2, param2_init, sizeof(float) * 1);
  static float param3_init[] = {1.5f};
  model->setOperandValue(param3, param3_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op1, param2, param3, layout}, {op4});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op1},
    {op4});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type31);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type33);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type34(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type36);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type3(Type::INT32, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param4 = model->addOperand(&type3);
  auto param5 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type37);
  // Phase 2, operations
  static int32_t param4_init[] = {3};
  model->setOperandValue(param4, param4_init, sizeof(int32_t) * 1);
  static int32_t param5_init[] = {3};
  model->setOperandValue(param5, param5_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param4, param5, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  OperandType type6(Type::TENSOR_FLOAT32, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type6);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type31(Type::TENSOR_FLOAT16, {1, 3, 3, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type31);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.600000023841858f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.600000023841858f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type33(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 2}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type33);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type34(Type::TENSOR_FLOAT32, {1, 2, 3, 3});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type34);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type29(Type::FLOAT16, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  OperandType type35(Type::TENSOR_FLOAT16, {1, 2, 3, 3});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type35);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.600000023841858f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.600000023841858f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type36(Type::TENSOR_QUANT8_ASYMM, {1, 2, 3, 3}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type36);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.600000023841858f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.600000023841858f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type37);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_relaxed_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type4(Type::FLOAT32, {});
  OperandType type5(Type::TENSOR_FLOAT32, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type5);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type26);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  // Phase 4: set relaxed execution
  model->relaxComputationFloat32toFloat16(true);
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_relaxed_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type30(Type::TENSOR_FLOAT16, {1, 2, 2, 2});
  // Phase 1, operands
  auto op11 = model->addOperand(&type30);
  auto param6 = model->addOperand(&type29);
  auto param7 = model->addOperand(&type29);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param6_init[] = {1.600000023841858f};
  model->setOperandValue(param6, param6_init, sizeof(_Float16) * 1);
  static _Float16 param7_init[] = {1.600000023841858f};
  model->setOperandValue(param7, param7_init, sizeof(_Float16) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_float16_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type32(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 2}, 0.25f, 0);
  OperandType type37(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.25f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op11 = model->addOperand(&type32);
  auto param6 = model->addOperand(&type4);
  auto param7 = model->addOperand(&type4);
  auto layout = model->addOperand(&type0);
  auto op41 = model->addOperand(&type37);
  // Phase 2, operations
  static float param6_init[] = {1.6f};
  model->setOperandValue(param6, param6_init, sizeof(float) * 1);
  static float param7_init[] = {1.6f};
  model->setOperandValue(param7, param7_init, sizeof(float) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op11, param6, param7, layout}, {op41});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op11},
    {op41});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_float16(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto op42 = model->addOperand(&type17);
  // Phase 2, operations
  static int32_t param8_init[] = {3};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {3};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param8, param9}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_quant8(Model *model) {
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static int32_t param8_init[] = {3};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {3};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param8, param9}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_float16(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type16);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto op42 = model->addOperand(&type27);
  // Phase 2, operations
  static int32_t param8_init[] = {3};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {3};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param8, param9}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_shape_dynamic_output_shape_quant8(Model *model) {
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type3(Type::INT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto param8 = model->addOperand(&type3);
  auto param9 = model->addOperand(&type3);
  auto op42 = model->addOperand(&type28);
  // Phase 2, operations
  static int32_t param8_init[] = {3};
  model->setOperandValue(param8, param8_init, sizeof(int32_t) * 1);
  static int32_t param9_init[] = {3};
  model->setOperandValue(param9, param9_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param8, param9}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_shape_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_float16(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type17(Type::TENSOR_FLOAT16, {1, 3, 3, 1});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type16);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto op42 = model->addOperand(&type17);
  // Phase 2, operations
  static _Float16 param10_init[] = {1.7999999523162842f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.7999999523162842f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param10, param11}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_scale_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_quant8(Model *model) {
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type19(Type::TENSOR_QUANT8_ASYMM, {1, 3, 3, 1}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type19);
  // Phase 2, operations
  static float param10_init[] = {1.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param10, param11}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_scale_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_float16(Model *model) {
  OperandType type16(Type::TENSOR_FLOAT16, {1, 2, 2, 1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type16);
  auto param10 = model->addOperand(&type29);
  auto param11 = model->addOperand(&type29);
  auto op42 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 param10_init[] = {1.7999999523162842f};
  model->setOperandValue(param10, param10_init, sizeof(_Float16) * 1);
  static _Float16 param11_init[] = {1.7999999523162842f};
  model->setOperandValue(param11, param11_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param10, param11}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_float16(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_scale_dynamic_output_shape_quant8(Model *model) {
  OperandType type18(Type::TENSOR_QUANT8_ASYMM, {1, 2, 2, 1}, 0.01f, 0);
  OperandType type28(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.01f, 0);
  OperandType type4(Type::FLOAT32, {});
  // Phase 1, operands
  auto op12 = model->addOperand(&type18);
  auto param10 = model->addOperand(&type4);
  auto param11 = model->addOperand(&type4);
  auto op42 = model->addOperand(&type28);
  // Phase 2, operations
  static float param10_init[] = {1.8f};
  model->setOperandValue(param10, param10_init, sizeof(float) * 1);
  static float param11_init[] = {1.8f};
  model->setOperandValue(param11, param11_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {op12, param10, param11}, {op42});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {op12},
    {op42});
  assert(model->isValid());
}

inline bool is_ignored_scale_dynamic_output_shape_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type11(Type::TENSOR_FLOAT32, {0, 4});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type14);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type15);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type14);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type15);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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

void CreateModel_zero_sized_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {0, 3, 3, 1}, 0.1f, 128);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type43);
  auto roi = model->addOperand(&type41);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type42);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type39);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type38);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type40);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type47(Type::TENSOR_FLOAT16, {0, 3, 3, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type51(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type50);
  auto roi = model->addOperand(&type48);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type29);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type29);
  auto param17 = model->addOperand(&type29);
  auto param18 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type51);
  auto roiOut = model->addOperand(&type49);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type46);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type29);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type45);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type47);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {0.30000001192092896f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static _Float16 param16_init[] = {0.4000000059604645f};
  model->setOperandValue(param16, param16_init, sizeof(_Float16) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  static _Float16 param18_init[] = {0.30000001192092896f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static _Float16 param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(_Float16) * 1);
  static _Float16 param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type53(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type52);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type53);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type53(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type52);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type53);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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

void CreateModel_zero_sized_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {0, 1, 3, 3}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type43);
  auto roi = model->addOperand(&type41);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type42);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type39);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type54);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type55);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type51(Type::TENSOR_FLOAT16, {0});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type57(Type::TENSOR_FLOAT16, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores = model->addOperand(&type50);
  auto roi = model->addOperand(&type48);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type29);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type29);
  auto param17 = model->addOperand(&type29);
  auto param18 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type51);
  auto roiOut = model->addOperand(&type49);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type46);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type29);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type56);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type57);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {0.30000001192092896f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static _Float16 param16_init[] = {0.4000000059604645f};
  model->setOperandValue(param16, param16_init, sizeof(_Float16) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  static _Float16 param18_init[] = {0.30000001192092896f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static _Float16 param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(_Float16) * 1);
  static _Float16 param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type14);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type14);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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

void CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type43);
  auto roi = model->addOperand(&type41);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type42);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type39);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type38);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type59(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type50);
  auto roi = model->addOperand(&type48);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type29);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type29);
  auto param17 = model->addOperand(&type29);
  auto param18 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type59);
  auto roiOut = model->addOperand(&type49);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type46);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type29);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type45);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {0.30000001192092896f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static _Float16 param16_init[] = {0.4000000059604645f};
  model->setOperandValue(param16, param16_init, sizeof(_Float16) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  static _Float16 param18_init[] = {0.30000001192092896f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static _Float16 param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(_Float16) * 1);
  static _Float16 param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type52);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type7);
  auto roi = model->addOperand(&type8);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type9);
  auto roiOut = model->addOperand(&type11);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type13);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type52);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type26);
  // Phase 2, operations
  static float scores_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores, scores_init, sizeof(float) * 2);
  static float roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(float) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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

void CreateModel_zero_sized_dynamic_output_shape_nchw_quant8(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores = model->addOperand(&type43);
  auto roi = model->addOperand(&type41);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type4);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type4);
  auto param17 = model->addOperand(&type4);
  auto param18 = model->addOperand(&type4);
  auto scoresOut = model->addOperand(&type44);
  auto roiOut = model->addOperand(&type42);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type39);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type4);
  auto param22 = model->addOperand(&type4);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type54);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t scores_init[] = {137, 129};
  model->setOperandValue(scores, scores_init, sizeof(uint8_t) * 2);
  static uint16_t roi_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi, roi_init, sizeof(uint16_t) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static float param13_init[] = {0.3f};
  model->setOperandValue(param13, param13_init, sizeof(float) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static float param16_init[] = {0.4f};
  model->setOperandValue(param16, param16_init, sizeof(float) * 1);
  static float param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(float) * 1);
  static float param18_init[] = {0.3f};
  model->setOperandValue(param18, param18_init, sizeof(float) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static float param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(float) * 1);
  static float param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(float) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in},
    {scoresOut, classesOut, out});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type59(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores = model->addOperand(&type50);
  auto roi = model->addOperand(&type48);
  auto param12 = model->addOperand(&type12);
  auto param13 = model->addOperand(&type29);
  auto param14 = model->addOperand(&type3);
  auto param15 = model->addOperand(&type3);
  auto param16 = model->addOperand(&type29);
  auto param17 = model->addOperand(&type29);
  auto param18 = model->addOperand(&type29);
  auto scoresOut = model->addOperand(&type59);
  auto roiOut = model->addOperand(&type49);
  auto classesOut = model->addOperand(&type10);
  auto batchSplitOut = model->addOperand(&type10);
  auto in = model->addOperand(&type46);
  auto param19 = model->addOperand(&type3);
  auto param20 = model->addOperand(&type3);
  auto param21 = model->addOperand(&type29);
  auto param22 = model->addOperand(&type29);
  auto param23 = model->addOperand(&type3);
  auto param24 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap = model->addOperand(&type56);
  auto param25 = model->addOperand(&type3);
  auto param26 = model->addOperand(&type3);
  auto out = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 scores_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores, scores_init, sizeof(_Float16) * 2);
  static _Float16 roi_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi, roi_init, sizeof(_Float16) * 8);
  static int32_t param12_init[] = {0};
  model->setOperandValue(param12, param12_init, sizeof(int32_t) * 1);
  static _Float16 param13_init[] = {0.30000001192092896f};
  model->setOperandValue(param13, param13_init, sizeof(_Float16) * 1);
  static int32_t param14_init[] = {-1};
  model->setOperandValue(param14, param14_init, sizeof(int32_t) * 1);
  static int32_t param15_init[] = {0};
  model->setOperandValue(param15, param15_init, sizeof(int32_t) * 1);
  static _Float16 param16_init[] = {0.4000000059604645f};
  model->setOperandValue(param16, param16_init, sizeof(_Float16) * 1);
  static _Float16 param17_init[] = {1.0f};
  model->setOperandValue(param17, param17_init, sizeof(_Float16) * 1);
  static _Float16 param18_init[] = {0.30000001192092896f};
  model->setOperandValue(param18, param18_init, sizeof(_Float16) * 1);
  static int32_t param19_init[] = {2};
  model->setOperandValue(param19, param19_init, sizeof(int32_t) * 1);
  static int32_t param20_init[] = {2};
  model->setOperandValue(param20, param20_init, sizeof(int32_t) * 1);
  static _Float16 param21_init[] = {2.0f};
  model->setOperandValue(param21, param21_init, sizeof(_Float16) * 1);
  static _Float16 param22_init[] = {2.0f};
  model->setOperandValue(param22, param22_init, sizeof(_Float16) * 1);
  static int32_t param23_init[] = {4};
  model->setOperandValue(param23, param23_init, sizeof(int32_t) * 1);
  static int32_t param24_init[] = {4};
  model->setOperandValue(param24, param24_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static int32_t param25_init[] = {3};
  model->setOperandValue(param25, param25_init, sizeof(int32_t) * 1);
  static int32_t param26_init[] = {3};
  model->setOperandValue(param26, param26_init, sizeof(int32_t) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores, roi, param12, param13, param14, param15, param16, param17, param18}, {scoresOut, roiOut, classesOut, batchSplitOut});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in, roiOut, batchSplitOut, param19, param20, param21, param22, param23, param24, layout}, {featureMap});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap, param25, param26, layout}, {out});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type14);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type15);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type15(Type::TENSOR_FLOAT32, {0, 3, 3, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type14);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type15);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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

void CreateModel_zero_sized_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type40(Type::TENSOR_QUANT8_ASYMM, {0, 3, 3, 1}, 0.1f, 128);
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto roi1 = model->addOperand(&type41);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type39);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type38);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type40);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type47(Type::TENSOR_FLOAT16, {0, 3, 3, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type51(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type50);
  auto roi1 = model->addOperand(&type48);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type29);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type29);
  auto param32 = model->addOperand(&type29);
  auto param33 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type51);
  auto roiOut1 = model->addOperand(&type49);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type46);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type45);
  auto param40 = model->addOperand(&type29);
  auto param41 = model->addOperand(&type29);
  auto out1 = model->addOperand(&type47);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.30000001192092896f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static _Float16 param31_init[] = {0.4000000059604645f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static _Float16 param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(_Float16) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param40_init[] = {1.600000023841858f};
  model->setOperandValue(param40, param40_init, sizeof(_Float16) * 1);
  static _Float16 param41_init[] = {1.600000023841858f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type53(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type52);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type53);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type53(Type::TENSOR_FLOAT32, {0, 1, 3, 3});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type52);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type53);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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

void CreateModel_zero_sized_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type55(Type::TENSOR_QUANT8_ASYMM, {0, 1, 3, 3}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto roi1 = model->addOperand(&type41);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type39);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type54);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type55);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type51(Type::TENSOR_FLOAT16, {0});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type57(Type::TENSOR_FLOAT16, {0, 1, 3, 3});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type50);
  auto roi1 = model->addOperand(&type48);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type29);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type29);
  auto param32 = model->addOperand(&type29);
  auto param33 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type51);
  auto roiOut1 = model->addOperand(&type49);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type46);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type56);
  auto param40 = model->addOperand(&type29);
  auto param41 = model->addOperand(&type29);
  auto out1 = model->addOperand(&type57);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.30000001192092896f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static _Float16 param31_init[] = {0.4000000059604645f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static _Float16 param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(_Float16) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param40_init[] = {1.600000023841858f};
  model->setOperandValue(param40, param40_init, sizeof(_Float16) * 1);
  static _Float16 param41_init[] = {1.600000023841858f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type14);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type14(Type::TENSOR_FLOAT32, {0, 2, 2, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type14);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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

void CreateModel_zero_sized_dynamic_output_shape_nhwc_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type38(Type::TENSOR_QUANT8_ASYMM, {0, 2, 2, 1}, 0.1f, 128);
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto roi1 = model->addOperand(&type41);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type39);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type38);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nhwc_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nhwc_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type45(Type::TENSOR_FLOAT16, {0, 2, 2, 1});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type59(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type50);
  auto roi1 = model->addOperand(&type48);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type29);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type29);
  auto param32 = model->addOperand(&type29);
  auto param33 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type59);
  auto roiOut1 = model->addOperand(&type49);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type46);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type45);
  auto param40 = model->addOperand(&type29);
  auto param41 = model->addOperand(&type29);
  auto out1 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.30000001192092896f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static _Float16 param31_init[] = {0.4000000059604645f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static _Float16 param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(_Float16) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {false};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param40_init[] = {1.600000023841858f};
  model->setOperandValue(param40, param40_init, sizeof(_Float16) * 1);
  static _Float16 param41_init[] = {1.600000023841858f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type52);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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
  OperandType type13(Type::TENSOR_FLOAT32, {1, 1, 1, 1});
  OperandType type26(Type::TENSOR_FLOAT32, {0, 0, 0, 0});
  OperandType type3(Type::INT32, {});
  OperandType type4(Type::FLOAT32, {});
  OperandType type52(Type::TENSOR_FLOAT32, {0, 1, 2, 2});
  OperandType type7(Type::TENSOR_FLOAT32, {1, 2});
  OperandType type8(Type::TENSOR_FLOAT32, {1, 8});
  OperandType type9(Type::TENSOR_FLOAT32, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type7);
  auto roi1 = model->addOperand(&type8);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type9);
  auto roiOut1 = model->addOperand(&type11);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type13);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type52);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type26);
  // Phase 2, operations
  static float scores1_init[] = {0.9f, 0.1f};
  model->setOperandValue(scores1, scores1_init, sizeof(float) * 2);
  static float roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(float) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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

void CreateModel_zero_sized_dynamic_output_shape_nchw_quant8_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type3(Type::INT32, {});
  OperandType type39(Type::TENSOR_QUANT8_ASYMM, {1, 1, 1, 1}, 0.1f, 128);
  OperandType type4(Type::FLOAT32, {});
  OperandType type41(Type::TENSOR_QUANT16_ASYMM, {1, 8}, 0.125f, 0);
  OperandType type42(Type::TENSOR_QUANT16_ASYMM, {0, 4}, 0.125f, 0);
  OperandType type43(Type::TENSOR_QUANT8_ASYMM, {1, 2}, 0.1f, 128);
  OperandType type44(Type::TENSOR_QUANT8_ASYMM, {0}, 0.1f, 128);
  OperandType type54(Type::TENSOR_QUANT8_ASYMM, {0, 1, 2, 2}, 0.1f, 128);
  OperandType type58(Type::TENSOR_QUANT8_ASYMM, {0, 0, 0, 0}, 0.1f, 128);
  // Phase 1, operands
  auto scores1 = model->addOperand(&type43);
  auto roi1 = model->addOperand(&type41);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type4);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type4);
  auto param32 = model->addOperand(&type4);
  auto param33 = model->addOperand(&type4);
  auto scoresOut1 = model->addOperand(&type44);
  auto roiOut1 = model->addOperand(&type42);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type39);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type4);
  auto param37 = model->addOperand(&type4);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type54);
  auto param40 = model->addOperand(&type4);
  auto param41 = model->addOperand(&type4);
  auto out1 = model->addOperand(&type58);
  // Phase 2, operations
  static uint8_t scores1_init[] = {137, 129};
  model->setOperandValue(scores1, scores1_init, sizeof(uint8_t) * 2);
  static uint16_t roi1_init[] = {8, 8, 80, 80, 0, 0, 80, 80};
  model->setOperandValue(roi1, roi1_init, sizeof(uint16_t) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static float param28_init[] = {0.3f};
  model->setOperandValue(param28, param28_init, sizeof(float) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static float param31_init[] = {0.4f};
  model->setOperandValue(param31, param31_init, sizeof(float) * 1);
  static float param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(float) * 1);
  static float param33_init[] = {0.3f};
  model->setOperandValue(param33, param33_init, sizeof(float) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static float param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(float) * 1);
  static float param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(float) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static float param40_init[] = {1.6f};
  model->setOperandValue(param40, param40_init, sizeof(float) * 1);
  static float param41_init[] = {1.6f};
  model->setOperandValue(param41, param41_init, sizeof(float) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
  // Phase 3, inputs and outputs
  model->identifyInputsAndOutputs(
    {in1},
    {scoresOut1, classesOut1, out1});
  assert(model->isValid());
}

inline bool is_ignored_zero_sized_dynamic_output_shape_nchw_quant8_2(int i) {
  static std::set<int> ignore = {};
  return ignore.find(i) != ignore.end();
}

void CreateModel_zero_sized_dynamic_output_shape_nchw_float16_2(Model *model) {
  OperandType type0(Type::BOOL, {});
  OperandType type10(Type::TENSOR_INT32, {0});
  OperandType type12(Type::TENSOR_INT32, {1});
  OperandType type27(Type::TENSOR_FLOAT16, {0, 0, 0, 0});
  OperandType type29(Type::FLOAT16, {});
  OperandType type3(Type::INT32, {});
  OperandType type46(Type::TENSOR_FLOAT16, {1, 1, 1, 1});
  OperandType type48(Type::TENSOR_FLOAT16, {1, 8});
  OperandType type49(Type::TENSOR_FLOAT16, {0, 4});
  OperandType type50(Type::TENSOR_FLOAT16, {1, 2});
  OperandType type56(Type::TENSOR_FLOAT16, {0, 1, 2, 2});
  OperandType type59(Type::TENSOR_FLOAT16, {0});
  // Phase 1, operands
  auto scores1 = model->addOperand(&type50);
  auto roi1 = model->addOperand(&type48);
  auto param27 = model->addOperand(&type12);
  auto param28 = model->addOperand(&type29);
  auto param29 = model->addOperand(&type3);
  auto param30 = model->addOperand(&type3);
  auto param31 = model->addOperand(&type29);
  auto param32 = model->addOperand(&type29);
  auto param33 = model->addOperand(&type29);
  auto scoresOut1 = model->addOperand(&type59);
  auto roiOut1 = model->addOperand(&type49);
  auto classesOut1 = model->addOperand(&type10);
  auto batchSplitOut1 = model->addOperand(&type10);
  auto in1 = model->addOperand(&type46);
  auto param34 = model->addOperand(&type3);
  auto param35 = model->addOperand(&type3);
  auto param36 = model->addOperand(&type29);
  auto param37 = model->addOperand(&type29);
  auto param38 = model->addOperand(&type3);
  auto param39 = model->addOperand(&type3);
  auto layout = model->addOperand(&type0);
  auto featureMap1 = model->addOperand(&type56);
  auto param40 = model->addOperand(&type29);
  auto param41 = model->addOperand(&type29);
  auto out1 = model->addOperand(&type27);
  // Phase 2, operations
  static _Float16 scores1_init[] = {0.8999999761581421f, 0.10000000149011612f};
  model->setOperandValue(scores1, scores1_init, sizeof(_Float16) * 2);
  static _Float16 roi1_init[] = {1.0f, 1.0f, 10.0f, 10.0f, 0.0f, 0.0f, 10.0f, 10.0f};
  model->setOperandValue(roi1, roi1_init, sizeof(_Float16) * 8);
  static int32_t param27_init[] = {0};
  model->setOperandValue(param27, param27_init, sizeof(int32_t) * 1);
  static _Float16 param28_init[] = {0.30000001192092896f};
  model->setOperandValue(param28, param28_init, sizeof(_Float16) * 1);
  static int32_t param29_init[] = {-1};
  model->setOperandValue(param29, param29_init, sizeof(int32_t) * 1);
  static int32_t param30_init[] = {0};
  model->setOperandValue(param30, param30_init, sizeof(int32_t) * 1);
  static _Float16 param31_init[] = {0.4000000059604645f};
  model->setOperandValue(param31, param31_init, sizeof(_Float16) * 1);
  static _Float16 param32_init[] = {1.0f};
  model->setOperandValue(param32, param32_init, sizeof(_Float16) * 1);
  static _Float16 param33_init[] = {0.30000001192092896f};
  model->setOperandValue(param33, param33_init, sizeof(_Float16) * 1);
  static int32_t param34_init[] = {2};
  model->setOperandValue(param34, param34_init, sizeof(int32_t) * 1);
  static int32_t param35_init[] = {2};
  model->setOperandValue(param35, param35_init, sizeof(int32_t) * 1);
  static _Float16 param36_init[] = {2.0f};
  model->setOperandValue(param36, param36_init, sizeof(_Float16) * 1);
  static _Float16 param37_init[] = {2.0f};
  model->setOperandValue(param37, param37_init, sizeof(_Float16) * 1);
  static int32_t param38_init[] = {4};
  model->setOperandValue(param38, param38_init, sizeof(int32_t) * 1);
  static int32_t param39_init[] = {4};
  model->setOperandValue(param39, param39_init, sizeof(int32_t) * 1);
  static bool8 layout_init[] = {true};
  model->setOperandValue(layout, layout_init, sizeof(bool8) * 1);
  static _Float16 param40_init[] = {1.600000023841858f};
  model->setOperandValue(param40, param40_init, sizeof(_Float16) * 1);
  static _Float16 param41_init[] = {1.600000023841858f};
  model->setOperandValue(param41, param41_init, sizeof(_Float16) * 1);
  model->addOperation(ANEURALNETWORKS_BOX_WITH_NMS_LIMIT, {scores1, roi1, param27, param28, param29, param30, param31, param32, param33}, {scoresOut1, roiOut1, classesOut1, batchSplitOut1});
  model->addOperation(ANEURALNETWORKS_ROI_ALIGN, {in1, roiOut1, batchSplitOut1, param34, param35, param36, param37, param38, param39, layout}, {featureMap1});
  model->addOperation(ANEURALNETWORKS_RESIZE_BILINEAR, {featureMap1, param40, param41, layout}, {out1});
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

